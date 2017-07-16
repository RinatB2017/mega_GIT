package com.psbelov.rpi.leds.server;

import com.psbelov.rpi.leds.LedHelper;
import com.psbelov.rpi.leds.data.Command;
import com.psbelov.rpi.leds.data.CommandsEnum;
import com.psbelov.rpi.leds.utils.ColorUtils;
import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.*;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;
import java.nio.charset.Charset;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

class ClientHandler {
    private Socket mClient;
    private boolean IO;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    private String mClientAddress;

    private LedHelper mLEDs;

    private Thread tRun;
    private boolean isStarted;
    private int mNumber;

    private boolean _linearAnimationToRight;
    private double _animationSpeed;
    private List<Integer> _latestColorsList;

    private double _framesCount; // 0.0 - 1.0. left means minimal delay, 1.0 means maximum delay

    private ArrayList<ArrayList<String>> _listOfColorsLists;
    public ClientHandler(LedHelper leds, int number) {
        mLEDs = leds;
        mNumber = number;
        IO = false;
        _latestColorsList = new ArrayList<Integer>();
        _listOfColorsLists = new ArrayList<ArrayList<String>>();
        _framesCount = 0.01;
    }

    public void processCommandString(String commandString) {
        try {
            JSONObject json = (JSONObject) new JSONParser().parse(commandString);
            String command = (String) json.get("command");
            switch (command) {
                case "Set Brightness": {
                    double brightness = ((Number)json.get("value")).doubleValue();
                    mLEDs.setBrightness(brightness);
                    break;
                }
                case "Set Frames Count": {
                    _framesCount = ((Number)json.get("value")).doubleValue();
                    break;
                }
                case "Animated Colors Sets": {
                    _listOfColorsLists.clear();
                    double speed = (double)((Number)json.get("speed")).doubleValue();
                    _framesCount = (double)((Number)json.get("framesCount")).doubleValue();

                    JSONArray colorsArrays = (JSONArray) json.get("colors-lists-array");

                    Iterator i = colorsArrays.iterator();
                    while (i.hasNext()) {
                        ArrayList<String> listOfColors = new ArrayList<String>();

                        JSONArray colors = (JSONArray) i.next();
                        Iterator ci = colors.iterator();
                        while (ci.hasNext()) {
                            String colorString =(String)ci.next();
                            listOfColors.add(colorString);
                        }
                        _listOfColorsLists.add(listOfColors);
                    }

                    _animationSpeed = speed;
                    Command cmd = new Command(CommandsEnum.ANIMATION_OF_COLORS_SETS, "");
                    processCommand(cmd);
                    break;
                }
                case "Linear Animation": {
                    double  speed = (double)((Number)json.get("speed")).doubleValue();
                    _animationSpeed = speed;
                    if (speed == 0.f) {
                        isStarted = false;
                        return;
                    }
                    if (isStarted) {
                        mLEDs.setDelay(speed);
                        return;
                    }
                    boolean toRight = (boolean)json.get("toRight");
                    _linearAnimationToRight = toRight;
                    _animationSpeed = speed;
                    Command cmd = new Command(CommandsEnum.LINEAR_ANIMATION, speed, toRight);
                    processCommand(cmd);
                    break;
                }
                case "Set Color": {
                    String setColorData = (String)json.get("value");
                    Command commandForProcess = new Command(CommandsEnum.FILL, setColorData);
                    processCommand(commandForProcess);
                    break;
                }
                case "Set Colors List": {
                    JSONArray colorsList =(JSONArray)json.get("value");
                    Command commandForProcess = new Command(CommandsEnum.COLORS_LIST, colorsList);
                    processCommand(commandForProcess);
                    break;
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void runThread(Thread thread) {
        if (_animationSpeed == 0.f) {
            isStarted = false;
            return;
        }
        isStarted = true;
        if (tRun != null && tRun.isAlive()) {
            //    tRun.interrupt();
            tRun.stop();
        }
        tRun = thread;

        tRun.start();
    }

    private void makeLinearAnimation() {
        mLEDs.setDelay(_animationSpeed);
        runThread(new Thread(new Runnable() {
            @Override
            public void run() {
                int counter = 0;
                while (isStarted) {
                    Integer[] integersArray = _latestColorsList.toArray(new Integer[_latestColorsList.size()]);
                    mLEDs.drawAnimationWithColorsList(integersArray, counter, _linearAnimationToRight);
                    ++counter;
                }
            }
        }));
    }

    private void makeAnimationOfColorsSets() {
        runThread(new Thread(new Runnable() {
            @Override
            public void run() {
                int counter = 0;
                while (isStarted) {
                    mLEDs.drawAnimationWithArrayOfColorsList(_listOfColorsLists, counter, (int)(_framesCount * 100));
                    ++counter;
                }
            }
        }));
    }

    private void processCommand(final Command c) {
        System.out.println("Command " + c.getCommand().toString() + " from " + mClientAddress + " data = " + c.getData());
        
        switch(c.getCommand()) {
            case ANIMATION_OF_COLORS_SETS:
            {
                makeAnimationOfColorsSets();
                break;
            }
            case COLORS_LIST:
            {
                JSONArray jsonArray = c.getArray();
                List<Integer> list = new ArrayList<Integer>();
                for (int i=0; i<jsonArray.size(); i++) {
                    String rgbColor = (String) jsonArray.get(i);
                    list.add( ColorUtils.parseColor(rgbColor) );
                }
                _latestColorsList = list;
                Integer[] integersArray = list.toArray(new Integer[list.size()]);
                if (_animationSpeed != 0.f && !isStarted) {
                    // AV: запускаю анимацию
                    makeLinearAnimation();
                }
                else {
                    mLEDs.drawColorsList(integersArray);
                    break;
                }
                break;
            }
            case LINEAR_ANIMATION: {
                makeLinearAnimation();
                break;
            }
            case CONNECT:
                if (c.getData() == null) {
                    System.out.println("Unauthorised access from " + mClientAddress + ". Used mPass = \"" + c.getData() + "\"");

                    disconnect("Incorrect password"); //disconnecting if CONNECT command doesn't contain correct mPass
                } else { // sending server version to mClient
                    sendCommand(new Command(CommandsEnum.VERSION, Server.VERSION));
                }
            break;

            case DISCONNECT:
                if (c.getData() == null) {
                    System.out.println("Incorrect Disconnect command");
                } else {
                    disconnect(c.getData().toString());
                }
                break;

            case KILL:
                return;

            case LENGTH:
        //        sendCommand(new Command(CommandsEnum.LENGTH, mLEDs.getCount()));

                break;
            case ON:
                mLEDs.on();
                break;

            case OFF:
                mLEDs.off();
                break;

            case COLOR:
                if (c.getData() == null) {
                    System.out.println("Incorrect COLOR command");
                } else {
                    mLEDs.on(c.getStringData());
                }
                break;

            case INDEX: {
                String data = c.getStringData();
                int i = Integer.parseInt(data.split(":")[0]);
                String rgbColor = data.split(":")[1];
                mLEDs.index(mLEDs.mCount, data, false);
            }
                break;

            case INDEX_REVERTED: {
                String data = c.getStringData();
                int i = Integer.parseInt(data.split(":")[0]);
                String rgbColor = data.split(":")[1];
                mLEDs.index(i, rgbColor, true);
            }
            break;

            case FILL: {
                isStarted = false;
                String data = c.getStringData();
                mLEDs.fill(mNumber, data, false);
            }
            break;

            case FILL_REVERTED: {
                String data = c.getStringData();
                int i = Integer.parseInt(data.split(":")[0]);
                String rgbColor = data.split(":")[1];
                mLEDs.fill(i, rgbColor, true);
            }

            break;

            case CANCEL: {
                isStarted = false;
                if (tRun != null && tRun.isAlive()) {
                   // tRun.interrupt();
                    tRun.stop();
                }

                mLEDs.off();
            }

            break;

            case CHASE: {
                isStarted = true;
                if (tRun != null && tRun.isAlive()) {
                    //tRun.interrupt();
                    tRun.stop();
                }
                tRun = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while (isStarted) {
                            String rgbColor = c.getStringData();
                            mLEDs.drawMode(LedHelper.MODE.CHASE, rgbColor);
                        }
                    }
                });

                tRun.start();
            }
            break;

            case RANDOM: {
                isStarted = true;

                tRun = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while (isStarted) {
                            mLEDs.drawMode(LedHelper.MODE.RANDOM, 0, 100);
                        }
                    }
                });

                tRun.start();
            }
            break;

            case RANDOM_SOLID: {
                isStarted = true;

                tRun = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while (isStarted) {
                            mLEDs.drawMode(LedHelper.MODE.RANDOM_SOLID, 0, 1000);
                        }
                    }
                });

                tRun.start();
            }
            break;

            case CHASE_CYCLED: {
                isStarted = true;
                if (tRun != null && tRun.isAlive()) {
                //    tRun.interrupt();
                    tRun.stop();
                }
                tRun = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while (isStarted) {
                            String rgbColor = c.getStringData();
                            mLEDs.drawMode(LedHelper.MODE.CHASE2, rgbColor);
                        }
                    }
                });

                tRun.start();
            }
            break;

            case STARS: {
                isStarted = true;
                final int rgbColor = ColorUtils.parseColor(c.getData().toString());
                tRun = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        while (isStarted) {

                            mLEDs.drawMode(LedHelper.MODE.STARS, rgbColor, 1);
                        }
                    }
                });

                tRun.start();
            }
            break;

            case STARS2: {
                isStarted = false;
                final int rgbColor = ColorUtils.parseColor(c.getData().toString());
                mLEDs.drawMode(LedHelper.MODE.STARS2, 0, 20, 49, rgbColor, 1);
            }
            break;

            case RAINBOW2: {
                mLEDs.drawMode(LedHelper.MODE.RAINBOW2, 0);
            }
            break;

            case RAINBOW_DYNAMIC: {
                isStarted = false;
                mLEDs.drawMode(LedHelper.MODE.RAINBOW_DYNAMIC, 0, 100);
            }
            break;

            default:
                System.out.println("Incorrect command from " + mClientAddress);
                break;
        }
    }
    
    public synchronized boolean sendCommand(Command c) {
        try {
            out.writeObject(c);
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
            IO = true;
            mLEDs.off();
            return false;
        }
        return true;        
    }
    
    private void disconnect(String reason) {
        sendCommand(new Command(CommandsEnum.DISCONNECT, reason));
        mLEDs.off();
        IO = true;

        System.out.println("mClient " + mClientAddress + " disconnected (" + reason + ")");
    }
}
