package com.ryb.songflow.search;

public class SearchResult {
	private String text;
	private String uri;
	private SearchResultType type;

	public SearchResult(String text, String uri, SearchResultType type) {
		this.text = text;
		this.uri = uri;
		this.type = type;
	}
	
	@Override
    public String toString() {
	    return text;
    }

	public String getText() {
    	return text;
    }

	public String getUri() {
    	return uri;
    }

	public SearchResultType getType() {
    	return type;
    }


	public enum SearchResultType {
		ARTIST, ALBUM, SONG
    }
}
