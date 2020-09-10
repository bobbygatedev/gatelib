package g.io;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileHelper
{

	public static String read_ToEnd(String aFilePath) throws IOException
	{
		InputStream stream = new FileInputStream(aFilePath);
		byte[] file_bytes = new byte[stream.available()];
		stream.read(file_bytes);
		String file_content = new String(file_bytes);		
		
		return file_content;
	}

}
