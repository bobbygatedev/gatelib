package g.text.parser;


public interface IParserMultipleOptionTypizedAbstract<T>
{

	/**
	 * Override for defining the alternative set.
	 * @return
	 */
	public abstract IParserTypized<T>[] getParserOptionSetTypized();

}