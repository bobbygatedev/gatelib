package g.assist;

import g.FatalException;
import g.io.FileHelper;
import g.prog.CppUtilities;
import g.text.RegexUtils;
import g.text.parser.IParserMultipleOptionTypizedAbstract;
import g.text.parser.IParserTypized;
import g.text.parser.ParserAbstract.ParserErrorException;
import g.text.parser.ParserAbstract.ParserNotMatchingException;
import g.text.scanner.TextScannerAbstract;

import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.regex.Matcher;

/**
 * Abstract class line 2 line parser for source code assist application. 
 * @author Bobby Gate
 */
public abstract class AssistSourceFileParserLine2LineAbstract 
implements IParserMultipleOptionTypizedAbstract<AssistGeneratedItemAbstract>
{
	private ArrayList<AssistGeneratedItemAbstract> mListGeneratedItems = null;
	private final int mVerbosityLevel;
	private ArrayList<String> mListOutputReport = new ArrayList<String>();

	private String mSourceFilePath = null;
	private ArrayList<String> mListIoErrors=null;
	
	/**
	 * Constructor.
	 * @param aVerbosityLevel Verbose level of the parser.  
	 * @param aOutputReport   Output file(report) of the parser.
	 */
	public AssistSourceFileParserLine2LineAbstract(int aVerbosityLevel)	{ mVerbosityLevel=aVerbosityLevel; }

	class InnerTestScanner extends TextScannerAbstract
	{
		private Matcher mMatcherNewLine = null;

		@Override
		public int searchNext(String aTextToBeScanned, int aIndex)
		throws ParserErrorException
		{
			IParserTypized<AssistGeneratedItemAbstract>[] parser_option_set = getParserOptionSetTypized();
			int index = aIndex;

			for (IParserTypized<AssistGeneratedItemAbstract> parser_option : parser_option_set)
			{
				try
				{
					index = parser_option.doTryWithException(aTextToBeScanned, aIndex);
					
					mListGeneratedItems.add(parser_option.getResultTypized());
										
					break;
				} 
				catch (ParserNotMatchingException e)
				{
					continue;
				}
			}

			if ( mMatcherNewLine.find(index) )
			{
				return mMatcherNewLine.end();
			}
			else
			{
				return -1;
			}
		}

		@Override
		public void onScanStart(String aTextToBeScanned)
		{
			mListGeneratedItems = new ArrayList<AssistGeneratedItemAbstract>();
			mMatcherNewLine = RegexUtils.pattern_NewLine.matcher(aTextToBeScanned);
		}

		@Override
		public void onScanEnd(String aTextToBeScanned) { }
	}	

	/**
	 * Starts the parsing on a file.
	 * @param aSourceFilePath Path of the file to be scanned/parsed.
	 * @throws IOException          Trouble creating output file or accessing to source.
	 * @throws ParserErrorException Possibly used parser has raised an exception. 
	 */
	public void doStartSourceFile(String aSourceFilePath) throws 
	IOException, 
	ParserErrorException		
	{
		InnerTestScanner inner_test_scanner_instance = new InnerTestScanner();

		File file = new File(aSourceFilePath);
				
		String uncommented_file_body = CppUtilities.remove_Comment(FileHelper.read_ToEnd(mSourceFilePath = file.getAbsolutePath())); 
		
		inner_test_scanner_instance.doStart(uncommented_file_body);
		
		if(mListIoErrors != null && mListIoErrors.size()>0)
		{
			throw new IOException("There are some I/O errors!");
		}
	}

	/**
	 * Generate output file(s).
	 * @param aSpecificOutputDir A specific destination path or null. In this case the source code directory is used.
	 */
	public void doGeneratedOutputPath(String aSpecificOutputDir)
	{
		if(mSourceFilePath==null)
		{
			new FatalException("Source file wasn't set").doStandardAction();
		}
		else
		{
			mListIoErrors = new ArrayList<String>();
							
			if (mListGeneratedItems.size()>0)
			{
				mFormatToVerboseMessageAndReport(
						2, "\nTrying to obtain %d output stream(s):",mListGeneratedItems.size());
			}
			
			for (AssistGeneratedItemAbstract generated_item : mListGeneratedItems)
			{
				try
				{
					generated_item.obtainStreamForOutputFile(mSourceFilePath,aSpecificOutputDir);
				} 
				catch (IOException e)
				{
					mListIoErrors.add(e.getMessage());
				}
			}

			if (mListGeneratedItems.size()>0)
			{
				mFormatToVerboseMessageAndReport(2, "ok\n");
			}
			
			if (mListIoErrors.size()>0)
			{
				System.err.println("Some I/O errors are present, file writing will be skipped:");

				for (String io_error : mListIoErrors)
				{
					System.err.println(io_error);											
				}

				return;
			}
							
			if(mListGeneratedItems.size()==0)
			{
				mFormatToVerboseMessageAndReport(1, "No item to be generated detected!\n");
			}
			
			for (AssistGeneratedItemAbstract generated_item :mListGeneratedItems)
			{
				mFormatToVerboseMessageAndReport(1, "Generating output file %s.\n",generated_item.getDestinationPath());
				mFormatToVerboseMessageAndReport(2, "Content:\n%s\n",generated_item.buildItemBody());
				
				generated_item.doSaveBody();
			}
		}			
	}
		
	/**
	 * Print output report messages onto a file.
	 * @param aOutputReportPath
	 * @throws IOException 
	 */
	public void outputReportToFile(String aOutputReportPath) throws IOException
	{
		m_CreateDirectoryForFileIfNecessary(aOutputReportPath);
				
		PrintStream print_stream = new PrintStream(aOutputReportPath);
		
		for (String line : mListOutputReport)
		{
			print_stream.println(line);
		}
		
		print_stream.close();
		
		System.out.printf("Precompile report in %s.\n",new File(aOutputReportPath).getCanonicalPath());		
	}	
	
	/**
	 * Add a message to output console if aVerboseLevel >= mVerbosityLevel and
	 * to the output report if mOutputReport is not null.
	 * @param aVerboseLevel Verbosity level of the message.
	 * @param aMsg          Body of the message ( as in printf )
	 * @param aOptionals    Optional values for printf 
	 */
	private void mFormatToVerboseMessageAndReport ( int aVerboseLevel , String aMsg , Object ... aOptionals ) 
	{
		if ( mVerbosityLevel>=aVerboseLevel)
		{
			System.out.printf(aMsg,aOptionals);
		}
		
		mListOutputReport.add(String.format(aMsg, aOptionals));	
	}
	
	private static void m_CreateDirectoryForFileIfNecessary(String aDestinationPath) throws IOException
	{
		String destination_file = new File(aDestinationPath).getCanonicalPath();				
		File directory_descriptor = new File(new File(destination_file).getParent());
		
		if (!directory_descriptor.exists())
		{
			if(!directory_descriptor.mkdirs())
			{
				throw new IOException("Can't create directory " + directory_descriptor.getPath());
			}
		}
	}		
}
