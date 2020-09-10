package g.text.parser;

public interface IParserTypized<T> extends IParser
{
	public abstract T getResultTypized();
}