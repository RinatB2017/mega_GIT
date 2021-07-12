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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5_lite.hpp"
#include "ui_serialbox5_lite.h"
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
SerialBox5_lite::SerialBox5_lite(QWidget *parent) :
    SerialWidget(parent),
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
    SerialWidget(parent),
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
#ifdef RS232_SEND
    if(sendBox5)
    {
        disconnect(sendBox5, &SendBox5::sendData, this, &SerialBox5_lite::sendData);

        sendBox5->close();
        sendBox5->deleteLater();
    }
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::set_caption(const QString &value)
{
    caption = value;
    o_name = value;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::init(void)
{
    ui->setupUi(this);

    createWidgets();
    initEnumerator();
    initSerial();

    ui->PortBox->setMinimumWidth(150);
    ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));

#ifdef RS232_FIXED_SIZE
    setFixedSize(sizeHint());
#endif

    setCloseState();
    updateText();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::createWidgets(void)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    ui->gridLayout->setMargin(0);
#endif
    ui->gridLayout->setSpacing(0);

    ui->PortBox->setProperty(NO_BLOCK, true);
    ui->btn_power->setProperty(NO_SAVE, true);
    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->btn_refresh->setToolTip("Обновить список портов");

    connect(ui->btn_power,      &QPushButton::clicked,  this,   &SerialBox5_lite::btnOpenPortClicked);
    connect(ui->btn_refresh,    &QPushButton::clicked,  this,   &SerialBox5_lite::refresh);

    connect(ui->BaudBox,     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5_lite::setBaudBox);

    connect(this, &SerialBox5_lite::output, this, &SerialBox5_lite::drawData);

#ifdef RS232_SEND
    sendBox5 = new SendBox5(this);
    connect(sendBox5, &SendBox5::sendData, this, &SerialBox5_lite::sendData);
    ui->layout_SEND->addWidget(sendBox5);
#endif
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox5_lite::add_frame_text(QFrame *parent,
                                     const QString &text)
{
    QHBoxLayout *hbox = new QHBoxLayout();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    hbox->setMargin(0);
#endif
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
    ui->PortBox->addItems(get_port_names());
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::initEnumerator(void)
{
    refresh();
    //---
    ui->BaudBox->clear();
#ifdef Q_OS_LINUX
    ui->BaudBox->addItem("Undefined Baud",  QSerialPort::UnknownBaud);
#endif
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
    ui->PortBox->setProperty(NO_SAVE, true);
    ui->BaudBox->setProperty(NO_SAVE, true);
    ui->btn_refresh->setToolTip("Обновить список портов");

    connect(ui->btn_power,  &QPushButton::toggled,  this,   &SerialBox5_lite::change_icon);

    connect(this,   &SerialBox5_lite::port_close,    this,   &SerialBox5_lite::setCloseState);
    connect(this,   &SerialBox5_lite::port_open,     this,   &SerialBox5_lite::setOpenState);

    refresh();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::change_icon(bool state)
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

    ok = setBaudRate(value);
    if(!ok)
    {
        emit error("error set baud");
        return;
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::setCloseState(void)
{
    ui->btn_refresh->setEnabled(true);
    ui->PortBox->setEnabled(true);
    ui->BaudBox->setEnabled(false);
    ui->btn_power->setChecked(false);
#ifdef RS232_SEND
    sendBox5->block_interface(true);
#endif
    ui->btn_power->setToolTip("Старт");
    emit port_is_active(false);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::setOpenState()
{
    ui->btn_refresh->setEnabled(false);
    ui->PortBox->setEnabled(false);
    ui->BaudBox->setEnabled(true);
    ui->btn_power->setChecked(true);
#ifdef RS232_SEND
    sendBox5->block_interface(false);
#endif
    ui->btn_power->setToolTip("Стоп");
    emit port_is_active(true);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::btnOpenPortClicked()
{
    bool result = isOpen();
    if (result)
    {
        serial_close();
        emit port_is_active(false);
        result = false;
    }
    else
    {
        QString text = ui->PortBox->currentText();
        if(text.isEmpty())
        {
            if(isOpen())
            {
                serial_close();
            }
            setCloseState();
            return;
        }
        setPortName(text);
        result = serial_open();
        if(result)
        {
            int idx = 0;
            idx = ui->BaudBox->findData(baudRate());
            if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

            get_parameter();
            emit port_is_active(result);
        }
        else
        {
            emit error(QString("ERROR: serial [%1] not open (%2)")
                       .arg(portName())
                       .arg(errorString()));
            emit port_is_active(false);
        }
    }

    (result) ? setOpenState() : setCloseState();
}
//--------------------------------------------------------------------------------
int SerialBox5_lite::input(const QByteArray &sending_data)
{
    if(!isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit port_is_active(false);
        return E_PORT_NOT_OPEN;
    }
    if(flag_byte_by_byte)
    {
        emit debug("flag_byte_by_byte");
        for(int n=0; n<sending_data.length(); n++)
            write(sending_data.constData()+n, 1);
    }
    else
    {
        emit debug(QString("send [%1]").arg(sending_data.toHex().toUpper().data()));
        write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5_lite::input(const QString &data)
{
#ifdef Q_DEBUG
    qDebug() << data;
#endif
    if(!isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit port_is_active(false);
        return E_PORT_NOT_OPEN;
    }
    QByteArray sending_data = data.toLatin1();
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
QString SerialBox5_lite::ByteArrayToHex(const QByteArray &data)
{
    return data.toHex();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::sendData(const QByteArray &sending_data)
{
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        write(sending_data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
    Q_UNUSED(data)
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::add_menu(int index)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    QMenu *menu = new QMenu(caption);
    Q_ASSERT(menu);

    QAction *action_flag_in_hex = new QAction(menu);
    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_in_hex->setCheckable(true);
    action_flag_byte_by_byte->setCheckable(true);

    action_flag_in_hex->setText("in HEX");
    action_flag_byte_by_byte->setText("byte to byte");

    menu->addAction(action_flag_in_hex);
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_in_hex,         &QAction::triggered,    this,   &SerialBox5_lite::set_flag_in_hex);
    connect(action_flag_byte_by_byte,   &QAction::triggered,    this,   &SerialBox5_lite::set_flag_byte_by_byte);

    //---
    mw->add_optionsmenu_menu(index, menu);
    //---

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    QMenu *menu = new QMenu(title);
    Q_ASSERT(menu);

    QAction *action_flag_in_hex = new QAction(menu);
    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_in_hex->setCheckable(true);
    action_flag_byte_by_byte->setCheckable(true);

    action_flag_in_hex->setText("in HEX");
    action_flag_byte_by_byte->setText("byte to byte");

    menu->addAction(action_flag_in_hex);
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_in_hex, &QAction::triggered, this, &SerialBox5_lite::set_flag_in_hex);
    connect(action_flag_byte_by_byte, &QAction::triggered, this, &SerialBox5_lite::set_flag_byte_by_byte);

    mw->add_menu(index, menu);

    return true;
}
//--------------------------------------------------------------------------------
qint32 SerialBox5_lite::get_baudRate(void)
{
    return baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits   SerialBox5_lite::get_dataBits(void)
{
    return dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity     SerialBox5_lite::get_parity(void)
{
    return parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits   SerialBox5_lite::get_stopBits(void)
{
    return stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl    SerialBox5_lite::get_flowControl(void)
{
    return flowControl();
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
    if(isOpen())
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
    if(isOpen() == false)
    {
        return;
    }

    QString temp;
    temp.clear();
    emit info(QString("baudRate %1").arg(baudRate()));
    emit info(QString("dataBits %1").arg(dataBits()));
    emit info(QString("parity %1").arg(parity()));
    emit info(QString("stopBits %1").arg(stopBits()));
    emit info(QString("flowControl %1").arg(flowControl()));
    emit info(temp);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::set_test(bool value)
{
    p_test = value;
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::get_test(void)
{
    emit error("Зачем жать куда попало?");
    return p_test;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::updateText(void)
{
#ifdef RS232_SEND
    sendBox5->updateText();
#endif
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_lite::save_setting(void)
{

}
//--------------------------------------------------------------------------------
