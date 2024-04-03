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
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "serialbox5_lite.hpp"
#include "wifi_frame.hpp"
#include "qhexedit.h"
#include "logbox.hpp"

#include "ui_wifi_frame.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
WIFI_frame::WIFI_frame(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::WIFI_frame)
{
    init();
}
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
    connect(ui->serial_widget,  SIGNAL(port_is_active(bool)),   this,   SLOT(unlock_interface(bool)));

    if(is_server)
        connect(ui->serial_widget, SIGNAL(readyRead()), this, SLOT(server_port_read()));
    else
        connect(ui->serial_widget, SIGNAL(readyRead()), this, SLOT(client_port_read()));

    connect(ui->serial_widget,  SIGNAL(readChannelFinished()),  this,   SLOT(readChannelFinished()));
//    connect(ui->serial_widget,  SIGNAL(s_error(QSerialPort::SerialPortError)),  this,   SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void WIFI_frame::readChannelFinished(void)
{
    emit error("readChannelFinished");
}
//--------------------------------------------------------------------------------
void WIFI_frame::add_grid_layout(void)
{
    ui->le_Network->setText("A00000001");
    ui->le_Password->setText("0000000000");
    ui->le_IP->setText("192.168.11.10");
    ui->le_Gate->setText("192.168.11.1");
    ui->le_Mask->setText("255.255.255.0");
    ui->le_RemoteIP->setText("192.168.11.20");
    ui->le_RemotePort->setText("8080");

    ui->cb_EncryptType->clear();
    ui->cb_EncryptType->addItem("none");
    ui->cb_EncryptType->addItem("wep_open");
    ui->cb_EncryptType->addItem("wep");
    ui->cb_EncryptType->addItem("wpa_tkip");
    ui->cb_EncryptType->addItem("wpa_aes");
    ui->cb_EncryptType->addItem("wpa2_tkip");
    ui->cb_EncryptType->addItem("wpa2_aes");
    ui->cb_EncryptType->addItem("wpawpa2_tkip");
    ui->cb_EncryptType->addItem("wpawpa2_aes");
}
//--------------------------------------------------------------------------------
void WIFI_frame::init(void)
{
    ui->setupUi(this);
    connect_serial();

    server_is_created = false;
    connect(this,   &WIFI_frame::info,  ui->logBox, &LogBox::infoLog);
    connect(this,   &WIFI_frame::debug, ui->logBox, &LogBox::debugLog);
    connect(this,   &WIFI_frame::error, ui->logBox, &LogBox::errorLog);
    connect(this,   &WIFI_frame::trace, ui->logBox, &LogBox::traceLog);

    ui->btn_server->setEnabled(is_server);

    connect(ui->btn_read_settings, &QToolButton::clicked,   this,   &WIFI_frame::read_settings);

    ui->caption_label->setAlignment(Qt::AlignHCenter);

    add_server_cmd_layout();
    add_client_cmd_layout();
    add_grid_layout();

    lock_interface(false);
}
//--------------------------------------------------------------------------------
void WIFI_frame::server_port_read(void)
{
    emit debug(QString("server_is_created %1").arg(server_is_created ? "true" : "false"));
    if(server_is_created)
    {
        serial_data.append(ui->serial_widget->readAll());
    }
    else
    {
        while(ui->serial_widget->bytesAvailable())
        {
            QCoreApplication::processEvents();
            //emit info(QString("bytesAvailable = %1").arg(serial->bytesAvailable()));
            serial_data.append(ui->serial_widget->readAll());
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
    while(ui->serial_widget->bytesAvailable())
    {
        QCoreApplication::processEvents();
        //emit info(QString("bytesAvailable = %1").arg(serial->bytesAvailable()));
        serial_data.append(ui->serial_widget->readAll());
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
#endif
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
        //default:
        //    emit error(QString("Unknown error %1").arg(serial_error));
        //    break;
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
            ui->le_Network->setText(sl.at(0));
            ui->cb_EncryptType->setCurrentText(sl.at(1));
            ui->le_Password->setText(sl.at(2));
        }
        else
        {
            ui->le_Network->setText("---");
            ui->cb_EncryptType->setCurrentText("---");
            ui->le_Password->setText("---");
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
            ui->le_IP->setText(sl.at(0));
            ui->le_Mask->setText(sl.at(1));
            ui->le_Gate->setText(sl.at(2));
        }
        else
        {
            ui->le_IP->setText("---");
            ui->le_Mask->setText("---");
            ui->le_Gate->setText("---");
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
bool WIFI_frame::send_at_command(const QString &cmd,
                                 int wait_ms,
                                 bool no_response)
{
    if(ui->serial_widget->isOpen() == false)
    {
        emit error(tr("Port not open!"));
        return false;
    }

//    QByteArray ba;
//    ba.clear();
//    ba.append(cmd);

    serial_data.clear();
    is_ready = false;
    ui->serial_widget->write(cmd.toLatin1());

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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    serial_data.replace(cmd, "").replace("\r", "").replace("\n", "");
#else
    serial_data.replace("\r", "").replace("\n", "");
#endif
    emit debug(serial_data);

    if(serial_data == "ERROR")
    {
        emit error(QString("cmd [%1] return [%2]")
                   .arg(cmd.toLatin1().replace("\r", "").data())
                   .arg(serial_data.data()));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool WIFI_frame::send_command(QString cmd,
                              int wait_ms)
{
    if(ui->serial_widget->isOpen() == false)
    {
        emit error(tr("Port not open!"));
        return false;
    }

//    QByteArray ba;
//    ba.clear();
//    ba.append(cmd);

    serial_data.clear();
    is_ready = false;
    ui->serial_widget->write(cmd.toLatin1());

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
    QElapsedTimer time;

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
            .arg(ui->le_Network->text())
            .arg(ui->cb_EncryptType->currentText())
            .arg(ui->le_Password->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    if(!is_silense) emit info("dhcpd");
    ok = send_at_command("at+dhcpd=0\r");
    if(!ok) return false;

    temp = QString("at+net_ip=%1,%2,%3\r")
            .arg(ui->le_IP->text())
            .arg(ui->le_Mask->text())
            .arg(ui->le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+net_dns=%1\r")
            .arg(ui->le_Gate->text());
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
            .arg(ui->le_Network->text())
            .arg(ui->cb_EncryptType->currentText())
            .arg(ui->le_Password->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    if(!is_silense) emit info("dhcpc");
    ok = send_at_command("at+dhcpc=0\r");
    if(!ok) return false;

    temp = QString("at+net_ip=%1,%2,%3\r")
            .arg(ui->le_IP->text())
            .arg(ui->le_Mask->text())
            .arg(ui->le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+net_dns=%1\r")
            .arg(ui->le_Gate->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+remoteip=%1\r")
            .arg(ui->le_RemoteIP->text());
    if(!is_silense) emit info(temp);
    ok = send_at_command(temp);
    if(!ok) return false;

    temp = QString("at+remoteport=%1\r")
            .arg(ui->le_RemotePort->text());
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
void WIFI_frame::add_server_cmd_layout(void)
{
    if(is_server)
    {
        connect(ui->btn_server, SIGNAL(clicked()), this, SLOT(create_server()));
    }
}
//--------------------------------------------------------------------------------
void WIFI_frame::add_client_cmd_layout(void)
{
    if(is_server == false)
    {
        connect(ui->btn_client, SIGNAL(clicked()), this, SLOT(create_client()));
    }
}
//-------------------------------------------------------------------------------
void WIFI_frame::lock_interface(bool state)
{
    if(is_server)
    {
        ui->btn_server->setDisabled(state);
    }
    else
    {
        ui->btn_client->setDisabled(state);
        ui->btn_send_data->setDisabled(state);
    }

    ui->btn_read_settings->setDisabled(state);

    ui->le_Gate->setDisabled(state);
    ui->le_IP->setDisabled(state);
    ui->le_Mask->setDisabled(state);
    ui->le_Network->setDisabled(state);
    ui->le_Password->setDisabled(state);
    ui->le_RemoteIP->setDisabled(state);
    ui->le_RemotePort->setDisabled(state);

    ui->cb_EncryptType->setDisabled(state);
}
//-------------------------------------------------------------------------------
void WIFI_frame::unlock_interface(bool state)
{
    if(is_server)
    {
        ui->btn_server->setEnabled(state);
    }
    else
    {
        ui->btn_client->setEnabled(state);
        ui->btn_send_data->setEnabled(state);
    }

    ui->btn_read_settings->setEnabled(state);

    ui->le_Gate->setEnabled(state);
    ui->le_IP->setEnabled(state);
    ui->le_Mask->setEnabled(state);
    ui->le_Network->setEnabled(state);
    ui->le_Password->setEnabled(state);
    ui->le_RemoteIP->setEnabled(state);
    ui->le_RemotePort->setEnabled(state);

    ui->cb_EncryptType->setEnabled(state);
}//--------------------------------------------------------------------------------
void WIFI_frame::show_hex_data(QByteArray &data)
{
    QHexEdit *hexedit = new QHexEdit();
    hexedit->setMinimumSize(640, 200);
    hexedit->setData(QHexEditData::fromMemory(data));
    hexedit->show();
}
//--------------------------------------------------------------------------------
void WIFI_frame::update_ports(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void WIFI_frame::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool WIFI_frame::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void WIFI_frame::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void WIFI_frame::save_setting(void)
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
