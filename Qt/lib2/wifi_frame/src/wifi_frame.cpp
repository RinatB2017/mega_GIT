/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QCoreApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QFrame>
#include <QLabel>
#include <QTime>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "serialbox5_lite.hpp"
#include "wifi_frame.hpp"
#include "qhexedit.h"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
WIFI_frame::WIFI_frame(const QString &caption,
                       bool is_server,
                       QWidget *parent) :
    MyWidget(parent),
    caption(caption),
    is_server(is_server)
{
    init();
}
//--------------------------------------------------------------------------------
void WIFI_frame::connect_serial(void)
{
    serial = new SerialBox5_lite;
    //serial->setFixedHeight(serial->sizeHint().height());

    connect(serial, SIGNAL(is_open()),  this,   SLOT(unlock_interface()));
    connect(serial, SIGNAL(is_close()), this,   SLOT(lock_interface()));

    if(is_server)
        connect(serial, SIGNAL(readyRead()), this, SLOT(server_port_read()));
    else
        connect(serial, SIGNAL(readyRead()), this, SLOT(client_port_read()));

    connect(serial, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this,   SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void WIFI_frame::readChannelFinished(void)
{
    emit error("readChannelFinished");
}
//--------------------------------------------------------------------------------
QGridLayout *WIFI_frame::add_grid_layout(void)
{
    le_Network      = new QLineEdit("A00000001");
    le_Password     = new QLineEdit("0000000000");
    le_IP           = new QLineEdit("192.168.11.10");
    le_Gate         = new QLineEdit("192.168.11.1");
    le_Mask         = new QLineEdit("255.255.255.0");
    le_RemoteIP     = new QLineEdit("192.168.11.20");
    le_RemotePort   = new QLineEdit("8080");

    cb_EncryptType = new QComboBox(this);
    cb_EncryptType->clear();
    cb_EncryptType->addItem("none");
    cb_EncryptType->addItem("wep_open");
    cb_EncryptType->addItem("wep");
    cb_EncryptType->addItem("wpa_tkip");
    cb_EncryptType->addItem("wpa_aes");
    cb_EncryptType->addItem("wpa2_tkip");
    cb_EncryptType->addItem("wpa2_aes");
    cb_EncryptType->addItem("wpawpa2_tkip");
    cb_EncryptType->addItem("wpawpa2_aes");

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("network"),      0, 0);
    grid->addWidget(le_Network,                 0, 1);
    grid->addWidget(new QLabel("password"),     0, 2);
    grid->addWidget(le_Password,                0, 3);
    grid->addWidget(new QLabel("IP"),           1, 0);
    grid->addWidget(le_IP,                      1, 1);
    grid->addWidget(new QLabel("gate"),         1, 2);
    grid->addWidget(le_Gate,                    1, 3);
    grid->addWidget(new QLabel("mask"),         2, 0);
    grid->addWidget(le_Mask,                    2, 1);
    grid->addWidget(new QLabel("remote IP"),    2, 2);
    grid->addWidget(le_RemoteIP,                2, 3);
    grid->addWidget(new QLabel("remote port"),  3, 0);
    grid->addWidget(le_RemotePort,              3, 1);
    grid->addWidget(new QLabel("encrypt type"), 3, 2);
    grid->addWidget(cb_EncryptType,             3, 3);

    return grid;
}
//--------------------------------------------------------------------------------
void WIFI_frame::init(void)
{
    connect_serial();

    server_is_created = false;

    logBox = new LogBox("WiFi", this);
    connect(this,   SIGNAL(info(QString)),  logBox, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)), logBox, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)), logBox, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)), logBox, SLOT(traceLog(QString)));

    QVBoxLayout *main_layout = new QVBoxLayout();

    main_layout->addWidget(serial);
    if(is_server)
            main_layout->addLayout(add_server_cmd_layout());
    else
    main_layout->addLayout(add_client_cmd_layout());

    btn_read_settings = new QPushButton(tr("read settings"));
    connect(btn_read_settings, SIGNAL(clicked()), this, SLOT(read_settings()));
    main_layout->addWidget(btn_read_settings);

    main_layout->addWidget(logBox, 1);

    QLabel *label = new QLabel(caption);
    label->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(label);
    vbox->addLayout(add_grid_layout());
    vbox->addLayout(main_layout);

    setLayout(vbox);

    lock_interface();
}
//--------------------------------------------------------------------------------
void WIFI_frame::server_port_read(void)
{
    //emit debug(QString("server_is_created %1").arg(server_is_created ? "true" : "false"));
    if(server_is_created)
    {
        serial_data.append(serial->readAll());
    }
    else
    {
        while(serial->bytesAvailable())
        {
            QCoreApplication::processEvents();
            //emit info(QString("bytesAvailable = %1").arg(serial->bytesAvailable()));
            serial_data.append(serial->readAll());
            //serial_data.append(serial->read(1));
            //serial->waitForReadyRead(300);
        }
        emit debug(QString(tr("receive: %1")).arg(serial_data.data()));
        send_command("Don't worry, be happy!");
    }
}
//--------------------------------------------------------------------------------
void WIFI_frame::client_port_read(void)
{
    //serial->waitForReadyRead(100);
    while(serial->bytesAvailable())
    {
        QCoreApplication::processEvents();
        //emit info(QString("bytesAvailable = %1").arg(serial->bytesAvailable()));
        serial_data.append(serial->readAll());
    }
    //is_ready = true;
}
//--------------------------------------------------------------------------------
void WIFI_frame::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
    default:
        emit error(QString("Unknown error %1").arg(serial_error));
        break;
    }
}
//--------------------------------------------------------------------------------
void WIFI_frame::read_settings(void)
{
    bool ok = false;

    emit info("begin read");

    ok = send_at_command("at+netmode=?\r");
    if(ok)
    {
        int temp = serial_data.toInt(&ok);
        if(ok)
        {
            switch(temp)
            {
            case 0:
                emit info("Default setup");
                break;
            case 1:
                emit info("Ethernet");
                break;
            case 2:
                emit info("Wifi client");
                break;
            case 3:
                emit info("Wifi ap");
                break;
            default:
                emit info(QString("Unknown value %1").arg(temp));
                break;
            }
        }
    }

    ok = send_at_command("at+wifi_conf=?\r");
    if(ok)
    {
        QStringList sl = QString(serial_data).split(',');
        if(sl.count() == 3)
        {
            le_Network->setText(sl.at(0));
            cb_EncryptType->setCurrentText(sl.at(1));
            le_Password->setText(sl.at(2));
        }
        else
        {
            le_Network->setText("---");
            cb_EncryptType->setCurrentText("---");
            le_Password->setText("---");
        }
    }

    ok = send_at_command("at+dhcpd=?\r");
    if(ok)
    {
        int temp = serial_data.toInt(&ok);
        if(ok)
        {
            switch(temp)
            {
            case 0:
                emit info("Close");
                break;
            case 1:
                emit info("Open");
                break;
            }
        }
    }

    ok = send_at_command("at+net_ip=?\r");
    if(ok)
    {
        QStringList sl = QString(serial_data).split(',');
        if(sl.count() == 3)
        {
            le_IP->setText(sl.at(0));
            le_Mask->setText(sl.at(1));
            le_Gate->setText(sl.at(2));
        }
        else
        {
            le_IP->setText("---");
            le_Mask->setText("---");
            le_Gate->setText("---");
        }
    }

    ok = send_at_command("at+net_dns=?\r");
    if(ok)
    {
        //TODO
    }

    ok = send_at_command("at+remotepro=?\r");
    if(ok)
    {
        //TODO
    }

    ok = send_at_command("at+mode=?\r");
    if(ok)
    {
        //TODO
    }

    emit info("end read");
}
//--------------------------------------------------------------------------------
QString WIFI_frame::get_server_string(void)
{
    QString temp;
    temp.clear();
    temp.append("at+netmode=3\r\n");
    temp.append("at+wifi_conf=A00000001,none,0000000000\r\n");
    temp.append("at+dhcpd=0\r\n");
    temp.append("at+net_ip=192.168.11.10,255.255.255.0,192.168.11.1\r\n");
    temp.append("at+net_dns=192.168.11.1\r\n");
    temp.append("\n");
    temp.append("at+remotepro=tcp\r\n");
    temp.append("at+timeout=0\r\n");
    temp.append("at+mode=server\r\n");
    temp.append("at+uart=115200,8,n,1\r\n");
    temp.append("at+uartpacklen=64\r\n");
    temp.append("at+uartpacktimeout=10\r\n");
    temp.append("at+net_commit=1\r\n");
    temp.append("at+out_trans=0\r\n");
    temp.append("at+reconn=1\r\n");

    return temp;
}
//--------------------------------------------------------------------------------
QString WIFI_frame::get_client_string(void)
{
    QString temp;
    temp.clear();
    temp.append("at+netmode=2\r\n");
    temp.append("at+wifi_conf=A00000001,none,0000000000\r\n");
    temp.append("at+dhcpc=0\r\n");
    temp.append("at+net_ip=192.168.11.20,255.255.255.0,192.168.11.1\r\n");
    temp.append("at+net_dns=192.168.11.1\r\n");
    temp.append("\n");
    temp.append("at+remoteip=192.168.11.10\r\n");
    temp.append("at+remoteport=8080\r\n");
    temp.append("at+remotepro=tcp\r\n");
    temp.append("at+timeout=0\r\n");
    temp.append("at+mode=client\r\n");
    temp.append("at+uart=115200,8,n,1\r\n");
    temp.append("at+uartpacklen=64\r\n");
    temp.append("at+uartpacktimeout=10\r\n");
    temp.append("at+net_commit=1\r\n");
    temp.append("at+out_trans=0\r\n");
    temp.append("at+reconn=1\r\n");

    return temp;
}
//--------------------------------------------------------------------------------
bool WIFI_frame::send_at_command(QString cmd,
                                 unsigned int wait_ms,
                                 bool no_response)
{
    if(serial->isOpen() == false)
    {
        emit error(tr("Port not open!"));
        return false;
    }

    QByteArray ba;
    ba.clear();
    ba.append(cmd);

    serial_data.clear();
    is_ready = false;
    serial->write(ba);

    wait_msec(wait_ms);

    if(no_response)
    {
        return true;
    }

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return false;
    }
    serial_data.replace(cmd, "").replace("\r", "").replace("\n", "");
    emit debug(serial_data);

    if(serial_data == "ERROR")
    {
        emit error(QString(tr("cmd [%1] return [%2]"))
                   .arg(cmd.replace("\r", ""))
                   .arg(serial_data.data()));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool WIFI_frame::send_command(QString cmd,
                              unsigned int wait_ms)
{
    if(serial->isOpen() == false)
    {
        emit error(tr("Port not open!"));
        return false;
    }

    QByteArray ba;
    ba.clear();
    ba.append(cmd);

    serial_data.clear();
    is_ready = false;
    serial->write(ba);

    if(caption == "server")
    {
        return true;
    }

    wait_msec(wait_ms);

    if(serial_data.isEmpty())
    {
        emit error("No data!");
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
void WIFI_frame::wait_msec(int timeout_msec)
{
    QTime time;

    if(timeout_msec < 1)
    {
        return;
    }

    time.start();
    while(time.elapsed() < timeout_msec)
    {
        QCoreApplication::processEvents();
        if(is_ready) break;
    }
}
//--------------------------------------------------------------------------------
bool WIFI_frame::send_cmd_create_server(bool is_silense)
{
    QString temp;
    bool ok = false;

    if(!is_silense) emit info("netmode");
    ok = send_at_command("at+netmode=3\r");
    if(!ok) return false;

    temp = QString("at+wifi_conf=%1,%2,%3\r")
            .arg(le_Network->text())
            .arg(cb_EncryptType->currentText())
            .arg(le_Password->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    if(!is_silense) emit info("dhcpd");
    ok = send_at_command("at+dhcpd=0\r");
    if(!ok) return false;

    temp = QString("at+net_ip=%1,%2,%3\r")
            .arg(le_IP->text())
            .arg(le_Mask->text())
            .arg(le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+net_dns=%1\r")
            .arg(le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    if(!is_silense) emit info("remotepro");
    ok = send_at_command("at+remotepro=tcp\r");
    if(!ok) return false;

    if(!is_silense) emit info("timeout");
    ok = send_at_command("at+timeout=0\r");
    if(!ok) return false;

    if(!is_silense) emit info("mode");
    ok = send_at_command("at+mode=server\r");
    if(!ok) return false;

    if(!is_silense) emit info("uart");
    ok = send_at_command("at+uart=115200,8,n,1\r");
    if(!ok) return false;

    if(!is_silense) emit info("uartpacklen");
    ok = send_at_command("at+uartpacklen=64\r");
    if(!ok) return false;

    if(!is_silense) emit info("uartpacktimeout");
    ok = send_at_command("at+uartpacktimeout=10\r");
    if(!ok) return false;

    if(!is_silense) emit info("net_commit");
    ok = send_at_command("at+net_commit=1\r");
    if(!ok) return false;

    if(!is_silense) emit info("out_trans");
    ok = send_at_command("at+out_trans=0\r", 200, true);
    if(!ok) return false;

    if(!is_silense) emit info("reconn");
    ok = send_at_command("at+reconn=1\r", 200, true);
    if(!ok) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool WIFI_frame::send_cmd_create_client(bool is_silense)
{
    QString temp;
    bool ok = false;

    if(!is_silense) emit info("netmode");
    ok = send_at_command("at+netmode=2\r");
    if(!ok) return false;

    temp = QString("at+wifi_conf=%1,%2,%3\r")
            .arg(le_Network->text())
            .arg(cb_EncryptType->currentText())
            .arg(le_Password->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    if(!is_silense) emit info("dhcpc");
    ok = send_at_command("at+dhcpc=0\r");
    if(!ok) return false;

    temp = QString("at+net_ip=%1,%2,%3\r")
            .arg(le_IP->text())
            .arg(le_Mask->text())
            .arg(le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+net_dns=%1\r")
            .arg(le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+remoteip=%1\r")
            .arg(le_RemoteIP->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+remoteport=%1\r")
            .arg(le_RemotePort->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    if(!is_silense) emit info("remotepro");
    ok = send_at_command("at+remotepro=tcp\r");
    if(!ok) return false;

    if(!is_silense) emit info("timeout");
    ok = send_at_command("at+timeout=0\r");
    if(!ok) return false;

    if(!is_silense) emit info("mode");
    ok = send_at_command("at+mode=client\r");
    if(!ok) return false;

    if(!is_silense) emit info("uart");
    ok = send_at_command("at+uart=115200,8,n,1\r");
    if(!ok) return false;

    if(!is_silense) emit info("uartpacklen");
    ok = send_at_command("at+uartpacklen=64\r");
    if(!ok) return false;

    if(!is_silense) emit info("uartpacktimeout");
    ok = send_at_command("at+uartpacktimeout=10\r");
    if(!ok) return false;

    if(!is_silense) emit info("net_commit");
    ok = send_at_command("at+net_commit=1\r");
    if(!ok) return false;

    if(!is_silense) emit info("out_trans");
    ok = send_at_command("at+out_trans=0\r", 200, true);
    if(!ok) return false;

    if(!is_silense) emit info("reconn");
    ok = send_at_command("at+reconn=1\r", 200, true);
    if(!ok) return false;

    return true;
}
//--------------------------------------------------------------------------------
void WIFI_frame::create_server(void)
{
    emit info("create server");
    server_is_created = true;

    bool ok = send_cmd_create_server();
    if(ok)
        emit info(tr("server created"));
    else
        emit error(tr("server NOT created"));

    server_is_created = false;
    emit info("The end!");
}
//--------------------------------------------------------------------------------
void WIFI_frame::create_client(void)
{
    emit info("create client");

    bool ok = send_cmd_create_client();
    if(ok)
        emit info(tr("client created"));
    else
        emit error(tr("client NOT created"));

    emit info("The end!");
}
//--------------------------------------------------------------------------------
void WIFI_frame::send_data(void)
{
    emit info("send data");
    bool ok = send_command("client: question", 1000);
    if(!ok)
        emit error("error send data!");
    else
        emit info(QString("received: %1").arg(serial_data.data()));
}
//--------------------------------------------------------------------------------
QVBoxLayout *WIFI_frame::add_server_cmd_layout(void)
{
    if(is_server)
    {
        btn_server = new QPushButton("create server");
        connect(btn_server, SIGNAL(clicked()), this, SLOT(create_server()));
    }

    QVBoxLayout *cmd_box = new QVBoxLayout();
    cmd_box->addWidget(btn_server);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(1);
    vbox->addLayout(cmd_box);
    vbox->addStretch(1);

    return vbox;
}
//--------------------------------------------------------------------------------
QVBoxLayout *WIFI_frame::add_client_cmd_layout(void)
{
    if(is_server == false)
    {
        btn_client = new QPushButton("create client");
        connect(btn_client, SIGNAL(clicked()), this, SLOT(create_client()));
    }

    btn_send_data = new QPushButton("send data");
    connect(btn_send_data, SIGNAL(clicked()), this, SLOT(send_data()));

    QVBoxLayout *cmd_box = new QVBoxLayout();
    cmd_box->addWidget(btn_client);
    cmd_box->addWidget(btn_send_data);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(1);
    vbox->addLayout(cmd_box);
    vbox->addStretch(1);

    return vbox;
}
//--------------------------------------------------------------------------------
void WIFI_frame::lock_interface(void)
{
    if(is_server)
    {
        btn_server->setEnabled(false);
    }
    else
    {
        btn_client->setEnabled(false);
        btn_send_data->setEnabled(false);
    }

    btn_read_settings->setEnabled(false);

    le_Gate->setEnabled(false);
    le_IP->setEnabled(false);
    le_Mask->setEnabled(false);
    le_Network->setEnabled(false);
    le_Password->setEnabled(false);
    le_RemoteIP->setEnabled(false);
    le_RemotePort->setEnabled(false);

    cb_EncryptType->setEnabled(false);
}
//--------------------------------------------------------------------------------
void WIFI_frame::unlock_interface(void)
{
    if(is_server)
    {
        btn_server->setEnabled(true);
    }
    else
    {
        btn_client->setEnabled(true);
        btn_send_data->setEnabled(true);
    }

    btn_read_settings->setEnabled(true);

    le_Gate->setEnabled(true);
    le_IP->setEnabled(true);
    le_Mask->setEnabled(true);
    le_Network->setEnabled(true);
    le_Password->setEnabled(true);
    le_RemoteIP->setEnabled(true);
    le_RemotePort->setEnabled(true);

    cb_EncryptType->setEnabled(true);
}
//--------------------------------------------------------------------------------
void WIFI_frame::show_hex_data(QByteArray &data)
{
    QHexEdit *hexedit = new QHexEdit();
    hexedit->setMinimumSize(640, 200);
    hexedit->setData(QHexEditData::fromMemory(data));
    hexedit->show();
}
//--------------------------------------------------------------------------------
void WIFI_frame::updateText(void)
{
}
//--------------------------------------------------------------------------------
void WIFI_frame::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        updateText();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
