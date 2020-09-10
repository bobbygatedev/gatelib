package g;

import java.awt.Component;
import javax.swing.JOptionPane;

public class MessageBox 
{	
	enum Result
	{
		is_abort(0),//= IDABORT , //Abort button was selected.
		is_cancel(1) ,//= IDCANCEL , // Cancel button was selected.
		is_continue(2) ,//= IDCONTINUE , // Continue button was selected.
		is_ignore(3) ,//= IDIGNORE ,  // Ignore button was selected.
		is_no(4) ,//= IDNO  , // No button was selected.
		is_ok(5) ,//= IDOK  , // OK button was selected.
		is_retry(6) ,//= IDRETRY  , // Retry button was selected.
		is_try_again(7) ,//= IDTRYAGAIN  , // Try Again button was selected.
		is_yes(8) ,//= IDYES  // Yes button was selected.
		is_help(9);//= IDHELP  // Help button was selected.
		
		Result (int aId)
		{
			id = aId;			
		}
		
		public final int id;

		public static Result from_Int(int int_result) throws Exception 
		{
			for (Result item : values()) 
			{
				if (item.id==int_result) 
				{
					return item;
				}
			}

			throw new Exception("Value not found!");
		}
	};
	
	public enum Type
	{			
		ok (0),
		ok_cancel (1),
		retry_cancel (2),
		cancel_retry_continue(3) ,
		yes_no (4),
		help (5),
		yes_no_cancel (6),
		abort_retry_continue(7);
		
		public final int id;

		Type (int aId)
		{
			id = aId;			
		}
		
		public static Type from_Int(int int_result) throws Exception 
		{
			for (Type item : values()) 
			{
				if (item.id==int_result) 
				{
					return item;
				}
			}

			throw new Exception("Value not found!");
		}
			
		static Type Parse(String aString) throws TypeParseException
		{
			try 
			{
				int type_int = Integer.parseInt(aString);
				
				try 
				{
					return from_Int(type_int);
				} 
				catch (Exception e) {}				
			} 
			catch (NumberFormatException e) 
			{
				try 
				{
					return Type.valueOf(aString);
				} 
				catch (IllegalArgumentException e1){} 
			}			
			
			throw new TypeParseException(aString + " is not a valid MessageBox.Result value");
		}
		
		public static class TypeParseException extends Exception
		{
			/**
			 * 
			 */
			private static final long serialVersionUID = -7636032409542160261L;

			public TypeParseException(String aMsg) 
			{
				super(aMsg);
			}			
		}
	}
		
	public static Result show ( Component aParent , String aMsg , String aTitle , Type aType ) throws Exception
	{
		String[] options = null;
		
		switch (aType) {
		case ok:
			options = new String[]{"Ok"};
			break;
		case ok_cancel:
			options = new String[]{"Ok","Cancel"};
			break;
		case retry_cancel:
			options = new String[]{"Retry","Cancel"};
			break;
		case cancel_retry_continue:
			options = new String[]{"Cancel","Retry","Continue"};
			break;
		case yes_no:
			options = new String[]{"Yes","No"};
			break;
		case help:
			options = new String[]{"Help"};
			break;
		case yes_no_cancel:
			options = new String[]{"Yes","No","Cancel"};
			break;
		case abort_retry_continue:
			options = new String[]{"Abort","Retry","Continue"};
			break;
		default:
			throw new Exception("Unexpected error: enum label missing!");
		}
		
		int int_result = JOptionPane.showOptionDialog(
				aParent, 
				ConsoleArgumentConverter.do_ConvertFromConsole(aMsg) , 
				ConsoleArgumentConverter.do_ConvertFromConsole(aTitle), 
				0, 
				JOptionPane.QUESTION_MESSAGE, 
				null , 
				options, 
				options[0]);
		
		String string_result = options[int_result];
		String string_for_enum = "is_" + string_result.toLowerCase();

		Result result = Result.valueOf(string_for_enum);
		
		System.out.println("Result is " + string_result);
		
		try 
		{
			return result;
		} 
		catch (IllegalArgumentException e) 
		{
			throw new Exception("Unexpected unrecognized enum!",e);
		}
	}
	
	public static void cmd_Body(String[] args)
	{
		try
		{
			String msg="";
			String title=""; 
			g.MessageBox.Type msg_box_type = Type.ok;
			g.MessageBox.Result result = null;		
						
			switch (args.length) 
			{
			case 1:		
				msg = ConsoleArgumentConverter.do_ConvertFromConsole(args[0]);				
				break;
			case 2:
				msg = ConsoleArgumentConverter.do_ConvertFromConsole(args[0]);
				title = ConsoleArgumentConverter.do_ConvertFromConsole(args[1]);
				break;
			case 3:
				msg = ConsoleArgumentConverter.do_ConvertFromConsole(args[0]);
				title = ConsoleArgumentConverter.do_ConvertFromConsole(args[1]);
				msg_box_type = g.MessageBox.Type.Parse(args[2]);
				break;
			default:
				System.out.println("msg_box() Nothing to do");
				System.exit(-1);
				break;
			}		
			
			result = show(null, msg, title,msg_box_type);

			System.out.printf("Read value %s.\n",result.toString());
			
			System.exit(result.id);
			
		}
		catch (Type.TypeParseException e) 
		{
			System.out.printf("Failed to translate reason.\n%s", e.getMessage());
			System.exit(-1);
		}
		catch (Exception e) 
		{
			System.out.printf("Unknown exception.\n%s", e.getMessage());
			System.out.println("Stack:");
			
			for (StackTraceElement stack_trace_element : e.getStackTrace()) 
			{
				System.out.println(stack_trace_element.toString());
			}
						
			System.exit(-1);
		}				
	}
	
	public static void main(String[] args) throws Exception 
	{
		cmd_Body(args);
	}
}
