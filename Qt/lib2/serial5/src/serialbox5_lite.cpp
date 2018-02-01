/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include "serialbox5_lite.hpp"
#include "ui_serialbox5_lite.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#include "sendbox5.hpp"
#endif
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
SerialBox5_lite::SerialBox5_lite(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::SerialBox5_lite),
    parent(parent),
    caption("no name"),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_lite::SerialBox5_lite(QWidget *parent,
                       const QString &caption,
                       const QString &o_name) :
    MyWidget(parent),
    ui(new Ui::SerialBox5_lite),
    parent(parent),
    caption(caption),
    o_name(o_name),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_lite::~SerialBox5_lite()
{
    delete serial5;
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::set_caption(QString value)
{
    caption = value;
    o_name = value;
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_lite::bytesAvailable(void)
{
    return serial5->bytesAvailable();
}
//--------------------------------------------------------------------------------
qint64 SerialBox5_lite::write(const char *data)
{
    return serial5->write(data);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::init(void)
{
    ui->setupUi(this);

    createWidgets();
    initEnumerator();
    initSerial();
    setCloseState();
#ifdef RS232_FIXED_SIZE
    setFixedSize(sizeHint());
#endif

    updateText();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::createWidgets(void)
{
    ui->gridLayout->setMargin(0);
    ui->gridLayout->setSpacing(0);

    connect(ui->btn_power,      SIGNAL(clicked(bool)),  this,   SLOT(btnOpenPortClicked()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(ui->BaudBox,     SIGNAL(currentIndexChanged(int)),  this,   SLOT(setBaudBox(int)));

    connect(this, SIGNAL(output(QByteArray)), this, SLOT(drawData(QByteArray)));
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox5_lite::add_frame_text(QFrame *parent,
                                const QString &text)
{
    QHBoxLayout *hbox = new QHBoxLayout;
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
void SerialBox5_lite::refresh(void)
{
    ui->PortBox->clear();
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts())
    {
        ui->PortBox->addItem(p_info.portName());
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::initEnumerator(void)
{
    refresh();
    //---
    ui->BaudBox->clear();
    ui->BaudBox->addItem("Undefined Baud",  QSerialPort::UnknownBaud);
    ui->BaudBox->addItem("1200 baud",       QSerialPort::Baud1200);
    ui->BaudBox->addItem("2400 baud",       QSerialPort::Baud2400);
    ui->BaudBox->addItem("4800 baud",       QSerialPort::Baud4800);
    ui->BaudBox->addItem("9600 baud",       QSerialPort::Baud9600);
    ui->BaudBox->addItem("19200 baud",      QSerialPort::Baud19200);
    ui->BaudBox->addItem("38400 baud",      QSerialPort::Baud38400);
    ui->BaudBox->addItem("57600 baud",      QSerialPort::Baud57600);
    ui->BaudBox->addItem("115200 baud",     QSerialPort::Baud115200);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::initSerial(void)
{
    serial5 = new QSerialPort(this);

    connect(serial5,    SIGNAL(readyRead()),                            this,   SIGNAL(readyRead(void)));
    connect(serial5,    SIGNAL(readChannelFinished()),                  this,   SIGNAL(readChannelFinished(void)));

    //TODO
    timer = new QTimer();
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(timer_stop()));

    connect(serial5, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));
    connect(serial5, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serial5_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::serial5_error(QSerialPort::SerialPortError err)
{
    if(err == QSerialPort::NoError) return;

    switch(err)
    {
    case QSerialPort::DeviceNotFoundError:          emit error("DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:              emit error("PermissionError"); break;
    case QSerialPort::OpenError:                    emit error("OpenError"); break;
    case QSerialPort::ParityError:                  emit error("ParityError"); break;
    case QSerialPort::FramingError:                 emit error("FramingError"); break;
    case QSerialPort::BreakConditionError:          emit error("BreakConditionError"); break;
    case QSerialPort::WriteError:                   emit error("WriteError"); break;
    case QSerialPort::ReadError:                    emit error("ReadError"); break;
    case QSerialPort::ResourceError:                emit error("ResourceError"); break;
    case QSerialPort::UnsupportedOperationError:    emit error("UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:                 emit error("UnknownError"); break;
    case QSerialPort::TimeoutError:                 emit error("TimeoutError"); break;
    case QSerialPort::NotOpenError:                 emit error("NotOpenError"); break;

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }

    setCloseState();
    refresh();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));

    get_parameter();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::setBaudBox(int index)
{
    bool ok = false;
    int value = ui->BaudBox->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setBaudRate(value);
    if(!ok) emit error("error set baud");
    else emit info(QString("set %1").arg(ui->BaudBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::setCloseState(void)
{
    ui->PortBox->setEnabled(true);
    ui->BaudBox->setEnabled(false);
    ui->btn_power->setChecked(false);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::setOpenState()
{
    ui->PortBox->setEnabled(false);
    ui->BaudBox->setEnabled(true);
    ui->btn_power->setChecked(true);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::btnOpenPortClicked()
{
    int idx;

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
            serial5->setPortName(ui->PortBox->currentText());
            result = serial5->open(QIODevice::ReadWrite);
            if(result)
            {
                idx = ui->BaudBox->findData(serial5->baudRate());
                if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

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
int SerialBox5_lite::input(const QByteArray &sending_data)
{
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        emit not_working();
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit not_working();
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
        emit debug(QString("send [%1]").arg(sending_data.toHex().toUpper().data()));
        serial5->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5_lite::input(const QString &data)
{
    qDebug() << data;
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        emit not_working();
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit not_working();
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
void SerialBox5_lite::procSerialDataReceive(void)
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
void SerialBox5_lite::timer_stop(void)
{
    emit output(serial5->readAll());
}
//--------------------------------------------------------------------------------
QString SerialBox5_lite::ByteArrayToHex(const QByteArray &data)
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
void SerialBox5_lite::sendData(const QByteArray &sending_data)
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
void SerialBox5_lite::drawData(const QByteArray &data)
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
bool SerialBox5_lite::isOpen(void)
{
    return serial5->isOpen();
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::add_menu(int index)
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
    QMenu *o_menu = mw->get_options_menu();
    QList<QAction *> menus = o_menu->actions();

    int pos = 0;
    foreach (QAction *current_menu, menus)
    {
        if(pos == index)
        {
            o_menu->insertSeparator(current_menu);
            o_menu->insertMenu(current_menu, menu);
            return true;
        }
        pos++;
    }
    //---

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::add_menu(int index, const QString &title)
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
void SerialBox5_lite::set_flag_in_hex(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::set_flag_byte_by_byte(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::set_default(void)
{
    if(serial5->isOpen())
    {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        ui->BaudBox->setCurrentIndex(ui->BaudBox->findText("9600 baud"));
#else
        ui->BaudBox->setCurrentText("9600 baud");
#endif
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::get_parameter(void)
{
    if(serial5->isOpen() == false)
    {
        return;
    }

    QString temp;
    temp.clear();
#if 1
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
QByteArray SerialBox5_lite::readAll(void)
{
    return serial5->readAll();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
