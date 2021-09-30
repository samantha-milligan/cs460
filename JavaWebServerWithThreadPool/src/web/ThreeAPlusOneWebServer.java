package web;

import java.io.*;
import java.net.*;
import java.util.*;

import java.util.logging.Level;
import java.util.logging.Logger;




class ThreeAPlusOneWebServer {

    // where to print log
    static PrintStream log = null;

    // takes idle worker threads
    static Vector threads = new Vector();


    // web servers port it is listening on
    static int port = 23657;

    // timeout on client connections
    static int timeout = 5000;

    // max # worker threads
    static int workers = 5;

    // default constructor
    ThreeAPlusOneWebServer() {}

    // print to stdout
    protected static void print(String s) {
        System.out.println(s);
    }

    // print to the log file
    static void log(String s) {
        synchronized (log) {
            log.println(s);
            log.flush();
        }
    }

    synchronized void workerHasStarted() {
        notify();
    }

    synchronized void run() throws Exception {

        /* start worker threads */
        for (int i = 0; i < workers; ++i) {
            ThreeAPlusOneWorker worker = new ThreeAPlusOneWorker(this, "worker #" + i);
            wait();
            threads.addElement(worker);
        }


        // ServerSocket ss = new ServerSocket(port);

        ServerSocket ss = null;
        
        try{
            ss = new ServerSocket(port);
        }  catch (IOException ex) {
            Logger.getLogger(ThreeAPlusOneWebServer.class.getName()).log(Level.SEVERE, null, ex);
            System.err.println("Error starting server on port " + port);
            System.exit(1);
        }
        ThreeAPlusOneWorker worker = null;

        print("Start server loop");
        while (true) {

            Socket clientSocket = ss.accept();

            synchronized (threads) {
                if (threads.isEmpty()) {
                    worker = new ThreeAPlusOneWorker(this, "additional worker");
                    wait();
                    worker.setSocket(clientSocket);
                } else {
                    worker = (ThreeAPlusOneWorker) threads.elementAt(0);
                    threads.removeElementAt(0);
                    worker.setSocket(clientSocket);
                }
            }
        }

    }

    public static void main(String[] args) throws Exception {

        (new ThreeAPlusOneWebServer()).run();

    }
}
