package package_1;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.lang.Thread;

public class EchoServer {
    
	private static ServerSocket serverSocket;
	private static int port;
	Socket clientSocket = null;
	
	public EchoServer(int port) {
	    try {
	        EchoServer.port = port;
	        EchoServer.serverSocket = new ServerSocket(port);
	    } catch (IOException ex) {
	        Logger.getLogger(EchoServer.class.getName()).log(Level.SEVERE, null, ex);
	        System.err.println("Error starting server on port " + port);
	        System.exit(1);
	    }
	
	    EchoServer.port = port;
	}

	public void runServerLoop() throws IOException {
	
	    System.out.println("Echo server started");
	
	    while (true) {
	    	
	        System.out.println("Waiting for connections on port #" + port);
	        try {
		        Socket cliSocket = serverSocket.accept();
		        new Thread(new EchoThread(cliSocket)).start();
	        }
	        catch(IOException ioe) { 
	            System.out.println("Exception found on accept. Ignoring. Stack Trace :"); 
	            ioe.printStackTrace(); 
	         }  
	    }
	    
	}
	
	public static void main(String args[]) throws Exception {
        // create instance of echo server
        // note that hard-coding the port is bad, here we do it just for simplicity reasons
        EchoServer echoServer = new EchoServer(23657);

        // fire up server loop
        echoServer.runServerLoop();
    }
}
