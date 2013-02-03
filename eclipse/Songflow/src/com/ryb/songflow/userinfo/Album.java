package com.ryb.songflow.userinfo;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import android.graphics.Bitmap;
import android.util.Log;

public class Album {
	private String uri;
	private String name;
	private String artist;
	private Rating rating = Rating.NONE;

	private boolean spinsLoaded = true;
//	private Collection<ReadableInstant> spins;

	public String getArtist() {
		return artist;
	}

	public Rating getRating() {
		return rating;
	}

	public Bitmap getCover() {
		return cover;
	}

	private Bitmap cover;

	private List<Song> songs = new ArrayList<Song>();

	Album(String uri, String name, String artist) {
		this.uri = uri;
		this.name = name;
		this.artist = artist;
		Log.d("rbd","new album: "+uri);
	}
	
	Album(String uri) {
		this(uri, null, null);
	}

	public static Album parse(Element element) throws IOException {
		if(element.getNodeName().equals("album")) {
			throw new IllegalArgumentException("Non-album node passed to Album.parse");
		}

		String uri = element.getAttribute("uri");
		String name = element.getAttribute("name");
		String artist = element.getAttribute("artist");

		Album res = new Album(uri, name, artist);
		
		try {
			res.rating = Rating.fromString(element.getAttribute("rating"));
		} catch(IllegalArgumentException iae) {
			throw new IOException(iae.getMessage());
		}

		NodeList nodes = element.getChildNodes();
		for(int i = 0; i < nodes.getLength(); ++i) {
			Node n = nodes.item(i);
			if(n.getNodeType() == Node.ELEMENT_NODE) {
				String nodeName = n.getNodeName();
				
				if(nodeName.equals("song")) {
					Song s = Song.parse((Element) n);
					res.songs.add(s);
				} else if(nodeName.equals("spin")) {
					long rawInstant;
					try {
						rawInstant = Long.parseLong(((Element)n).getAttribute("instant"));
						
//						res.spins.add(new Instant(rawInstant));
					} catch(NumberFormatException nfe) {
						Log.w("rbd", "Unable to load spin instant for album '" + name + "'");
						res.spinsLoaded = false;
					}
				}
			}
		}
		
		res.test();

		return res;
	}

	private void test() {
		StringBuilder err = new StringBuilder();
		
		if(uri.isEmpty()) err.append("URI, ");
	    if(name.isEmpty()) err.append("name, ");
	    if(artist.isEmpty()) err.append("artist, ");
	    if(songs.isEmpty()) err.append("songs, ");

	    if(err.length() > 0) {
	    	err.insert(0, "Loaded album with no: ");
	    	Log.w("rbd", err.toString());
	    }
    }

	public void updateInfo() {
	    if(uri.equals("news")) {
	    	name = "Good News for People";
	    	artist = "Modest Mouse";
	    } else if(uri.equals("dreaming")) {
	    	name = "Hurry Up, We're Dreaming";
	    	artist = "M83";
	    }
	    
	    Log.d("rbd","updating info: " + uri);
    }

	public String getName() {
		return name;
    }
}
