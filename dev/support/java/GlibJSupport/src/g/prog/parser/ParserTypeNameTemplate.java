package g.prog.parser;

import g.text.RegexUtils;
import g.text.parser.ParserRegex;
import g.text.parser.ParserTypizedAbstract;

import java.util.regex.Pattern;

/**
 * Parses a type name that can include template declaration like:
 * type1<t1> ::  type<t2>
 */
public class ParserTypeNameTemplate extends ParserTypizedAbstract<String>
{
	private String mResult = null;
	private ParserCppName mParserCppExpression = new ParserCppName();
	private ParserRegex mParserSymbols = new ParserRegex(RegexUtils.get_RegexSurroundedByOptionalSpaces("::|<|>|,"));

	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
	throws ParserErrorException
	{
		int index = aStartIndex;
		int template_count = 0;
		String symbol;

		try
		{
			index= mParserCppExpression.doTryWithException(aPossiblyParseEpression, index );

			while(true)
			{
				int old_index = index;//to be use in case of template_count==0

				try
				{
					index = mParserSymbols.doTryWithException(aPossiblyParseEpression, index);				
					symbol = mParserSymbols.getResultTypized().trim();
				} 
				catch (ParserNotMatchingException e) {	break; }

				if (symbol.equals(","))
				{
					if (template_count>0)
					{
						try { index= mParserCppExpression.doTryWithException(aPossiblyParseEpression, index ); } 
						catch (ParserNotMatchingException e)
						{
							throw new ParserErrorException("Expected a valid type name here!", aPossiblyParseEpression, index);					
						}
					}
					else if(template_count==0)
					{
						//In this case the i.e in MyCls ( int , ... ) the ',' is part of next expression 
						//so the index is reset to before the symbol match (end of 'int'
						index = old_index;
						break;
					}
					else { throw new ParserErrorException("',' not expected here!", aPossiblyParseEpression, index); }
				}
				else if (symbol.equals(">"))
				{
					if (template_count>0) { template_count--; }
					else { throw new ParserErrorException("'>' not expected here!", aPossiblyParseEpression, index); }
				}
				else if (symbol.equals("<"))
				{
					template_count++;

					try { index= mParserCppExpression.doTryWithException(aPossiblyParseEpression, index ); } 
					catch (ParserNotMatchingException e)
					{
						throw new ParserErrorException("Expected a valid type name here!", aPossiblyParseEpression, index);					
					}
				}
				else if (symbol.equals("::"))
				{
					try { index = doTryWithException(aPossiblyParseEpression, index); } 
					catch (ParserNotMatchingException e)
					{
						throw new ParserErrorException("Invalid expression after '::'!", aPossiblyParseEpression, index);
					}
				}
			}

			if(template_count==0)
			{
				mResult = Pattern.compile("\\s*").matcher(aPossiblyParseEpression.substring(aStartIndex,index)).replaceAll("");
			}
			else 
			{
				throw new ParserErrorException("Too manu '<'!", aPossiblyParseEpression, index);				
			}
			return index;
		} 
		catch (ParserNotMatchingException e)
		{
			mFailedIndex = index;
			return -1;
		}
	}

	@Override
	public String getResultTypized()	{ return mResult; }

	public static void main(String[] args)
	{
		String[] test_expressions = new String[]{
				"a1<int,a<int,int>>",
				"a1<int,a<int,int>> :: a2"
		};

		for (String test_expr : test_expressions)
		{
			ParserTypeNameTemplate p = new ParserTypeNameTemplate();

			try
			{
				int index = p.doTry(test_expr, 0);

				if (index != -1)
				{
					System.out.printf("Translation of %s succesfull:\n%s\n",test_expr,p.getResultTypized());
				}
				else 
				{
					System.out.printf("Translation of %s failed!\n",test_expr);
				}
							
			} 
			catch (ParserErrorException e)
			{
				System.out.printf("Translation of %s failed with msg in %d:\n%s",test_expr,e.getFailedIndex(), e.getMessage());
			}				
		}	
	}

}
