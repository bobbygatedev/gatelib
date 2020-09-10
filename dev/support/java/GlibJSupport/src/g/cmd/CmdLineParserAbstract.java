package g.cmd;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Abstract class for implementing a command line application.
 * Override getCmdSet() and pass cmd line arguments to doCmdLineParse.
 * @see g.cmd.CmdLineParserAbstract.getCmdSet
 * @see g.cmd.CmdLineParserAbstract.doCmdLineParse
 * @author Bobby Gate
 *
 */
public abstract class CmdLineParserAbstract 
{
	class DummyCommandHelp extends CmdAbstract
	{
		private CmdLineParserAbstract mCmdLineParserAbstract;
		private boolean               mShowListOnly= false; //option -l
		private CmdAbstract           mSpecificCmd= null; //option name

		public DummyCommandHelp(CmdLineParserAbstract aCmdLineParserAbstract)
		{
			mCmdLineParserAbstract = aCmdLineParserAbstract;
		}

		@Override
		public int doExecute()
		{
			if (mShowListOnly)
			{
				ArrayList<CmdAbstract> cmd_list =  mCmdLineParserAbstract.mGetCompleteListCmd();

				for (CmdAbstract cmd : cmd_list)
				{
					if (cmd.getCmdOption()!=null)
					{
						System.out.printf("[%s]\n", cmd.getCmdOption());					
					}
				}								
			}
			else if (mSpecificCmd == null)
			{
				ArrayList<CmdAbstract> cmd_list =  mCmdLineParserAbstract.mGetCompleteListCmd();

				System.out.printf("-v%%d (i.e. -v1) selects the verbosity level (0=no verbose to +inf).\n\n");
				
				for (CmdAbstract cmd : cmd_list)
				{
					if (cmd.getCmdOption()!=null)
					{
						System.out.printf("[%s]\n%s\n\n", cmd.getCmdOption(),cmd.getHelpString());					
					}
				}				
			}
			else
			{
				System.out.printf("[%s]\n%s\n\n", mSpecificCmd.getCmdOption(),mSpecificCmd.getHelpString());					
			}
			
			return 0;
		}

		@Override
		public boolean doCmdParse(String[] args) throws CmdLineParserException
		{		
			mShowListOnly = false;
			mSpecificCmd = null;
			
			switch (args.length)
			{
			case 0: return true;
			case 1: 
				if (args[0]=="-l")
				{
					mShowListOnly = true;
				}
				else
				{
					mSpecificCmd = mSearchForSpecificOption( args[0] );
				}
				break;
			default:
				throw new WrongNumberOfParameterException(this);
			}
									
			return true;
		}

		@Override
		public String getHelpString() { return 
			"\t[-h] Print out the help.\n"+
			"\t[-h -l] Print the list of commands only.\n"+
			"\t[-h cmd_opt] Print out the help of a spefic command option.\n"; }

		@Override
		public String getCmdOption(){ return "-h"; }		

		private CmdAbstract mSearchForSpecificOption(String aCmdOption) throws CmdLineParserException
		{
			ArrayList<CmdAbstract> cmd_list =  mCmdLineParserAbstract.mGetCompleteListCmd();

			for (CmdAbstract cmd : cmd_list)
			{
				if (cmd.getCmdOption()!=null && cmd.getCmdOption().equals(aCmdOption))
				{
					return cmd;					
				}
			}				

			throw new CmdLineParserException("A command option name " + aCmdOption + " doesn't exist!");
		}
}

	class DummyCommandTooManyParse extends CmdAbstract
	{
		private int mNumArgs;

		@Override
		public int doExecute()
		{
			System.err.printf("Too many parameters: %d\n-h for help\n" , mNumArgs);
			return -1;
		}

		@Override
		public boolean doCmdParse(String[] args) {	return ( ( mNumArgs = args.length ) > 3 ); }

		@Override
		public String getHelpString(){ return null; }

		@Override
		public String getCmdOption() { return null; }
	}

	class DummyCommandNotEnoughParse extends CmdAbstract
	{
		private int mNumArgs;
		@Override
		public int doExecute()
		{
			System.err.printf("Not enough parameters: %d\n-h for help\n" , mNumArgs);
			return -1;
		}

		@Override
		public boolean doCmdParse(String[] args) {	return ( ( mNumArgs = args.length ) < 2 ); }

		@Override
		public String getHelpString(){ return null; }

		@Override
		public String getCmdOption() { return null; }
	}

	/**
	 * 
	 * @return
	 */
	public abstract CmdAbstract[] getCmdSet(); 
	
	/**
	 * 
	 * @param args
	 * @return
	 */
	public int doCmdLineParse(String[] args)
	{
		try
		{
			ArrayList<CmdAbstract> list_cmd = mGetCompleteListCmd ();

			for (CmdAbstract cmd : list_cmd)
			{
				if (cmd.getCmdOption()!=null)
				{
					int opt_pos_discriminant = m_SearchForOption ( cmd.getCmdOption() , args , 0 );

					if (opt_pos_discriminant != -1)
					{				
						int opt_pos_verbose = m_SearchForOption(
								"-v\\d", args= m_RemoveOption(args, opt_pos_discriminant),-1);

						if(opt_pos_verbose!=-1)
						{
							cmd.verbosityLevel = Integer.parseInt(args[opt_pos_verbose].substring(2));
							args = m_RemoveOption(args, opt_pos_verbose);																									
						}
					}
					else
					{
						continue;//it causes skip parsing 
					}
				}
				
				if(cmd.doCmdParse(args))
				{
					return cmd.doExecute();
				}									
			}			

			System.err.println("No an option matched!\n-h for help\n");				
		} 
		catch (CmdLineParserException e)
		{
			System.err.printf("Parse Error:\n%s\n-h for help\n",e.getMessage());
		}

		return -1;
	}

	/**
	 * Gets the complete list of commands ( including the one created inside the dummy ones.
	 * @return
	 */
	private ArrayList<CmdAbstract> mGetCompleteListCmd()
	{
		ArrayList<CmdAbstract> list_cmd = new ArrayList<CmdAbstract>();

		for (CmdAbstract cmd : getCmdSet())
		{
			list_cmd.add(cmd);
		}

		list_cmd.add(new DummyCommandHelp(this));
		list_cmd.add(new DummyCommandNotEnoughParse());
		list_cmd.add(new DummyCommandTooManyParse());
		
		return list_cmd;
	}

	/**
	 * Remove option a certain position if != -1, otherwise returns aArgs.
	 * @param aArgs
	 * @param aPosition
	 * @return
	 */
	private static String[] m_RemoveOption(String[] aArgs, int aPosition)
	{
		if (aPosition != -1)
		{
			LinkedList<String> linked_list = new LinkedList<String>();

			for (int i = 0; i < aArgs.length; i++)
			{
				if (i != aPosition)
				{
					linked_list.add(aArgs[i]);				
				}
			}
			
			return linked_list.toArray(new String[aArgs.length-1]);			
		}
		else
		{
			return aArgs;
		}
	}

	/**
	 * search for an option in a certain position (if aPosition == -1) the position is don't care,
	 * if the option is found aArgs is returned excluding the found option i.e
	 * option = -b args={-v,-b foo} pos = 3  returns true and args become { -v ,foo } 
	 * search is made just between the first argument that starts with -
	 * If search succeeds result is the position otherwise is 
	 * @param aOptionRegex option to be searched as regular expression i.e "-v\\d+"
	 * @param aArgs
	 * @param aPosition
	 * @return
	 */
	protected static int m_SearchForOption(String aOptionRegex, String[] aArgs, int aPosition)
	{
		Pattern pattern = Pattern.compile(aOptionRegex);
		
		for (int i = 0; i < aArgs.length ; i++)
		{			
			if (aArgs[i].startsWith("-"))
			{
				Matcher matcher = pattern.matcher(aArgs[i]);
				
				if ( ( aPosition == -1 || aPosition == i ) && matcher.matches())
				{
					return i;
				}
			}
			else
			{
				return -1;
			}
		}

		return -1;			
	}
	
	static class InnerTest extends CmdLineParserAbstract
	{
		class Cmd1 extends CmdAbstract
		{
			@Override
			public int doExecute()
			{
				System.out.printf("Verbosity Level = %d\n", verbosityLevel);
				System.out.println("CmdLineParserAbstract.InnerTest.Cmd1.doExecute()");
				return 0;
			}

			@Override
			public boolean doCmdParse(String[] args)
					throws CmdLineParserException
			{
				if (args.length==1 && args[0].equals("p"))
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			@Override
			public String getHelpString()
			{
				return "CmdLineParserAbstract.InnerTest.Cmd1.getHelpString()";				
			}

			@Override
			public String getCmdOption()
			{
				return "-c1";
			}			
		};


		class Cmd2 extends CmdAbstract
		{
			@Override
			public int doExecute()
			{
				System.out.printf("Verbosity Level = %d\n", verbosityLevel);				
				System.out.println("CmdLineParserAbstract.InnerTest.Cmd2.doExecute()");
				return 0;
			}

			@Override
			public boolean doCmdParse(String[] args)
					throws CmdLineParserException
			{
				if (args.length==1 && args[0].equals("p"))
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			@Override
			public String getHelpString()
			{
				return "CmdLineParserAbstract.InnerTest.Cmd2.getHelpString()";				
			}

			@Override
			public String getCmdOption()
			{
				return "-c2";
			}			
		};
				
		@Override
		public CmdAbstract[] getCmdSet()
		{
			return new CmdAbstract[]{ new Cmd1() , new Cmd2() };
		}		
	}
	
	public static void main(String[] args)
	{
		{
			String[] my_args = new String[] { "-c" , "-v5" };
	
			InnerTest inner_test = new InnerTest(); 
			
			inner_test.doCmdLineParse(my_args);
		}
		{
			String[] my_args = new String[] { "-c1" , "-v5" , "p"};
	
			InnerTest inner_test = new InnerTest(); 
			
			inner_test.doCmdLineParse(my_args);
		}
		{
			String[] my_args = new String[] { "-c2" , "-v50" , "p"};
	
			InnerTest inner_test = new InnerTest(); 
			
			inner_test.doCmdLineParse(my_args);
		}
	}
};
