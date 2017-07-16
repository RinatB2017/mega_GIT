package com.psbelov.rpi.leds.utils;
import java.nio.ByteBuffer;

public class ColorUtils {
    public static final String COLOR_WHITE = "white";
    public static final String COLOR_RED = "red";
    public static final String COLOR_GREEN = "green";
    public static final String COLOR_YELLOW = "yellow";
    public static final String COLOR_ORANGE = "orange";
    public static final String COLOR_AQUA = "aqua";
    public static final String COLOR_BLUE = "blue";

    public static byte[] separateColor(int color) {
        int r = 0, g = 0, b = 0;

        r = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        b = color & 0xFF;

        return new byte[] {(byte)r, (byte)g, (byte)b};
    }

    public static int convertColor(int r, int g, int b) {
        return parseColor(String.format("%02x%02x%02x", r, g, b));
    }

    public static int dimColor(int color, double d) {
        int r = 0, g = 0, b = 0;

        r = (int)(((color >> 16) & 0xFF) * d);
        g = (int)(((color >> 8) & 0xFF) * d);
        b = (int)((color & 0xFF) * d);

        int rgb = r;
        rgb = (rgb << 8) + g;
        rgb = (rgb << 8) + b;

        return rgb;
    }

    public static int dimColor(int color, int d) {
        int r = 0, g = 0, b = 0;

        r = (((color >> 16)) & 0xFF) / 0xFF * d;
        g = (((color >> 8) & 0xFF)) / 0xFF * d;
        b = ((color & 0xFF) * 0xFF) / d;

        return convertColor(r, g, b);
    }

    public static int MixColors(String colorOne, String colorTwo, double percentage) {
        int leftColor = parseColor(colorOne);
        int rightColor = parseColor(colorTwo);

        byte[] leftBytes = ByteBuffer.allocate(4).putInt(leftColor).array();
        byte[] rightBytes = ByteBuffer.allocate(4).putInt(rightColor).array();

        String ouptutColor = "";

        int counter = 0;
        for (byte leftByte : leftBytes) {
            if (counter == 0) {
                ++counter;
                continue;
            }
            int unsLeftByte = leftByte & 0xff;
            int unsRightByte = rightBytes[counter] & 0xff;
            int delta = unsRightByte - unsLeftByte;
            delta *= percentage;
            unsLeftByte += delta;
            ouptutColor += String.format("%02X", unsLeftByte);
            ++counter;
        }

        return parseColor(ouptutColor);
    }

    public static int parseColor(String color) {
        int rgbColor = 0x000000;

        switch (color) {
            case COLOR_WHITE:
                rgbColor = 0xFFFFFF;
                break;

            case COLOR_RED:
                rgbColor = 0xFF0000;
                break;

            case COLOR_GREEN:
                rgbColor = 0x00FF00;
                break;

            case COLOR_BLUE:
                rgbColor = 0x0000FF;
                break;

            case COLOR_ORANGE:
                rgbColor = 0xFF8000;
                break;

            case COLOR_AQUA:
                rgbColor = 0x00FFFF;
                break;

            case COLOR_YELLOW:
                rgbColor = 0xFFFF00;
                break;

            default:
                try {
                    rgbColor = Integer.parseInt(color, 16);
                } catch (NumberFormatException nfe) {
                    System.out.println("Incorrect color");
                }

                if (rgbColor < 0) {
                    rgbColor = 0;
                } if (rgbColor >= 0xFFFFFF) {
                    rgbColor = 0xFFFFFF;
                }
        }

        return rgbColor;
    }
}
