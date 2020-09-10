package g.text.scanner;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import g.text.parser.ParserAbstract.ParserErrorException;

/**
 * Abstract class for a text scanner. 
 * A text scanner search some match inside a text ( i.e. from a regular expression ).  
 * @author Bobby Gate
 *
 */
public abstract class TextScannerAbstract
{
	private int mInternalIndex = -1;
	private String mInternalText = null; 

	/**
	 * Implement for define the action to be taken for search.
	 * a ParserErrorException 
	 * @param aTextToBeScanned Input text.
	 * @param aIndex           Strign index/offset where search start.
	 * @return If search fails method MUST return -1 otherwise the index at the end of the match.
	 * @throws ParserErrorException Possibly used Parser has raised an exception. 
	 */
	public abstract int searchNext(String aTextToBeScanned , int aIndex) throws ParserErrorException;

	/**
	 * Override for implementing action at scan init.
	 * @param aTextToBeScanned Input text.
	 */
	public abstract void onScanStart ( String aTextToBeScanned );
	/**
	 * Override for implementing action at scan end.
	 * @param aTextToBeScanned Input text.
	 */
	public abstract void onScanEnd ( String aTextToBeScanned );
	
	/**
	 * Starts the elaboration, that consist of repeated call of searchNext until it fails(returns -1). 
	 * @param aTextToBeScanned Input text.
	 * @throws ParserErrorException Possibly used Parser has raised an exception. 
	 */
	public void doStart ( String aTextToBeScanned ) throws ParserErrorException
	{
		mInternalIndex = 0;
		mInternalText = aTextToBeScanned;

		onScanStart(mInternalText);

		while(true)
		{
			int new_position = searchNext ( mInternalText , mInternalIndex );

			if (new_position==-1)
			{
				onScanEnd(mInternalText);
				return;
			}
			else
			{
				mInternalIndex=new_position;
			}
		}
	}

	static class InnerTest extends TextScannerAbstract
	{
		private Pattern mPattern = Pattern.compile("\\w+");
		private Matcher mMatcher = null;

		@Override
		public int searchNext(String aTextToBeScanned, int aIndex)
		{
			if ( mMatcher.find(aIndex) )
			{			
				System.out.println(aTextToBeScanned.substring(mMatcher.start(),mMatcher.end()));
				return mMatcher.end();
			}			
			else
			{
				return -1;	
			}
		}

		@Override
		public void onScanStart(String aTextToBeScanned)
		{
			System.out.println("TextScannerAbstract.InnerTest.onScanStart()");
			mMatcher = mPattern.matcher(aTextToBeScanned);
		}

		@Override
		public void onScanEnd(String aTextToBeScanned)
		{
			System.out.println("TextScannerAbstract.InnerTest.onScanEnd()");
		}
	}

	public static void main(String[] args)
	{
		InnerTest inner_test = new InnerTest();
		String test = "pippo pluto paperino";

		System.out.println("TestExpression: '" +test+"'." );		
		try
		{
			inner_test.doStart(test);
		} 
		catch (ParserErrorException e)
		{
			System.out.println(e.getMessageWithFailedLineColInfo());
		}		
	}
}
