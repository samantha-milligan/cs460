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

/*
    NOTES FROM CLASS
*/

/* For Big Endian, fields described left to right, most significant octect on
    left, and least significant on right.

    Most networks use Big Endian. Java uses Big Endian

    C uses Little Endian. Need to convert bytes from Java server to Little
    Endian so C can correctly display the long or int.

    C can vary between Big and Little Endian based on the OS and / or platform

       Can use macros to convert, convert it yourself, or use pointer
        assignment. Best to use macros because if C is not in Little Endian but
        in Big Endian, macros do nothing.

    Next week's assignment is a stress test. A multithreaded 3A + 1 Server on C.
    Use EchoServer from Programming Assignment 2.

    More importantly, write a sophiscated C client. Need to know what you're
    doing, not much code in contrast. Use given ThreadPool library. Create a
    client that has a pool of threads that are connected to the server running
    in parallel. After the client reaches a max number of actions, the client
    is closed.

    Will give 1000 tasks to the threads and the threads will work down the
    tasks. On average, a pool of 10 threads will have run 10 tasks.

    Some problems don't show up on localhost servers so stress test is being
    used to make those errors come to the forefront.

    Heavily recommends having a digital environment.

    Suggested a DigitalOcean account. Can have a free account if using Github
    student. Without it, pricing of $5 a month. Also recommended a Hostwinds
    account. Provides users with static IP address. Weaknesses of code show up
    better on digital environments.

    Also, a good skill to have to use a digital environment.

    Additinoal part of assignment is to understand the ThreadPool library Dr.
    Otte sent on #general Slack channel. ThreadPool is a client that doesn't do
    any networking. In the for loop, a task is added to the thread pool after
    the thread pool has been created. Add 3A + 1 wrappper to task. Task is code
    you want to be added to thread.

    Need to pass pointer to functions. Use a pointer so you don't need to make
    assumptions about what kind of data it is. Can know what data is behind the
    pointer. Also, don't need to worry about memory management with pointers as
    much as you would have if not using pointer.

    Passing taskcount address to (void) as argument.

    3A + 1 wrapper function does nothing right now. We can add networking logic
    in this function.

    Simple Steps:
        1. Get code running
        2. Rewrite code so client stress tests server
        3. Write portable code. Use macros(mtoh, htoml, homl) to bypass Endian
            issues.

        1. Write a server 3A + 1 multithreaded in c
        2. Write a client in c using ThreadPool as basis to stress test server

    Weeded out errors with raise conditions and pointer misuse. Raise conditions
    aren't addressed so couldn't fix these issues.
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
