package g.text.parser;

/**
 * Parsing passes at the first option that matches.
 * @author Bobby Gate 
 */
public abstract class ParserMultipleOptionAbstract extends ParserAbstract
{		
	protected IParser mParserSuccessfull = null;

	/**
	 * Implementation defines the ordered set of possibilities.
	 * @return
	 */
	public abstract IParser[] getParserOptionSet();

	/**
	 * Gets the first parser that has successfully parsed.
	 * @return
	 */
	public IParser getParserSuccessfull ( ){ return mParserSuccessfull; }

	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex) throws ParserErrorException
	{
		mParserSuccessfull = null;
		
		for (IParser sequential_parser : getParserOptionSet())
		{
			int index = sequential_parser.doTry(aPossiblyParseEpression, aStartIndex); 

			if (index!= -1)
			{
				mParserSuccessfull = sequential_parser;
				return index;
			}
		}
		return -1;
	}

	@Override
	public Object getResult()
	{
		return (mParserSuccessfull!=null)?mParserSuccessfull.getResult():null;
	}	
}

