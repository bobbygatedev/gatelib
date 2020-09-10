package g.text;

import g.FatalException;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Text Template Builder class: allows the user to define a text template with
 * certain rules, template has label, that can be expanded using values of an
 * interface class.<br>
 * @see g.text.TextTemplateBuilder.setTemplateString
 * @author Bobby Gate
 */
public class TextTemplateBuilder
{
	// check for a || the for a |
	final static Pattern m_PatternSeparator = Pattern
			.compile("(\\[\\[)|(\\[)|(\\]\\])|(\\])");
	final static Pattern m_PatternLabel = Pattern
			.compile("\\[(?<lname>\\w+)(.(?<sfield>\\w+))?\\]");
	// i.e. [var:[var.name] [var.type];]
	final static Pattern m_PatternArraySectionBegin = Pattern
			.compile("\\[(?<lname>\\w+):");
	// i.e[par[,]:[par.name] [par.type]]
	final static Pattern m_PatternArraySectionWithSeparatorBegin = Pattern
			.compile("\\[(?<lname>\\w+)\\[");

	private String mOutputExpression;
	private List<InnerReplaciableItemAbstract> mListReplaciableItem;
	private Object mInterfaceInstance = null;
	private Class<?> mInterfaceClass;
	private String mTemplateString;

	/**
	 * Constructor pass the interface class.
	 * 
	 * @param aInterfaceClass
	 *            Class containing (no parameters) method or fields, that can be
	 *            expanded into template.
	 */
	public TextTemplateBuilder(Class<?> aInterfaceClass)
	{
		if (aInterfaceClass != null)
		{
			mInterfaceClass = aInterfaceClass;
		}
		else
		{
			new FatalException("Interface Class parameter can't be null!")
					.doStandardAction();
		}
	}

	private abstract class InnerReplaciableItemAbstract
	{
		protected String mName;
		protected String mReplaceExpression = null;
		protected Object mMethodOrField;

		protected int mFrom = -1;
		protected int mTo = -1;

		protected abstract void mValorizeFromInstance(Object aInterface);
		public String getReplaceExpression(){ return mReplaceExpression; }
		public String getExpression() {	return mTemplateString.substring(mFrom, mTo); }

		protected Object mGetFromMethodOrField(Object aMethodOrField,
				Object aObjectInstance)
		{
			Object the_object = null;
			
			try
			{
				if (aMethodOrField instanceof Field)
				{
					the_object = ((Field) aMethodOrField).get(aObjectInstance);
				} 
				else if (aMethodOrField instanceof Method)
				{
					the_object = ((Method) aMethodOrField).invoke(aObjectInstance);
				}
				else
				{
					new FatalException("Neither a method nor a Field!")
							.doStandardAction();
				}
			}
			catch (Exception e)
			{
				new FatalException(e).doStandardAction();
			}
			
			return the_object;
		}

		protected Object mRetrieveMethodOrFieldWithoutTypeCheck(
				Class<?> aClass, String aName)
				throws NotAValidTemplateStringFormat
		{
			try
			{
				Field field = aClass.getField(aName);

				return field;
			} catch (NoSuchFieldException e)
			{
				Method method;

				try
				{
					method = aClass.getMethod(aName);
				} catch (NoSuchMethodException e1)
				{
					try
					{
						// no param method call
						method = aClass.getMethod("get" + aName);
					} catch (NoSuchMethodException e2)
					{
						throw new NotAValidTemplateStringFormat(
								"Neither a method, nor a field with name "
										+ aName + " found!");
					}
				}

				return method;
			}
		}
	}

	private class InnerReplaciableLabel extends InnerReplaciableItemAbstract
	{
		private Object mSubMethodOrField = null;
		private String mSubName = null;

		public InnerReplaciableLabel(int aFrom, int aTo)
				throws NotAValidTemplateStringFormat
		{
			mFrom = aFrom;
			mTo = aTo;

			String expression = getExpression();

			Matcher matcher = m_PatternLabel.matcher(expression);

			if (matcher.matches())
			{
				mName = matcher.group("lname");
				mSubName = matcher.group("sfield");
			}

			mMethodOrField = mRetrieveMethodOrField(mInterfaceClass, mName,
					mSubName);

			if (mSubName != null)
			{
				if (mMethodOrField instanceof Field)
				{
					mSubMethodOrField = mRetrieveMethodOrField(
							((Field) mMethodOrField).getType(), mSubName, null);
				} else if (mMethodOrField instanceof Method)
				{
					mSubMethodOrField = mRetrieveMethodOrField(
							((Method) mMethodOrField).getReturnType(),
							mSubName, null);
				}
			}
		}

		/**
		 * Let's suppose that aName = Name If aSubName == null retrieve a string
		 * field called "Name" ( case sensitive ) if not found search for a
		 * method called "String Name()" or "String getName()" else (aSubName =
		 * SubName) retrieve a class field called Name that returns a class
		 * having a String field named SubName (or a Mehod String SubName() or
		 * String getSubName())
		 * 
		 * @param aClass
		 * @param aName
		 * @param aSubName
		 * @return
		 * @throws NotAValidTemplateStringFormat
		 */
		private Object mRetrieveMethodOrField(Class<?> aClass, String aName,
				String aSubName) throws NotAValidTemplateStringFormat
		{
			Object result = mRetrieveMethodOrFieldWithoutTypeCheck(aClass,
					aName);

			if (result instanceof Field)
			{
				mCheckFieldClass(((Field) result).getType(), aSubName);
			} else if (result instanceof Method)
			{
				mCheckFieldClass(((Method) result).getReturnType(), aSubName);
			} else
			{

			}

			mMethodOrField = result;

			if (mSubName != null)
			{
				if (mMethodOrField instanceof Field)
				{
					mSubMethodOrField = mRetrieveMethodOrField(
							((Field) mMethodOrField).getType(), mSubName, null);
				} else if (mMethodOrField instanceof Method)
				{
					mSubMethodOrField = mRetrieveMethodOrField(
							((Method) mMethodOrField).getReturnType(),
							mSubName, null);
				}
			}

			return result;
		}

		/**
		 * If aSubName == null aClass must be String.class otherwise search for
		 * a method or field named 'aSubName'
		 * 
		 * @param aClass
		 * @param aSubName
		 * @throws NotAValidTemplateStringFormat
		 */
		private void mCheckFieldClass(Class<?> aClass, String aSubName)
				throws NotAValidTemplateStringFormat
		{
			if (aSubName == null)
			{
				if (aClass != String.class)
				{
					throw new NotAValidTemplateStringFormat(
							"Simple Field must be of type String!");
				}
			} else
			{
				mRetrieveMethodOrField(aClass, aSubName, null);
			}
		}

		@Override
		protected void mValorizeFromInstance(Object aInterface)
		{
			Object the_object;

			the_object = mGetFromMethodOrField(mMethodOrField,
					(Object) aInterface);

			if (mSubMethodOrField == null)
			{
				mReplaceExpression = (String) the_object;
			} else
			{
				mReplaceExpression = (String) mGetFromMethodOrField(
						mSubMethodOrField, the_object);
			}
		}
	}

	private class InnerReplaciableArraySection extends
			InnerReplaciableItemAbstract
	{
		protected String mTemplate;
		protected Class<?> mArrayItemClass;
		protected TextTemplateBuilder mIterateTextBuilder;

		protected InnerReplaciableArraySection(){}

		public InnerReplaciableArraySection(int aFrom, int aTo)
				throws NotAValidTemplateStringFormat
		{
			mFrom = aFrom;
			mTo = aTo;
			String expression = getExpression();

			Matcher matcher = m_PatternArraySectionBegin.matcher(expression);

			if (matcher.find())
			{
				mName = matcher.group("lname");

				mTemplate = expression
						.substring(matcher.end(), expression.length() - 1)
						.replace("[[", "[").replace("]]", "]");

				mRetrieveMethodOrField();

				mIterateTextBuilder = new TextTemplateBuilder(mArrayItemClass);
				mIterateTextBuilder.setTemplateString(mTemplate);
			} 
			else
			{
				throw new NotAValidTemplateStringFormat("Expression "
						+ expression
						+ "not matching a ReplaciableArraySection!");
			}
		}

		protected void mRetrieveMethodOrField()
				throws NotAValidTemplateStringFormat
		{
			Object method_or_field = mRetrieveMethodOrFieldWithoutTypeCheck(
					mInterfaceClass, mName);

			if (method_or_field instanceof Field)
			{
				Field field = (Field) method_or_field;

				if (!field.getType().isArray())
				{
					throw new NotAValidTemplateStringFormat("Field of field "
							+ field.getName() + " must be an array!");
				}

				mArrayItemClass = field.getType().getComponentType();
			} else if (method_or_field instanceof Method)
			{
				Method method = (Method) method_or_field;

				if (!method.getReturnType().isArray())
				{
					throw new NotAValidTemplateStringFormat(
							"Returned type of method " + method.getName()
									+ " must be an array!");
				}

				mArrayItemClass = method.getReturnType().getComponentType();
			}

			mMethodOrField = method_or_field;
		}

		@Override
		protected void mValorizeFromInstance(Object aInterfaceInstance)
		{
			Object[] array_field_instances = (Object[]) mGetFromMethodOrField(
					mMethodOrField, aInterfaceInstance);

			mReplaceExpression = "";

			for (Object array_field_single_instance : array_field_instances)
			{
				mIterateTextBuilder
						.setInterfaceInstance(array_field_single_instance);
				mReplaceExpression += mIterateTextBuilder.getOutputExpression();
			}
		}
	}

	private class InnerReplaciableArraySectionWithSeparator extends
			InnerReplaciableArraySection
	{
		String mSeparator;

		public InnerReplaciableArraySectionWithSeparator(int aFrom, int aTo)
				throws NotAValidTemplateStringFormat
		{
			mFrom = aFrom;
			mTo = aTo;
			String expression = getExpression();

			Matcher matcher = m_PatternArraySectionWithSeparatorBegin
					.matcher(expression);

			if (matcher.find())
			{
				mName = matcher.group("lname");

				String temp = expression.substring(matcher.end(),
						expression.length() - 1);

				matcher = m_PatternSeparator.matcher(temp);

				while (matcher.find())
				{
					String separator = matcher.group();

					if (separator.equals("["))
					{
						throw new NotAValidTemplateStringFormat(
								"Too much [ in separator expression!");
					} else if (separator.equals("]"))
					{
						// check for :
						if (temp.charAt(matcher.end()) == ':')
						{
							mTemplate = temp.substring(matcher.end() + 1)
									.replace("[[", "[").replace("]]", "]");
							mSeparator = temp.substring(0, matcher.end() - 1)
									.replace("[[", "[").replace("]]", "]");

							mRetrieveMethodOrField();

							mIterateTextBuilder = new TextTemplateBuilder(
									mArrayItemClass);
							mIterateTextBuilder.setTemplateString(mTemplate);
							return;
						} else
						{
							throw new NotAValidTemplateStringFormat(
									": not found after separator");
						}
					}
				}
			}

			throw new NotAValidTemplateStringFormat("Expression " + expression
					+ "not matching a ReplaciableArraySectionWithSeparator!");
		}

		@Override
		protected void mValorizeFromInstance(Object aInterfaceInstance)
		{
			Object[] array_field_instances = (Object[]) mGetFromMethodOrField(
					mMethodOrField, aInterfaceInstance);

			mReplaceExpression = "";

			if (array_field_instances.length > 0)
			{
				mIterateTextBuilder
						.setInterfaceInstance(array_field_instances[0]);
				mReplaceExpression += mIterateTextBuilder.getOutputExpression();

				for (int i = 1; i < array_field_instances.length; i++)
				{
					mIterateTextBuilder
							.setInterfaceInstance(array_field_instances[i]);
					mReplaceExpression += mSeparator
							+ mIterateTextBuilder.getOutputExpression();
				}
			}
		}
	}

	/**
	 * The signum || will replaced by ||
	 * 
	 * @author Bobby Gate
	 */
	private class InnerReplaciableSignum extends InnerReplaciableItemAbstract
	{
		public InnerReplaciableSignum(int aFrom, int aTo)
		{
			mFrom = aFrom;
			mTo = aTo;
			mName = "From " + mFrom + " to " + mTo;
		}

		@Override
		protected void mValorizeFromInstance(Object aInterface)
		{
			// take just half
			mReplaceExpression = getExpression().substring(1);
		}
	}

	private class InnerRawExpression
	{
		private int mFrom;
		private int mTo;

		public InnerRawExpression(int aFrom, int aTo)
		{
			mFrom = aFrom;
			mTo = aTo;
			getExpression();
		}

		public String getExpression() {	return mTemplateString.substring(mFrom, mTo); }

		public InnerReplaciableItemAbstract mGetReplaciableItem()
				throws NotAValidTemplateStringFormat
		{
			String expression = getExpression();

			if (expression.equals("[[") || expression.equals("]]"))
			{
				return new InnerReplaciableSignum(mFrom, mTo);
			} else
			{
				Matcher matcher = m_PatternLabel.matcher(expression);

				if (matcher.matches())
				{
					return new InnerReplaciableLabel(mFrom, mTo);
				} else
				{
					matcher = m_PatternArraySectionBegin.matcher(expression);

					if (matcher.find() && matcher.start() == 0)
					{
						return new InnerReplaciableArraySection(mFrom, mTo);
					} else
					{
						matcher = m_PatternArraySectionWithSeparatorBegin
								.matcher(expression);

						if (matcher.find() && matcher.start() == 0)
						{
							return new InnerReplaciableArraySectionWithSeparator(
									mFrom, mTo);
						}
					}
				}
			}

			throw new NotAValidTemplateStringFormat(expression
					+ " doesn't correspond to any valid format");
		}
	}

	/**
	 * Gets the output generated expression ( computed after call
	 * setInterfaceInstance ).
	 * 
	 * @see g.text.TextTemplateBuilder.setInterfaceInstance
	 * @return
	 */
	public String getOutputExpression() { return mOutputExpression; }

	/**
	 * Gets the object encapsulating the interface.
	 * 
	 * @return
	 */
	public Object getInterfaceInstance() { return mInterfaceInstance; }

	/**
	 * Sets the instance of the object, then compute the output expression.
	 * @param aInterfaceInstance Instance of the class containing the values of the label.
	 */
	public void setInterfaceInstance(Object aInterfaceInstance)
	{
		if (mInterfaceClass.isInstance(aInterfaceInstance))
		{
			for (InnerReplaciableItemAbstract repl_item : mListReplaciableItem)
			{
				repl_item.mValorizeFromInstance(aInterfaceInstance);
			}

			mInterfaceInstance = aInterfaceInstance;
			mOutputExpression = mComputeExpression();
		}
		else
		{
			new FatalException("The object must be a valid instance of class "
					+ mInterfaceClass.getName()).doStandardAction();
		}
	}

	/**
	 * Sets the template of the builder.<br>
	 * Template is a string containing some format label with any of the followings format:<br>
	 *   - [field] 
	 *   	this label is replaced by a String field in interface class named 'field' <br>
	 *   	or a no-parametric String method called 'String field() or 'String getField()'<br>
	 *   - [arrayfield[sep]:[[arrayfield.item expression]]] (like in '[Par[,]:[[Type]]')<br>
	 *   	this label is expanded repeatedly with an array field no-parametric String method called arrayfield<br>
	 *   	[[arrayfield.item expression]]] is a sub-expression with same rules of top level expression<br>
	 *   	and whose field/methods come from arrayfield type.<br>
	 *   	[sep] is the separator
	 *   An example<br>
	 *   <br>
	 *   class NameSurName<br>
	 *   {<br>
	 *   	public String getName()..<br>
	 *   	public String getSurname()..<br>
	 *   }<br>
	 *   <br>
	 *   class DocCls<br> 
	 *   {<br>
	 *   	public String TeamName = "Manchester United";<br>
	 * <br>  
	 *   	public NameSurNames[] getNameList()<br> 
	 *   		{<br>
	 *   			return new NameSurName[] = {<br>
	 *   					new NameSurName("David","Beckham"),<br>
	 *   					new NameSurName("Ryan","Giggs")<br>
	 *   				};<br>
	 *   		} <br>
	 *   }<br>
	 *   <br>
	 *   template = <br>
	 *   "Team [TeamName]\n"<br>
	 *   "[NameList[,]:[ [Name]] [[SurName]] ]\n"<br>
	 *   Output will be:<br>
	 *   Team Manchester United<br>
	 *    David Beckham , Ryan Giggs<br>
	 *   <br>
	 *    
	 * @param aTemplateString Template to be expanded.
	 * @throws NotAValidTemplateStringFormat Format of template is not valid. 
	 */
	public void setTemplateString(String aTemplateString)
			throws NotAValidTemplateStringFormat
	{
		mTemplateString = aTemplateString;
		mListReplaciableItem = new ArrayList<InnerReplaciableItemAbstract>();

		List<InnerRawExpression> raw_expressions = mDoParse(aTemplateString);

		for (InnerRawExpression raw_expression : raw_expressions)
		{
			InnerReplaciableItemAbstract repl_item = raw_expression
					.mGetReplaciableItem();

			mListReplaciableItem.add(repl_item);
		}
	}

	/**
	 * Calls setTemplateString reading to end an input stream.<br>
	 * For using a template from an internal resource use this code snippet:<br>
	 * <br>
	 * class TemplateClass { ... };<br>
	 * TextTemplateBuilder instance = new TextTemplateBuilder(TemplateClass.class);<br>
	 * instance.setTemplateStream(getClass().getResourceAsStream(REFERENCE_TEMPLATE_RESOURCE));<br>
	 * instance.setInterfaceInstance(this);<br>
	 * @see g.text.TextTemplateBuilder.setTemplateString
	 * @param aInputStream Input stream containing the template string.
	 * @throws NotAValidTemplateStringFormat Format of template is not valid. 
	 */
	public void setTemplateStream(InputStream aInputStream)
			throws NotAValidTemplateStringFormat
	{
		try
		{
			byte[] file_bytes = new byte[aInputStream.available()];
			aInputStream.read(file_bytes);
			String file_content = new String(file_bytes);

			setTemplateString(file_content);
		} 
		catch (IOException e)
		{
			new FatalException(e).doStandardAction();
		}
	}

	/**
	 * build output expression by reading the list of replaceable items and
	 * adding the replace expression to the end
	 */
	private String mComputeExpression()
	{
		String result = "";
		int offset = 0;

		for (InnerReplaciableItemAbstract repl_item : mListReplaciableItem)
		{
			result += mTemplateString.substring(offset, repl_item.mFrom);
			result += repl_item.getReplaceExpression();
			offset = repl_item.mTo;
		}

		result += mTemplateString.substring(offset);
		return result;
	}

	private List<InnerRawExpression> mDoParse(String aTemplateString)
			throws NotAValidTemplateStringFormat
	{
		List<InnerRawExpression> result = new ArrayList<InnerRawExpression>();

		Matcher matcher = m_PatternSeparator.matcher(aTemplateString);

		int start_expr = -1;
		int par_count = 0;

		while (matcher.find())
		{
			String signum = matcher.group();

			if (signum.equals("["))
			{
				if (start_expr == -1)
				{
					start_expr = matcher.start();
				}

				par_count++;
			} 
			else if (signum.equals("]"))
			{
				if (--par_count == 0)
				{
					InnerRawExpression raw_expr = new InnerRawExpression(
							start_expr, matcher.end());
					result.add(raw_expr);
					start_expr = -1;
				}
			}
			else if (signum.equals("[[") || signum.equals("]]"))
			{
				// else the double || lies inside another expression i.e. such
				// in |decl||,:[par.type] [par.name]|
				if (start_expr == -1)
				{
					InnerRawExpression raw_expr = new InnerRawExpression(
							matcher.start(), matcher.end());
					result.add(raw_expr);
				}
			}
		}

		if (par_count > 0)
		{
			// not closed expression
			throw new NotAValidTemplateStringFormat("The '[' occurence in "
					+ start_expr + " wasn't closed!");
		}

		return result;
	}

	static class InnerTestInterfaceClass
	{
		public String myField = "myFieldValue";

		public class Decl
		{
			public String Type;
			public String Name;
		}

		public Decl[] Decl()
		{
			List<Decl> list = new ArrayList<InnerTestInterfaceClass.Decl>();

			{
				Decl result = new Decl();

				result.Type = "int32";
				result.Name = "aPar32";

				list.add(result);
			}

			{
				Decl result = new Decl();

				result.Type = "int64";
				result.Name = "aPar64";

				list.add(result);
			}

			return list.toArray(new Decl[list.size()]);
		}

		public Decl[] getPar()
		{
			List<Decl> list = new ArrayList<InnerTestInterfaceClass.Decl>();

			{
				Decl result = new Decl();

				result.Type = "int1";
				result.Name = "aPar1";

				list.add(result);
			}

			{
				Decl result = new Decl();

				result.Type = "int2";
				result.Name = "aPar2";

				list.add(result);
			}

			return list.toArray(new Decl[list.size()]);
		}
	}

	public static void main(String[] args) throws Exception
	{
		try
		{
			String test_expression = "[myField]\nvoid function([Par[,]:[[Type]] [[Name]]])\n{\n[Decl:[[Type]] [[Name]];\n]\na[[1]]=2;\n}\n";

			InnerTestInterfaceClass my_if = new InnerTestInterfaceClass();

			TextTemplateBuilder ttb = new TextTemplateBuilder(
					InnerTestInterfaceClass.class);

			ttb.setTemplateString(test_expression);
			ttb.setInterfaceInstance(my_if);

			String output_expression = ttb.getOutputExpression();

			System.out.printf("Template was:\n%s\nOutput was:\n%s\n",
					test_expression, output_expression);
		} catch (TextTemplateBuilderException e)
		{
			System.out.printf("Parsing failed reason:\n%s\n", e.getMessage());
		}
	}
}
