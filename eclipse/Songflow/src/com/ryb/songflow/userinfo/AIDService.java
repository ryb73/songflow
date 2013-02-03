package com.ryb.songflow.userinfo;

import java.util.LinkedList;
import java.util.Queue;

import android.util.Log;

public class AIDService extends Thread {
	private Queue<Album> updateQueue = new LinkedList<Album>();
	private boolean running;
	
	public AIDService() {
		setDaemon(true);
	}
	
	@Override
    public void run() {
	    running = true;
	    while(running) {
	    	for(Album a : updateQueue) {
	    		synchronized(a) {
	    			a.updateInfo();
	    		}
	    	}
	    	
	    	try {
	    		synchronized(this) {
	    			wait();
	    		}
            } catch (InterruptedException ie) { }
	    }
    }

	public synchronized void enqueue(Album a) {
	    updateQueue.add(a);
	    try {
	    	if(!isAlive()) start();
	    } catch(IllegalStateException ise) { } // just to be safe
    }
}
