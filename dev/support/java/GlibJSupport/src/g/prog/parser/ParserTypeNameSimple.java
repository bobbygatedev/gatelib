package g.prog.parser;

import g.prog.CppUtilities;
import g.text.parser.ParserRegex;
import g.text.parser.ParserTypizedAbstract;

/**
 * Parses a simple(no template)type name:
 * type1::type
 */
public class ParserTypeNameSimple extends ParserTypizedAbstract<String>
{
	private String mResult = null;
	private ParserCppName mParserCppName = new ParserCppName();
	private ParserRegex mParserSymbols = new ParserRegex("\\s*::\\s*");

	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
	throws ParserErrorException
	{
		int index = aStartIndex;

		try
		{
			index= mParserCppName.doTryWithException(aPossiblyParseEpression, index );

			while(true)
			{
				try { index = mParserSymbols.doTryWithException(aPossiblyParseEpression, index); } 
				catch (ParserNotMatchingException e) {	break; }

				try { index = doTryWithException(aPossiblyParseEpression, index); } 
				catch (ParserNotMatchingException e)
				{
					throw new ParserErrorException("Invalid expression after '::'!", aPossiblyParseEpression , index);
				}
			}

			mResult = CppUtilities.format_Spaces(aPossiblyParseEpression.substring(aStartIndex,index));
			return index;
		} 
		catch (ParserNotMatchingException e)
		{
			mFailedIndex = index;
			return -1;
		}
	}

	@Override
	public String getResultTypized()
	{
		return mResult;
	}

	public static void main(String[] args)
	{
		String[] test_expressions = new String[]{
				"a1",
				"a1 :: a2 ::"
		};

		for (String test_expr : test_expressions)
		{
			ParserTypeNameSimple p = new ParserTypeNameSimple();

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
