package com.ryb.songflow.search;

import android.net.Uri;
import android.os.Bundle;
import android.app.ListActivity;
import android.app.SearchManager;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.widget.ArrayAdapter;

import com.ryb.songflow.*;
import com.ryb.songflow.spotify.MetadataUpdate;
import com.ryb.songflow.spotify.MetadataUpdate.UpdateType;
import com.ryb.songflow.spotify.MetadataUpdateListener;
import com.ryb.songflow.spotify.SongInfoUpdate;
import com.ryb.songflow.spotify.Spotify;
import com.ryb.songflow.userinfo.Preferences;

public class SearchActivity extends ListActivity implements MetadataUpdateListener {
	public static final String INTENT_ARTIST = "com.ryb.songflow.search.Artist";
	public static final String INTENT_ALBUM = "com.ryb.songflow.search.Album";
	public static final String INTENT_SONG = "com.ryb.songflow.search.Song";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = getIntent();
		String action = intent.getAction();
		if(Intent.ACTION_SEARCH.equals(action)) {
			handleSearch(intent.getStringExtra(SearchManager.QUERY));
		} else if(INTENT_ARTIST.equals(action)) {
			handleArtist(intent.getData());
		} else if(INTENT_ALBUM.equals(action)) {
			handleAlbum(intent.getData());
		} else if(INTENT_SONG.equals(action)) {
			handleSong(intent.getData());
		}
	}

	private void handleSong(Uri data) {
	    Spotify.getSongInfo(data.toString(), this);
	    finish();
    }

	private void handleAlbum(Uri data) {
		Preferences.getInstance().addAlbum(data.toString());
		finish();
    }

	private void handleArtist(Uri data) {
		Log.d("rbd","artist: " + data);
    }

	private void handleSearch(String query) {
	    ResultListAdapter adapter = new ResultListAdapter(this);
	    Spotify.search(query, adapter);
	    getListView().setAdapter(adapter);
    }

	@Override
    public void metadataUpdated(MetadataUpdate update) {
	    if(update.getUpdateType() == UpdateType.SONG_INFO) {
	    	Preferences.getInstance().addAlbum(((SongInfoUpdate)update).getAlbumUri());
	    }
    }
}
