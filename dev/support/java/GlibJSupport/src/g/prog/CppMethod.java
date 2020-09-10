package g.prog;

import g.prog.parser.NotAValidParamIntialisation;
import g.prog.parser.ParserCppMethodDeclarationEuristic;
import g.text.parser.ParserAbstract.ParserNotMatchingException;
import g.text.parser.ParserAbstract.ParserErrorException;

import java.util.ArrayList;

/**
 * Encapsulates a C/C++ method or function. 
 * @author Bobby Gate
 *
 */
public class CppMethod
{
	private String mName;
	private String mReturnType = null;
	private String mClsName = null;
	private ParameterSet mParameterSet = new ParameterSet(); 
	
	/**
	 * Default constructor.
	 */
	public CppMethod() {}
	
	/**
	 * Encapsulation for parameter set. 
	 * @author Bobby Gate
	 *
	 */
	public static class ParameterSet
	{
		private ArrayList<Parameter> mListParameters = new ArrayList<Parameter>();
		
		/**
		 * Default constructor.
		 */
		public ParameterSet(){}
		
		/**
		 * Constructor with parameter array input.
		 * @param aParameterArray Array of parameters or null if is list is empty.
		 * @throws NotAValidParamIntialisation
		 */
		public ParameterSet(Parameter[] aParameterArray) throws NotAValidParamIntialisation
		{
			if(aParameterArray!=null)
			{
				for (Parameter parameter : aParameterArray)
				{
					doAddParameter(parameter);
				}
			}
		}
		
		
		public Parameter get(int index){ return mListParameters.get(index);}
		public int size() { return mListParameters.size(); }
		public void clear ( ) { mListParameters.clear(); }
		
		/**
		 * Adds a parameter.
		 * @param aType
		 * @param aName
		 * @param aVectorPart
		 * @param aInitPart
		 * @return
		 * @throws NotAValidParamIntialisation
		 */
		public Parameter doAddParameter(
				String aType, String aName,String aVectorPart, String aInitPart)
		throws NotAValidParamIntialisation
		{
			Parameter result = new Parameter(aType, aName, aVectorPart, aInitPart);
			mListParameters.add(result);

			mCheckInitValidity();

			return result;
		}		

		/**
		 * Adds a parameter.
		 * @param aParameter
		 * @throws NotAValidParamIntialisation 
		 */
		public void doAddParameter(Parameter aParameter) throws NotAValidParamIntialisation
		{
			mListParameters.add(aParameter);

			mCheckInitValidity();			
		}
		
		/**
		 * Removes a parameter.
		 * @param aParameter Parameter to be removed.
		 * @return true if the paramter is effectively removed.
		 * @throws NotAValidParamIntialisation 
		 */
		public boolean removeParameter ( Parameter aParameter ) throws NotAValidParamIntialisation 
		{
			boolean result = mListParameters.remove(aParameter);
			
			mCheckInitValidity();
			
			return result;
		}
		
		/**
		 * Set parameters without a name with a default value = par# where # is the param position i.e.:
		 * void method ( int , int b , int )--> void method ( int p0, int b , int p2) 
		 */
		public void fillParamTypesWithDefault()
		{
			if (mListParameters!=null)
			{
				for (int i = 0; i < mListParameters.size(); i++)
				{
					if (mListParameters.get(i).getName()==null)
					{
						mListParameters.get(i).setName(String.format("par%d", i));
					}
				}
			}
		}

		/**
		 * Gets the body of the parameters block i.e
		 * (int a, int , float c = 2)
		 * @return
		 */
		public String getBody()
		{
			String result = "(";

			int i;
			
			for (i = 0; i < mListParameters.size() - 1; i++)
			{
				result += mListParameters.get(i).getParamBody() + ",";
			}

			if (i < mListParameters.size())
			{
				result += mListParameters.get(i).getParamBody();
			}

			result += ")";
			return result;
		}
		
		/**
		 * Returns an array of parameters.
		 * @return
		 */
		public Parameter[] toArray()
		{
			return (Parameter[]) mListParameters.toArray(new Parameter[mListParameters.size()]);
		}		

		/**
		 * Check the initialization affects just the last variables i.e.:
		 * 'int f(int a=2,int b )' is an error
		 */
		private void mCheckInitValidity() throws NotAValidParamIntialisation
		{
			boolean has_param = false;

			for (Parameter param : mListParameters)
			{
				if (has_param)
				{
					if (param.getInitPart() == null)
					{
						throw new NotAValidParamIntialisation();
					}
				} else
				{
					has_param = (param.getInitPart() != null);
				}
			}
		}
	}
	
	
	/**
	 * Encapsulation for method parameter.
	 * @author Bobby Gate
	 *
	 */
	public static final class Parameter
	{
		private String mType = null;
		private String mName1 = null;
		private String mVectorPart = null;
		private String mInitPart = null;

		/**
		 * Creates a parameter(can used just inside)
		 */
		public Parameter(String aType, String aName, String aVectorPart,
				String aInitPart)
		{
			mType = aType;
			mName1 = m_GetValue(aName);
			mVectorPart = m_GetValue(aVectorPart);
			mInitPart = m_GetValue(aInitPart);
		}

		public String getInitPart()	{ return mInitPart;	}
		public String getType()	{ return mType;	}
		public String getName()	{ return mName1; }
		public String getVectorPart() {	return mVectorPart;	}

		/**
		 * Returns the body of the param i.e.
		 * int a[] = 3
		 */
		public String getParamBody()
		{
			String name = (mName1 != null) ? mName1 : "";
			String vector_part = (mVectorPart != null) ? mVectorPart : "";
			String init_part = (mInitPart != null) ? " = " + mInitPart : "";

			return CppUtilities.format_Spaces(String.format("%s %s%s%s", mType, name, vector_part,init_part));
		}

		public void setName(String aName)
		{
			mName1 = m_GetValue(aName);
		}
	}

	/**
	 * Gets the set of the parameters.
	 * @return
	 */
	public ParameterSet getParameterSet() { return mParameterSet; }
	
	/**
	 * Set the class name, null if aClsName.trim()==""
	 * @param aClsName
	 */
	public void setClsName(String aClsName)	{ mClsName = m_GetValue(aClsName); }

	public String getClsName()	{ return mClsName;	}

	public String getName() { return mName;	}

	public void setName(String aName) { mName = m_GetValue(aName);	}

	/*
	 * Gets return type of the method or null if the method is a constructor.
	 */
	public String getReturnType() {	return mReturnType;	}


	public void setReturnType(String aReturnType) { mReturnType = m_GetValue(aReturnType);	}

	/*
	 * Method has been specified toghether with a class
	 */
	public boolean hasClass() {	return mClsName == null; }

	/**
	 * If the item is a class returns the body
	 */
	public String getFullBody()
	{
		if (mClsName == null)
		{
			return getBody();
		} 
		else
		{
			String preambole = "";

			if (!isConstructor())
			{
				preambole = mReturnType + " ";
			}

			preambole += mClsName + "::" + mName;

			String param_part = mParameterSet.getBody();

			return CppUtilities.format_Spaces(preambole + param_part);				
		}
	}

	public Boolean isConstructor()
	{
		return mReturnType == null;
	}

	/*
	 * Gets the body of the method (with no class indication)
	 */
	public String getBody()
	{
		String preambole = "";

		if (!isConstructor())
		{
			preambole = mReturnType + " ";
		}

		preambole += mName;

		String param_part = getParameterSet().getBody();

		return CppUtilities.format_Spaces(preambole + param_part);
	}
	
	/**
	 * 
	 * @param aParameterSet
	 */
	public void setParameterSet(ParameterSet aParameterSet)
	{
		mParameterSet = aParameterSet;
	}

	/**
	 * Return null if the aValue is made by empty space only
	 * otherwise trims() the string
	 */
	private static String m_GetValue ( String aValue )
	{
		if (aValue != null)
		{
			String temp = aValue.trim();

			return (temp.length()>0)?temp:null;
		}
		else
		{
			return null;
		}
	}

	public static void main(String[] args)
	{
		String[] test_cases = new String[] {
				"mytype ( int )",
				"mytype ( int )",
				"mytype ( const mytype&  )",
				"mytype (int& a = 2, int b=3 )",
				"mytype (const int* const v1[5],float* v2 , float* v3[] )",
				"mytype (const char* a , int b = 2 )",
				"mytype (const char* a = NULL, int b )",
				"mytypez::mytype (int* v1[5],float* v2 , float* v3[] )",
				"mytype (int* v1[5][N],float* v2 , float* v3[] )",
				"void mytype (int* v1[5],float* v2 , float* v3[] )",
				"void mytype::mymethod (int* v1[5],float* v2 , float* v3[] )",
				"myfunc (int* v1[5],float* v2 , float* v3[] )",
		"int myfunc (int* v1[5],float* v2 , float* v3[] )" };


		ParserCppMethodDeclarationEuristic parser = new ParserCppMethodDeclarationEuristic();

		for (String test_case : test_cases)
		{
			try
			{
				System.out.println("Input: " + test_case);

				parser.doTryWithException(test_case, 0);

				CppMethod method = parser.getResultTypized();

				if (method != null)
				{
					System.out.println(method.getBody());
					System.out.println(method.getFullBody());
				}
			} 
			catch (ParserNotMatchingException e)
			{
				System.out.println("Expression doesn't match!");
			} 
			catch (ParserErrorException e)
			{
				System.out.println("Error:");
				System.out.println(e.getMessageWithFailedIndexInfo());
			}

			System.out.println();
		}

		System.exit(0);
	}
}
