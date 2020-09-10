package g.prog.parser;

import g.prog.CppUtilities;
import g.prog.ProgRegex;
import g.text.RegexUtils;
import g.text.parser.ParserRegex;

public class ParserCppName extends ParserRegex
{
	private String mResult = null;
	
	public ParserCppName()
	{
		super( RegexUtils.get_RegexForSequenceWithOptionalSpaces(ProgRegex.REGEX_CPP_NAME) );
	}
			
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
	{
		int index = super.doTry(aPossiblyParseEpression, aStartIndex);
		
		if (index != -1)
		{
			mResult = CppUtilities.format_Spaces(super.getResultTypized());
		}
		
		return index;
	}

	@Override
	public String getResultTypized() { return mResult; }	
}
