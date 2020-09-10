package g.text.parser;

import g.text.TextLineColumn;

/**
 * Abstract class for a generic parser, override doTry implements behavior.
 * @author Bobby Gate
 */
public abstract class ParserAbstract implements IParser
{
	protected int mFailedIndex=-1;

	/**
	 * Base class for ParserAbstract exceptions.
	 * @author Bobby Gate
	 *
	 */
	public static class ParserBaseException extends Exception
	{
		private int mFailedIndex;
		private String mFailedText;

		protected ParserBaseException(String aFailedText , int aFailedIndex) 
		{ 
			mFailedText = aFailedText;
			mFailedIndex = aFailedIndex;			
		}		
		
		protected ParserBaseException(String aErrorMsg , String aFailedText , int aFailedIndex) 
		{
			super(aErrorMsg);
			mFailedText = aFailedText;
			mFailedIndex = aFailedIndex;
		}		
		
		/**
		 * Gets the last successful index during parsing.
		 * @return
		 */
		public int getFailedIndex(){ return mFailedIndex; }

		/**
		 * Gets line,col info of the failure (last successful line,col).
		 * @return
		 */
		public TextLineColumn getFailedLineColumn(){ return TextLineColumn.convert_FromIndex(mFailedText, mFailedIndex); }  
	
		/**
		 * Gets an error description complete with line,col info of the failure position and failed line. 
		 * @return
		 */
		public String getMessageWithFailedLineColInfo()
		{
			return String.format("'%s'\nError in %s: %s", getFailedLineColumn().getLineText(),getFailedLineColumn(),getMessage());
		}		

		/**
		 * Gets an error description complete with index of the failure position. 
		 * @return
		 */
		public String getMessageWithFailedIndexInfo()
		{
			return String.format("Error in position %d: %s", getFailedIndex(),getMessage());
		}		
		
		private static final long serialVersionUID = 1L;		
	}
	
	/**
	 * Used by doTryWithException in case of failure
	 * Means that parse has failed since the expression doesn't match,
	 * but is possible to try to match elsewhere.
	 * @author Bobby Gate
	 *
	 */
	public static class ParserNotMatchingException extends ParserBaseException
	{
		//Can be invoked just from inside doTryWithException
		private ParserNotMatchingException(String aFailedText , int aFailedIndex) {super(aFailedText,aFailedIndex);}

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
	}

	/**
	 * It should be thrown from inside doTry in case of a matched expression
	 * that contains any error.
	 * Throwing this exception will prevents any further processing (i.e. in case of ParserMultipleOption.. ).
	 * @author Bobby Gate
	 * @see g.text.ParserMultipleOptionAbstract
	 * @see g.text.ParserMultipleOptionTypizedAbstract
	 * @see g.text.ParserMultipleOptionLine2LineAbstract
	 */
	public static class ParserErrorException extends ParserBaseException
	{		
		private static final long serialVersionUID = 1L;
		private int mErrorCode=-1;

		/**
		 * 
		 * @param aErrorMessage
		 * @param aFailedText
		 * @param aFailingIndex
		 */
		public ParserErrorException(String aErrorMessage, String aFailedText , int aFailingIndex)
		{
			super(aErrorMessage,aFailedText,aFailingIndex);
		}

		/**
		 * 
		 * @param aErrorMessage
		 * @param aErrorCode
		 * @param aFailedText
		 * @param aFailingIndex
		 */
		public ParserErrorException(String aErrorMessage, int aErrorCode ,String aFailedText , int aFailingIndex)
		{
			super(aErrorMessage,aFailedText,aFailingIndex);
			
			mErrorCode = aErrorCode;
		}

		/**
		 * 
		 * @return
		 */
		public int getErrorCode(){ return mErrorCode; }
	}

	/**
	 * Gets the failed index (last position where text was successfully parsed)
	 * @return
	 */
	public int getFailedIndex ( ) { return mFailedIndex; }

	/**
	 * Equal to doTry, but it throws exception if try fails.
	 */
	public int doTryWithException (String aPossiblyParseEpression, int aStartIndex) 
	throws ParserNotMatchingException,ParserErrorException
	{
		int new_index = doTry(aPossiblyParseEpression, aStartIndex);
		
		if (new_index != -1)
		{
			return new_index;
		}
		else 
		{
			throw new ParserNotMatchingException(aPossiblyParseEpression , mFailedIndex);
		}
	}
}

