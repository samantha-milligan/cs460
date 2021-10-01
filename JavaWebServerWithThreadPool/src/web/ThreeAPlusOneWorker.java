package web;

import java.io.*;
import java.net.*;
import java.util.*;

class ThreeAPlusOneWorker extends Thread {

    final static int BUF_SIZE = 2048;
    static final byte[] EOL = {(byte) '\r', (byte) '\n'};

    /* buffer to use for requests */
    byte[] buffer;
    /* Socket to client we're handling */
    private Socket socket;
    /* ThreeAPlusOneWebServer the worker works for */
    ThreeAPlusOneWebServer webServer;

    ThreeAPlusOneWorker(ThreeAPlusOneWebServer webServer, String name) {
        super(name);
        this.webServer = webServer;
        buffer = new byte[BUF_SIZE];
        this.start();
    }

    synchronized void setSocket(Socket socket) {
        this.socket = socket;
        notify();
    }

    public synchronized void run() {

        webServer.workerHasStarted();

        while (true) {
            /* nothing to do */
            try {
                wait();
            } catch (InterruptedException e) {
                /* should not happen */
                continue;
            }

            try {
                handleClient();
            } catch (Exception e) {
                e.printStackTrace();
            }
            /* go back in wait queue if there's fewer
             * than numHandler connections.
             */

            Vector pool = webServer.threads;
            synchronized (pool) {
                if (pool.size() >= webServer.workers) {
                    /* too many threads, exit this one */
                    return;
                } else {
                    pool.addElement(this);
                }
            }
        }
    }

    void handleClient() throws IOException {
        System.out.println("Client connected.\n");
        
        DataInputStream fromClient = new DataInputStream(socket.getInputStream());
        DataOutputStream toClient = new DataOutputStream(socket.getOutputStream());

        int numSteps = 0;
        int integer = fromClient.readByte();
        System.out.print("Integer: ");
        System.out.println(integer);

        while (integer != 1) {
            if(integer % 2 == 0 ) {
                integer /= 2;
            }
            else {
                integer = integer * 3 + 1;
            }
            
            numSteps += 1;
        }
            toClient.writeByte(numSteps);
            System.out.print("Steps: ");
            System.out.println(numSteps);
            
            System.out.println("\nConnection closed.");
            socket.close();
            
    }
}
