package com.psbelov.rpi.leds.server;

import com.psbelov.rpi.leds.LedHelper;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.AsynchronousServerSocketChannel;
import java.nio.channels.AsynchronousSocketChannel;
import java.nio.channels.CompletionHandler;
import java.nio.charset.Charset;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.io.*;
import java.net.*;

class Server implements Runnable {
    public static final String VERSION = "0.4.2";

    private int mPort;

    public LedHelper mLEDs;
    public AsynchronousServerSocketChannel mServerSocket;
    public int clientCount = 0;

    private boolean run;
    ClientHandler _ledClient;

    public Server(int port, LedHelper leds, int ledsCount) throws Exception {
        mPort = port;
        mLEDs = leds;
        run = true;
        _ledClient = new ClientHandler(mLEDs, ledsCount);
    }

    @Override
    public void run() {
        DatagramSocket serverSocket = null;
        try {
            serverSocket = new DatagramSocket(mPort);
        } catch (SocketException e) {
            e.printStackTrace();
        }
        byte[] receiveData = new byte[1024];
        while(null != serverSocket) {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            try {
                serverSocket.receive(receivePacket);
            } catch (IOException e) {
                e.printStackTrace();
            }
            String sentence =  new String(receivePacket.getData()).substring(0, receivePacket.getLength());
            _ledClient.processCommandString(sentence);
        }
    }
}
