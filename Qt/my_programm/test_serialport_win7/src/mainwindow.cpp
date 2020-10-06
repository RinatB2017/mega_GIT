#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead,        this,   &MainWindow::read_data);
    connect(serial, &QSerialPort::errorOccurred,    this,   &MainWindow::serial5_error);

    connect(ui->btn_port,   &QToolButton::clicked,  this,   &MainWindow::get_portname);
    connect(ui->btn_open,   &QToolButton::clicked,  this,   &MainWindow::port_open);
    connect(ui->btn_close,  &QToolButton::clicked,  this,   &MainWindow::port_close);
    connect(ui->btn_send,   &QPushButton::clicked,  this,   &MainWindow::port_send);
}

void MainWindow::serial5_error(QSerialPort::SerialPortError err)
{
    if(err == QSerialPort::NoError)
    {
        return;
    }

    switch(err)
    {
    case QSerialPort::DeviceNotFoundError:          ui->te_log->append("DeviceNotFoundError");          break;
    case QSerialPort::PermissionError:              ui->te_log->append("PermissionError");              break;
    case QSerialPort::OpenError:                    ui->te_log->append("OpenError");                    break;
    case QSerialPort::ParityError:                  ui->te_log->append("ParityError");                  break;
    case QSerialPort::FramingError:                 ui->te_log->append("FramingError");                 break;
    case QSerialPort::BreakConditionError:          ui->te_log->append("BreakConditionError");          break;
    case QSerialPort::WriteError:                   ui->te_log->append("WriteError");                   break;
    case QSerialPort::ReadError:                    ui->te_log->append("ReadError");                    break;
    case QSerialPort::ResourceError:                ui->te_log->append("ResourceError");                break;
    case QSerialPort::UnsupportedOperationError:    ui->te_log->append("UnsupportedOperationError");    break;
    case QSerialPort::UnknownError:                 ui->te_log->append("UnknownError");                 break;
    case QSerialPort::TimeoutError:                 ui->te_log->append("TimeoutError");                 break;
    case QSerialPort::NotOpenError:                 ui->te_log->append("NotOpenError");                 break;

    default:
        ui->te_log->append(QString("unknown error %1").arg(err));
        break;
    }

    if(err != QSerialPort::NoError)
    {
        if(serial->isOpen())
        {
            serial->close();
        }
    }
}

void MainWindow::get_portname(void)
{
    ui->cb_port->clear();
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts())
    {
        ui->cb_port->addItem(p_info.portName());
    }
}

void MainWindow::port_open(void)
{
    ui->te_log->append(QString("%1 open").arg(ui->cb_port->currentText()));

    serial->setPortName(ui->cb_port->currentText());
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    serial->open(QIODevice::ReadWrite);
}

void MainWindow::port_close(void)
{
    ui->te_log->append("close");
    serial->close();
}

void MainWindow::port_send(void)
{
    //ui->te_log->append("send data");
    ui->te_log->append(QString("send %1 bytes").arg(serial->write("test")));
}

void MainWindow::read_data(void)
{
    QByteArray ba = serial->readAll();
    ui->te_log->append(ba);
}

MainWindow::~MainWindow()
{
    if(serial)
    {
        serial->disconnect();
        serial->deleteLater();
    }

    delete ui;
}

