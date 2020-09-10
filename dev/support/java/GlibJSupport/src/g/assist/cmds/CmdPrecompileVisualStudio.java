package g.assist.cmds;

import java.io.File;
import java.io.IOException;

import org.xml.sax.SAXException;

import g.FatalException;
import g.cmd.CmdAbstract;
import g.cmd.CmdLineParserException;
import g.cmd.WrongNumberOfParameterException;

/**
 * Implement precompiler for VISUAL STUDIO(Tested on 2005)<br>
 * G_ASSIST -pc {-vX} [.vcproj] {-o [success_report_path_output_dir]}<br>" +
 * Pre-compile a MS Visual Studio 2005 project ([.vcproj]) by calling parsing for each .cpp,.c,.h,.cxx,.hxx,.hpp file!<br>" +
 * If -o is specified [success_report_path_output_dir] contains the output report path which is [source_name.ext.txt].<br>"+
 * If -o is not specified success report path is generated inside .vcproj path";
 * @author Bobby Gate
 */
public class CmdPrecompileVisualStudio extends CmdAbstract
{
	private String mSuccessReportOutputPath=null;
	private String mVcprojPath=null;
		
	@Override
	public String getCmdOption() { return "-pc"; }

	@Override
	public String getHelpString()
	{
		return  "G_ASSIST -pc {-vX} [.vcproj] {-o [success_report_path_output_dir]}\n" +
		"Pre-compile a MS Visual Studio 2005 project ([.vcproj]) by calling parsing for each .cpp,.c,.h,.cxx,.hxx,.hpp file!\n" +
		"If -o is specified [success_report_path_output_dir] contains the output report path which is [source_name.ext.txt].\n"+
		"If -o is not specified success report path is generated inside .vcproj path";
	}

	@Override
	public int doExecute()
	{
		try
		{
			String success_report_dir = (mSuccessReportOutputPath!=null)?
					new File( mSuccessReportOutputPath).getCanonicalPath() :new File(mVcprojPath).getParent();
			String[] source_file_path_array = g.prog.visualstudio.MsVisualCppTools.get_SourceFilesFromPrj(mVcprojPath);
					
			for (String source_file_path : source_file_path_array)
			{
				File source_file = new File(source_file_path);			
				//= report dir/SourceName.txt (i.e. \Debug\source.cpp.txt) 
				File destination_file = new File(success_report_dir + File.separator + source_file.getName() + ".txt");
				
				if (!destination_file.exists() || source_file.lastModified() > destination_file.lastModified() )
				{
					CmdParseSourceCode.direct_DoExecute ( verbosityLevel , source_file_path , destination_file.getCanonicalPath() , null );				
				}
				else
				{
					System.out.printf("%s is up-to-date!\n",source_file.getCanonicalPath());
				}
			}
			
			return 0;
		} 
		catch (IOException e)
		{
			System.err.println("\nCan't read file " + mVcprojPath + " reason:");
			System.err.println(e.getMessage());
			return -1;
		} 
		catch (SAXException e)
		{
			System.err.println("\nError in project format!");
			return -1;			
		}		
	}

	@Override
	public boolean doCmdParse(String[] args) throws CmdLineParserException
	{
		if(args.length>=1)
		{
			mVcprojPath = new File(args[0]).getAbsolutePath();

			new OptionReader()
			{				
				@Override
				public void onOption(String aOption, String aOptionArgument)
				{
					if(aOption.equals("-o"))
					{
						mSuccessReportOutputPath = aOptionArgument;
					}
					else
					{
						new FatalException("Unexpected to get here!");
					}
				}
			}.tryToRead(args, 1, new String[]{"-o"});
			
			return true;
		}
		else
		{
			throw new WrongNumberOfParameterException(this);			
		}
	}
}
