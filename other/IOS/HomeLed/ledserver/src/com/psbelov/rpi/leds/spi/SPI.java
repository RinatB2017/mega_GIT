package com.psbelov.rpi.leds.spi;
import java.nio.ByteBuffer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class SPI {
    private long mDelay = 1000;
    private FileOutputStream output = null;
    private boolean isOpened = false;
    private String mDevice = null;
    private double _brightness;

    public SPI(String spidev, long delay) {
        this(spidev);
        _brightness = 1.f;
        setDelay(delay);
    }

    public SPI(String spidev) {
        mDevice = spidev;
        openSPI();
    }

    private void openSPI() {
        if (mDevice == null || mDevice.isEmpty()) {
            System.out.println("Device name null or empty");
            isOpened = false;
            return;
        }

        System.out.println("Using device = " + mDevice);
        isOpened = true;

        try {
            output = new FileOutputStream(new File(mDevice)); // default is "/dev/spidev0.0"

            System.out.println("output = " + output);
            if (output == null) {
                System.err.println("!!!");
            }
        } catch (Exception e) {
            System.out.println("Exception = " + e.toString());

            e.printStackTrace();
            isOpened = false;
        }

        isOpened = true;
    }

    public boolean isOpened() {
        return output != null;
    }

    public String toString() {
        return "device = " + mDevice;
    }

    public void setDelay(long delay) {
        if (delay < 1) {
            mDelay = 1;
        } else {
            mDelay = delay;
        }
    }

    public void setBrightness(double value) {
        _brightness = value;
    }

    public boolean writeData(byte[] array) {
        if (isOpened() == false) {
            System.out.println("spidev is closed!");
            return false;
        }
        return writeData(array, mDelay);
    }

    public boolean writeData(byte[] array, long delay) {
        if (delay < 1) {
            delay = 1;
        }

        boolean ret = true;

        ByteBuffer bb = ByteBuffer.allocate(array.length);
        for (int i=0;i<array.length;++i) {
            int color = array[i] & 0xff;
            color *= _brightness;
            bb.put((byte)color);
        }

        byte[] newArray = bb.array();
        if (newArray.length != array.length) {
            return false;
        }

        try {
            output.write(newArray);
            output.flush();
        } catch (IOException e) {
            if (e.getMessage() != null && e.getMessage().contains("Stream Closed")) {
                System.out.println("SPI closed. Trying to open again");
                openSPI();
            }
            ret = false;
            e.printStackTrace();
        }

        try {
            Thread.sleep(delay);
        } catch (InterruptedException e) {
            ret = false;
            e.printStackTrace();
        }

        return ret;
    }

    public void close() {
        System.out.println("SPI: closing");
        try {
            output.close();
            isOpened = false;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
