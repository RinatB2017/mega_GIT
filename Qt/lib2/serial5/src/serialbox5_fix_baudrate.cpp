/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QIODevice>
#include <QDateTime>
#include <QWidget>
#include <QEvent>
#include <QMenu>
#include <QTimer>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5_fix_baudrate.hpp"
#include "ui_serialbox5_fix_baudrate.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#   include "sendbox5.hpp"
#endif
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#define MAX_TIME_MSEC   100
//--------------------------------------------------------------------------------
typedef enum
{
    E_NO_ERROR = 0,
    E_PORT_NOT_INIT,
    E_PORT_NOT_OPEN
} ERRORS;
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate::SerialBox5_fix_baudrate(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::SerialBox5_fix_baudrate),
    parent(parent),
    caption("no name"),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate::SerialBox5_fix_baudrate(QWidget *parent,
                                                 const QString &caption,
                                                 const QString &o_name) :
    MyWidget(parent),
    ui(new Ui::SerialBox5_fix_baudrate),
    parent(parent),
    caption(caption),
    o_name(o_name),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate::~SerialBox5_fix_baudrate()
{
    if(serial5)
    {
        serial5->disconnect();
        serial5->close();
        delete serial5;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::set_caption(QString value)
{
    caption = value;
    o_name = value;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::set_fix_baudrate(int value)
{
    bool ok = false;
    fix_baudrate = value;
    if(serial5)
    {
        ok = serial5->setBaudRate(value);
    }
    return ok;
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_fix_baudrate::bytesAvailable(void)
{
    return serial5->bytesAvailable();
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_fix_baudrate::write(const char *data)
{
    return serial5->write(data);
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::init(void)
{
    ui->setupUi(this);

    createWidgets();
    initSerial();
    setCloseState();

    ui->PortBox->setMinimumWidth(150);

    ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));
    ui->btn_refresh->setToolTip("Обновить список портов");

    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedWidth(sizeHint().width());
    setFixedHeight(sizeHint().height());
    updateText();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::createWidgets(void)
{
    ui->gridLayout->setMargin(0);
    ui->gridLayout->setSpacing(0);

    ui->btn_power->setProperty(NO_BLOCK, true);
    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->PortBox->setProperty(NO_BLOCK, true);

    connect(ui->btn_power,      SIGNAL(clicked(bool)),  this,   SLOT(btnOpenPortClicked()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(this, SIGNAL(output(QByteArray)), this, SLOT(drawData(QByteArray)));
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox5_fix_baudrate::add_frame_text(QFrame *parent,
                                             const QString &text)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->setSpacing(0);
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter);
    hbox->addWidget(label);
    parent->setLayout(hbox);
    parent->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}
#endif
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::refresh(void)
{
    ui->PortBox->clear();
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts())
    {
        ui->PortBox->addItem(p_info.portName());
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::initSerial(void)
{
    serial5 = new QSerialPort(this);
    Q_CHECK_PTR(serial5);

    connect(serial5,    SIGNAL(readyRead()),            this,   SIGNAL(readyRead(void)));
    connect(serial5,    SIGNAL(readChannelFinished()),  this,   SIGNAL(readChannelFinished(void)));

    //TODO
    timer = new QTimer();
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(timer_stop()));

    connect(serial5, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));
    connect(serial5, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serial5_error(QSerialPort::SerialPortError)));

    connect(ui->btn_power,  SIGNAL(toggled(bool)),  this,   SLOT(change_icon(bool)));

    refresh();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::change_icon(bool state)
{
    if(state)
    {
        ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop)));
    }
    else
    {
        ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::serial5_error(QSerialPort::SerialPortError err)
{
    if(err == QSerialPort::NoError)
    {
        return;
    }

    switch(err)
    {
    case QSerialPort::DeviceNotFoundError:          emit error("DeviceNotFoundError");          break;
    case QSerialPort::PermissionError:              emit error("PermissionError");              break;
    case QSerialPort::OpenError:                    emit error("OpenError");                    break;
    case QSerialPort::ParityError:                  emit error("ParityError");                  break;
    case QSerialPort::FramingError:                 emit error("FramingError");                 break;
    case QSerialPort::BreakConditionError:          emit error("BreakConditionError");          break;
    case QSerialPort::WriteError:                   emit error("WriteError");                   break;
    case QSerialPort::ReadError:                    emit error("ReadError");                    break;
    case QSerialPort::ResourceError:                emit error("ResourceError");                break;
    case QSerialPort::UnsupportedOperationError:    emit error("UnsupportedOperationError");    break;
    case QSerialPort::UnknownError:                 emit error("UnknownError");                 break;
    case QSerialPort::TimeoutError:                 emit error("TimeoutError");                 break;
    case QSerialPort::NotOpenError:                 emit error("NotOpenError");                 break;

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }

    if(err != QSerialPort::NoError)
    {
        if(serial5->isOpen())
        {
            serial5->close();
        }
    }

    setCloseState();
    refresh();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));

    get_parameter();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::setCloseState(void)
{
    //ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop)));

    ui->btn_refresh->setEnabled(true);
    ui->PortBox->setEnabled(true);
    ui->btn_power->setChecked(false);
    emit is_close();
    ui->btn_power->setToolTip("Старт");
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::setOpenState()
{
    //ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));

    ui->btn_refresh->setEnabled(false);
    ui->PortBox->setEnabled(false);
    ui->btn_power->setChecked(true);
    emit is_open();
    ui->btn_power->setToolTip("Стоп");
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::btnOpenPortClicked()
{
    if (serial5)
    {
        bool result = serial5->isOpen();
        if (result)
        {
            serial5->close();
            emit is_close();
            result = false;
        }
        else
        {
            QString text = ui->PortBox->currentText();
            if(text.isEmpty())
            {
                if(serial5->isOpen())
                {
                    serial5->close();
                }
                setCloseState();
                return;
            }
            serial5->setPortName(text);
            result = serial5->open(QIODevice::ReadWrite);
            if(result)
            {
                bool ok = serial5->setBaudRate(fix_baudrate);
                if(!ok)
                {
                    emit error(QString("Не удалось установить baudrate %1").arg(fix_baudrate));
                }
                get_parameter();
                emit is_open();
            }
            else
            {
                emit error(QString("ERROR: serial [%1] not open (%2)")
                           .arg(serial5->portName())
                           .arg(serial5->errorString()));
                emit is_close();
            }
        }

        (result) ? setOpenState() : setCloseState();
    }
}
//--------------------------------------------------------------------------------
int SerialBox5_fix_baudrate::input(const QByteArray &sending_data)
{
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        emit is_close();
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit is_open();
        return E_PORT_NOT_OPEN;
    }
    if(flag_byte_by_byte)
    {
        emit debug("flag_byte_by_byte");
        for(int n=0; n<sending_data.length(); n++)
            serial5->write(sending_data.constData()+n, 1);
    }
    else
    {
        QString temp = sending_data;
        bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
        if(containsNonASCII == false)
        {
            if(temp.isEmpty() == false)
            {
                QString x_str = temp.remove('\r').remove('\n');
                //emit debug(QString("send: %1").arg(x_str));
            }
        }
        else
        {
            emit debug(QString("send [%1]").arg(sending_data.toHex().toUpper().data()));
        }
        serial5->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5_fix_baudrate::input(const QString &data)
{
    qDebug() << data;
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        emit is_close();
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit is_close();
        return E_PORT_NOT_OPEN;
    }
    QByteArray sending_data;
    sending_data.clear();
    sending_data.append(data);
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            serial5->write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        serial5->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::procSerialDataReceive(void)
{
    if(!serial5)
    {
        return;
    }
    if(!serial5->isOpen())
    {
        return;
    }

    //TODO было
    // emit output(serial5->readAll());

    if (!timer->isActive())
        timer->singleShot(MAX_TIME_MSEC, this, SLOT(timer_stop()));
    else
        timer->stop();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::timer_stop(void)
{
    emit output(serial5->readAll());
}
//--------------------------------------------------------------------------------
QString SerialBox5_fix_baudrate::ByteArrayToHex(const QByteArray &data)
{
#if 0
    QByteArray temp_addr;
    QByteArray temp;
    QString str;
    int i,n;

    int addr=0;

    temp_addr.append(((unsigned char*)&addr)[3]);
    temp_addr.append(((unsigned char*)&addr)[2]);
    temp_addr.append(((unsigned char*)&addr)[1]);
    temp_addr.append(((unsigned char*)&addr)[0]);
    str.append(temp_addr.toHex().toUpper());
    str.append("  ");
    temp = data.toHex().toUpper();
    n=0;
    for(i=0;i<temp.length();i+=2)
    {
        str.append(temp.at(i));
        str.append(temp.at(i+1));
        str.append(" ");
        n++;
        if(n>15 && i!=(temp.length()-2))
        {
            n=0;
            addr+=16;
            str.append("\n");
            temp_addr.clear();
            temp_addr.append(((unsigned char*)&addr)[3]);
            temp_addr.append(((unsigned char*)&addr)[2]);
            temp_addr.append(((unsigned char*)&addr)[1]);
            temp_addr.append(((unsigned char*)&addr)[0]);
            str.append(temp_addr.toHex().toUpper());
            str.append("  ");
        }
    }
    return str;
#else
    return data.toHex();
#endif
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::sendData(const QByteArray &sending_data)
{
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            serial5->write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        serial5->write(sending_data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
#ifdef RS232_LOG
    if(flag_in_hex)
    {
        emit info(ByteArrayToHex(data));
    }
    else
    {
        emit info(data.data());
    }
    if(flag_in_hex)
    {
        emit info(ByteArrayToHex(data));
    }
    else
    {
        //emit info(data);
    }
#else
    Q_UNUSED(data);
#endif
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::isOpen(void)
{
    return serial5->isOpen();
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::add_menu(int index)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    QMenu *menu = new QMenu(caption);
    Q_CHECK_PTR(menu);

    QAction *action_flag_in_hex = new QAction(menu);
    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_in_hex->setCheckable(true);
    action_flag_byte_by_byte->setCheckable(true);

    action_flag_in_hex->setText("in HEX");
    action_flag_byte_by_byte->setText("byte to byte");

    menu->addAction(action_flag_in_hex);
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_in_hex,         SIGNAL(triggered(bool)),    this,   SLOT(set_flag_in_hex(bool)));
    connect(action_flag_byte_by_byte,   SIGNAL(triggered(bool)),    this,   SLOT(set_flag_byte_by_byte(bool)));

    //---
    mw->add_optionsmenu_menu(index, menu);
    //---

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    QMenu *menu = new QMenu(title);
    Q_CHECK_PTR(menu);

    QAction *action_flag_in_hex = new QAction(menu);
    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_in_hex->setCheckable(true);
    action_flag_byte_by_byte->setCheckable(true);

    action_flag_in_hex->setText("in HEX");
    action_flag_byte_by_byte->setText("byte to byte");

    menu->addAction(action_flag_in_hex);
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_in_hex, SIGNAL(triggered(bool)), this, SLOT(set_flag_in_hex(bool)));
    connect(action_flag_byte_by_byte, SIGNAL(triggered(bool)), this, SLOT(set_flag_byte_by_byte(bool)));

    mw->add_menu(index, menu);

    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::set_flag_in_hex(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::set_flag_byte_by_byte(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::get_parameter(void)
{
    if(serial5->isOpen() == false)
    {
        return;
    }

    QString temp;
    temp.clear();
#if 1
    emit info(QString("baudRate %1").arg(serial5->baudRate()));
    emit info(QString("dataBits %1").arg(serial5->dataBits()));
    emit info(QString("parity %1").arg(serial5->parity()));
    emit info(QString("stopBits %1").arg(serial5->stopBits()));
    emit info(QString("flowControl %1").arg(serial5->flowControl()));
#else
    temp = QString("%1, %2, %3, %4, %5")
            .arg(serial5->baudRate())
            .arg(serial5->dataBits())
            .arg(serial5->parity())
            .arg(serial5->stopBits())
            .arg(serial5->flowControl());
#endif
    emit info(temp);
}
//--------------------------------------------------------------------------------
QByteArray SerialBox5_fix_baudrate::readAll(void)
{
    return serial5->readAll();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::set_test(bool value)
{
    p_test = value;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::get_test(void)
{
    emit error("Зачем жать куда попало?");
    return p_test;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
