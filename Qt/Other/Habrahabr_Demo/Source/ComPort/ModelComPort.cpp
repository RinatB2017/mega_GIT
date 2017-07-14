#include "ModelComPort.h"
#include "Commands.h"

#include <QSerialPortInfo>
#include <QDebug>

ModelComPort::ModelComPort() :
    m_portName(""),
    m_baudrate(QSerialPort::Baud9600),
    m_dataBits(QSerialPort::Data8),
    m_parity(QSerialPort::NoParity),
    m_stopBits(QSerialPort::OneStop),
    m_flowControl(QSerialPort::NoFlowControl),
    m_connected(false)
{
    thread = ComPortThread::getInstance();

    searchComPorts();
}

ModelComPort::~ModelComPort()
{
    qDebug("~ModelComPort()");
    if (thread)
    {
        thread->free();
        thread = 0;
    }
}

void ModelComPort::connectToComPort()
{
    if (!m_connected)
    {
        if (m_portName == "")
        {
            return;
        }

        if (!thread->isRunning())
        {
            thread->connectCom(m_portName, m_baudrate, m_dataBits, m_dataBits, m_stopBits, m_flowControl);
            thread->wait(500);

            // В случае успешного подключения
            if (thread->isConnect())
            {
                m_connected = true;
            }
        }
    }
    else
    {
        if (thread->isConnect())
        {
            thread->disconnectCom();
        }
        m_connected = false;
    }
}

void ModelComPort::setPortName(QString portName)
{
    m_portName = portName;
}

QString ModelComPort::getPortName() const
{
    return m_portName;
}

void ModelComPort::setBaudrate(int baudrate)
{
    m_baudrate = baudrate;
}

int ModelComPort::getBaudrate() const
{
    return m_baudrate;
}

bool ModelComPort::isConnect() const
{
    return m_connected;
}

void ModelComPort::onCommand(QString command)
{
    // Переводим текстовую команду.
    if (command == "On")
    {
        sendCommand(ON);
    }
    else if (command == "Off")
    {
        sendCommand(OFF);
    }
}

void ModelComPort::response(QByteArray msg)
{
    // Разбор пришедшего пакета
}

QList<QString> ModelComPort::getListNamePorts() const
{
    return m_listPorts;
}

void ModelComPort::searchComPorts()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        m_listPorts.append(info.portName());
    }
}

void ModelComPort::sendCommand(int command)
{
    QByteArray buffer;
    quint8 checkSumm = 0;

    buffer[0] = '#';
    buffer[1] = '<';
    buffer[2] = 0;
    checkSumm ^= buffer[3];
    buffer[3] = command;
    checkSumm ^= buffer[4];
    buffer[4] = checkSumm;

    thread->transaction(buffer, 250);
}
