package com.ryb.songflow.search;

import com.ryb.songflow.spotify.Spotify;

import android.app.SearchManager;
import android.content.ContentProvider;
import android.content.ContentValues;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.provider.BaseColumns;
import android.util.Log;

public class SpotifySearchSuggestionProvider extends ContentProvider implements SearchListener {
	private DynamicMatrixCursor cursor;

	public SpotifySearchSuggestionProvider() {
	}

	@Override
	public int delete(Uri uri, String selection, String[] selectionArgs) {
		// Implement this to handle requests to delete one or more rows.
		throw new UnsupportedOperationException("Not yet implemented");
	}

	@Override
	public String getType(Uri uri) {
		return null;
	}

	@Override
	public Uri insert(Uri uri, ContentValues values) {
		// TODO: Implement this to handle requests to insert a new row.
		throw new UnsupportedOperationException("Not yet implemented");
	}

	@Override
	public boolean onCreate() {
		// TODO: Implement this to initialize your content provider on startup.
		return false;
	}

	@Override
	public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
		cursor = new DynamicMatrixCursor(new String[] {
				BaseColumns._ID,
				SearchManager.SUGGEST_COLUMN_TEXT_1,
				SearchManager.SUGGEST_COLUMN_INTENT_ACTION,
				SearchManager.SUGGEST_COLUMN_INTENT_DATA
		});
		
		String query = uri.getLastPathSegment();
		Spotify.getSuggestions(query, this);
		return cursor;
	}

	@Override
	public int update(Uri uri, ContentValues values, String selection,
	        String[] selectionArgs) {
		// TODO: Implement this to handle requests to update one or more rows.
		throw new UnsupportedOperationException("Not yet implemented");
	}

	@Override
    public void searchComplete(SearchResult[] results) {
	    for(int i = 0; i < results.length; ++i) {
	    	String action = null;
	    	switch(results[i].getType()) {
	    		case ARTIST:
	    			action = SearchActivity.INTENT_ARTIST;
	    			break;
	    		case ALBUM:
	    			action = SearchActivity.INTENT_ALBUM;
	    			break;
	    		case SONG:
	    			action = SearchActivity.INTENT_SONG;
	    			break;
	    	}

	    	Object[] values = {
	    		i + 1,
	    		results[i].getText(),
	    		action,
	    		results[i].getUri()
	    	};

	    	cursor.addRow(values);
	    }
	    
	    cursor.finishRetrieval();
    }
}
