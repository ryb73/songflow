package com.ryb.songflow.userinfo;

import java.io.IOException;
import org.w3c.dom.Element;


public class Song {
	private String name;
	private String artist;
	private Rating rating;

	public static Song parse(Element element) throws IOException {
		if(element.getNodeName().equals("song")) {
			throw new IllegalArgumentException("Non-song node passed to Song.parse");
		}

		Song res = new Song();
		
		res.name = element.getAttribute("name");
		res.artist = element.getAttribute("artist");
		
		try {
			res.rating = Rating.fromString(element.getAttribute("rating"));
		} catch(IllegalArgumentException iae) {
			throw new IOException(iae.getMessage());
		}

		return res;
    }
}
