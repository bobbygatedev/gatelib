package g.assist.cmds;

import g.assist.signal.ParserSignalInterface;
import g.cmd.CmdAbstract;
import g.cmd.CmdLineParserException;
import g.cmd.WrongNumberOfParameterException;
import g.text.parser.ParserAbstract.ParserNotMatchingException;
import g.text.parser.ParserAbstract.ParserErrorException;

public class CmdSignalBodyToClipboard extends CmdAbstract
{				
	private String mSignalInterface=null;

	@Override
	public String getCmdOption(){ return "-sc"; }

	@Override
	public String getHelpString()
	{
		return 
			"Parse a signal expression like:\n" +
			"sig_type(partype1 par1,partype1 par2) into the clipboard i.e." +
			"java -jar GLibJSupport.jar -sc \"MySig(int par1,int par2)\"";
	}

	@Override
	public int doExecute()
	{
		ParserSignalInterface parser = new ParserSignalInterface();

		try
		{
			parser.doTryWithException(mSignalInterface, 0);
			parser.getResultTypized().doCopyBodyToClipboard();			
			
			if(verbosityLevel>=1)
			{
				System.out.printf(
						"Signal interface '%s' correctly parsed:\n\n%s",
						mSignalInterface , parser.getResultTypized().buildItemBody());
			}
			
			return 0;
		} 
		catch (ParserNotMatchingException e)
		{
			System.err.printf("'%s' not matching a signal interface!",mSignalInterface);
			return -1;
		}
		catch (ParserErrorException e)
		{
			System.err.printf(
					"Parsing of signal interface '%s' raised an error:\n%s" , 
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
		case 1:
			mSignalInterface = args[0];
			return true;
		default:
			throw new WrongNumberOfParameterException(this);
		}
	}
}
