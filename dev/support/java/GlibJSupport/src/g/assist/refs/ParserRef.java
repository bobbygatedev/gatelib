package g.assist.refs;

import g.assist.AssistGeneratedItemAbstract;
import g.text.RegexUtils;
import g.text.parser.ParserRegex;
import g.text.parser.ParserTypizedAbstract;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * 
 * @author Bobby Gate
 *
 */
public class ParserRef extends ParserTypizedAbstract<AssistGeneratedItemAbstract>
{	
	private ParserRegex mParserKeyWord = new ParserRegex("^\\s*G_REF\\s*$",Pattern.MULTILINE);
	private AssistGeneratedItemRef mResult = null;
	
	@Override
	public AssistGeneratedItemAbstract getResultTypized(){ return mResult; }
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
	throws ParserErrorException
	{
		int index = aStartIndex;

		try { index = mParserKeyWord.doTryWithException(aPossiblyParseEpression, index); } 
		catch (ParserNotMatchingException e)
		{
			mFailedIndex = index;
			return -1;
		}

		mResult = new AssistGeneratedItemRef();

		Matcher matcher = RegexUtils.pattern_NewLine.matcher(aPossiblyParseEpression);

		if ( matcher.find(index) )
		{
			while(true)
			{
				index = matcher.end();

				try { index = mResult.doTryToAddConstructor(aPossiblyParseEpression,index); } 
				catch (ParserNotMatchingException e) 
				{
					matcher.find(index);
					return matcher.end(); 
				}

				matcher.find(index);
			}
		}
		else
		{
			return index;
		}		
	}
}
