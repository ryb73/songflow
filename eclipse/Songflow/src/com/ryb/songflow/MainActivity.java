package com.ryb.songflow;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.ryb.songflow.mylist.MyListActivity;
import com.ryb.songflow.search.SearchActivity;
import com.ryb.songflow.spotify.SpotifyException;
import com.ryb.songflow.spotify.Spotify;
import com.ryb.songflow.userinfo.Preferences;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Log.d("rbd","main load");

		try {
			Preferences.loadPreferences(this);

			Spotify.initialize();
			Spotify.tryConnect();
		} catch(SpotifyException se) {
			
		} catch (IOException ioe) {
	        // TODO Auto-generated catch block
	        Log.w("rbd", ioe);
        } catch (ParserConfigurationException pce) {
	        // TODO Auto-generated catch block
	        Log.w("rbd", pce);
        } catch (SAXException saxe) {
	        // TODO Auto-generated catch block
	        Log.w("rbd", saxe);
        }
	}

	@Override
	public boolean onSearchRequested() {
		startSearch(null, false, null, false);
		return super.onSearchRequested();
	}

	public void butClick(View v) {
		Intent intent = new Intent(this, SearchActivity.class);
		startActivity(intent);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	@Override
    public boolean onOptionsItemSelected(MenuItem item) {
	    switch(item.getItemId()) {
	    	case R.id.menu_list:
	    		Intent intent = new Intent(this, MyListActivity.class);
	    		startActivity(intent);
	    		return true;
	    	default:
	    		Log.wtf("rbd", "unhandled menu item " + item.getItemId());
	    		return false;
	    }
    }
}
