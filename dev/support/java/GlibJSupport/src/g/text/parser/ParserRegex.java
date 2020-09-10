package g.text.parser;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * 
 * @author Bobby Gate
 *
 */
public class ParserRegex extends ParserTypizedAbstract<String>
{
	private String mRegex;
	private Pattern mPattern;
	private String mResult = null;
	
	/**
	 * Create a parser based on regular expression,
	 * succeeds if regular expression matches.
	 * @param aRegex regular expression string.
	 * @param aFlags see Pattern flags.
	 */
	public ParserRegex(String aRegex,int aFlags)
	{
		mPattern = Pattern.compile(mRegex = aRegex,aFlags);
	}

	/**
	 * Create a parser based on regular expression,
	 * succeeds if regular expression matches.
	 * @param aRegex regular expression string.
	 */
	public ParserRegex(String aRegex)
	{
		mPattern = Pattern.compile(mRegex = aRegex);
	}
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)				
	{
		int index = aStartIndex;
		Matcher m = mPattern.matcher(aPossiblyParseEpression);
		
		if (m.find(index) && m.start()==index)
		{
			mResult = aPossiblyParseEpression.substring(index,m.end());
			return m.end();
		}
		else
		{
			return -1;
		}		
	}
	
	@Override
	public String getResultTypized() { return mResult; }
	
	public String getRegex ( ) { return mRegex; }
}
