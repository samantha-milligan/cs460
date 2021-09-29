package web;

import java.io.*;
import java.net.*;
import java.util.*;

/*
    BIG CONCEPTS THAT APPLY THROUGHOUT THE PROGRAM
*/

/*
    Synchronized Methods

    * A thread can become the owner of an object's monitor by executing a 
        synchronized instance method of that object
        ** This becomes important later on for the notify() and wait() methods 
            of type Object
        ** Only one thread at a time can own an object's monitor
        ** 3 total ways to make a thread the owner of an object's monitor
            *** The other method we will see in this program is by executing the
                body of a synchronized statements that synchronizes on the 
                object, e.g. synchronized(object)

    * To make a method synchronized, add the synchronized keyword as you would
        for example add "static" as a keyword

    * Two effects of making a method synchronized
        ** It is not possible for two invocations of synchronized methods on the
            same object to interleave. Or, when one thread is executing a 
            synchronized method, all other threads that invoke synchronized 
            methods for the same object are suspend execution until the first
            thread is done. The next thread to run is chosen at random if 
            multiple
        ** When a synchronized methods exits, it automatically establishes a 
            happens-before relationship with any subsequent invocatin of a 
            synchronized method for the same object. This ensures that changes 
            to the state of the object are visible to all threads

    * For more info, go here to read more about synchronized methods:
        https://docs.oracle.com/javase/tutorial/essential/concurrency/syncmeth.html
*/

/*
    Type Object's notify() method

    Wakes up a single thread that is waiting on this object's monitor. If 
    another thread has a lock on the object aka is the owner of the object's 
    monitor, the thread will wait in line with any threads also waiting to 
    become the object's monitor.

    For more info, go here to read more about the notify() method:
        https://docs.oracle.com/javase/7/docs/api/java/lang/Object.html#notify()
*/

/*
    Type Object's wait() method

    Causes current thread to wait until either another thread in invokes 
    notify() or notifyAll() methods. The current method must own this object's 
    monitor.

    Thread releases ownership of this object's monitor and waits until another 
    thread notifies threads waiting on this object's monitor to wake up through 
    a call to notify() or notifyAll(). The thread then waits until it can 
    re-obtain ownership of the monitor and resumes execution.
    
    For more info, go here to read more about the wait() method:
        https://docs.oracle.com/javase/7/docs/api/java/lang/Object.html#wait()
*/

class AnnotatedWebServer {

    // where to print log
    static PrintStream log = null;

    // properties
    protected static Properties properties = new Properties();

    // takes idle worker threads
    static Vector threads = new Vector();

    // the web server's root directory
    static File root = new File(System.getProperty("user.dir") + File.separator + "docs");

    // web servers port it is listening on
    static int port = 23657;

    // timeout on client connections
    static int timeout = 5000;

    // max # worker threads
    static int workers = 5;

    // default constructor
    AnnotatedWebServer() {}
        
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
    
    // load properties from directory config
    static void loadProps() throws IOException {
        String fileString = System.getProperty("user.dir") + File.separator + "config" + File.separator + "www-server.properties";
        File file = new File(fileString);
        
        if (file.exists()) {
            InputStream propertiesInputStream = new BufferedInputStream(new FileInputStream(file));
            properties.load(propertiesInputStream);
            propertiesInputStream.close();
            
            String propertyString = properties.getProperty("root");
            if (propertyString != null) {
                root = new File(propertyString);
                if (!root.exists()) {
                    throw new Error(root + " doesn't exist");
                }
            }
            
            propertyString = properties.getProperty("timeout");
            if (propertyString != null) {
                timeout = Integer.parseInt(propertyString);
            }

            propertyString = properties.getProperty("port");
            if (propertyString != null) {
                port = Integer.parseInt(propertyString);
            }

            propertyString = properties.getProperty("workers");
            if (propertyString != null) {
                workers = Integer.parseInt(propertyString);
            }

            propertyString = properties.getProperty("log");
            if (propertyString != null) {
                print("opening log file: " + propertyString);
                log = new PrintStream(new BufferedOutputStream(
                        new FileOutputStream(propertyString)));
            } else {
                log = System.out;                
            }
        }
    }

    static void printProps() {
        print("doc root=" + root);
        print("port=" + port);
        print("timeout=" + timeout);
        print("workers=" + workers);
    }
     
    synchronized void workerHasStarted() {
        notify();
    }

    synchronized void run() throws Exception {

        /* start worker threads */
        for (int i = 0; i < workers; ++i) {
            AnnotatedWorker worker = new AnnotatedWorker(this, "worker #" + i);
            wait();
            threads.addElement(worker);
        }


        ServerSocket ss = new ServerSocket(port);
        AnnotatedWorker worker = null;

        print("Start server loop");
        while (true) {

            Socket clientSocket = ss.accept();

            synchronized (threads) {
                if (threads.isEmpty()) {
                    worker = new AnnotatedWorker(this, "additional worker");
                    wait();
                    worker.setSocket(clientSocket);
                } else {
                    worker = (AnnotatedWorker) threads.elementAt(0);
                    threads.removeElementAt(0);
                    worker.setSocket(clientSocket);
                }
            }
        }

    }

    public static void main(String[] args) throws Exception {

        loadProps();
        printProps();

        (new AnnotatedWebServer()).run();

    }
}
