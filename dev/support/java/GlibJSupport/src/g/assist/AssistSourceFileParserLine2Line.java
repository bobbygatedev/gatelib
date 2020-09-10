package g.assist;

import g.assist.refs.ParserRef;
import g.assist.signal.ParserSignalByInstance;
import g.text.parser.IParserTypized;

/**
 * Encapsulation class for source file parser.<br> 
 * Two
 * @author Bobby Gate
 * @see g.assist.AssistSourceFileParserLine2LineAbstract
 */
public class AssistSourceFileParserLine2Line extends AssistSourceFileParserLine2LineAbstract 
{
	/**
	 * Constructor.
	 * @param aVerbosityLevel
	 * @see g.assist.AssistSourceFileParserLine2LineAbstract.AssistSourceFileParserLine2LineAbstract
	 */
	public AssistSourceFileParserLine2Line(int aVerbosityLevel)
	{
		super(aVerbosityLevel);
	}

	@SuppressWarnings("unchecked")
	@Override
	public IParserTypized<AssistGeneratedItemAbstract>[] getParserOptionSetTypized()
	{
		return new IParserTypized[] {
				new ParserSignalByInstance(),
				new ParserRef()
		};
	}
}
