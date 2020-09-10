package g.cmd;


public class WrongNumberOfParameterException extends CmdLineParserException
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public WrongNumberOfParameterException(CmdAbstract aCmd)
	{
		super ( String.format("Wrong number of parameters for cmd", aCmd.getCmdOption() ) );
	}
}
