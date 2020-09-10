package g.prog.visualstudio;

import g.FatalException;
import g.prog.CppUtilities;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * Provides utilities for MS Visual Studio ( tested on 2005 ) manipulation. 
 * @author Bobby Gate
 *
 */
public class MsVisualCppTools
{	
	private static final String FILES_XPATH = "/VisualStudioProject/Files";
	
	/**
	 * Given a visual C project, it returns the list of all items, where allowed extension are the allowed source code extensions (.c,cpp,.h,.hpp,..). 
	 * @param aVcprojPath Path of the visual c studio project.
	 * @return Array of (absolute) files path.
	 * @throws SAXException 
	 * @see g.prog.CppUtilities.ALL_SOURCE_EXTENSIONS
	 */
	public static String[] get_SourceFilesFromPrj(String aVcprojPath) throws IOException, SAXException
	{
		return get_FilesFromPrj ( aVcprojPath , CppUtilities.ALL_SOURCE_EXTENSIONS );
	}

	/**
	 * Given a visual C project, it returns the list of all items, where allowed extension are to be specified. 
	 * @param aVcprojPath Path of the visual c studio project.
	 * @param aAllowedExtensions Array of allowed extensions or null for all (i.e. { ".cpp" , ".h" } ). 
	 * @return
	 * @throws IOException  Problems accessing to project path.
	 * @throws SAXException Problems with project format(XML).
	 */
	public static String[] get_FilesFromPrj(String aVcprojPath,String[] aAllowedExtensions)throws 
		IOException,SAXException
	{		
		try
		{
			XPath x_path_creator =  XPathFactory.newInstance().newXPath();			
			DocumentBuilder doc_builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
			ArrayList<String> result = new ArrayList<String>();
			
			Document doc = doc_builder.parse(aVcprojPath);
			String vc_project_dir = new File(aVcprojPath).getParent();
						
			XPathExpression files_xpath_expr = x_path_creator.compile(FILES_XPATH);
			XPathExpression single_file_xpath_expr = x_path_creator.compile("//File");
			
			//String email = xPath.compile(expression).evaluate(doc); //read an xml node using xpathNode 
			Node vc_project_file_node =(Node) files_xpath_expr.evaluate(doc, XPathConstants.NODE); //read a nodelist using xpathNodeList 
			NodeList source_node_list = (NodeList)single_file_xpath_expr.evaluate(vc_project_file_node,XPathConstants.NODESET);
			
			for (int i = 0; i < source_node_list.getLength(); i++)
			{
				Node source_code_node = source_node_list.item(i);
				
				String relative_path = source_code_node.getAttributes().getNamedItem("RelativePath").getNodeValue();
				File relative_item = new File(relative_path);
				File file_item = relative_item.isAbsolute()?new File(relative_path):new File( vc_project_dir + File.separator + relative_path); 
								
				if(m_MatchExtension(file_item,aAllowedExtensions))
				{
					result.add(file_item.getCanonicalPath());
				}
			}			
			
			return result.toArray(new String[result.size()]);
		} 
		catch (ParserConfigurationException e)
		{
			new FatalException(e).doStandardAction();
		} 
		catch (XPathExpressionException e)
		{
			new FatalException(e).doStandardAction();
		} 

		return null;
	}
	
	private static boolean m_MatchExtension(File aFileItem,String[] aAllowedExtensions)
	{
		if (aAllowedExtensions==null)
		{
			return true;
		}
		else
		{
			for (String ext : aAllowedExtensions)
			{
				if (aFileItem.getName().endsWith(ext))
				{
					return true;
				}
			}
			
			return false;
		}
	}

	public static void main(String[] args)
	{		
		String test_file = "C:\\temp\\svn\\dev\\prj\\Win32\\VisualStudio2005\\glib_compatibility\\glib_compatibility.vcproj";
		
		try
		{
			for (String source_path : get_SourceFilesFromPrj(test_file))
			{
				System.out.println(source_path);
			}
		} 
		catch (IOException e)
		{
			System.err.println(e.getMessage());
		} 
		catch (SAXException e)
		{
			System.err.println(e.getMessage());
		}
	}
}
