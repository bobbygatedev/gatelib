package g.cmd;

/**
 * Override for defining a command.
 */
public abstract class CmdAbstract
{
	/**
	 * Defines the verbosity level from 0 (no verbosity) on. 
	 */
	public int verbosityLevel = 0; 

	protected abstract class OptionReader 
	{
		public void tryToRead ( String[] aArgs , int aArgsOffset , String[] aAllowedOptions ) throws CmdLineParserException
		{
			int position = aArgsOffset;

			while(position < aArgs.length)
			{
				if(m_IsIn(aArgs[position],aAllowedOptions))
				{
					if (++position < aArgs.length)
					{
						onOption(aArgs[position-1], aArgs[position]);
						position++;
					}
					else
					{
						throw new CmdLineParserException(String.format("Expected another argument after %s!", aArgs[position-1]) );
					}
				}
				else
				{
					throw new CmdLineParserException(String.format("Option %s not expected here!", aArgs[position]) );
				}
			}
		}

		public abstract void onOption (String aOption , String aOptionArgument );

		private boolean m_IsIn(String aOption, String[] aAllowedOptions)
		{
			for (String option : aAllowedOptions)
			{
				if (option.equals(aOption))
				{
					return true;
				}
			}
			return false;
		}
	}
	
	/**
	 * doExecute MUST capture all exception and generate console error message.
	 * Return value is exit value of application.
	 */
	public abstract int doExecute(); 
	
	/**
	 * Implements parse behavior 
	 * args is deprivated of the discriminant.
	 */
	public abstract boolean doCmdParse(String[] args) throws CmdLineParserException;
			
	/**
	 * Gets help string.
	 */
	public abstract String getHelpString();
	
	/**
	 * Gets the cmd_option 
	 * cmd_option is the value of first option that allows to select the command correctly i.e.
	 * java -jar [xx.jar] -o name in that case the command with cmd_option -o will be called
	 * if null, return value of doParse is used.
	 * cmd_option MUST be of the format -[option] like in -o
	 */
	public abstract String getCmdOption ( );
}
