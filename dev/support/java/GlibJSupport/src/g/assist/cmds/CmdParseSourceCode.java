package g.assist.cmds;

import g.assist.AssistSourceFileParserLine2Line;
import g.cmd.CmdAbstract;
import g.cmd.CmdLineParserException;
import g.cmd.WrongNumberOfParameterException;
import g.text.parser.ParserAbstract.ParserErrorException;

import java.io.File;
import java.io.IOException;

/**
 * Implement command line function of parsing source code format<br>
 * G_ASSIST -p [-vx] [SourceFilePath]  {-od [specific_output_dir]}  {-or [success_report_path]}
 * @author Bobby Gate
 */
public class CmdParseSourceCode extends CmdAbstract
{
	private String mSourceFilePath;
	private String mOutputSuccessReportPath = null;
	private String mSpecificOutputDir=null;

	@Override
	public String getCmdOption() { return "-p"; }

	@Override
	public String getHelpString()
	{
		return  "G_ASSIST -p {-vX} [path_to_parse] {-od [specific_output_dir]}  {-or [success_report_path]}\n" +
		"Parse a source file interpreting the signals (G_SIGNAL) and refs(G_REF)!\n" +
		"If -od is specified [specific_output_dir] becomes the base dir for output files (instead of the dir of input file).\n"+
		"If -or is specified an output report will be generated in [success_report_path] in case of success ONLY.\n";
	}

	/**
	 * Execute the command without pasring it.
	 * @param aVerbosityLevel 	Verbosity Level.
	 * @param aSourceFilePath    Input source file path.
	 * @param aOutputSuccessReportPath  Path of the success report path.
	 * @param aSpecificOutputDir Specifies the generator base dir , if it is null the source dir will be used.
	 */
	public static int direct_DoExecute(int aVerbosityLevel, String aSourceFilePath, String aOutputSuccessReportPath, String aSpecificOutputDir)
	{
		CmdParseSourceCode instance = new CmdParseSourceCode();
		
		instance.mSourceFilePath = aSourceFilePath;
		instance.mOutputSuccessReportPath = aOutputSuccessReportPath;
		instance.mSpecificOutputDir = aSpecificOutputDir;
		instance.verbosityLevel = aVerbosityLevel;
		
		return instance.doExecute();
	}
	
	/**
	 * Source file is parsed and output generated in case of success.
	 * 	@see g.assist.AssistSourceFileParserLine2Line
	 *  @see g.assist.AssistSourceFileParserLine2Line.doStartSourceFile
	 *  @see g.assist.AssistSourceFileParserLine2Line.doGeneratedOutputPath
	 */
	@Override
	public int doExecute()
	{
		try 
		{
			AssistSourceFileParserLine2Line instance = 
				new AssistSourceFileParserLine2Line(verbosityLevel);

			System.out.printf("Pre-compiling %s..\n" , mSourceFilePath );
			instance.doStartSourceFile(mSourceFilePath);
			System.out.printf("Pre-compile ok\n");			
			instance.doGeneratedOutputPath(mSpecificOutputDir);

			if(mOutputSuccessReportPath != null)
			{
				instance.outputReportToFile(mOutputSuccessReportPath);
			}

			System.out.println("ok");

			return 0;
		}
		catch (IOException e)
		{
			System.err.println("\nFailed read file " + mSourceFilePath + " Reason:");
			System.err.println(e.getMessage());
		} 				
		catch (ParserErrorException e)
		{
			System.err.println("\nParser syntax error in " + mSourceFilePath + ":");
			System.err.println(e.getMessageWithFailedLineColInfo());
		} 

		return -1;
	}
	
	@Override
	public boolean doCmdParse(String[] args) throws CmdLineParserException
	{
		if(args.length>=1)
		{
			mSourceFilePath = new File(args[0]).getAbsolutePath();

			new OptionReader()
			{				
				@Override
				public void onOption(String aOption, String aOptionArgument)
				{
					if(aOption.equals("-or"))
					{
						mOutputSuccessReportPath = aOptionArgument;
					}
					else if(aOption.equals("-od"))
					{
						mSpecificOutputDir = aOptionArgument;
					}
				}
			}.tryToRead(args, 1, new String[]{"-or","-od"});
			
			return true;
		}
		else
		{
			throw new WrongNumberOfParameterException(this);			
		}
	}
}
