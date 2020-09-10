package g.text.parser;

import g.text.parser.ParserAbstract.ParserErrorException;
import g.text.parser.ParserAbstract.ParserNotMatchingException;

/**
 * Interface for a text parser.
 * A text parser is a parser, that gets a string as an input and a start index(or offset) and returns the
 * position where the parser finsih or -1 in case of error.
 * @author Bobby Gate
 *
 */
public interface IParser
{
	/**
	 * Tries to parse a string starting from a certain index, if it is successful returns the index of the string after parsing and causes getResult()
	 * returns a valid value.
	 * @param aPossiblyParseEpression Input expression.
	 * @param aStartIndex             Start index where parser should start.
	 * @return                        The string index where the parsing has finished or -1 if parsing fails.
	 * @throws ParserErrorException   Implementing classes should throw this exception if some match has occurred but any error has occured. 
	 */
	public abstract int doTry(String aPossiblyParseEpression, int aStartIndex) throws ParserErrorException;

	/**
	 * Calls doTry, but raises an exception if it returns -1. 
	 * @param aPossiblyParseEpression Input expression.
	 * @param aStartIndex             Start index where parser should start.
	 * @return                        The string index where the parsing has finished.
	 * @throws ParserErrorException   Implementing classes should throw this exception if some match has occurred but any error has occured. 
	 * @throws ParserNotMatchingException In case of not matching ( doTry returns -1, this exception wlll be called.
	 */
	public abstract int doTryWithException(String aPossiblyParseEpression,int aIndex)throws ParserErrorException,ParserNotMatchingException;
	/**
	 * Gets the result of parsing as an instance of 'Object'. 
	 * @return
	 */
	public abstract Object getResult();
}