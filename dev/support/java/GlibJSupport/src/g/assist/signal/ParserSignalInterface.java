package g.assist.signal;

import g.FatalException;
import g.prog.CppMethod.ParameterSet;
import g.prog.parser.NotAValidParamIntialisation;
import g.prog.parser.ParserCppMethodDeclarationEuristic.ParserParameterSet;
import g.prog.parser.ParserTypeNameSimple;
import g.text.parser.IParser;
import g.text.parser.ParserSequenceTypizedAbstract;

import java.util.regex.Pattern;

/**
 * Parser an expression of the format 
 * 'nspace::sig_name ( sigpar_type1 sigpar_1 , sigpar_type2 sigpar_2 ) )' i.e.:
 * g::signals::MySignal,(GInt32_t aPar1 , GUint64_t aPar2)
 * NOTICE! Namespace are separated by :: differently from SignalDeclarationParser.
 * @author Bobby Gate
 */
public class ParserSignalInterface extends ParserSequenceTypizedAbstract<AssistGeneratedItemSignal>
{
	private ParserSignalFullName mParserSignalFullName = new ParserSignalFullName();
	private ParserParameterSet mParserSignalParameterSet = new ParserParameterSet();
	private AssistGeneratedItemSignal mResult = null;

	/**
	 * Parses a name of type n1::n2::Type.
	 * @author Bobby Gate
	 *
	 */
	static class ParserSignalFullName extends ParserSequenceTypizedAbstract<AssistGeneratedItemSignal>
	{		
		private AssistGeneratedItemSignal mResult = null;
		private ParserTypeNameSimple mInnerParserSignalFullName = new ParserTypeNameSimple();
		
		@Override
		public int doTry(String aPossiblyParseEpression, int aStartIndex) throws ParserErrorException
		{
			int index = super.doTry(aPossiblyParseEpression, aStartIndex);
			
			if ( index != -1)
			{			
				String[] temp = 
					Pattern.compile("\\s*").matcher(aPossiblyParseEpression.substring(aStartIndex,index)).
						replaceAll("").split("::");

				mResult = new AssistGeneratedItemSignal(temp[temp.length-1], m_GetNamespace(temp));
			}

			return index;
		}

		private static String m_GetNamespace(String[] aFullSignalNameSplitted)
		{
			if (aFullSignalNameSplitted.length<=1)
			{
				return null;
			}
			else
			{
				String result = aFullSignalNameSplitted[0];
				
				for (int i = 1; i < aFullSignalNameSplitted.length-1; i++)
				{
					result += ( "::" + aFullSignalNameSplitted[i] );
				}
				
				return result;
			}						
		}

		@Override
		public AssistGeneratedItemSignal getResultTypized() { return mResult; }

		@Override
		public IParser[] getParserSequence() { return new IParser[] { mInnerParserSignalFullName };	}
	}
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
			throws ParserErrorException
	{
		int index = super.doTry(aPossiblyParseEpression, aStartIndex);
								
		if (index != -1)
		{			
			//signal is instantiated inside parse signal full name
			mResult =  mParserSignalFullName.getResultTypized();
			
			try
			{
				ParameterSet parameter_set = new ParameterSet(mParserSignalParameterSet.getResultTypized());
				
				parameter_set.fillParamTypesWithDefault();
				
				mResult.setParameterSet(parameter_set);
			} 
			catch (NotAValidParamIntialisation e)
			{
				throw new ParserErrorException(e.getMessage(), aPossiblyParseEpression, index);
			}
			
			return index;
		}
		else if( getFailedParser() == mParserSignalFullName )
		{
			throw new ParserErrorException("Not a valid signal name!", aPossiblyParseEpression, mFailedIndex);
		}
		else if( getFailedParser() == mParserSignalParameterSet )
		{
			throw new ParserErrorException("Expected a parameter declaration or at least ()", aPossiblyParseEpression, mFailedIndex);
		}
		else 
		{
			new FatalException("Unexpected here").doStandardAction();
			return -1;
		}		
	}

	@Override
	public AssistGeneratedItemSignal getResultTypized(){ return mResult; }

	@Override
	public IParser[] getParserSequence()
	{
		return new IParser[] {mParserSignalFullName ,mParserSignalParameterSet};		
	}
	
	public static void main(String[] args)
	{
		String[] test_expressions = new String[]{
				"g::signals::MySignal(GInt32_t aPar1 , GUint64_t)",
				"g::signals::MySignal(GInt32_t aPar1 , GUint64_t aPar2)",
				"g::signals::MySignal(GInt32_t aPar1 , "//causes error
		};

		for (String test_expr : test_expressions)
		{
			ParserSignalInterface parser = new ParserSignalInterface();

			try
			{
				int index = parser.doTry(test_expr, 0);

				if (index != -1)
				{
					System.out.printf("Translation of %s succesfull:\n%s\n",test_expr,parser.getResultTypized());
					System.out.println(parser.getResultTypized().buildItemBody());
				}
				else 
				{
					System.out.printf("Translation of %s failed!\n",test_expr);
				}
			} 
			catch (ParserErrorException e)
			{
				System.out.printf(
						"Translation of %s failed with msg in %d:\n%s\n%s\n%s",
						test_expr,
						e.getFailedIndex(), 
						e.getMessage(),
						test_expr.substring(0,e.getFailedIndex()),
						test_expr.substring(e.getFailedIndex()));
			}				
		}	
	}
	
}
