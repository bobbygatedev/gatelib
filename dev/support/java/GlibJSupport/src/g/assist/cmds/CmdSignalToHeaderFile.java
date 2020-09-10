package g.assist.cmds;

import java.io.IOException;

import g.assist.signal.ParserSignalInterface;
import g.cmd.CmdAbstract;
import g.cmd.CmdLineParserException;
import g.cmd.WrongNumberOfParameterException;
import g.text.parser.ParserAbstract.ParserNotMatchingException;
import g.text.parser.ParserAbstract.ParserErrorException;

public class CmdSignalToHeaderFile extends CmdAbstract
{
	private String mSignalInterface=null;
	private String mDestinationPath=null;
	
	@Override
	public String getCmdOption(){ return "-sh"; }

	@Override
	public String getHelpString()
	{
		return 
			"-sh sig_type(partype1 par1,partype1 par2) 'sig_dir' parse a signal into sig_dir i.e.:\n" +
			"java -jar GLibJSupport.jar -sh \"MySig(int par1,int par2)\" \"c:\\temp\"\n"+
			"will generate a MySig.h with the result.";
	}

	@Override
	public int doExecute()
	{
		ParserSignalInterface parser = new ParserSignalInterface();

		try
		{
			parser.doTryWithException(mSignalInterface, 0);
			
			if(verbosityLevel>=1)
			{
				System.out.printf(
						"Signal interface '%s' correctly parsed writing onto %s:\n\n%s..",
						mSignalInterface , mDestinationPath , parser.getResultTypized().buildItemBody());
				
			}
			
			parser.getResultTypized().obtainStreamForOutputFile(mDestinationPath,null);
			parser.getResultTypized().doSaveBody();
			System.out.println("ok!");
			
			return 0;
		} 
		catch (IOException e)
		{
			System.err.println(" writing output failed!");
			return -1;
		}
		catch (ParserNotMatchingException e)
		{
			System.err.printf("Signal interface '%s' not matching.\n",mSignalInterface);
			return -1;
		}
		catch (ParserErrorException e)
		{
			System.err.printf(
					"Signal interface '%s' parsing raised an error:\n%s" ,
					mSignalInterface ,
					e.getMessageWithFailedIndexInfo());
			return -1;
		}
	}

	@Override
	public boolean doCmdParse(String[] args) throws CmdLineParserException
	{
		switch (args.length)
		{
		case 2:
			mSignalInterface = args[0];
			mDestinationPath = args[1];
			return true;
		default:
			throw new WrongNumberOfParameterException(this);
		}
	}
}
