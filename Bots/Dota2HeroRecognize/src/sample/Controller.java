package sample;

import com.kuldiegor.recognize.*;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.*;

public class Controller {
    private Stage stage;
    private Thread threadSocket;
    private Thread threadDatagramSocket;
    private DefaultHero defaultHero;

    @FXML
    javafx.scene.control.Label label1;
    @FXML
    javafx.scene.control.TextField textfield1;
    @FXML
    javafx.scene.control.TextField textfield2;

    public void startThreadSocketAndUDP(){
        this.defaultHero = new DefaultHero("convolutions.txt",1);
        try {
            InetAddress localIP = InetAddress.getLocalHost();
            final String strip = localIP.getHostAddress();

        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                textfield1.setText(strip);

            }
        });
        } catch (Exception e){
            e.printStackTrace();
        }
        threadDatagramSocket = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    DatagramSocket socket = new DatagramSocket(6001);
                    byte buffer[] = new byte[1024];
                    DatagramPacket packet = new DatagramPacket(buffer, 1024);
                    InetAddress localIP= InetAddress.getLocalHost();
                    while (!Thread.currentThread().isInterrupted()) {
                        //Ждем широковещательного пакета
                        socket.receive(packet);
                        String s=new String(packet.getData(),0,packet.getLength());
                        if (StringTool.parse(s,"",":").equals("BroadCastFastDefinition")){
                            String str = "OK:"+localIP.getHostAddress();
                            byte buf[] = str.getBytes();
                            DatagramPacket p = new DatagramPacket(buf,buf.length,packet.getAddress(),6001);
                            //Отправляем ответ что мы сервер
                            socket.send(p);
                        }

                    }
                }catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        threadDatagramSocket.setDaemon(true);
        threadDatagramSocket.start();
        threadSocket = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    ServerSocket socket = new ServerSocket(6000);
                    while (!Thread.currentThread().isInterrupted()){
                        Platform.runLater(new Runnable() {
                            @Override
                            public void run() {
                                label1.setText("Не подключено");
                                label1.setTextFill(Color.RED);
                                textfield2.setText("");
                            }
                        });
                        try {
                            //Ждём клиента
                            Socket client = socket.accept();
                            final String ipclient = client.getInetAddress().getHostAddress();
                            Platform.runLater(new Runnable() {
                                @Override
                                public void run() {
                                    label1.setText("Подключен");
                                    label1.setTextFill(Color.GREEN);
                                    textfield2.setText(ipclient);
                                }
                            });
                            DataOutputStream streamWriter = new DataOutputStream(client.getOutputStream());
                            BufferedImage image = null;
                            while (client.isConnected()){
                                try {
                                    //Делаем скриншот
                                    image = new Robot().createScreenCapture(new Rectangle(Toolkit.getDefaultToolkit().getScreenSize()));
                                } catch (AWTException e) {
                                    e.printStackTrace();
                                }
                                //Производим распознавание
                                HRecognize hRecognize = new HRecognize(image,defaultHero);
                                StringBuilder stringBuilder = new StringBuilder();
                                for (int i=0;i<5;i++){
                                    stringBuilder.append(hRecognize.heroes[i]).append(";");
                                }
                                stringBuilder.append(":");
                                for (int i=5;i<10;i++){
                                    stringBuilder.append(hRecognize.heroes[i]).append(";");
                                }
                                stringBuilder.append("\n");
                                String str = stringBuilder.toString();
                                //Отправляем то что распознали
                                streamWriter.writeUTF(str);
                                try {
                                    Thread.sleep(300);

                                } catch (InterruptedException e){
                                    threadSocket.interrupt();
                                }

                            }
                        }catch (IOException e){

                        }
                    }

                } catch (Exception e){
                    e.printStackTrace();
                }
            }
        });
        threadSocket.setDaemon(true);
        threadSocket.start();

    }
    public void setStage(Stage stage){
        this.stage = stage;
    }
}
