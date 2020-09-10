package g.text.parser;

public abstract class ParserMultipleOptionTypizedAbstract<T> extends ParserTypizedAbstract<T> implements IParserMultipleOptionTypizedAbstract<T>
{		
	/**
	 * @see g.text.parser.IParserMultipleOptionTypizedAbstract#getParserOptionSetTypized()
	 */
	@Override
	public abstract IParserTypized<T>[] getParserOptionSetTypized();

	private IParserTypized<T> mSuccessfullParser = null;
	
	public IParserTypized<T> getSuccessfullParser(){ return mSuccessfullParser;}
	
	@Override
	public int doTry(String aPossiblyParseEpression, int aStartIndex) throws ParserErrorException
	{
		for (IParserTypized<T> parser : getParserOptionSetTypized())
		{
			int result = ((IParser) parser).doTry(aPossiblyParseEpression, aStartIndex); 

			if (result!= -1)
			{
				mSuccessfullParser = parser;
				return result;
			}
		}
		return -1;
	}

	@Override
	public T getResultTypized()
	{
		return ( mSuccessfullParser != null )?mSuccessfullParser.getResultTypized():null;
	}		
}
