package g.text;

import java.util.regex.Matcher;

/**
 * Encapsulates position inside a text.
 * @author Bobby Gate
 *
 */
public class TextLineColumn
{
	private int mLine = -1; 
	private int mColumn = -1; 
	private String mRefText;

	/**
	 * 
	 * @param aRefText
	 */
	public TextLineColumn(String aRefText)
	{
		mRefText = aRefText;	
	}
	
	/**
	 * 
	 * @param aRefText
	 * @param aLine
	 * @param aColumn
	 */
	public TextLineColumn(String aRefText,int aLine, int aColumn)
	{ 
		mRefText = aRefText;
		mLine = aLine;
		mColumn = aColumn;
	}

	/**
	 * 
	 * @return
	 */
	public String getRefText(){ return mRefText; }
	
	/**
	 * Gets the text line (counted from 1 ).
	 * @return
	 */
	public int getLine() { return mLine;}
	/**
	 * Gets the text column (counted from 0).
	 * @return
	 */
	public int getColumn() { return mColumn;}

	/**
	 * Returns the content formatted as (ln,col) i.e. (1,5)
	 */
	@Override	
	public String toString() { return String.format("(%d,%d)", mLine,mColumn); }


	/**
	 * Returns the content of the line or null if it is out of the text.
	 * @return
	 */
	public String getLineText ( )
	{
		Matcher matcher = RegexUtils.pattern_NewLine.matcher(mRefText);		
		int line = 1;
		int line_start = 0;

		while(true)
		{
			if(line++==mLine)
			{
				if (matcher.find())
				{
					return mRefText.substring(line_start,matcher.start());
				}
				else
				{
					return mRefText.substring(line_start,mRefText.length());
				}
			}
			else
			{
				if (matcher.find())
				{
					line_start = matcher.end();
				}
				else
				{
					return null;
				}
			}
		}
	}

	/**
	 * Accept a text index (offset from init of the text) and returns the 
	 * line-column pair
	 * @param aIndex
	 * @return
	 */
	public static TextLineColumn convert_FromIndex(String aInputText , int aIndex)
	{
		Matcher matcher = RegexUtils.pattern_NewLine.matcher(aInputText);		
		int line = 1;
		int line_start=0;

		while(matcher.find())
		{
			int new_line_start = matcher.end();

			if(new_line_start<=aIndex)
			{
				line++;
				line_start = new_line_start;
			}
			else
			{
				break;
			}
		}

		return new TextLineColumn(aInputText , line, aIndex-line_start);
	}

	public static void main(String[] args)
	{
		String text = "123\n56789\r\n1213";

		System.out.println(text);

		for (int i = 0; i < text.length(); i++)
		{
			System.out.printf("Index %-2d %c: %s\n" , i , text.charAt(i) , TextLineColumn.convert_FromIndex(text, i).toString() );
		}		

		for (int i = 0; i < 3; i++)
		{
			TextLineColumn line_col = new TextLineColumn(text, i+1,0);

			System.out.printf("Line %d:%s\n" ,i+1, line_col.getLineText());
		}
	}
}
