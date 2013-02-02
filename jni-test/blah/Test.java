package blah;
public class Test {
	private native String sup();

	public static void main(String[] args) {
		new Test().sup();
	}

	static {
		System.loadLibrary("Test");
	}
}