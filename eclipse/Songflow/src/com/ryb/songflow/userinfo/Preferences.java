package com.ryb.songflow.userinfo;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import android.content.Context;
import android.util.Log;


public class Preferences {
	private static final String FILE_NAME = "preferences.xml";

	private static Preferences instance;
	
	private AIDService aidService;

	private List<Album> albums = new ArrayList<Album>();
	
	public static Preferences getInstance() {
		return instance;
	}
	
	public void addAlbum(String uri) {
		Album a = new Album(uri);
		albums.add(a);

		if(aidService == null) {
			aidService = new AIDService();
		}

		synchronized (aidService) {
			aidService.enqueue(a);
			aidService.notifyAll();
		}
	}
	
	public List<Album> getAlbums() {
		return Collections.unmodifiableList(albums);
	}

	public static Preferences loadPreferences(Context context) throws IOException, ParserConfigurationException, SAXException {
		if(instance != null) {
			Log.w("rbd", "Requesting preferences multiple times");
			return instance;
		}
		
		instance = new Preferences();

		if(!context.getFileStreamPath(FILE_NAME).exists()) {
			return instance;
		}

		InputStream is = context.openFileInput(FILE_NAME);

		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
		Document doc = dBuilder.parse(is);

		Element root = doc.getDocumentElement();
		root.normalize();
		
		if(!root.getNodeName().equals("preferences")) {
			throw new IOException("Root node must be of type \"preferences\"");
		}

		NodeList nodes = root.getChildNodes();
		for(int i = 0; i < nodes.getLength(); ++i) {
			Node n = nodes.item(i);
			if(n.getNodeType() == Node.ELEMENT_NODE) {
				String nodeName = n.getNodeName();

				if(nodeName.equals("album")) {
					Album a = Album.parse((Element) n);
					instance.albums.add(a);
				} else {
					throw new IOException("Unrecognized node type \"" + nodeName + "\" in preferences");
				}
			}
		}
		
		return instance;
    }
}
