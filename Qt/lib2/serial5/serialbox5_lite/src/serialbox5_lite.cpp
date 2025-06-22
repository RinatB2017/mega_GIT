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
    // save_setting();
    if(m_timer)
    {
        delete m_timer;
    }

#ifdef RS232_SEND
    if(sendBox5)
    {
        disconnect(sendBox5, &SendBox5::sendData, this, &SerialBox5_lite::sendData);

        sendBox5->close();
        delete sendBox5;
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
    init_timer();

    ui->cb_PortBox->setMinimumWidth(150);
    ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));

    setFixedHeight(sizeHint().height());

    QTimer::singleShot(100, [this]{
        setCloseState();
    });
    updateText();
    // load_setting();
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::init_timer(void)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerialBox5_lite::checkPorts);
    m_timer->start(5000);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::checkPorts(void)
{
    QStringList sl_ports = get_port_names();

    // Сравнение с предыдущим состоянием
    if (sl_ports != m_portNames)
    {
        m_portNames = sl_ports;
        sl_ports = get_port_names();

        // Обновление QComboBox
        QString current_text = ui->cb_PortBox->currentText();

        ui->cb_PortBox->clear();
        ui->cb_PortBox->addItems(sl_ports);

        ui->cb_PortBox->setCurrentText(current_text);
        //TODO прерывание при физическом отключении устройства
        if(isOpen())
        {
            QList<QSerialPortInfo> sl_ports_info = QSerialPortInfo::availablePorts();
            foreach (QSerialPortInfo port, sl_ports_info)
            {
                if(port.portName() == current_text)
                {
                    return;
                }
            }
            emit error("Прервано!");
            emit port_close();
            serial_close();
        }
    }
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::createWidgets(void)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    ui->gridLayout->setMargin(0);
#endif
    ui->gridLayout->setSpacing(0);

    ui->cb_PortBox->setProperty(NO_BLOCK, true);
    ui->btn_power->setProperty(NO_SAVE, true);
    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->btn_refresh->setToolTip("Обновить список портов");

    connect(ui->btn_power,      &QPushButton::clicked,  this,   &SerialBox5_lite::btnOpenPortClicked);
    connect(ui->btn_refresh,    &QPushButton::clicked,  this,   &SerialBox5_lite::refresh);

    connect(ui->cb_BaudBox,     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5_lite::set_baudRate);

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
    ui->cb_PortBox->clear();
    ui->cb_PortBox->addItems(get_port_names());
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::initEnumerator(void)
{
    refresh();
    //---
    ui->cb_BaudBox->clear();
#ifdef Q_OS_LINUX
    // ui->cb_BaudBox->addItem("Undefined Baud",  QSerialPort::UnknownBaud);
#endif
    // ui->cb_BaudBox->addItem("1200 baud",       QSerialPort::Baud1200);
    // ui->cb_BaudBox->addItem("2400 baud",       QSerialPort::Baud2400);
    // ui->cb_BaudBox->addItem("4800 baud",       QSerialPort::Baud4800);
    ui->cb_BaudBox->addItem("9600 baud",       QSerialPort::Baud9600);
    ui->cb_BaudBox->addItem("19200 baud",      QSerialPort::Baud19200);
    ui->cb_BaudBox->addItem("38400 baud",      QSerialPort::Baud38400);
    ui->cb_BaudBox->addItem("57600 baud",      QSerialPort::Baud57600);
    ui->cb_BaudBox->addItem("115200 baud",     QSerialPort::Baud115200);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::initSerial(void)
{
    ui->cb_PortBox->setProperty(NO_SAVE, true);
    ui->cb_BaudBox->setProperty(NO_SAVE, true);
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
void SerialBox5_lite::set_baud_box(int index)
{
    bool ok = false;
    int value = ui->cb_BaudBox->itemData(index, Qt::UserRole).toInt(&ok);
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
    ui->cb_PortBox->setEnabled(true);
    ui->cb_BaudBox->setEnabled(true);
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
    ui->cb_PortBox->setEnabled(false);
    ui->cb_BaudBox->setEnabled(false);
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
        QString text = ui->cb_PortBox->currentText();
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
            idx = ui->cb_BaudBox->findData(baudRate());
            if (idx != -1) ui->cb_BaudBox->setCurrentIndex(idx);

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

    if(mw)
    {
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

        mw->add_optionsmenu_menu(index, menu);
    }
    else
    {
        emit error("mw not found");
    }

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
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
    }
    else
    {
        emit error("mw not found");
    }

    return true;
}
//--------------------------------------------------------------------------------
bool SerialBox5_lite::set_baudRate(int value)
{
    bool ok;
    ok = setBaudRate(value);
    if(ok)
    {
        int idx = 0;
        idx = ui->cb_BaudBox->findData(value);
        if (idx != -1) ui->cb_BaudBox->setCurrentIndex(idx);
    }
    return ok;
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
        ui->cb_BaudBox->setCurrentIndex(ui->cb_BaudBox->findText("9600 baud"));
#else
        ui->cb_BaudBox->setCurrentText("9600 baud");
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
    temp = QString("%1, %2, %3, %4, %5")
            .arg(baudRate())
            .arg(dataBits())
            .arg(parity())
            .arg(stopBits())
            .arg(flowControl());
    emit debug(temp);
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
    QString portname = load_string(SERIALBOX5_LITE_PORTNAME);
    QString baudrate = load_string(SERIALBOX5_LITE_BAUDRATE);

    ui->cb_PortBox->setCurrentText(portname);
    ui->cb_BaudBox->setCurrentText(baudrate);
}
//--------------------------------------------------------------------------------
void SerialBox5_lite::save_setting(void)
{
    QString portname = ui->cb_PortBox->currentText();
    QString baudrate = ui->cb_BaudBox->currentText();

    save_string(SERIALBOX5_LITE_PORTNAME,   portname);
    save_string(SERIALBOX5_LITE_BAUDRATE,   baudrate);
}
//--------------------------------------------------------------------------------
