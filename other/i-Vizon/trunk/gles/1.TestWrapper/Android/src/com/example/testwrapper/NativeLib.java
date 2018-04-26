package com.example.testwrapper;

public class NativeLib {
    static {
        System.loadLibrary("testWrapper");
    }

   /**
    * @param width the current view width
    * @param height the current view height
    */
    public static native void init(int width, int height);
    public static native void step();
}
