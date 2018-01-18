import javax.swing.*;
import javax.swing.JOptionPane;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.lang.*;
import java.io.*;
import java.beans.XMLEncoder;

class Dashboard extends JFrame implements ActionListener,KeyListener{

	JTextField console_cmd;
	JTextArea console;

	public Dashboard(String titre, int locX, int locY, int sizeX, int sizeY){
		super(titre);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLocation(locX,locY);
		setSize(sizeX,sizeY);

		// conteneur contient un BorderLayout remplace
		// setLayout(new BorderLayout()); dans awt
		Container contentPane = getContentPane();

		// ---------- Création des menus --------------
		JMenuBar m = new JMenuBar();

		// Menu "Fichier"
		JMenu m_file = new JMenu("Fichier");
		// Sous-menu "Nouveau"
		JMenuItem m_file_new = new JMenuItem("Nouveau");
		m_file_new.addActionListener(this);
		m_file.add(m_file_new);
		// Sous-menu "Ouvrir"
		JMenuItem m_file_open = new JMenuItem("Ouvrir");
		m_file_open.addActionListener(this);
		m_file.add(m_file_open);
		// Sous-menu "Enregistrer"
		JMenuItem m_file_save = new JMenuItem("Enregistrer");
		m_file_save.addActionListener(this);
		m_file.add(m_file_save);
		// On sépare d'un trait
		m_file.addSeparator();
		// Sous-menu "Quitter"
		JMenuItem m_file_exit = new JMenuItem("Quitter");
		m_file_exit.addActionListener(this);
		m_file.add(m_file_exit);

		m.add(m_file);

		// Menu "Import/Export"
		JMenu m_impexp = new JMenu("Instructions");
		// Sous-menu "Export"
		JMenuItem m_impexp_exp = new JMenuItem("Exporter");
		m_impexp_exp.addActionListener(this);
		m_impexp.add(m_impexp_exp);
		// Sous-menu "Importer"
		JMenuItem m_impexp_imp = new JMenuItem("Importer");
		m_impexp_imp.addActionListener(this);
		m_impexp.add(m_impexp_imp);

		m.add(m_impexp);

		// Menu "A propos"
		JMenu m_about = new JMenu("A propos");
		// Sous-menu "Aide"
		JMenuItem m_about_help = new JMenuItem("Aide");
		m_about_help.addActionListener(this);
		m_about.add(m_about_help);
		// Séparateur
		m_about.addSeparator();
		// Sous-menu "Auteurs"
		JMenuItem m_about_authors = new JMenuItem("Auteurs");
		m_about_authors.addActionListener(this);
		m_about.add(m_about_authors);

		m.add(m_about);

		setJMenuBar(m);
		// --------------------------------------------------------

		// ------------- Création du panneau console --------------
		console_cmd = new JTextField("");
		console_cmd.addActionListener(this);
		contentPane.add(console_cmd,"South");

		console = new JTextArea("");
		console.setEditable(false);
		contentPane.add(console,"Center");

		// --------------------------------------------------------
		// --------------------------------------------------------
		setVisible(true);

		this.addKeyListener(this);
	}

	// ----- Gestion des évènements (ActionListener) ----------
	public void actionPerformed(ActionEvent e){
		String cmd = e.getActionCommand();
		String console_resp = console.getText();
		JOptionPane msg = new JOptionPane();
		if(cmd.equalsIgnoreCase("quitter") || cmd.equalsIgnoreCase("exit")){				// Menu "Fichier"
			System.exit(0);
		} else if(cmd.equalsIgnoreCase("auteurs") || cmd.equalsIgnoreCase("authors")){			// Menu "A propos"
			console.append("Tu as appuye sur "+cmd+"");
			msg.showMessageDialog(this,"Projet de deuxième année : RobotENSEA.\nMené par Paul DAVIAU et Paul CHANVIN\nAnnée 2017-2018");
		} else if(cmd.equalsIgnoreCase("aide") || cmd.equalsIgnoreCase("help")){
			console.append("List of available commands :\n");
			console.append("- 'exit'/'quitter' : quitter le Dashboard ;\n");
			console.append("- 'authors'/'auteurs' : message des auteurs ;\n");
			console.append("- 'help'/'aide' : affiche cette aide.");
		} else {
			console.append("Erreur : Commande inconnue '"+cmd+"'\nTapez 'help' ou 'aide' pour une liste des commandes disponibles.");
		}
		console.append("\n");
		console_cmd.setText("");
	}
	// --------------------------------------------------------

	public void keyTyped(KeyEvent e) {
	//Invoked when a key has been typed.
	//This event occurs when a key press is followed by a key release.
	}

	public void keyPressed(KeyEvent e) {
		System.out.println("Press : "+e.getKeyChar()+" ("+e.getKeyCode()+")");
	}

	public void keyReleased(KeyEvent e) {
		System.out.println("Release : "+e.getKeyChar()+" ("+e.getKeyCode()+")");
	}

	// Main program
	public static void main(String [] args){
		Dashboard win = new Dashboard("RobotENSEA Dashboard",100,100,640,480);
	}
}
