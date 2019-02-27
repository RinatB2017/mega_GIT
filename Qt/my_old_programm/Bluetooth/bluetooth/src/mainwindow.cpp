#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>


#define MYARDUINO_ADDRESS "00:14:02:10:09:04"

//#define MYARDUINO_ADDRESS "20:13:05:21:11:19"
#define MYARDUINO_NAME "hc05-arduino-nano"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Check if Bluetooth is available on this device
    if (!localDevice.isValid())
    {
        QMessageBox::critical(this, "Erreur fatale","Bluetooth local inactif!");
        return;
    }

    // Turn Bluetooth on
    localDevice.powerOn();

    // Read local device name
    localDeviceName = localDevice.name();

    // Make it visible to others
    localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    // Create a discovery agent and connect to its signals
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    // Start a discovery
    discoveryAgent->start();

    // Get connected devices, not used here!
    remotes = localDevice.connectedDevices();

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReady2Read()));

    //    interface
    this->setCursor(Qt::WaitCursor);
    ui->toolButton->setEnabled(false);
}


//          bluetooth device discovery
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
    QString br ="Found: "+device.address().toString();
    ui->textEdit->append(br);                   // show on interface

    if (localDevice.pairingStatus(device.address())== QBluetoothLocalDevice::Paired)
    {
        qDebug()<<"paired";
    }

    // device information not used here
    int classes = device.serviceClasses();
    QList<QBluetoothUuid> uidd = device.serviceUuids();
    int rssi = device.rssi();

    qDebug() << "classes" << classes;
    qDebug() << "rssi" << rssi;

    // we proceed only our remote device (Arduino/HC05 address), nothing for others...
    if (device.address().toString()!= MYARDUINO_ADDRESS)
    {
        qDebug() << "device not found";
        return;
    }

    qDebug() << "address" << device.address();

    // this is my Arduino device
    //socket->connectToService(device.address(), 1);      //   works under Android but not on Desktop
    // here is a misterious ????
    socket->connectToService(device.address(),QBluetoothUuid(QBluetoothUuid::SerialPort));      // only on Desktop

    socket->open(QIODevice::ReadWrite);
    if (socket->isOpen())          // write on interface
    {
        ui->textEdit->append("open "+device.address().toString());
    }

    this->setCursor(Qt::ArrowCursor);
}

//____________________________________________________________________________________________________
//   SLOT when data ready on bluetooth socket
void MainWindow::dataReady2Read()
{
    QByteArray data;
    data = socket->readAll();
    // juste show on interface
    ui->textEdit->append("receive:"+QString(data));
}

//____________________________________________________________________________________________________
//  SLOT when socket Connected
void MainWindow::socketConnected()
{
    // show on interface
    ui->textEdit->append("Connected to: "+socket->peerAddress().toString()+":"+socket->peerPort());
    ui->toolButton->setIcon(QIcon(":/interOff.png"));
    socket->write("OFF\n");
    // enable the Button
    ui->toolButton->setEnabled(true);
}

//____________________________________________________________________________________________________
//  destructor
MainWindow::~MainWindow()
{
    if (socket && socket->isOpen())
    {
        socket->close();
        delete socket;
    }
    delete ui;
}

//____________________________________________________________________________________________________
// SLOT button pushed
void MainWindow::on_toolButton_clicked()
{
    static bool state = false;

    if (!socket->isOpen() || !socket->isWritable())
    {
        return;      // a problem
    }

    if (state)         // make the remote led on, writing "ON"
    {
        ui->toolButton->setIcon(QIcon(":/interOn.png"));
        socket->write("ON\n");
        ui->textEdit->append("Send: on");
    }
    else               // make the remote led off, writing "OFF"
    {
        ui->toolButton->setIcon(QIcon(":/interOff.png"));
        socket->write("OFF\n");
        ui->textEdit->append("Send: off");
    }
    state = ! state;    // get the remote led state
}
