package com.ryb.songflow.search;

import com.ryb.songflow.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import android.widget.TextView.BufferType;

public class ResultListAdapter extends BaseAdapter implements SearchListener {
	private Context context;
	private SearchResult[] results = null;

	public ResultListAdapter(Context c) {
		context = c;
	}

	@Override
	public int getCount() {
		return (results == null) ? 1 : results.length;
	}

	@Override
	public Object getItem(int position) {
		return (results == null) ? null : results[position];
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

		if(results == null) {
			if(convertView == null) {
				TextView view = (TextView) inflater.inflate(R.layout.search_result, parent, false);
				view.setText("Loading", BufferType.NORMAL);
				convertView = view;
			}
		} else {
			if(convertView == null) {
				TextView view = (TextView) inflater.inflate(R.layout.search_result, parent, false);
				view.setText(results[position].getText(), BufferType.NORMAL);
				convertView = view;
			}
		}
		
		return convertView;
	}

	@Override
    public void searchComplete(SearchResult[] results) {
	    this.results = results;
	    notifyDataSetChanged();
    }

}
