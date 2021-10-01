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
            InputStream is = new BufferedInputStream(socket.getInputStream());
            OutputStream os = socket.getOutputStream();
          
            for (int i = 0; i < BUF_SIZE; i++) {
            buffer[i] = 0;
            }
            
            int nread = 0;
            
            int r = is.read(buffer, nread, BUF_SIZE - nread);
            
            int input = (int) buffer[0];

            int steps = 0;
  
            while (input != 1) {
                if( input % 2 == 0 ) {
                    input = input / 2;
                }  
                else {
                    input = (input * 3) + 1;
                }
                
                steps = steps + 1;
            }
            
            os.write((byte) steps);
            
            System.out.println("\nConnection closed.");
            socket.close();
            
    }
}
