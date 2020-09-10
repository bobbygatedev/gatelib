package g.prog;

import g.io.FileHelper;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CppUtilities
{	
	public final static String[] ALL_SOURCE_EXTENSIONS = new String[]{".cpp" , ".c" , ".cxx" , ".hpp" , ".h" , ".hxx" };
	
	private static final Pattern m_PatternComment = Pattern.compile("//|(/\\*)|(\\*/)|\r\n|\n");
	
	public static String remove_Comment(String aFileContent)
	{
		String result = "";
		Matcher matcher = m_PatternComment.matcher(aFileContent);
		int file_pos = 0;
		
		while (matcher.find())
		{
			String separator = matcher.group();
			
			if(separator.equals("//"))
			{
				result += aFileContent.substring(file_pos,matcher.start());
				
				file_pos = matcher.start();
				
				String end_separator = null;
				
				while(matcher.find())
				{
					end_separator = matcher.group();
					
					if(end_separator.matches("\r\n|\n"))
					{
						result += new String(new char[matcher.start()-file_pos]).replace('\0', ' ');
						result += end_separator;
						file_pos = matcher.end();
						break;
					}
				}	
				
				if(end_separator==null)
				{
					//at end!
					result += new String(new char[aFileContent.length()-file_pos]).replace('\0', ' ');	
					return result;
				}
			}
			else if(separator.equals("/*"))
			{
				result += aFileContent.substring(file_pos,matcher.start());
				
				file_pos = matcher.start();
				
				String end_separator = null;
				
				while(matcher.find())
				{
					end_separator = matcher.group();
					
					if(end_separator.matches("\\*/"))
					{
						result += new String(new char[matcher.start()-file_pos+2]).replace('\0', ' ');
						file_pos = matcher.end();
						break;
					}
				}	
				
				if(end_separator==null)
				{
					//at end!
					result += new String(new char[aFileContent.length()-file_pos]).replace('\0', ' ');		
					return result;
				}
				
			}
		}
		
		result += aFileContent.substring(file_pos,aFileContent.length());
		return result;
	}
	
	public static String format_Spaces(String aCppExpression)
	{
		return Pattern.compile("\\s{2,}").matcher(aCppExpression).replaceAll(" ").trim();
	}
	
	public static void main(String[] args) throws Exception 
	{
		String file_path = "c:\\temp\\signals\\g_mthreadThreadTypes.h";
		
		String file_content = FileHelper.read_ToEnd(file_path);
		String result = CppUtilities.remove_Comment(file_content );
		
		System.out.println(result);
	}
}
