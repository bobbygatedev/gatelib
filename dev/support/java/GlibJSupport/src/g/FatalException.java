package g;

public class FatalException extends Exception
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 6904681100524222570L;
	
	public FatalException(Exception aInnerException)
	{
		super(aInnerException);
	}
	
	public FatalException(String aMessage)
	{
		super(aMessage);
	}

	public void doStandardAction()
	{
		System.err.printf("Fatal Exception, reason:\n%s\nQutting..\n", getMessage());
		printStackTrace();
		System.exit(-1);
	}
}
