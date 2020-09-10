package g.text.parser;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.regex.Pattern;

/**
 * Parses an item that is a list of identical(of same format) item.
 * i.e '( a, b , c )' that has 
 * '(' as init string
 * ')' as end string
 * ',' as separator
 * @author Bobby Gate
 *
 * @param <T>
 */
public abstract class ParserSequenceSeparatorAbstract<T> extends ParserTypizedAbstract<T[]>
{
	private T[] mResult = null;
	/*
	 * 
	 */
	public abstract String getInitString ( );
	/*
	 * 
	 */
	public abstract String getSeparator ( );
	/*
	 * 
	 */
	public abstract String getEndString ( );
	/*
	 * 
	 */
	public abstract IParserTypized<T> getItemParser ( );
	/*
	 * If it returns true space will not be kept in count (regular expression for init,end,separator will 
	 * have \s*[x]\s* where [x] is init,end,separator
	 */
	public abstract boolean dontCareSpace ( );

	@Override
	public T[] getResultTypized() { return mResult;	}

	@SuppressWarnings("unchecked")
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
	throws ParserErrorException
	{
		int index = aStartIndex;

		ParserRegex parser_init = mGetParserFor ( getInitString() );
		ParserRegex parser_end = mGetParserFor ( getEndString() );
		ParserRegex parser_separator = mGetParserFor ( getSeparator() );
		IParserTypized<T> parser_item = getItemParser();

			ArrayList<T> result = new ArrayList<T>(); 

			try { index = parser_init.doTryWithException(aPossiblyParseEpression, index);}
			catch (ParserNotMatchingException e)
			{
				//if init separator isn't found parser return -1 without throwing exceptions 
				mFailedIndex = index;
				return -1;
			}

			while(true)
			{
				try
				{
					index =parser_item.doTryWithException(aPossiblyParseEpression, index); 					
					result.add(parser_item.getResultTypized());					
				}
				catch (ParserNotMatchingException e)
				{
					//after the first separator a valid item must be found,
					//if you want to have a optional item implement it in item parser
					//empty list like '()' are accepted
					if (result.size()>0)
					{
						throw new ParserErrorException(String.format(
								"Expected a valid expression after %s!", getInitString()),aPossiblyParseEpression , index);
					}
					else
					{
						break;//checking for expression end
					}
				}
				

				//if separator doesn't match exit
				try { index = parser_separator.doTryWithException(aPossiblyParseEpression, index); } 
				catch (ParserNotMatchingException e){break;}
			}

			try { index = parser_end.doTryWithException(aPossiblyParseEpression, index);} 
			catch (ParserNotMatchingException e)
			{				
				throw new ParserErrorException(
						String.format("'%s' or '%s' was expected here!", getSeparator() , getEndString()), aPossiblyParseEpression, mFailedIndex=index);
			}

			if(result.size()>0)
			{			
				//init of a generic array
				T[] temp_array =(T[]) Array.newInstance( result.get(0).getClass(), result.size());

				mResult = result.toArray(temp_array);
			}
			else
			{
				mResult = null;//(T[])new Object[0];
			}

			return index;
	}

	private ParserRegex mGetParserFor(String aString)
	{
		if (dontCareSpace())
		{
			return new ParserRegex (String.format("\\s*%s\\s*",Pattern.quote( aString)));
		}
		else
		{
			return new ParserRegex(aString);
		}
	}

	/*
	 * Example like in main class comment
	 */
	static class ParserTest extends ParserSequenceSeparatorAbstract<String>
	{		
		private ParserRegex mParserRegex = new ParserRegex("\\s*\\w+\\s*");

		@Override
		public ParserTypizedAbstract<String> getItemParser() 
		{
			return mParserRegex;
		}

		@Override
		public String getInitString()
		{
			return "(";
		}

		@Override
		public String getSeparator()
		{			
			return ",";		
		}

		@Override
		public String getEndString()
		{
			return ")";
		}

		@Override
		public boolean dontCareSpace()
		{
			return true;		
		}
	}

	public static void main(String[] args)
	{
		ParserTest p = new ParserTest();

		String[] tests	 = new String[] {
				" ( a ,b ,c ) " ,
				" ( a ,b ,c " 
		};
		
		for (String test : tests)
		{
			try
			{
				System.out.printf("Trying to translate: %s\n" ,test );
				
				p.doTryWithException(test, 0);

				for (String string : p.getResultTypized())
				{
					System.out.println(string);
				}			
			}
			catch (ParserErrorException e)
			{
				System.out.printf("Error in %d\n%s",e.getFailedIndex() , e.getMessage());
			} 
			catch (ParserNotMatchingException e)
			{
				System.out.printf("Failed translation in index %d\n",e.getFailedIndex());
			}					
		}
		
	}
}
