package com.ryb.songflow.spotify;

import com.ryb.songflow.search.SearchListener;
import com.ryb.songflow.search.SearchResult;
import com.ryb.songflow.search.SearchResult.SearchResultType;

public class Spotify {
	public static void initialize() throws SpotifyException {
		String error = "";//nInit();
		if(error.length() > 0) {
			throw new SpotifyException();
		}
	}

	public static void tryConnect() {
		
	}

	public static void search(String query, SearchListener listener) {
		//nSearch(query, 0, 3, 0, 3, 0, 3, listener);
		SearchResult[] res = new SearchResult[4];
		res[0] = new SearchResult("The Photo Album by Death Cab for Cutie", "photo", SearchResultType.ALBUM);
		res[1] = new SearchResult("Good News for People Who Like Bad News by Modest Mouse", "news", SearchResultType.ALBUM);
		res[2] = new SearchResult("Say Anything", "anything", SearchResultType.ARTIST);
		res[3] = new SearchResult("Midnight City by M83", "city", SearchResultType.SONG);
		listener.searchComplete(res);
	}

	public static void getSuggestions(String query, SearchListener listener) {
		//nSuggestions(query, 0, 3, 0, 3, 0, 3, listener);
		SearchResult[] res = new SearchResult[4];
		res[0] = new SearchResult("The Photo Album by Death Cab for Cutie", "photo", SearchResultType.ALBUM);
		res[1] = new SearchResult("Good News for People Who Like Bad News by Modest Mouse", "news", SearchResultType.ALBUM);
		res[2] = new SearchResult("Say Anything", "anything", SearchResultType.ARTIST);
		res[3] = new SearchResult("Midnight City by M83", "city", SearchResultType.SONG);
		listener.searchComplete(res);
    }

	public static void getSongInfo(String uri, MetadataUpdateListener listener) {
	    if(uri.equals("city")) {
	    	SongInfoUpdate update = new SongInfoUpdate("dreaming");
	    	listener.metadataUpdated(update);
	    }
    }

//	private static native String initialize();
//	private static native void search(String query, int trackOffest, int trackNum, int albumOffset, int albumCount,
//									   int artistOffset, int artistCount, SearchListener listener);
//	private static native void getSuggestions(String query, int trackOffest, int trackNum, int albumOffset, int albumCount,
//			   int artistOffset, int artistCount, SearchListener listener);
//	private static native getSongInfo(String uri, MetadataUpdateListener listener);
}
