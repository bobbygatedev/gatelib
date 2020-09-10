package g.text;

import java.io.IOException;
import java.util.regex.Pattern;

/**
 * Utilities library for regular expression.
 * @author Bobby Gate
 *
 */
public class RegexUtils
{
	/**
	 *Could be a space, tab, or new line. 
	 */
	public static final String SPACE_GENERIC = "\\s";
	/**
	 *Strictly a space (ASCII code=32).  
	 */
	public static final String SPACE_ONLY = " ";
	/**
	 * New line \n(0xA) or \r\n(0x0D 0x0A).
	 */
	public static final String NEW_LINE = "(\\r\\n|\\n)";
	/**
	 * New line UNIX \n(0xA).
	 */
	public static final String NEW_LINE_UNIX = "\\n";
	
	/**
	 * New line \n(0xA) or \r\n(0x0D 0x0A).
	 */
	public static final Pattern pattern_NewLine = Pattern.compile(NEW_LINE);

	/**
	 * New line UNIX \n(0xA).
	 */
	public static final Pattern pattern_NewLineUnix = Pattern.compile(NEW_LINE_UNIX);

	/**
	 *Could be a space, tab, or new line. 
	 */
	public static final Pattern pattern_SpaceGeneric = Pattern.compile(SPACE_GENERIC);
	
	/**
	 *Strictly a space (ASCII code=32).  
	 */
	public static final Pattern pattern_SpaceOnly = Pattern.compile(SPACE_ONLY);

	/**
	 * Gets the input regular expression formatted for a group tag i.e (?<tag> regex) .
	 * @param aRegex    a regular expression.
	 * @param aGroupTag tag for the 
	 * @return
	 */
	public static String get_RegexForGroup(String aRegex, String aGroupTag)
	{
		return String.format("(?<%s>(%s))", aGroupTag, aRegex);
	}

	/**
	 * 
	 * @param aRegex input regular expression.
	 * @return
	 */
	public static String get_RegexSurroundedByOptionalSpaces( String aRegex )
	{
		return String.format("\\s*(%s)\\s*", aRegex);
	}

	/**
	 * Gets a regular expression for alternative regex 
	 * i.e regex1,regex2,..regexN --> ((regex1)|(regex2)|...|(regexN))
	 * @param aAlternativeRegex list of alternative regex.
	 * @return
	 */
	public static String get_RegexForAlternatives(String... aAlternativeRegex)
	{
		String[] temp = aAlternativeRegex.clone();
		
		for (int i = 0; i < aAlternativeRegex.length; i++)
		{
			temp[i]=String.format("(%s)", Pattern.quote(aAlternativeRegex[i]));
		}
		
		return "(" + m_GetStringSeparator("|", temp) +")";
	}

	/**
	 * Creates a sequence of regex where optional spaces can be inserted i.e.
	 * regex1,regex2,regex3 --> "//s*regex1//s+regex2//s+regex3//s*"
	 * @param aRegexSequence a sequence of regular expression passed as parameters.
	 * @return
	 */
	public static String get_RegexForSequenceWithAtLeastOneSpace(
			String... aRegexSequence)
	{
		if (aRegexSequence.length > 0)
		{
			String result = "(" + SPACE_GENERIC + "*";

			int i;

			for (i = 0; i < aRegexSequence.length - 1; i++)
			{
				result += aRegexSequence[i] + SPACE_GENERIC + "+";
			}

			return result + aRegexSequence[i] + SPACE_GENERIC + "*)";
		} else
		{
			return "";
		}
	}

	/**
	 * Creates a sequence of regex where optional spaces can be inserted i.e.
	 * regex1,regex2,regex3 --> "//s*regex1//s*regex2//s*regex3//s*"
	 * @param aRegexSequence a sequence of regular expression passed as parameters.
	 * @return
	 */
	public static String get_RegexForSequenceWithOptionalSpaces(
			String... aRegexSequence)
	{
		if (aRegexSequence.length > 0)
		{
			String result = "(" + RegexUtils.SPACE_GENERIC + "*";

			for (String aRegex : aRegexSequence)
			{
				result += aRegex + RegexUtils.SPACE_GENERIC + "*";
			}

			return result + ")";
		} 
		else
		{
			return "";
		}
	}

	/**
	 * Gets the input regular expression formatted for beeing optional (in_regex)?
	 * @param aRegex input regular expression.
	 * @return
	 */
	public static String getRegexOptional(String aRegex)
	{
		return String.format("(%s)?", aRegex);
	}
	
	private static String m_GetStringSeparator ( String aSeparator , String ...aStrings )
	{
		if (aStrings.length==0)
		{
			return "";
		}
		else
		{
			String result = aStrings[0];
			
			for (int i = 1; i < aStrings.length; i++)
			{
				result += ( aSeparator + aStrings[i] );
			}
			
			return result;
		}
	}
	
	public static void main(String[] args) throws IOException
	{
		String[] temp = { "a", "b","c", "d" };
		
		System.out.printf("%s\n", get_RegexForAlternatives(temp));

		System.out.printf("%s\n", get_RegexForGroup("regex", "group"));
		System.out.printf("%s\n", get_RegexForGroup("regex", "group"));
	}
}
