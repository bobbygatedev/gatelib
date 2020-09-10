package g.text.parser;


/*
 * Is a parser based on a sequence of parser that returns a result of a certain type:
 * The singles parser of the sequence MUST NOT returns the SAME RESULT TYPE 
 * 
 */
public abstract class ParserSequenceTypizedAbstract<T> extends ParserSequenceAbstract implements IParserTypized<T>
{
	@Override
	public Object getResult()
	{
		return getResultTypized();
	}
		
}
