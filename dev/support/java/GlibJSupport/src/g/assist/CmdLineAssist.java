package g.assist;

import g.assist.cmds.*;
import g.cmd.CmdLineParserAbstract;
import g.cmd.CmdAbstract;

/**
 * Implementation of command class defining the list of signals.
 * @author Bobby Gate
 */
public class CmdLineAssist extends CmdLineParserAbstract
{
	/**
	 * Gets the command set applicable for set.
	 */
	@Override
	public CmdAbstract[] getCmdSet()
	{
		return new CmdAbstract[] {                            
				new CmdSignalBodyToClipboard(),
				new CmdSignalToHeaderFile(),
				new CmdParseSourceCode(),
				new CmdPrecompileVisualStudio(),
				new CmdMsgBox() 
		};
	}

	public static void main(String[] args) 
	{
		CmdLineAssist assist_instance = new CmdLineAssist();

		System.exit( assist_instance.doCmdLineParse(args));
	}
}
