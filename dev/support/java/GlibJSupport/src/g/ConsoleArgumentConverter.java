package g;

/**
 * Provide utilities for converting from a console argument.
 * @author Bobby Gate.
 *
 */
public class ConsoleArgumentConverter
{
	//If the string starts with a @(not @@) the argument is a byte sequence
	public static String do_ConvertFromConsole(String aStringFromConsole) 
	{
		String result = "";
		
		for (int i = 0; i < aStringFromConsole.length(); i++ )
		{
			if(i < aStringFromConsole.length()-1)
			{
				String subset = aStringFromConsole.substring(i,i+2);
								
				if (subset.equals("\\\\"))
				{
					result += "\\";
					i++;
				}
				else if (subset.equals("\\n"))
				{
					result += "\n";
					i++;
				}
				else if (subset.equals("\\t"))
				{
					result += "\t";
					i++;
				}				
				else if (subset.equals("\\r"))
				{
					result += "\r";
					i++;
				}				
				else if (subset.equals("\\b"))
				{
					result += "\b";
					i++;
				}				
				else if (subset.equals("\\\""))
				{
					result += "\"";
					i++;
				}		
				else
				{					
					result += aStringFromConsole.substring(i,i+1);					
				}
			}
			else
			{
				result += aStringFromConsole.substring(i,i+1);				
			}
		}
		
		return result;
	}
}
