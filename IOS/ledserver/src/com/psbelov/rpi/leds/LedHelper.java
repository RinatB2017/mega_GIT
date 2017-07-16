package com.psbelov.rpi.leds;

import com.psbelov.rpi.leds.spi.SPI;
import com.psbelov.rpi.leds.utils.ColorUtils;

import java.util.ArrayList;

public class LedHelper {
    public static enum MODE {
        OFF,
        ON,
        SOLID,
        RGB,
        RGB2,
        RGB3,
        RAINBOW,
        RAINBOW2,
        RAINBOW_DYNAMIC,
        CHASE,
        CHASE2,
        SNAKE,
        STACK,
        STACK2,
        RANDOM,
        RANDOM_SOLID,
        RANDOM2,
        RANDOM3,
        STARS,
        STARS2,
        CANCEL,
    }

    private SPI spi;
    private long delay;
    private byte[] array;
    public int mCount;

    public LedHelper(SPI spi, long delay, int count) {
        this.mCount = count;
        this.spi = spi;
        this.delay = delay;
        this.array = new byte[count * 3];
    }

    private SPI getSPI() {
        return spi;
    }

    public int getCount() {
        return array.length / 3;
    }

    public void off() {
        LedDrawer.isStarted = false;
        on(0);
    }

    public void on() {
        on(0xFFFFFF);
    }

    public void on(int color) {
        spi.writeData(LedDrawer.drawAllOn(spi, array, color), delay);
    }

    public void on(String color) {
        on(ColorUtils.parseColor(color));
    }

    public void index(int i, String rgbColor, boolean isReverted) {
        index(i, ColorUtils.parseColor(rgbColor), isReverted);
    }

    public void index(int i, int rgbColor, boolean isReverted) {
        LedDrawer.drawAllOff(array);
        if (i >= 0) {
            if (isReverted) {
                array = LedDrawer.drawPixel(array, getCount() - i - 1, rgbColor);
            } else {
                array = LedDrawer.drawPixel(array, i, rgbColor);
            }
            spi.writeData(array);
        } else {
            off();
        }
    }

    public void setDelay(double value) {
        value = value * 1000;
        spi.setDelay((long) value);
    }

    public void setBrightness(double value) {
        spi.setBrightness(value);
    }

    public void setDelayFloat(double value) {
        int msecInSec = 100;
        // 1.0 means 1000, so 0.01 means 10
        if (0 == value) {
            spi.setDelay(1);
        }
        else {
            int valueForSet = (int)(msecInSec * value);
            spi.setDelay(valueForSet);
        }
    }

    public void fill(int i, String rgbColor, boolean isReverted) {
        fill(i, ColorUtils.parseColor(rgbColor), isReverted);
    }

    public void fill(int i, int rgbColor, boolean isReverted) {
        LedDrawer.drawAllOff(array);
        if (i >= 0) {
            if (isReverted) {
                LedDrawer.drawSolid(array, getCount() - i - 1, getCount(), rgbColor);
            } else {
                LedDrawer.drawSolid(array, 0, i, rgbColor);
            }
        }
        spi.writeData(array);
    }

    public void fill2(int i, int rgbColor, int rgbColor2, boolean isReverted) {
        LedDrawer.drawAllOff(array);
        if (i >= 0) {
            if (isReverted) {
                LedDrawer.drawGradient(array, getCount() - i - 1, 0, i, rgbColor, rgbColor2);
            } else {
                LedDrawer.drawGradient(array, 0, getCount() - 1, i, rgbColor, rgbColor2);
            }
        }
        spi.writeData(array);
    }

    public void drawMode(MODE mode, String rgbColor) {
        drawMode(mode, 0, 0, getCount(), ColorUtils.parseColor(rgbColor));
    }

    public void drawMode(MODE mode, int count, int start, int end, int rgbColor) {
        drawMode(mode, count, start, end, rgbColor, this.delay);
    }

    public void drawAnimationWithColorsList(Integer[] list, int stepCounter, boolean toRight) {
        drawColorsList(list, stepCounter);
    }

    String colorFromList(ArrayList<ArrayList<String>> listOfArrays, int listIndex, int colorIndex) {
        ArrayList<String> colorsList = listOfArrays.get(listIndex);
        colorIndex = colorIndex % colorsList.size();
        return(String)colorsList.get(colorIndex);
    }

    ArrayList<String> listOfColorsForMix(ArrayList<ArrayList<String>> listOfArrays, int colorIndex) {
        ArrayList<String> list = new ArrayList<String>();
        for (int i=0;i<listOfArrays.size();++i) {
            list.add(colorFromList(listOfArrays, i, colorIndex));
        }
        return list;
    }

    public void drawAnimationWithArrayOfColorsList(ArrayList<ArrayList<String>> listOfArrays, int counter, int framesCount) {
        if (framesCount == 0) {
            framesCount = 1;
        }
        int arraysCount = listOfArrays.size();
        int cycle = framesCount * arraysCount;
        int frameInCycle = counter % cycle;
        int activeListIndex = frameInCycle / framesCount;
        int stepInPeriod = frameInCycle % framesCount;
        double percentage = (double)stepInPeriod / (double)framesCount;


        for (int i=0;i<mCount;++i) {
            ArrayList<String> colors = listOfColorsForMix(listOfArrays, i);
            String activeColor = colors.get(activeListIndex);
            String futureColor = colors.get(activeListIndex < arraysCount - 1 ? activeListIndex + 1 : 0);


            int colorIndex = counter % colors.size();
            String rgbColor = colors.get(colorIndex);
            Integer color = ColorUtils.MixColors(activeColor, futureColor, percentage);

            LedDrawer.drawSolid(array, i, i+1, color);
            ++colorIndex;
        }
        spi.writeData(array);



    }

    public void drawColorsList(Integer[] list) {
        drawColorsList(list, 0);
    }

    public void drawColorsList(Integer[] list, int colorIndex) {
        if (list.length == 0) {
            off();
            return;
        }
        int ledsCount = mCount;
        int listSize = list.length;
        colorIndex %= listSize;
        for (int i=0;i<ledsCount;++i) {
            Integer color = list[colorIndex];
            LedDrawer.drawSolid(array, i, i+1, color);
            ++colorIndex;
            if (colorIndex >= listSize) {
                colorIndex = 0;
            }
        }
        spi.writeData(array);
    }

    public void drawMode(MODE mode, int rgbColor) {
        drawMode(mode, 0, 0, getCount(), rgbColor);
    }

    public void drawMode(MODE mode, int rgbColor, long delay) {
        drawMode(mode, 0, 0, getCount(), rgbColor, delay);
    }


    public void drawMode(MODE mode, int count, int start, int end, int rgbColor, long delay) {
        if (array == null) {
            System.out.println("array is null");
            return;
        }

        switch (mode) {
            case ON:
                LedDrawer.drawSolid(array, start, end, 0xFFFFFF);
                spi.writeData(array);
                break;

            case OFF:
                LedDrawer.drawAllOff(array);
                spi.writeData(array);
                break;


            case SOLID:
                LedDrawer.drawSolid(array, start, end, rgbColor);
                spi.writeData(array);
                break;

            case RGB:
                LedDrawer.drawRGB(array, start, end);
                spi.writeData(array);
                break;

            case RGB2:
                LedDrawer.drawRGB2(array, start, end);
                spi.writeData(array);
                break;

            case RGB3:
                LedDrawer.drawRGB3(spi, delay, array, start, end, count);
                break;

            case STACK:
                LedDrawer.drawStack(spi, delay, array, start, end, rgbColor);
                break;

            case RANDOM:
                LedDrawer.drawRandom(array, start, end);
                spi.writeData(array, delay);
                break;

            case RANDOM_SOLID:
                LedDrawer.drawSolidRandom(array, start, end);
                spi.writeData(array, delay);
                break;

            case RANDOM2:
                LedDrawer.drawChaseRandom(spi, delay, array, start, end);
                break;

            case RANDOM3:
                LedDrawer.drawDynamicRandom(spi, delay, array, start, end);
                break;

            case SNAKE:
                LedDrawer.drawSnake(spi, delay, array, 5, start, end, rgbColor);
                break;

            case CHASE:
                LedDrawer.drawChase(spi, delay, array, start, end, rgbColor);
                break;

            case CHASE2:
                LedDrawer.drawChase2(spi, delay, array, start, end, rgbColor);
                break;

            case RAINBOW:
                LedDrawer.drawRainbow(array, start, end);
                spi.writeData(array);
                break;

            case RAINBOW2:
                LedDrawer.drawRainbow2(array);
                spi.writeData(array);
                break;

            case RAINBOW_DYNAMIC:
                LedDrawer.drawRainbow2Dynamic(spi, array, delay);
                break;

            case STARS:
                LedDrawer.drawStars(spi, delay, array, start, end, rgbColor);
                break;

            case STARS2:
                LedDrawer.drawStars2(spi, delay, array, start, end, rgbColor);
                break;

            case CANCEL:
                LedDrawer.isStarted = false;
                break;
        }
    }

    public void close() {
        spi.close();
    }
}
