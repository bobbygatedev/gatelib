package g.prog.parser;

import g.FatalException;
import g.prog.CppMethod.ParameterSet;
import g.prog.CppUtilities;
import g.prog.CppMethod;
import g.prog.CppMethod.Parameter;
import g.prog.ProgRegex;
import g.text.RegexUtils;
import g.text.parser.IParser;
import g.text.parser.IParserTypized;
import g.text.parser.ParserMultipleOptionTypizedAbstract;
import g.text.parser.ParserRegex;
import g.text.parser.ParserSequenceSeparatorAbstract;
import g.text.parser.ParserSequenceTypizedAbstract;
import g.text.parser.ParserTypizedAbstract;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * WARNING able to parse just SIMPLE METHOD DECLARATION LIKE
 * MyCls(int a , int b = 2 )
 * int MyMethod ( int , int );
 * @author Bobby Gate
 *
 */
public class ParserCppMethodDeclarationEuristic extends ParserMultipleOptionTypizedAbstract<CppMethod>
{
	@SuppressWarnings("unchecked")
	private IParserTypized<CppMethod>[] mParserOptionSetTypized = new IParserTypized[]{
		new ParserConstructorDeclaration (),
		new ParserMethodDeclaration (),
	};

	@Override
	public IParserTypized<CppMethod>[] getParserOptionSetTypized()	{ return mParserOptionSetTypized; }
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
			throws ParserErrorException
	{
		String parser_expression_without_comments = CppUtilities.remove_Comment(aPossiblyParseEpression);

		return super.doTry(parser_expression_without_comments, aStartIndex);
	}

	public abstract static class ParserMethodAbstract extends ParserSequenceTypizedAbstract<CppMethod>
	{
		protected ParserTypeNameTemplate mParserReturnType = new ParserTypeNameTemplate();
		protected ParserCppName mParserClsName = new ParserCppName();
		protected ParserCppName mParserMethodName = new ParserCppName();
		protected ParserParameterSet mParserParameterList = new ParserParameterSet();		
		protected CppMethod mResult = null;

		@Override
		public CppMethod getResultTypized() { return mResult; }

		@Override
		public int doTry(String aPossiblyParseEpression, int aStartIndex)
		throws ParserErrorException
		{
			int index = super.doTry(aPossiblyParseEpression, aStartIndex);

			if (index != -1)
			{
				mResult = new CppMethod();

				mResult.setReturnType(mParserReturnType.getResultTypized());
				mResult.setClsName(mParserClsName.getResultTypized());
				mResult.setName(mParserMethodName.getResultTypized());
				
				ParameterSet parameter_set;
				
				try
				{
					parameter_set = new ParameterSet(mParserParameterList.getResultTypized());
				}
				catch (NotAValidParamIntialisation e)
				{
					throw new ParserErrorException(e.getMessage(), aPossiblyParseEpression, index);
				}				
				
				mResult.setParameterSet (parameter_set );

				return index;					
			}
			else
			{
				return -1;
			}
		}		
	}

	public static class ParserConstructorDeclaration extends ParserMethodAbstract
	{		
		@Override
		public IParser[] getParserSequence()
		{
			return new IParser[]{mParserMethodName , mParserParameterList}; 
		}
	}

	public static class ParserMethodDeclaration extends ParserMethodAbstract
	{
		@Override
		public IParser[] getParserSequence()
		{
			return new IParser[]{mParserReturnType , mParserMethodName , mParserParameterList}; 
		}
	}

	public static class ParserParameterSet extends ParserSequenceSeparatorAbstract<Parameter>
	{
		private ParameterSet mParameterSet = new ParameterSet();
		
		public ParserParameterSet(){}
		
		@Override
		public String getInitString(){ return "("; }
		@Override
		public String getSeparator(){ return ","; }
		@Override
		public String getEndString(){ return ")"; }
		@Override
		public boolean dontCareSpace(){return true;}
		
		@Override
		public IParserTypized<Parameter> getItemParser() { return new ParserParameter(mParameterSet); }
	}

	/*
	 * Parsing terminate after a ')' or ',' is found with parenthesis and graph count == 0
	 * index is set 
	 */
	public static class ParserInitPartEurhistic extends ParserTypizedAbstract<String>
	{
		private String mResult = null;
		private ParserRegex mParserEqual = new ParserRegex("\\s*=\\s*");
		private Pattern mPatternSeparators = Pattern.compile(String.format(
				"\\s*%s\\s*", RegexUtils.get_RegexForAlternatives(",","(",")","[","]")));

		private enum InsideOf
		{
			none , graph , simple
		}

		@Override
		public int doTry(String aPossiblyParseEpression, int aStartIndex)
		throws ParserErrorException
		{
			int index = aStartIndex;
			int par_graph_num = 0;
			int par_simple_num = 0;
			InsideOf inside_of = InsideOf.none;

			try
			{
				index = mParserEqual.doTryWithException(aPossiblyParseEpression, index);
			} 
			catch (ParserNotMatchingException e)
			{
				mResult = null;
				return index;
			}

			Matcher matcher  = mPatternSeparators.matcher(aPossiblyParseEpression);

			while (true)
			{
				if (matcher.find(index))
				{
					int match_start=matcher.start();

					String symbol = aPossiblyParseEpression.substring(match_start, index = matcher.end()).trim();

					if (symbol.equals("("))
					{
						par_simple_num++;

						if (inside_of==InsideOf.none)
						{
							inside_of = InsideOf.simple;
						}
					}
					else if ( symbol.equals(",") )
					{
						if (par_graph_num>0 && par_simple_num == 0)
						{
							//ie.e int a[2,]    --> #[=1 #(=0 error
							//int a[ pow(3,2) ] --> #[=1 #(=1 correct
							throw new ParserErrorException("',' may not appear inside '[expr]'", aPossiblyParseEpression , index);
						}
						else if (par_graph_num == 0 && par_simple_num==0)
						{
							mResult = mBuildResult ( aPossiblyParseEpression, aStartIndex , match_start );							
							return match_start;							
						}
					}
					else if (symbol.equals(")"))
					{
						if (--par_simple_num < 0)
						{
							if (par_graph_num == 0)
							{
								mResult = mBuildResult ( aPossiblyParseEpression, aStartIndex , match_start );
								return match_start;							
							}
							else
							{
								throw new ParserErrorException("')' closing inside '[]'!", aPossiblyParseEpression , index);
							}
						}
						else if ( inside_of == InsideOf.simple && par_graph_num >0 )
						{
							throw new ParserErrorException("')' closing before closing '['(i.e. '([ )]' )!", aPossiblyParseEpression , index);							
						}
					}
					else if (symbol.equals("["))
					{
						par_graph_num++;

						if (inside_of==InsideOf.none)
						{
							inside_of = InsideOf.graph;
						}
					}
					else if (symbol.equals("]"))
					{
						if (--par_graph_num < 0)
						{
							throw new ParserErrorException("Too many']'!", aPossiblyParseEpression , index);
						}
						else if (inside_of == InsideOf.graph && par_simple_num >0 )
						{
							throw new ParserErrorException("']' closing before closing '('(i.e. '[( ])' )!", aPossiblyParseEpression , index);							
						}
					}						
				}
				else
				{
					if ( par_graph_num > 0 )
					{
						throw new ParserErrorException("Not enough ']'", aPossiblyParseEpression , index);						
					}
					else if (par_simple_num >=0)
					{
						throw new ParserErrorException("Not enough ')' or untermined parameter!", aPossiblyParseEpression , index);						
					}
					else
					{					
						new FatalException("Unexpected here!").doStandardAction();
					}
				}
			}
		}

		private String mBuildResult(String aPossiblyParseEpression, int aStartIndex, int aMatchStart) throws ParserErrorException
		{
			String result = CppUtilities.format_Spaces(aPossiblyParseEpression.substring(aStartIndex,aMatchStart));
			
			result = result.substring(1).trim();//remove the =
			
			if (result.length()==0)
			{
				throw new ParserErrorException("Init can't be void!", aPossiblyParseEpression , aMatchStart);
			}
			else
			{
				return result;
			}			
		}

		@Override
		public String getResultTypized(){ return mResult;}
	}

	public static class ParserVectorPartEurhistic extends ParserTypizedAbstract<String>
	{		  
		//search starts if this expression matches
		private ParserRegex mParserVectorPartInit = new ParserRegex("\\s*\\[\\s*");
		private Pattern mPatternSeparators = Pattern.compile(String.format(
				"\\s*%s\\s*", RegexUtils.get_RegexForAlternatives("(",")","[","]")));		
		private String mResult = null;

		@Override
		public int doTry(String aPossiblyParseEpression, int aStartIndex)
		throws ParserErrorException
		{						
			int index = aStartIndex;					
			int par_graph_num = 0;
			int par_simple_num = 0;

			try
			{
				index = mParserVectorPartInit.doTryWithException(aPossiblyParseEpression, index);
			} 
			catch (ParserNotMatchingException e)
			{
				//since it is inserted in a sequence 
				mResult = null;
				return index;
			}
			
			Matcher matcher  = mPatternSeparators.matcher(aPossiblyParseEpression);
			par_graph_num = 1;

			while (true)
			{
				if (matcher.find(index))
				{
					int match_start=matcher.start();

					String symbol = aPossiblyParseEpression.substring(match_start, index = matcher.end()).trim();

					if (symbol.equals("("))
					{
						if (par_graph_num > 0)
						{
							par_simple_num++;
						}
						else 
						{
							throw new ParserErrorException("'(' may appear just inside []!",aPossiblyParseEpression, index);
						}
					}
					else if (symbol.equals(")"))
					{
						if (--par_simple_num < 0)
						{
							throw new ParserErrorException("'(' may appear just inside []!",aPossiblyParseEpression, index);
						}
					}
					else if (symbol.equals("["))
					{
						par_graph_num++;
					}
					else if (symbol.equals("]"))
					{
						if (--par_graph_num<0)
						{
							throw new ParserErrorException("Too many ']'!", aPossiblyParseEpression , index);
						}
						else if (par_simple_num>0)
						{
							throw new ParserErrorException("'(expr)' may appear just inside []!", aPossiblyParseEpression , index);
						}
						else if( par_graph_num ==0  )
						{
							//next signum MUST BE '[' otherwise the expression is finished
							try
							{
								index = mParserVectorPartInit.doTryWithException(aPossiblyParseEpression, index);
								par_graph_num=1;								
							} 
							catch (ParserNotMatchingException e)
							{
								//since it is inserted in a sequence 
								mResult = CppUtilities.format_Spaces(aPossiblyParseEpression.substring(aStartIndex,index));
								return index;
							}							
						}
					}						
				}
				else
				{
					if ( par_graph_num > 0 )
					{
						throw new ParserErrorException("Not enough ']'", aPossiblyParseEpression , index);						
					}
					else if (par_simple_num >0)
					{
						throw new ParserErrorException("Not enough ')'", aPossiblyParseEpression , index);						
					}
					else
					{
						new FatalException("Not expected heer!").doStandardAction();
					}
				}
			}
		}

		@Override
		public String getResultTypized() { return mResult; }

	}

	public static class ParserParameter extends ParserSequenceTypizedAbstract<Parameter>
	{
		private Parameter mResult = null;
		private ParameterSet mParameterSet = null;
		
		public ParserParameter(ParameterSet aParameterSet)
		{
			if(aParameterSet!=null)
			{
				mParameterSet = aParameterSet;
			}
			else
			{
				new FatalException("Null ParameterSet instance not allowed!");
			}			
		}

		private ParserRegex mParserTypeNamePreamble = new ParserRegex(
				String.format("\\s*((%s)|(%s))*\\s*", 
						ProgRegex.REGEX_PARAMETER_ATTRIBUTE , 
						ProgRegex.REGEX_SIGNED_UNSIGNED));

		private ParserTypeNameTemplate mParserTypeName = new ParserTypeNameTemplate();

		private ParserRegex mParserPointerPart = new ParserRegex(
				String.format("\\s*((%s)*(\\*)(%s)*)*&?\\s*", 
						ProgRegex.REGEX_PARAMETER_ATTRIBUTE,
						ProgRegex.REGEX_PARAMETER_ATTRIBUTE));

		private ParserRegex mParseParamName = new ParserRegex(String.format("(%s)?", ProgRegex.REGEX_CPP_NAME) );
		private ParserVectorPartEurhistic mParserVectorPartEurhistic = new ParserVectorPartEurhistic();
		private ParserInitPartEurhistic mParserInitPartEurhistic = new ParserInitPartEurhistic();

		@Override
		public int doTry(String aPossiblyParseEpression, int aStartIndex) throws ParserErrorException
		{
			int index = aStartIndex;

			index = super.doTry(aPossiblyParseEpression, index);

			if (index !=-1)
			{
				String type = CppUtilities.format_Spaces(
						mParserTypeNamePreamble.getResultTypized() + " " + 
						mParserTypeName.getResultTypized()) + " " +
						mParserPointerPart.getResultTypized();

				String name = mParseParamName.getResultTypized().trim();
				String vector_part = mParserVectorPartEurhistic.getResultTypized();
				String init_part = mParserInitPartEurhistic.getResultTypized();

				try
				{
					mResult= mParameterSet.doAddParameter(type, (name.length()>=0)?name:null, vector_part, init_part);
				} 
				catch (NotAValidParamIntialisation e)
				{
					throw new ParserErrorException(e.getMessage(), aPossiblyParseEpression , index);
				}

				return index;
			}
			else
			{
				return -1;
			}
		}

		@Override
		public Parameter getResultTypized(){ return mResult; }

		@Override
		public IParser[] getParserSequence()
		{
			return new IParser[] { 
					mParserTypeNamePreamble , 
					mParserTypeName ,   
					mParserPointerPart ,
					mParseParamName ,
					mParserVectorPartEurhistic ,
					mParserInitPartEurhistic };			              
		}
	}

	public static void  main(String[] args)
	{
		ParserConstructorDeclaration parser = new ParserConstructorDeclaration();

		String test_expr = 
			"MyCls(int , int b)";
			//"SemaphoreNamed ( const char*, NamedSyncObjectCreationOptions_t aCreationOptions = g_create_always , GInt32_t initial_count = 0 );";

		try
		{
			parser.doTryWithException(test_expr, 0);
			System.out.printf("Translation of %s ok!:\n'%s'" ,test_expr,parser.getResultTypized().getBody());			
		} 
		catch (ParserNotMatchingException e)
		{
			System.out.printf("Translation of %s failed!\n:%s" ,test_expr);			
		}
		catch (ParserErrorException e)
		{
			System.out.printf("Translation of %s failed raeson:\n'%s'." ,test_expr,e.getMessage());			
		}
	}
}
