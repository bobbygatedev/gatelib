package g.text.parser;

/*
 * Parsing matches if and only if every parser in the sequence matches.
 */
public abstract class ParserSequenceAbstract extends ParserAbstract 
{
	private IParser mFailedParser = null;
	
	public abstract IParser[] getParserSequence();	
	public IParser getFailedParser ( ) { return mFailedParser; }
				
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex) throws ParserErrorException
	{
		int index = aStartIndex;

		for (IParser parser : getParserSequence())
		{
			try
			{
			int new_index =parser.doTry(aPossiblyParseEpression, index); 

			if (new_index!=-1)
			{
				index = new_index;
			}
			else
			{
				mFailedParser = parser;
				mFailedIndex = index;
				return -1;
			}
			}
			catch (ParserErrorException e) 
			{
				mFailedParser = parser;
				throw e;
			}
		}

		return index;
	}		
}
