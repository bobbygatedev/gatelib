	package g.prog;
	
	import g.text.RegexUtils;
	
	import java.util.regex.Matcher;
	import java.util.regex.Pattern;
	
	public class ProgRegex extends RegexUtils
	{
		public static final String REGEX_PARAMETER_ATTRIBUTE = "(volatile|const)";
		public static final String REGEX_SIGNED_UNSIGNED = "(signed|unsigned)";
	
		/*
		 * Starts with a letter or '_' then alphanumeric.
		 */
		public static final String REGEX_CPP_NAME = "[_a-zA-Z]\\w*";
	
		/*
		 * Programming variable plus a variable number of '*' optionally spaces.
		 */
		public static final String REGEX_CPP_RETURN_VALUE_WITH_SPACE = 
			"("+REGEX_PARAMETER_ATTRIBUTE + "\\s+)*" + 
			REGEX_CPP_NAME + "(\\s*&)?" + //type part 
			String.format("(\\s+%s)*", REGEX_PARAMETER_ATTRIBUTE)+
			String.format("(\\s*\\*(\\s*%s)*)*", REGEX_PARAMETER_ATTRIBUTE)+
			"\\s+";//
	
		public static final String REGEX_FLOAT = "\\d+(\\.d*)";
	
		public static final String REGEX_DOUBLE = REGEX_FLOAT+"f";
	
		public static final String REGEX_SGN = "(\\+|\\-)";
	
		public static final String REGEX_INT = "?\\d+(L)?";
	
		public static final String REGEX_HEX = "0x([0-9a-f])+";
	
		public static final String REGEX_STRING = "(L)?\"[^\"]*\"";
	
		public static final String REGEX_INIT_VAR_LABEL = get_RegexForAlternatives(
				REGEX_STRING , REGEX_FLOAT,REGEX_INT,REGEX_DOUBLE,REGEX_HEX,REGEX_CPP_NAME);
	
		/*
		 * Foror a C/C++ method/function param vector part i.e
		 * i.e in float matrix[R][C] '[R][C]' represents the vector part
		 */
		public static final String REGEX_CPP_METHOD_PARAM_VECTOR_PART = "(\\s*\\[\\s*\\w*\\s*\\]\\s*)*";
	
		/*
		 * 
		 */
		public static String get_RegexForClass ( String aClassTag , String aNameTag )
		{
			return String.format("((?<%s>\\w+)::)?(?<%s>\\w+)", aClassTag,aNameTag);
		}
	
		/*
		 * 
		 */
		public static String get_RegexForProgrammingVariable(String aVarNameTag)
		{
			return RegexUtils.get_RegexForGroup(REGEX_CPP_NAME, aVarNameTag);
		}
	
		/*
		 * Gets a regular expression for a type, that is a word of at least
		 * one character and one or more '*' expression or a space alternatively its
		 * value should be purged of spaces it cvo
		 */
		public static String get_RegexForTypeAndAtLeastOneSpace(String aGroupTag)
		{
			return RegexUtils.get_RegexForSequenceWithAtLeastOneSpace(
					RegexUtils.get_RegexForGroup( REGEX_CPP_RETURN_VALUE_WITH_SPACE , aGroupTag));
		}
	
		/*
		 * Gets a regular expression string for the vector part of a C/C++ method parameter, i.e.
		 * [] , [2] , [A] , [5][4] , ..
		 * With respect to the standard the argument can't be a complex math expression like [2+3]
		 * but is limited to a number or a #define label
		 * the vector part is declared as optional ()?
		 */
		public static String get_RegexForSimpleOptionalCppMethodVectorPart(String aGroupTag)
		{
			return get_RegexForGroup(REGEX_CPP_METHOD_PARAM_VECTOR_PART , aGroupTag);
		}
	
		/*
		 * Gets a regular expression for parameter(init with vector not valid) initialization expression
		 * i.e  '= 3' '' 
		 */
		public static String get_RegexForParameterInitialisation(String aInitTag)
		{
			return "("  + get_RegexForSequenceWithOptionalSpaces("=", get_RegexForGroup(REGEX_INIT_VAR_LABEL, aInitTag)) + ")?";
		}
	
		public static void main(String[] args)
		{
			String xx = "(\\s*&)?";
			
			Matcher m = Pattern.compile(xx).matcher("  &");
			
			if (m.matches())
			{
				System.out.println("Miao");
			}
			
			
			System.out.println(REGEX_CPP_RETURN_VALUE_WITH_SPACE);
	
			{
				String[] vars = { 
						"int " ,
						"int& " , 
						"const volatile char* " , 
						"consat char* " , 
						"const char const* " ,
						"const char const*const " ,
						"const char const const*const ",
						"const char const const* ",
				"const char& * const const*const " };
	
				for (String var : vars)
				{
					Matcher matcher = Pattern.compile(REGEX_CPP_RETURN_VALUE_WITH_SPACE).matcher(var);
	
					if (matcher.matches())
					{
						System.out.printf("'%s' is valid as c++ return type.\n", var);				
					}
					else 
					{
						System.out.printf("'%s' is NOT valid as c++ return type.\n", var);
					}		
				}
			}
			{
				String[] init_expr = { 
						" 5 " ,
						" 5." , 
						" 5.2" , 
						" 5.3f" , 
						" 5L" , 
						" 0x0123456789abcdef" , 
						" a2" ,//label ,
						"\"pippo\"",
						"L\"pippo\"",
						"\"pi\"ppo\"",
				};
	
				for (String var : init_expr)
				{
					Matcher matcher = Pattern.compile( get_RegexForSequenceWithOptionalSpaces(REGEX_INIT_VAR_LABEL)).matcher(var);
	
					if (matcher.matches())
					{
						System.out.printf("'%s' is valid as c++ return type.\n", var);				
					}
					else 
					{
						System.out.printf("'%s' is NOT valid as c++ return type.\n", var);
					}		
				}
			}
		}
	}
