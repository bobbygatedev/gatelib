package g.assist;

import g.FatalException;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Implement abstract methods of this class to define an item,
 * that can be generated after parsing a certain source code file.
 * @author Bobby Gate
 */
public abstract class AssistGeneratedItemAbstract
{
	private FileOutputStream mDestinationStream = null;
	private String mDestinationPath = null;
	private String mSourcePath = null;

	/**
	 * Build the body (content) of the file and returns it.
	 * Override for implementing file generation.
	 * @return
	 */
	public abstract String buildItemBody();
	
	/**
	 * Gets the file name of the generated generation item name i.e. sig.h
	 * @return
	 */
	public abstract String getGenerationItemName();

	/**
	 * Gets the generation directory name.
	 * Override for defining the directory where the item has to be generated.
	 * i.e. public String getGenerationDirName() { return "_g_ref"; }
	 * @return
	 * 
	 */
	public abstract String getGenerationDirName();
	
	/**
	 * Gets the source path, where the item has been declared.
	 * @return
	 */
	public String getSourcePath(){ return mSourcePath;}

	/**
	 * Gets the destination path, calculated based on getSourcePath(),getGenerationDirName() and getGenerationItemName()
	 * during invoke of obtainStreamForOutputFile.
	 * @return The destination path or null, if obtainStreamForOutputFile has not been called yet.   
	 * 
	 * @see g.assist.AssistGeneratedItemAbstract.obtainStreamForOutputFile
	 */
	public String getDestinationPath(){ return mDestinationPath;}
		
	/**
	 * Computes the destination path and try to obtain the output writing stream.
	 * @param aGeneratingSourceFilePath Source file path.
	 * @param aSpecificOutputDir Destination directory or null in that 
	 * @throws IOException
	 */
	public void obtainStreamForOutputFile(String aGeneratingSourceFilePath, String aSpecificOutputDir) throws IOException
	{
		mDestinationPath = mBuildDestinationPath(mSourcePath = aGeneratingSourceFilePath,aSpecificOutputDir);

		try
		{
			if ( mDestinationStream==null)
			{
				m_CreateDirectoryForFileIfNecessary(mDestinationPath);				
								
				mDestinationStream = new FileOutputStream(mDestinationPath);								
				mDestinationStream.write(new byte[0]);//dummy writing check file availability
			}
			else 
			{
				new FatalException(
						"Not allowed to obtainStreamForOutputFile if output strea already exists").doStandardAction();
			}
		} 
		catch (FileNotFoundException e)
		{
			throw new IOException("Error trying to access to file " + mDestinationPath +".");
		} 
	}

	/**
	 * Builds the destination path. 
	 * @param aGeneratingSourceFilePath Path of the source file that generated this item.
	 * @param aSpecificDestinationDir
	 * @return
	 */
	private String mBuildDestinationPath(String aGeneratingSourceFilePath,String aSpecificDestinationDir)
	{
		if(aSpecificDestinationDir!=null)
		{
			File specific_dest_dir = new File(aSpecificDestinationDir);
			
			return new File (
					specific_dest_dir.getAbsolutePath() +File.separator + 
					getGenerationDirName () +File.separator + 
					getGenerationItemName()).getAbsolutePath();						
		}
		else
		{
			File source_file = new File(aGeneratingSourceFilePath);
			
			return new File (
					source_file.getParent() +File.separator + 
					getGenerationDirName () +File.separator + 
					getGenerationItemName()).getAbsolutePath();			
		}
	}
	
	/**
	 * Save the file on obtained output stream
	 */
	public void doSaveBody() 
	{
		try
		{
			mDestinationStream.write( buildItemBody().getBytes());
			mDestinationStream.close();			
			mDestinationStream = null;			
		} 
		catch (IOException e)
		{
			new FatalException(e).doStandardAction();
		}		
	}

	/**
	 * Copies the body of generated item to clipboard.
	 */
	public void doCopyBodyToClipboard()
	{
		StringSelection selection = new StringSelection(buildItemBody());
		Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
		clipboard.setContents(selection, selection);		
	}
	
	private static void m_CreateDirectoryForFileIfNecessary(String aDestinationPath) throws IOException
	{
		File directory_descriptor = new File(new File(aDestinationPath).getParent());
		
		if (!directory_descriptor.exists())
		{
			if(!directory_descriptor.mkdirs())
			{
				throw new IOException("Can't create directory " + directory_descriptor.getPath());
			}
		}
	}	
}