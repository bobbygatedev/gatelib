	package g.assist.cmds;

import g.MessageBox;
import g.cmd.CmdAbstract;
import g.cmd.CmdLineParserException;
import g.cmd.WrongNumberOfParameterException;

public class CmdMsgBox extends CmdAbstract
{
	private String[] mArguments = null;
	@Override
	public int doExecute()
	{
		MessageBox.cmd_Body(mArguments);		
		return 0;
	}

	@Override
	public boolean doCmdParse(String[] args) throws CmdLineParserException
	{
		if (args.length>0)
		{
			mArguments = args;
			return true;
		}
		else if(args.length>3)
		{
			throw new WrongNumberOfParameterException(this);
		}
		else
		{
			throw new CmdLineParserException("At least 1 argument required after -m!");
		}		
	}

	@Override
	public String getHelpString()
	{
		return 
		"Shows a message box with possibly 3 format:\n" +
		"[-m message]\n"+
		"[-m message title]\n"+
		"[-m message title msg_type] where msg_type can be\n"+
		m_GetMsgBoxValueTypeStr();
	}

	@Override
	public String getCmdOption()	{ return "-m"; }

	private String m_GetMsgBoxValueTypeStr()
	{
		String result = "";
		
		for (MessageBox.Type msg_box_type : MessageBox.Type.values())
		{
			result += "\t" +  msg_box_type.toString() + "\n";
		}
		
		return result.substring(0,result.length());
	}
}
