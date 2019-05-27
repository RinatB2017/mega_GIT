package com.psbelov.rpi.leds;

import com.psbelov.rpi.leds.spi.SPI;
import com.psbelov.rpi.leds.utils.ColorUtils;

import java.util.ArrayList;
import java.util.Random;

public class LedDrawer {
    private static void println(String str) {
        System.out.println(str);
    }

    public static void drawAllOff(SPI spi, byte[] array) {
        spi.writeData(drawAllOff(array), 1);
    }

    public static byte[] drawAllOff(byte[] array) {
        return drawOff(array, array.length / 3);
    }

    public static byte[] drawAllOn(SPI spi, byte[] array, int color) {
        return drawSolid(array, array.length / 3, color);
    }

    public static byte[] drawOff(byte[] array, int l) {
        return drawOff(array, 0, l);
    }

    public static byte[] drawOff(byte[] array, int start, int end) {
        return drawSolid(array, start, end, 0);
    }

    public static byte[] drawSolid(byte[] array, int l, int r, int g, int b) {
        return drawSolid(array, 0, l, r, g, b);
    }

    public static byte[] drawSolid(byte[] array, int l, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        return drawSolid(array, 0, l, color[0], color[1], color[2]);
    }

    public static byte[] drawSolid(byte[] array, int start, int end, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        return drawSolid(array, start, end, color[0], color[1], color[2]);
    }


    public static byte[] drawSolid(byte[] array, int start, int end, int r, int g, int b) {
        if (start < 0 || start > end || start > array.length / 3) {
            println("Incorrect index of Start");
        }

        if (end < 0 || start > end || end > array.length / 3) {
            println("Incorrect index of End");
        }


        for (int i = start; i < end; i++) {
            drawPixel(array, i, r, g, b);
        }

        return array;
    }

    public static byte[] drawGradient(byte[] array, int start, int end, int index, int rgb1, int rgb2) {
        byte[] color1 = ColorUtils.separateColor(rgb1);
        byte[] color2 = ColorUtils.separateColor(rgb2);

        return drawGradient(array, start, end, index, color1[0], color1[1], color1[2], color2[0], color2[1], color2[2]);
    }

    public static byte[] drawGradient(byte[] array, int start, int end, int index, int r1, int g1, int b1, int r2, int g2, int b2) {
        if (start < 0 || start > end || start > array.length / 3) {
            println("Incorrect index of Start");
        }

        if (end < 0 || start > end || end > array.length / 3) {
            println("Incorrect index of End");
        }

        println("Start = " + start + ", end = " + end);

        int d = Math.abs(end - start) + 1;
        println("d = " + d);

        for (int i = start; i < end; i++) {
            if (i < index) {
                drawPixel(array, i, 255 - (i * 255 / d) , i * 255 / d, 0);
            }
        }

        return array;
    }

    public static byte[] drawPixel(byte[] array, int i, int r, int g, int b) {
        array[i * 3] = (byte)r;
        array[i * 3 + 1] = (byte)g;
        array[i * 3 + 2] = (byte)b;

        return array;
    }

    public static byte[] drawPixel(byte[] array, int i, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        array[i * 3] = color[0];
        array[i * 3 + 1] = color[1];
        array[i * 3 + 2] = color[2];

        return array;
    }

    public static byte[] drawRGB(byte[] array, int l) {
        return drawRGB(array, 0, l);
    }

    //TODO: add brightness
    public static byte[] drawRGB(byte[] array, int start, int end) {
        for (int i = start; i < end; i++) {
            int index = i * 3;
            if (i % 3 == 0) {
                array[index] = (byte)255;
                array[index + 1] = (byte)0;
                array[index + 2] = (byte)0;
            } else if (i % 3 == 1) {
                array[index] = (byte)0;
                array[index + 1] = (byte)255;
                array[index + 2] = (byte)0;
            } else {
                array[index] = (byte)0;
                array[index + 1] = (byte)0;
                array[index + 2] = (byte)255;
            }
        }

        return array;
    }

    public static void drawChase(SPI spi, long delay, byte[] array, int start, int end, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawChase(spi, delay, array, start, end, color[0], color[1], color[2]);
    }

    public static void drawChase(SPI spi, long delay, byte[] array, int start, int end, int r, int g, int b) {
        for (int i = start; i < end; i++) {
            drawOff(array, start, end);
            drawPixel(array, i, r, g, b);

            spi.writeData(array, delay);
        }
    }

    public static void drawChase2(SPI spi, long delay, byte[] array, int l, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawChase2(spi, delay, array, 0, l, color[0], color[1], color[2]);
    }

    public static void drawChase2(SPI spi, long delay, byte[] array, int start, int end, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawChase2(spi, delay, array, start, end, color[0], color[1], color[2]);
    }

    public static void drawChase2(SPI spi, long delay, byte[] array, int start, int end, int r, int g, int b) {
        for (int i = start; i < end * 2; i++) {
            drawOff(array, start, end);
            if (i < end) {
                drawPixel(array, i, r, g, b);

            } else if ((2 * end - i - 1) > start) {
                drawPixel(array, 2 * end - i - 1, r, g, b);
            }

            spi.writeData(array, delay);
        }
    }

    public static byte[] drawRGB2(byte[] array, int l) {
        return drawRGB(array, 0, l);
    }

    //TODO: add brightness
    public static byte[] drawRGB2(byte[] array, int start, int end) {
        for (int i = start; i < end; i++) {
            int index = i * 3;
            if (i % 6 == 0) {
                array[index] = (byte)255;
                array[index + 1] = (byte)0;
                array[index + 2] = (byte)0;
            } else if (i % 6 == 1) {
                array[index] = (byte)0;
                array[index + 1] = (byte)255;
                array[index + 2] = (byte)0;
            } else if (i % 6 == 2) {
                array[index] = (byte)0;
                array[index + 1] = (byte)0;
                array[index + 2] = (byte)255;
            } else if (i % 6 == 3) {
                array[index] = (byte)255;
                array[index + 1] = (byte)255;
                array[index + 2] = (byte)0;
            } else if (i % 6 == 4) {
                array[index] = (byte)0;
                array[index + 1] = (byte)255;
                array[index + 2] = (byte)255;
            } else {
                array[index] = (byte)255;
                array[index + 1] = (byte)0;
                array[index + 2] = (byte)255;
            }
        }

        return array;
    }

    public static void drawRGB3(SPI spi, long delay, byte[] array, int start, int end, int count) {
        int index2 = 0;
        for (int c = 0; c < count; c++) {
            drawAllOff(array);
            index2 = c % 3;
            for (int i = start; i < end - 13; i++) {
                if (c % 3 == 0) {
                    drawPixel(array, index2, 0x00FF00);
                    drawPixel(array, index2, 0x0000FF);
                    drawPixel(array, index2, 0xFF0000);
                } else if (c % 3 == 1) {
                    drawPixel(array, index2, 0x0000FF);
                    drawPixel(array, index2, 0xFF0000);
                    drawPixel(array, index2, 0x00FF00);
                } else {
                    drawPixel(array, index2, 0xFF0000);
                    drawPixel(array, index2, 0x00FF00);
                    drawPixel(array, index2, 0x0000FF);
                }
            }
            spi.writeData(array, delay);
        }

    }

    public static byte[] drawRainbow(byte[] array, int start, int end) {
        int r = 0, g = 0, b = 0;
        int d = (end - start) / 6;
        for (int i = start; i < end; i++) {
            if (i < d) {
                r += 256 / d - 1;                          // 255-0-0
            } else if (i < 2 * d) {
                g += (256 / d)  - 1;             // 255-255-0
            } else if (i < 3 * d) {
                r -= (256 / d) - 1;               // 0-255-0
            } else if (i < 4 * d) {
                b += (256 / d)  - 1;               // 0-255-255
            } else if (i < 5 * d) {
                g -= (256 / d)  - 1;
            } else if (i < 6 * d) {
                b = (256 / d)  - 1;               // 255-0-255
            } else if (i < 7 * d) {
                r += (256 / d)  - 1;               // 255-0-0
            } else {

                //g -= (256 / d) - 1;
            }

            // TODO remove array = ?
            array = drawPixel(array, i, r, g, b);
        }

        return array;
    }

    public static byte[] drawRainbow2(byte[] array) {
        int max = array.length / 3 -  (array.length / 3) % 7;
        for (int i = 0; i < max; i++) {
            int index = i % 7;
            switch (index) {
                case 0:
                    drawPixel(array, i, 0xFF0000);
                    break;
                case 1:
                    drawPixel(array, i, 0xFF4000);
                    break;
                case 2:
                    drawPixel(array, i, 0xFFFF00);
                    break;
                case 3:
                    drawPixel(array, i, 0x00FF00);
                    break;
                case 4:
                    drawPixel(array, i, 0x00FFFF);
                    break;
                case 5:
                    drawPixel(array, i, 0x00FF);
                    break;
                case 6:
                    drawPixel(array, i, 0xFF00FF);

                break;
            }
        }

        return array;
    }


    public static byte[] drawRainbow(byte[] array, int l) {
        return drawRainbow(array, 0, l);
    }

    public static void drawSnake(SPI spi, long delay, byte[] array, int length, int start, int end, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawSnake(spi, delay, array, length, start, end, color[0], color[1], color[2]);
    }

    public static void drawSnake(SPI spi, long delay, byte[] array, int length, int l, int r, int g, int b) {
        drawSnake(spi, delay, array, 0, l, length, r, g, b);
    }

    public static void drawSnake(SPI spi, long delay, byte[] array, int length, int l, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawSnake(spi, delay, array, 0, l, length, color[0], color[1], color[2]);
    }

    public static void drawSnake(SPI spi, long delay, byte[] array, int length, int start, int end, int r, int g, int b) {
        int fade = 2;
        for (int i = start; i < end; i++) {
            drawOff(array, start, end);
            for (int l = 0; l < length; l++) {
                if (i - l >= 0) {
                    array = drawPixel(array, i - l, r / (int)Math.pow(fade, (l)), g / (int)Math.pow(fade, (l)), b / (int)Math.pow(fade, (l)));
                }
            }

            spi.writeData(array, delay);
        }
    }

    public static void drawStack(SPI spi, long delay, byte[] array, int l, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawStack(spi, delay, array, 0, l, color[0], color[1], color[2]);
    }

    public static void drawStack(SPI spi, long delay, byte[] array, int l, int r, int g, int b) {
        drawStack(spi, delay, array, 0, l, r, g, b);
    }
    public static void drawStack(SPI spi, long delay, byte[] array, int start, int end, int rgb) {
        byte[] color = ColorUtils.separateColor(rgb);

        drawStack(spi, delay, array, start, end, color[0], color[1], color[2]);

    }

    public static void drawStack(SPI spi, long delay, byte[] array, int start, int end, int r, int g, int b) {
        for (int i = start; i < end; i++) {
            drawAllOff(array);

            for (int j = start; j < end; j++) {
                drawAllOff(array);
                if (i <= j) { //TODO: bug
                    drawPixel(array, j - i, r, g, b);
                }
                for (int z = start; z < i; z++) {
                    drawPixel(array, end - 1 - z, r, g, b);
                }


                spi.writeData(array, delay);
            }
        }
    }

    public static void drawChaseRandom(SPI spi, long delay, byte[] array, int l) {
        drawChaseRandom(spi, delay, array, 0, l);
    }
    public static void drawChaseRandom(SPI spi, long delay, byte[] array, int start, int end) {
        Random r = new Random(System.currentTimeMillis());

        for (int i = start; i < end; i++) {
            drawAllOff(array);
            drawPixel(array, i, r.nextInt(255), r.nextInt(255), r.nextInt(255));
            spi.writeData(array, delay);
        }
    }

    public static byte[] drawSolidRandom(byte[] array, int l) {
        return drawSolidRandom(array, 0, l);
    }

    public static byte[] drawSolidRandom(byte[] array, int start, int end) {
        Random rnd = new Random(System.currentTimeMillis());

        int x = rnd.nextInt(1);

        int r = rnd.nextInt(256) * rnd.nextInt(2);
        int g = rnd.nextInt(256) * rnd.nextInt(2);
        int b = rnd.nextInt(256) * rnd.nextInt(2);

        for (int i = start; i < end; i++) {
            array = drawPixel(array, i, r, g, b);
        }

        return array;
    }

    public static byte[] drawRandom(byte[] array, int l) {
        return drawRandom(array, 0, l);
    }

    public static byte[] drawRandom(byte[] array, int start, int end) {
        Random rnd = new Random(System.currentTimeMillis());

        for (int i = start; i < end; i++) {
            int r = rnd.nextInt(256) * rnd.nextInt(2);
            int g = rnd.nextInt(256) * rnd.nextInt(2);
            int b = rnd.nextInt(256) * rnd.nextInt(2);

            array = drawPixel(array, i, r, g, b);
        }

        return array;
    }

    public static void drawDynamicRandom(SPI spi, long delay, byte[] array, int start, int end) {
        spi.writeData(drawDynamicRandom(array, start, end), delay);
    }


    public static void drawDynamicRandom(SPI spi, long delay, byte[] array, int l) {
        spi.writeData(drawSolidRandom(array, 0, l), delay);
    }

    public static byte[] drawDynamicRandom(byte[] array, int l) {
        return drawSolidRandom(array, 0, l);
    }
    public static byte[] drawDynamicRandom(byte[] array, int start, int end) {
        Random rnd = new Random(System.currentTimeMillis());

        for (int i = start; i < end; i++) {
            array = drawPixel(array, i, rnd.nextInt(0xFFFFFF));
        }

        return array;
    }

    public static void drawSolidRandom(SPI spi, long delay, byte[] array, int l) {
        spi.writeData(drawSolidRandom(array, 0, l), delay);
    }

    private static int getPhaseColor(int phase, int base) {
        if (phase >= 0 && phase < base) {
            return phase;
        } else if (phase < 0 || phase > 2 * base) {
            return 0;
        } else {
            return base + base - 1 - phase;
        }
    }

    private static double dimM(int i, int b, boolean back) {
        if (back) {
            return (double)((b) - i) * ((b) - i) / b / b;
        } else {
            return (double) (i * i / b / b);
        }
    }

    public static void drawStars(SPI spi, long delay, byte[] array, int start, int end, int rgb) {
        Random rnd = new Random(System.currentTimeMillis());

        // count of stars of each cycle
        // maximum is leds count / 3 + 3 and minimum is 3
        int count = rnd.nextInt(6) + 3;

        // list with LEDs numbers of count
        ArrayList<Integer> list = new ArrayList<Integer>(count);
        for (int i = 0; i < count; i++) {
            list.add(rnd.nextInt(array.length / 3));
        }

        int i = 0;
        int s = 128;
        for (int f = 0; f < s; f++) {
            for (int p = 0; p < count; p++) {
                i = list.get(p);
                drawPixel(array, i, ColorUtils.dimColor(rgb, (double) f * f / s / s));
            }
            spi.writeData(array, delay);
        }

        for (int f = 0; f < s; f++) {
            for (int p = 0; p < count; p++) {
                i = list.get(p);
                drawPixel(array, i, ColorUtils.dimColor(rgb, (double)((s) - f) * ((s) - f) / s / s));
            }
            spi.writeData(array, delay);
        }
    }

    public static boolean isStarted = false;
    public static void drawStars2(final SPI spi, final long delay, final byte[] array, int start, int end, final int rgb) {
        System.out.println("drawStars2");
        isStarted = true;
        final Random rnd = new Random(System.currentTimeMillis());

        final int count = 10;
        final int max_phase = 128;

        // list with LEDs numbers of count
        final ArrayList<Integer> list = new ArrayList<Integer>(count);
        for (int i = 0; i < count; i++) {
            list.add(start + rnd.nextInt((end) - start));
        }

        final ArrayList<Integer> phase_list = new ArrayList<Integer>(count);
        for (int i = 0; i < count; i++) {
            phase_list.add(rnd.nextInt(max_phase));
        }

//        final ArrayList<Double> fade_list = new ArrayList<Double>(count);
//        for (int i = 0; i < count; i++) {
//            fade_list.add(rnd.nextDouble() / 2);
//        }

        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                while (isStarted) {

                    for (int phase = 0; phase < max_phase * 2; phase++) {
                        for (int index = 0; index < count; index++) {
                            int i = list.get(index);
                            double grad = Math.toRadians((double)phase / max_phase * 90) + phase_list.get(index);
                            drawPixel(array, i, ColorUtils.dimColor(rgb, Math.sin(grad) * Math.sin(grad)));
                        }
                        spi.writeData(array, delay);
                    }
                }
            }
        });

        t.start();
    }

    public static void drawRainbow2Dynamic(final SPI spi, final byte[] array, final long delay) {
        System.out.println("drawRainbow2Dynamic");
        isStarted = true;

        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                int d = 0;

                while (isStarted) {
                    d++;
                    for (int i = 0; i < array.length / 3; i++) {
                        int index = 7 - i % 7 + d;
                        index = index % 7;
                        switch (index) {
                            case 0:
                                drawPixel(array, i, 0xFF0000);
                                break;
                            case 1:
                                drawPixel(array, i, 0xFF4000);
                                break;
                            case 2:
                                drawPixel(array, i, 0xFFFF00);
                                break;
                            case 3:
                                drawPixel(array, i, 0x00FF00);
                                break;
                            case 4:
                                drawPixel(array, i, 0x00FFFF);
                                break;
                            case 5:
                                drawPixel(array, i, 0x0000FF);
                                break;
                            case 6:
                                drawPixel(array, i, 0xFF00FF);

                                break;
                        }
                    }
                    spi.writeData(array, delay);
                }
            }
        });

        t.start();
    }
}
