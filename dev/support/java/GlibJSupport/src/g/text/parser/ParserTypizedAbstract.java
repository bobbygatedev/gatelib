package g.text.parser;

public abstract class ParserTypizedAbstract<T> extends ParserAbstract implements IParserTypized<T>
{
	@Override
	public abstract T getResultTypized();
			
	@Override
	public Object getResult()
	{
		return getResultTypized();
	}
}





