package com.psbelov.rpi.leds.server;

import com.psbelov.rpi.leds.LedHelper;
import com.psbelov.rpi.leds.spi.SPI;
import com.sun.deploy.util.StringUtils;

public class Main {
    private static void println(String str) {
        System.out.println(str);
    }
    public static boolean isNumeric(String str)
    {
        return str.matches("-?\\d+(\\.\\d+)?");  //match a number with optional '-' and decimal.
    }
    public static void main(String[] args) throws InterruptedException {

        int port = 8090;               // Server port
        String device = "/dev/spidev0.0"; // "/dev/spidev0.0";          // SPI device that used for leds control
        int ledsCount = 50;          // leds count. default is 50.
        long delay = 1;             // delay between writing data to leds. should be minimum 1

        boolean ledsSetted = false;
        for (String arg: args) {
            if (isNumeric(arg)) {
                if (ledsSetted == false) {
                    ledsSetted = true;
                    ledsCount = Integer.parseInt(arg);
                }

            }
            System.out.println("arg: " + arg);
        }

        /*
        AV: ��� ���������� ������� ledserver.jar � ������ �������� ��������� �� ����, ����� ��������� �� RPI �������
        java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005 -jar ledserver.jar
         */
        SPI spi;
        LedHelper leds;

        /*

        if (args.length > 0) {
            try {
                port = Integer.parseInt(args[0]);
            } catch (NumberFormatException nfe) {
                println("Incorrect port");
                return;
            }
        }
*/

        if (port <= 1024 || port >= 65536) {
            println("Incorrect port [" + port + "]. Should be more then 1024 and less then 65535");
            return;
        }

        /*
        if (args.length > 3) {
            try {
                ledsCount = Integer.parseInt(args[3]);
            } catch (NumberFormatException nfe) {
                println("Incorrect LEDs count format");
                return;
            }
        }
        */

        if (ledsCount < 0) {
            println("Leds count should be more than 0");
            return;
        } else if (ledsCount == 0) {
            println("0 leds. NO WAY!");
            return;
        }

        /*
        if (args.length > 2) {
            try {
                delay = Long.parseLong(args[2]);
            } catch (NumberFormatException nfe) {
                println("Incorrect delay");
                return;
            }
        }
        */

        if (delay <= 0) {
            println("Delay should be more than 0");
            return;
        } else if (delay >= 1000) {
            println("Delay is " + delay + ". Are you sure? Okay");
        }

        /*
        if (args.length > 1) {
            device = args[1];
        }
        */

        spi = new SPI(device, delay);
        if (spi.isOpened() == false) {
            println("Something went wrong with the device " + spi);
            return;
        }

        leds = new LedHelper(spi, delay, ledsCount);

        try {
            println("port = " + port + "\ndevice = " + device + "\ndelay = " + delay + "\nleds count = " + ledsCount);
            Server s = new Server(port, leds, ledsCount);
            new Thread(s).start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
