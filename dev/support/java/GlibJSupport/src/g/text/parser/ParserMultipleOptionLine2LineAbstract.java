package g.text.parser;

import g.text.RegexUtils;

import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public abstract class ParserMultipleOptionLine2LineAbstract<T> 
	extends ParserTypizedAbstract<ArrayList<T>> implements IParserMultipleOptionTypizedAbstract<T> 
{
	private Pattern mNewLinePattern = Pattern.compile(RegexUtils.NEW_LINE);
	private ArrayList<T> mResult = null;	
		
	class InnerParserClass extends ParserMultipleOptionTypizedAbstract<T>
	{
		private ParserMultipleOptionLine2LineAbstract<T> mParent;
		
		public InnerParserClass(ParserMultipleOptionLine2LineAbstract<T> aParent){ mParent = aParent; }
		
		@Override
		public IParserTypized<T>[] getParserOptionSetTypized() {return mParent.getParserOptionSetTypized();	}
	}
	
	@Override
	public ArrayList<T> getResultTypized(){return mResult; }
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
			throws ParserErrorException
	{
		Matcher matcher = mNewLinePattern.matcher(aPossiblyParseEpression);
		InnerParserClass inner_parser = new InnerParserClass(this);
		int index = aStartIndex;
		
		mResult = new ArrayList<T>();
				
		while(true)
		{				
			int new_index = inner_parser.doTry(aPossiblyParseEpression, index);
				
			if(new_index !=-1)
			{
				mResult.add(inner_parser.getResultTypized());
				index = new_index;
			}
						
			if ( matcher.find(index) )
			{
				index = matcher.end();
			}
			else
			{
				return index;
			}
		}
	}

	static class InnerParserTest extends ParserMultipleOptionLine2LineAbstract<String>
	{
		private ParserRegex mParser1 = new ParserRegex("^\\s*Bee\\d\\s*\\n\\s*\\d\\s*$",Pattern.MULTILINE);
		private ParserRegex mParser2 = new ParserRegex("^\\s*Buu\\d\\s*\\n\\s*\\d\\s*$",Pattern.MULTILINE);
		
		@SuppressWarnings("unchecked")
		@Override
		public IParserTypized<String>[] getParserOptionSetTypized()
		{
			return new IParserTypized[] { mParser1,mParser2 };
		}
	}
	
	public static void main(String[] args)
	{
		String example = 
			"better\n" +
			"Bee5\n" +
			" 3 \n" +
			"ccc\n" +
			"\n"+ 
			"Buu5\n" +
			" 3 " ;
		
		InnerParserTest parser = new InnerParserTest();
				
		try
		{
			parser.doTryWithException(example, 0);
			
			for (String result : parser.getResultTypized())
			{
				System.out.println(result);
			}
		} 
		catch (ParserErrorException e)
		{
			System.err.println(e.getMessageWithFailedLineColInfo());
		} 
		catch (ParserNotMatchingException e)
		{
			System.out.println("Not matching!");
		}			
	}
	
}
