/*
	Panneau de contrôle du robot semi-autonome
	By : DAVIAU Paul & CHANVIN Paul
	------------------------------------------
	(Projet ENSEA 2A 2017-2018)
 */

// Bibliothèques nécessaires :
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

class RobotENSEA extends JFrame implements ActionListener{
	
	// Déclaration de variables globales :
	JTextField console_cmd;
	JTextArea console;

	boolean connected,cleared;
	Socket socket;
	DataInputStream userInput;
	PrintStream theOutputStream;

	// Constructeur :
	public RobotENSEA(String titre, int locX, int locY, int sizeX, int sizeY){
		super(titre);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLocation(locX,locY);
		setSize(sizeX,sizeY);

		// conteneur contient un BorderLayout remplace
		// setLayout(new BorderLayout()); dans awt
		Container contentPane = getContentPane();

		// ---------- Création des menus --------------
		JMenuBar m = new JMenuBar();

		// ---- Menu "Robot" ----
		JMenu m_dash = new JMenu("Robot");
		// Sous-menu "Connecter"
		JMenuItem m_dash_co = new JMenuItem("Connect");
		m_dash_co.addActionListener(this);
		m_dash.add(m_dash_co);
		// Sous-menu "Deconnecter"
		JMenuItem m_dash_disco = new JMenuItem("Disconnect");
		m_dash_disco.addActionListener(this);
		m_dash.add(m_dash_disco);
		// On sépare d'un trait
		m_dash.addSeparator();
		// Sous-menu "Quitter"
		JMenuItem m_dash_exit = new JMenuItem("Exit");
		m_dash_exit.addActionListener(this);
		m_dash.add(m_dash_exit);

		m.add(m_dash);

		// ---- Menu "A propos" ----
		JMenu m_about = new JMenu("About/Help");
		// Sous-menu "Aide"
		JMenuItem m_about_help = new JMenuItem("Help");
		m_about_help.addActionListener(this);
		m_about.add(m_about_help);
		// Séparateur
		m_about.addSeparator();
		// Sous-menu "Auteurs"
		JMenuItem m_about_authors = new JMenuItem("Authors");
		m_about_authors.addActionListener(this);
		m_about.add(m_about_authors);

		m.add(m_about);

		setJMenuBar(m);
		// --------------------------------------------------------

		// ------------- Création du panneau console --------------
		// Barre d'entrée de commandes :
		console_cmd = new JTextField("");
		console_cmd.addActionListener(this);
		contentPane.add(console_cmd,"South");
		
		// Zone d'affichage :
		console = new JTextArea("");
		console.setEditable(false);
		contentPane.add(console,"Center");

		// --------------------------------------------------------
		// --------------------------------------------------------
		connected = false;
		cleared = false;

		// --------------------------------------------------------
		setVisible(true);
	}

	// ----- Gestion des évènements (ActionListener) ----------
	public void actionPerformed(ActionEvent e){
		String cmd = e.getActionCommand();
		String console_resp = console.getText();
		JOptionPane msg = new JOptionPane();
		if(cmd.equalsIgnoreCase("exit")){
			// =========================== Extinction du panneau de contrôle =================================
			if (connected) {
				console.append("Veuillez vous déconnecter du robot avant de quitter le panneau de contrôle.");
			} else {
				System.exit(0);
			}
		} else if(cmd.equalsIgnoreCase("connect")){
			// =========================== Connexion au robot ===============================================
			if (!connected) {
				console.append("Connexion au robot...\n");
				try {
					InetAddress serveur = InetAddress.getByName("172.24.1.1");
					socket = new Socket(serveur,63744);  // ip 172.24.1.1 ; port 63744
					connected = true;
					console.append("Connexion réussie !\n");
				} catch (Exception ex) {
					console.append("Erreur de connexion au robot.\n");
					errorLogFunction(ex);
				}
				if (connected) {
					try {
						console.append("Initialisation du robot.\n");
						Timestamp timestamp = new Timestamp(System.currentTimeMillis());
						PrintStream out = new PrintStream(socket.getOutputStream());
						String comnd = "init:"+String.valueOf(timestamp.getTime());  // mise à jour de l'horloge de la Raspberry
						out.println(comnd);
						console.append("Initialisation terminée.");
					} catch (Exception ex) {
						console.append("Erreur de communication avec le robot.\n");
						errorLogFunction(ex);
					}
				}
			} else {
				console.append("Connexion au robot déjà établie.\n");
			}
		} else if(cmd.equalsIgnoreCase("disconnect")){
			// =========================== Déconnexion simple du robot ===============================================
			if (connected){
				try {
					socket.close();
					connected = false;
					console.append("Déconnexion réussie !");
				} catch (Exception ex) {
					console.append("Erreur de déconnexion au robot.");
					errorLogFunction(ex);
				}
			} else {
				console.append("Connexion au robot inexistante.");
			}
		} else if(cmd.equalsIgnoreCase("shutdown")){
			// =========================== Extinction et déconnexion du robot ===============================================
			if (connected){
				try {
					console.append("Envoi de l'ordre d'extinction à distance du robot...\n");
					PrintStream out = new PrintStream(socket.getOutputStream());
					String comnd = "shutdown";
					out.println(comnd);
					console.append("Déconnexion en cours...\n");
					socket.close();
					connected = false;
					console.append("Déconnexion réussie !\n");
					console.append("Vous pourrez déconnecter l'alimentation de la Raspberry Pi.");
				} catch (Exception ex) {
					console.append("Erreur de déconnexion complète du robot.");
					errorLogFunction(ex);
				}
			}
		} else if(cmd.toLowerCase().startsWith("send")){
			// =========================== Envoi d'une instruction au robot ===============================================
			String parts[] = cmd.split(" ");
			String sout;
			if (connected && parts.length==2){
				sendComm(parts[1],socket);
			} else {
				if (!connected){
					console.append("Connexion au robot inexistante.");
				}
				if (parts.length!=2){
					console.append("Utilisation : send [command]:[args]");
				}
			}
		} else if(cmd.equalsIgnoreCase("authors")){
		// =========================== Menu "A propos" ===============================================
			console.append("Tu as appuyé sur "+cmd+"");
			msg.showMessageDialog(this,"Projet de deuxième année : RobotENSEA.\nMené par Paul DAVIAU et Paul CHANVIN\nAnnée 2017-2018");
		} else if(cmd.equalsIgnoreCase("aide") || cmd.equalsIgnoreCase("help")){
			// =========================== Aide ===============================================
			console.append("Liste des commandes disponibles :\n");
			console.append("- 'exit' : quitte le panneau de contrôle ;\n");
			console.append("- 'authors' : message des auteurs ;\n");
			console.append("- 'help' : affiche cette aide ;\n");
			console.append("- 'clear' : efface cette présente fenêtre ;\n");
			console.append("- 'connect'/'disconnect' : se connecte ou se déconnecte au/du robot ;\n");
			console.append("- 'send [cmd]' : envoie la commande 'cmd' au robot lorsqu'il est connecté ;\n");
			console.append("- 'shutdown' : éteint complètement le robot.");
		} else if(cmd.equalsIgnoreCase("clear") || cmd.equalsIgnoreCase("effacer")) {
			// =========================== Effacer la fenêtre ===============================================
			console.setText("");
			cleared = true;
		} else {
			// =========================== Commande inconnue ===============================================
			console.append("Erreur : Commande inconnue '"+cmd+"'\nTapez 'help' ou 'aide' pour une liste des commandes disponibles.");
		}
		if (cleared) {
			cleared = false;
		} else {
			console.append("\n============================\n");
		}
		console_cmd.setText("");
	}
	// --------------------------------------------------------

	// --------------------------------------------------------
	// ---------------------FUNCTIONS--------------------------
	// --------------------------------------------------------

	// error Log Function :
	// @brief : fonction d'écriture de toutes les erreurs dans
	// un fichier de journal 'robotensea_error.log'.
	// @arg : 'e' l'erreur renvoyée
	public void errorLogFunction(Exception e) {
		try {
			File monFichier = new File("robotensea_error.log");
			FileWriter fw = new FileWriter(monFichier,true);
			BufferedWriter bw = new BufferedWriter(fw);

			StringWriter sw = new StringWriter();
			PrintWriter pw = new PrintWriter(sw);
			e.printStackTrace(pw);
			String sStackTrace = sw.toString(); // stack trace as a string

			Date maDate = new Date();
			bw.write("RobotENSEA-Control-Panel-Dashboard-Error(s)-log : ");
			bw.write(maDate.toString());
			bw.newLine();
			bw.write(sStackTrace);
			bw.newLine();
			bw.close();
			console.append("Veuillez vous referer au journal d'erreurs créé.");
		} catch (Exception ex2) {
			e.printStackTrace();
			console.append("Le journal d'erreurs 'robotensea_error.log' n'a pas pu être créé/édité.\n");
			console.append("Veuillez vous referer à la console du terminal.");
		}
	}
	
	// classic log function :
	// @brief : fonction d'écriture de rapports quelconques dans
	// un fichier de journal 'robotensea_log.log'.
	// @arg : 'towrite' une chaîne de caractères
	public void classicLogFunction(String towrite) {
		try {
			File monFichier = new File("robotensea_log.log");
			FileWriter fw = new FileWriter(monFichier,true);
			BufferedWriter bw = new BufferedWriter(fw);
			
			String sStackTrace = towrite;

			Date maDate = new Date();
			bw.write("RobotENSEA-Control-Panel-Dashboard-log : ");
			bw.write(maDate.toString());
			bw.newLine();
			bw.write(sStackTrace);
			bw.newLine();
			bw.close();
			console.append("Veuillez vous référer au fichier de journal créé.\n");
		} catch (Exception ex) {
			ex.printStackTrace();
			console.append("Le fichier de journal 'robotensea_log.log' n'a pas pu être créé/édité.\n");
			console.append("Veuillez vous referer à la console du terminal.");
		}
	}

	// send command
	// @brief : simple commande d'envoi d'instruction
	// @arg : 'comnd' la commande à envoyer et 'socket' le socket courant.
	public void sendComm(String comnd, Socket socket){
		try {
			PrintStream out = new PrintStream(socket.getOutputStream());
			out.println(comnd);
			console.append("Sended : ");
			console.append(comnd);
			console.append("\n");
		} catch (Exception ex) {
			console.append("Erreur de communication avec le robot.\n");
			errorLogFunction(ex);
		}
	}

	// receive command
	// @brief : commande de réception de données
	// @arg : 'socket' le socket courant.
	// ATTENTION : FONCTION INCOMPLETE ET NON UTILISEE
	//	(soucis de gestion des buffers de reception)
	public String rcvComm(Socket socket){
		try {
			BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			String sout = "";
			String recept = "";
			if (!in.ready()){
				recept = in.readLine();
			} else {
				recept = "NONE";
			}
			// ============ RECEPT MANAGEMENT ===================
			sout = recept;
			// ==================================================
			return sout;
		} catch (Exception ex){
			console.append("Erreur de communication avec le robot.\n");
			errorLogFunction(ex);
			return "";
		}
	}

	// Main program
	public static void main(String [] args){
		RobotENSEA win = new RobotENSEA("RobotENSEA Control Panel",100,100,640,480);
	}
}


