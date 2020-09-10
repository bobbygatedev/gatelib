package g.assist.signal;

import g.FatalException;
import g.text.parser.IParser;
import g.text.parser.ParserRegex;
import g.text.parser.ParserSequenceTypizedAbstract;

import java.util.regex.Pattern;

/**
 * Parser an expression of the format 
 * 'G_SIGNAL ( sig_name (partype1 [par1],partype2 [par2],..,partypeN parN=init_val) )' i.e.:
 * G_SIGNAL(g::signals::MySignal(GInt32_t aPar1 , GUint64_t = 5))
 * @author Bobby gate
 */
public class ParserSignalByInstance extends ParserSequenceTypizedAbstract<AssistGeneratedItemSignal>
{
	private AssistGeneratedItemSignal mResult = null;
				
	private ParserRegex mParserPreambole = new ParserRegex("^\\s*G_SIGNAL\\s*\\(\\s*",Pattern.MULTILINE);
	private ParserSignalInterface mParserSignalInterface = new ParserSignalInterface();
	private ParserRegex mParserParenthesysCloseAndLineEnd = new ParserRegex("\\s*\\)\\s*$",Pattern.MULTILINE);
	
	@Override
	public AssistGeneratedItemSignal getResultTypized() { return mResult; }
	
	@Override
	public IParser[] getParserSequence()
	{
		return new IParser[] {
				mParserPreambole ,
				mParserSignalInterface ,
				mParserParenthesysCloseAndLineEnd
		};		
	}
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex)
			throws ParserErrorException
	{
		int index = super.doTry(aPossiblyParseEpression, aStartIndex);
								
		if (index != -1)
		{			
			//signal is instanciatee inside parse signal full name
			mResult =  mParserSignalInterface.getResultTypized();
			
			return index;
		}
		else if ( getFailedParser() == mParserPreambole ) 
		{
			return -1;//if the preambole doesn't match parser don't try to parse either
			//otherwise an error causes an exception.
		}
		else if( getFailedParser() == mParserSignalInterface )
		{
			throw new ParserErrorException("Not a valid signal name!", aPossiblyParseEpression , mFailedIndex);
		}
		else if( getFailedParser() == mParserParenthesysCloseAndLineEnd )
		{
			throw new ParserErrorException("')' plus line end expected here!", aPossiblyParseEpression , mFailedIndex);
		}
		else 
		{
			new FatalException("Unexpected here").doStandardAction();
			return -1;
		}		
	}
		
	public static void main(String[] args)
	{
		String[] test_expressions = new String[]{
				"G_SIGNAL(g::signals::MySignal(int , int aPar2=2))\n",
				" G_SIGNAL(g::signals::MySignal(GInt32_t aPar1 , GUint64_t aPar2))\n",
				" G_SIGNAL(g::signals::MySignal(GInt32_t aPar1 , GUint64_t aPar2))",
				"  G_SIGNAL(g::signals::MySignal(GInt32_t aPar1 , GUint64_t aPar2)))",
				" G_SIGNAL(g::signals::MySignal(GInt32_t aPar1 , GUint64_t aPar2)"//causes error
		};

		for (String test_expr : test_expressions)
		{
			ParserSignalByInstance parser = new ParserSignalByInstance();

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
