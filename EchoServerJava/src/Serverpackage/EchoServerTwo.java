package Serverpackage;

import java.io.DataInputStream;
import java.io.DataOutputStream;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.lang.Thread;

public class EchoServerTwo {
    
private static ServerSocket serverSocket;
private static int port;
Socket clientSocket = null;

public EchoServerTwo(int port) {
    try {
        EchoServerTwo.port = port;
        EchoServerTwo.serverSocket = new ServerSocket(port);
    } catch (IOException ex) {
        Logger.getLogger(EchoServerTwo.class.getName()).log(Level.SEVERE, null, ex);
        System.err.println("Error starting server on port " + port);
        System.exit(1);
    }

    EchoServerTwo.port = port;
}
public void runServerLoop() throws IOException {

    System.out.println("Echo server started");

    while (true) {
    	
        System.out.println("Waiting for connections on port #" + port);
        try {
        Socket cliSocket = serverSocket.accept();
        EchoThread echoThread = new EchoThread(cliSocket);
        echoThread.start();
        }catch(IOException ioe) { 
            System.out.println("Exception found on accept. Ignoring. Stack Trace :"); 
            ioe.printStackTrace(); 
         }  
    }
    
}
class EchoThread extends Thread{
    Socket cliSocket;
    public EchoThread() { 
       super(); 
    } 
    EchoThread(Socket s)
    {
    	cliSocket = s; 
    }

    public void handleClient() {

        DataInputStream fromClient = null;
        DataOutputStream toClient = null;

        int charFromClient = 0;
        int state = 0;
        boolean keepGoing = true;

        // show that we are connected to client
        System.out.println("Client connected ...");

        // first get the streams
        try {
            fromClient = new DataInputStream(cliSocket.getInputStream());
            toClient = new DataOutputStream(cliSocket.getOutputStream());
        } catch (IOException e) {
            System.err.println("Error opening network streams");
            return;
        }

        // now talk to the client
        while (keepGoing) {
            try {
                charFromClient = fromClient.readByte();
                System.out.print((char)charFromClient);
            } catch (IOException e) {
                System.err.println("Error reading character from client");
                return;
            }

            try {
                toClient.writeByte(charFromClient);
            } catch (IOException e) {
                System.err.println("Error writing character to client");
                return;            }
            
            if (charFromClient == 'q') {
                System.out.println("\nBailing out!");
                keepGoing = false;
            }
        }

        try {
            clientSocket.close();
        } catch (IOException e) {
            System.err.println("Error closing socket to client");
        }

    }

    }
public static void main(String args[]) throws Exception {
        // create instance of echo server
        // note that hardcoding the port is bad, here we do it just for simplicity reasons
        EchoServer echoServer = new EchoServer(23657);

        // fire up server loop
        echoServer.runServerLoop();
    }

}
