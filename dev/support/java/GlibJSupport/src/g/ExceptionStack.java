package g;

import java.awt.Dialog.ModalityType;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JTextArea;
import javax.swing.SpringLayout;

public class ExceptionStack {

	static class MyFrame implements ActionListener
	{
		private JDialog mDialog;
		private JTextArea mTextMessage;
		private JTextArea mTextContextList;
		private JButton mButtonOk;
		
		MyFrame()
		{
			mDialog = new JDialog();
			mTextMessage = new JTextArea();
			mTextContextList = new JTextArea();
			mButtonOk = new JButton();

			mDialog.setPreferredSize(new Dimension(350, 400));
			
			mTextMessage.setBorder(BorderFactory.createRaisedSoftBevelBorder());
			mTextMessage.setEditable(false);
			mTextContextList.setBorder(BorderFactory.createRaisedSoftBevelBorder());
			mTextContextList.setEditable(false);
			mButtonOk.setText("Ok");
			
			SpringLayout layout = new SpringLayout();
					
			mDialog.setLayout(layout);
									
			mDialog.getContentPane().add(mTextMessage);
			mDialog.getContentPane().add(mTextContextList);
			mDialog.getContentPane().add(mButtonOk);
			
			//creating vertical layout with this order
			//mTextMessage on top with left and right border placed at a distance of 5 pixels from left and right border of dialog  
			layout.putConstraint(SpringLayout.NORTH, mTextMessage , 5 , SpringLayout.NORTH, mDialog.getContentPane());		
			layout.putConstraint(SpringLayout.EAST, mTextMessage, -5,SpringLayout.EAST, mDialog.getContentPane());			
			layout.putConstraint(SpringLayout.WEST, mTextMessage , 5, SpringLayout.WEST, mDialog.getContentPane());
			//mTextContextList placed 5 pixel under mTextMessage and aligned in left and right border with it.   			
			layout.putConstraint(SpringLayout.NORTH, mTextContextList,5,SpringLayout.SOUTH, mTextMessage);
			layout.putConstraint(SpringLayout.WEST, mTextContextList , 0, SpringLayout.WEST, mTextMessage);
			layout.putConstraint(SpringLayout.EAST, mTextContextList , 0, SpringLayout.EAST, mTextMessage);		
			//mButtonOk 5 pixel from dialog left border and placed 5 pixel under mTextContextList
			layout.putConstraint(SpringLayout.NORTH, mButtonOk , 5, SpringLayout.SOUTH, mTextContextList);
			layout.putConstraint(SpringLayout.WEST, mButtonOk , 5, SpringLayout.WEST, mDialog.getContentPane());
			//dialog bottom border 5 pixel from dialog bottom 
			layout.putConstraint(SpringLayout.SOUTH, mDialog.getContentPane() , 5 , SpringLayout.SOUTH,mButtonOk);

			mButtonOk.addActionListener(this);
			mDialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
			mDialog.setModalityType(ModalityType.APPLICATION_MODAL);
			mDialog.setAlwaysOnTop(true);			
		}

		public void setMsg(String aMsg)
		{
			mTextMessage.setText(aMsg);
			mDialog.pack();
		}

		public String getMsg()
		{
			return mTextMessage.getText();
		}

		public void setTag(String aTag)
		{
			mDialog.setTitle(aTag);			
		}

		public String getTag()
		{
			return mDialog.getTitle();
		}

		public void setContextList(String aContextList)
		{
			mTextContextList.setText(aContextList);
			mDialog.doLayout();
			mDialog.pack();
		}

		public String getContextList()
		{
			return mTextContextList.getText();
		}				
		
		public void doShow()
		{
			Dimension screen_size = Toolkit.getDefaultToolkit().getScreenSize();
			
			mDialog.pack();
			mDialog.setLocation(new Point(
					(screen_size.width-mDialog.getSize().width)/2,
					(screen_size.height-mDialog.getSize().height)/2 ));
			mDialog.setVisible(true);			
		}

		@Override
		public void actionPerformed(ActionEvent e)
		{
			if (e.getSource()==mButtonOk)
			{
				mDialog.setVisible(false);
			}			
		}
	}
	
	public static void do_Show ( String aTag , String aMessage , String aContextList )
	{		
		MyFrame the_frame = new MyFrame();
		
		the_frame.setTag(aTag);
		the_frame.setMsg(aMessage);
		the_frame.setContextList(aContextList);		
		the_frame.doShow();
	}
	
	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception 
	{
		String tag ="";
		String msg = "";
		String context_list = "";
		
		MyFrame my_frame = new MyFrame();
		
		switch(args.length)
		{
		case 0:
			tag = ConsoleArgumentConverter.do_ConvertFromConsole("tag");
			msg = ConsoleArgumentConverter.do_ConvertFromConsole("Is \\\\ My Message \\n Miao");
			context_list = ConsoleArgumentConverter.do_ConvertFromConsole("Context \\n Context 2");
			
			my_frame.setTag(tag);
			my_frame.setMsg(msg);
			my_frame.setContextList(context_list);
			my_frame.doShow();
			break;
		case 3:
			tag = ConsoleArgumentConverter.do_ConvertFromConsole(args[0]);
			msg = ConsoleArgumentConverter.do_ConvertFromConsole(args[1]);
			context_list = ConsoleArgumentConverter.do_ConvertFromConsole(args[2]);
			
			my_frame.setTag(tag);
			my_frame.setMsg(msg);
			my_frame.setContextList(context_list);
			my_frame.doShow();
			break;
		default:
			throw new Exception("Wrong input parameter number:\nit must be 3(or 0 for test)");
		}
		
		// 
		System.exit(0);
	}

}
