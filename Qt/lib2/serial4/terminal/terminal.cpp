//--------------------------------------------------------------------------------
#include <QSettings>
#include <QDateTime>
#include <QString>
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "serialdeviceenumerator.h"
#include "abstractserial.h"
//--------------------------------------------------------------------------------
#include "terminal.hpp"
//--------------------------------------------------------------------------------
Terminal::Terminal(const QString &filename, QWidget *parent) :
    QObject(parent)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }

    init(filename);
}
//--------------------------------------------------------------------------------
Terminal::~Terminal()
{
}
//--------------------------------------------------------------------------------
void Terminal::init(const QString &filename)
{
    read_ini(filename);
    init_serial();
}
//--------------------------------------------------------------------------------
void Terminal::read_ini(const QString &filename)
{
    emit debug(QString("read %1").arg(filename));

    QSettings *settings = new QSettings("terminal.ini", QSettings::IniFormat);
    device    = settings->value("Device",     "/dev/ttyUSB0").toString();
    baud_rate = settings->value("Baud_rate",  "4800 baud").toString();
    data_bits = settings->value("Data_bits",  "8 bit").toString();
    parity    = settings->value("Parity",     "None").toString();
    stop_bits = settings->value("Stop_bits",  "1").toString();
    flow      = settings->value("Flow",       "Disable").toString();

    settings->endGroup();
    delete settings;
}
//--------------------------------------------------------------------------------
void Terminal::init_serial(void)
{
    serial = new AbstractSerial(this);
    connect(serial, SIGNAL(ctsChanged(bool)),  this, SIGNAL(procCtsChanged(bool)));
    connect(serial, SIGNAL(dsrChanged(bool)),  this, SIGNAL(procDsrChanged(bool)));
    connect(serial, SIGNAL(ringChanged(bool)), this, SIGNAL(procRingChanged(bool)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));

    connect(serial, SIGNAL(signalStatus(QString,QDateTime)), this, SLOT(getStatus(QString,QDateTime)));

    // Enable emmiting signal signalStatus
    serial->enableEmitStatus(true);

    serial->setDeviceName(device);
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(baud_rate);
    serial->setDataBits(data_bits);
    serial->setParity(parity);
    serial->setStopBits(stop_bits);
    serial->setFlowControl(flow);
}
//--------------------------------------------------------------------------------
void Terminal::procSerialDataReceive()
{
    QByteArray data;
    if(!this->serial)
    {
        return;
    }
    if(!this->serial->isOpen())
    {
        return;
    }
    data = serial->readAll();
    emit getdata(data);
}
//--------------------------------------------------------------------------------
void Terminal::getStatus(const QString &status, QDateTime current)
{
    Q_UNUSED(current);
    emit info(status);
}
//--------------------------------------------------------------------------------
void Terminal::send(const QByteArray &data)
{
    if(!this->serial)
    {
        return;
    }
    if(!this->serial->isOpen())
    {
        return;
    }
    serial->write(data);
}
//--------------------------------------------------------------------------------
