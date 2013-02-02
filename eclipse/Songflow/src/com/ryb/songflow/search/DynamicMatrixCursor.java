package com.ryb.songflow.search;

import android.app.SearchManager;
import android.database.ContentObserver;
import android.database.DataSetObserver;
import android.database.MatrixCursor;
import android.os.Bundle;
import android.provider.BaseColumns;
import android.util.Log;

public class DynamicMatrixCursor extends MatrixCursor {
	private boolean retrievalFinished = false;
	
	/* Assuming only one observer. Oh well. */
	private DataSetObserver dataSetObserver = null;

	public DynamicMatrixCursor(String[] colNames) {
		super(colNames);
	}
	
	public void finishRetrieval() {
		retrievalFinished = true;
		if(dataSetObserver != null)
			dataSetObserver.onChanged();
	}

	@Override
    public void addRow(Object[] columnValues) {
	    super.addRow(columnValues);
	    
	    if(dataSetObserver != null)
	    	dataSetObserver.onChanged();
    }

	@Override
    public void registerDataSetObserver(DataSetObserver observer) {
	    dataSetObserver = observer;
	    super.registerDataSetObserver(observer);
    }

	@Override
    public void unregisterDataSetObserver(DataSetObserver observer) {
	    dataSetObserver = null;
	    super.unregisterDataSetObserver(observer);
    }

	@Override
    public Bundle getExtras() {
	    if(retrievalFinished) return super.getExtras();
	    
	    Bundle res = new Bundle();
	    res.putBoolean(SearchManager.CURSOR_EXTRA_KEY_IN_PROGRESS, true);
	    return res;
    }
}
