package blah;
public class Test {
	public native String sup();
	static {
		System.loadLibrary("spotify");
    	System.loadLibrary("Test");
    }
}
