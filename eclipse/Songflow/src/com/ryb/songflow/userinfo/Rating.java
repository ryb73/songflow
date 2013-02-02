package com.ryb.songflow.userinfo;

public class Rating implements Comparable<Rating> {
	public static final Rating GOOD = new Rating(5);
	public static final Rating BAD = new Rating(1);
	public static final Rating NONE = new Rating(-1);

	private final int value;
	
	private Rating(int value) {
		this.value = value;
	}

	@Override
    public int compareTo(Rating another) {
	    if(another == null) return value;
	    return value - another.value;
    }

	public static Rating fromString(String strValue) {
	    try {
	    	int value = Integer.parseInt(strValue);
	    	if(value == GOOD.value) return GOOD;
			if(value == BAD.value) return BAD;
			if(value == NONE.value) return NONE;
			
			throw new IllegalArgumentException("Rating value not recognized");
	    } catch(NumberFormatException nfe) {
	    	throw new IllegalArgumentException("Rating not a number");
	    }
    }
}
