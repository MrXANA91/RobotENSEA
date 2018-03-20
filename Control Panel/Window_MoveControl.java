import javax.swing.*;
import javax.swing.JOptionPane;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.sql.Timestamp;
import java.lang.*;
import java.io.*;
import java.beans.XMLEncoder;

import java.net.*;

class Window_MoveControl extends JFrame implements ActionListener{
	
	public Window_MoveControl(String titre, int locX, int locY, int sizeX, int sizeY){
		super(titre);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setLocation(locX,locY);
		setSize(sizeX,sizeY);
		
		Container contentPane = getContentPane();
		
		JButton btn_forward = new JButton("Forward");
		btn_forward.addActionListener(this);
		contentPane.add(btn_forward,"North");
		
		JButton btn_left = new JButton("Left");
		btn_left.addActionListener(this);
		contentPane.add(btn_left,"West");
		
		JButton btn_right = new JButton("Right");
		btn_right.addActionListener(this);
		contentPane.add(btn_right,"East");
		
		JButton btn_stop = new JButton("Stop");
		btn_stop.addActionListener(this);
		contentPane.add(btn_stop,"Center");
		
		JButton btn_backward = new JButton("Backward");
		btn_backward.addActionListener(this);
		contentPane.add(btn_backward,"South");
		
		setVisible(true);
	}
	
	public void actionPerformed(ActionEvent e){
		
	}
}