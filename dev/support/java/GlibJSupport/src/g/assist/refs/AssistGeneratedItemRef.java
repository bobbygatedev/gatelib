package g.assist.refs;

import g.FatalException;
import g.assist.AssistGeneratedItemAbstract;
import g.prog.CppMethod;
import g.prog.parser.ParserCppMethodDeclarationEuristic.ParserConstructorDeclaration;
import g.text.NotAValidTemplateStringFormat;
import g.text.TextTemplateBuilder;
import g.text.parser.ParserAbstract.ParserNotMatchingException;
import g.text.parser.ParserAbstract.ParserErrorException;
import g.text.parser.ParserTypizedAbstract;

import java.util.ArrayList;

/**
 * Generator class for reference, encapsulates a constructor list referred to a certain class. 
 * @author Bobby Gate
 *
 */
public class AssistGeneratedItemRef extends AssistGeneratedItemAbstract
{

	private static final String REFERENCE_CONSTRUCTROR_TEMPLATE_RESOURCE = "ClassWithRefConstructorTemplate.txt";
	private static final String REFERENCE_TEMPLATE_RESOURCE = "ClassWithRefTemplate.txt";

	private String mClsName = null;
	private ArrayList<ConstructorWrapper> mListConstructors = new ArrayList<ConstructorWrapper>();


	@Override
	public String getGenerationItemName(){ return String.format("%s.h", mClsName); }

	@Override
	public String getGenerationDirName(){ return "_g_ref"; }

	/**
	 * Used by template builder: MUST BE PUBLIC!
	 * Wrappes a CppMethod instance
	 * @see g.prog.CppMethod
	 * @author Bobby Gate
	 *
	 */
	public static class ConstructorWrapper
	{
		private CppMethod mCppMethod;
		private String mClsName;

		static class Parser extends ParserTypizedAbstract<ConstructorWrapper>
		{
			private ParserConstructorDeclaration mInnerConstructorParser = new ParserConstructorDeclaration();
			private ConstructorWrapper mResult = null;

			@Override
			public int doTry(String aPossiblyParseEpression, int aStartIndex)
			throws ParserErrorException
			{
				try
				{
					int index = mInnerConstructorParser.doTryWithException(aPossiblyParseEpression, aStartIndex);
					mResult = new ConstructorWrapper(mInnerConstructorParser.getResultTypized());	
					return index;
				} 
				catch (ParserNotMatchingException e)
				{
					mFailedIndex = e.getFailedIndex();
					return -1;
				}
			}

			@Override
			public ConstructorWrapper getResultTypized(){ return mResult; }
		}

		public ConstructorWrapper(CppMethod aCppMethod) 
		{
			if(aCppMethod.isConstructor())
			{
				mCppMethod = aCppMethod;
				mClsName = aCppMethod.getName();
				mCppMethod.getParameterSet().fillParamTypesWithDefault();
			}
			else
			{
				new FatalException("A C/C++ constructor is required here! ").doStandardAction();
			}
		}


		public String getClsName() { return mClsName; }

		public String getBody()
		{
			try
			{
				TextTemplateBuilder text_template_builder = new TextTemplateBuilder(
						ConstructorWrapper.class);
				text_template_builder.setTemplateStream(getClass()
						.getResourceAsStream(
								REFERENCE_CONSTRUCTROR_TEMPLATE_RESOURCE));
				text_template_builder.setInterfaceInstance(this);

				return text_template_builder.getOutputExpression();
			}
			catch (NotAValidTemplateStringFormat e)
			{
				new FatalException(e).doStandardAction();
			} 

			return null;// mMethod.getBody();
		}

		public CppMethod.Parameter[] getParameters() { return mCppMethod.getParameterSet().toArray();	}
	}

	/**
	 * Gets the name of the class where the references are defined. i.e. class
	 * MyCls { public: #include G_REF_HELPER(MyCls) MyCls (int v1) :
	 * m1(v1),m2(0) {} MyCls ( int v1 , int v2 ) : m1(v1),m2(v2) {} int get1()
	 * const { return m1; } int get2() const { return m2; } private: int m1,m2;
	 * };
	 * @return
	 */
	public String getClsName() { return mClsName;}

	/**
	 * 
	 */
	public String buildItemBody()
	{
		try
		{
			TextTemplateBuilder text_template_builder = new TextTemplateBuilder(
					AssistGeneratedItemRef.class);

			text_template_builder.setTemplateStream(getClass().getResourceAsStream(REFERENCE_TEMPLATE_RESOURCE));
			text_template_builder.setInterfaceInstance(this);

			return text_template_builder.getOutputExpression();

		} 
		catch (NotAValidTemplateStringFormat e)
		{
			new FatalException(e).doStandardAction();
		}

		// not reachable
		return null;
	}

	/**
	 * Try to add a constructor declaration using source code from 'aIndex' i.e.
	 * Cls(int , int )
	 * @param aSourceCodeText The entire source code text file.
	 * @param aIndex Index/Offset where the constructor should lie.
	 * @return the index where the declaration ends.
	 * @throws ParserErrorException
	 * @throws ParserNotMatchingException 
	 * 
	 * @see g.text.parser.IParser.doTry
	 */
	public int doTryToAddConstructor(String aSourceCodeText , int aIndex )
	throws ParserErrorException, ParserNotMatchingException
	{
		ConstructorWrapper.Parser parser = new ConstructorWrapper.Parser();

		int index = parser.doTryWithException(aSourceCodeText, aIndex);
		mListConstructors.add(parser.getResultTypized());
		
		if(mClsName==null)
		{
			mClsName = parser.getResultTypized().getClsName();
		}
		else if(!mClsName.equals(parser.getResultTypized().getClsName()))
		{
			throw new ParserErrorException("Constructrors not having same name!", aSourceCodeText, index);
		}
		
		return index;
	}

	/**
	 * Gets number of the encapsulated constructors.
	 * @return
	 */
	public int getNumOfConstructors() { return mListConstructors.size(); }

	/**
	 * Gets the constructor wrapper list.
	 * @return
	 */
	public ConstructorWrapper[] getConstructors()
	{
		return mListConstructors.toArray(new ConstructorWrapper[mListConstructors.size()]);
	}

	public static void main(String[] args)
	{
		String test = 
			"SemaphoreNamed ( const char* , NamedSyncObjectCreationOptions_t aCreationOptions = g_create_always , GInt32_t initial_count = 0 );";

		try
		{
			AssistGeneratedItemRef instance = new AssistGeneratedItemRef();

			if ( instance.doTryToAddConstructor(test, 0) !=-1 )
			{
				System.out.println(instance.buildItemBody());		
			}
			else
			{
				System.out.println("Translation failed somehow!");
			}			
		}
		catch (ParserErrorException e)
		{			
			System.out.println(e.getMessageWithFailedIndexInfo());
			System.out.printf("Remaining expression was:\n%s",test.substring(e.getFailedIndex()));
		} 
		catch (ParserNotMatchingException e)
		{
			System.out.println("Translation failed somehow!");
		}

		return;
	}
}
