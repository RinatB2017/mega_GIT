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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "ui_serialbox5.h"
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
SerialBox5::SerialBox5(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SerialBox5),
    parent(parent),
    serial5(0),
    caption("no name"),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5::SerialBox5(QWidget *parent,
                       const QString &caption,
                       const QString &o_name) :
    QFrame(parent),
    ui(new Ui::SerialBox5),
    parent(parent),
    serial5(0),
    caption(caption),
    o_name(o_name),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5::~SerialBox5()
{
#ifdef RS232_LOG
    delete logBox;
#endif

#ifdef RS232_SEND
    delete sendBox5;
#endif

    delete serial5;

    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void SerialBox5::init(void)
{
    ui->setupUi(this);
    connect_log();

    createWidgets();
    initEnumerator();
    initSerial();
    setCloseState();
#ifdef RS232_FIXED_SIZE
    setFixedSize(sizeHint());
#endif

    connect(ui->btn_default, SIGNAL(clicked()), this, SLOT(set_default()));

    updateText();
}
//--------------------------------------------------------------------------------
void SerialBox5::createWidgets(void)
{
    QFont font("Liberation Sans", 8, QFont::Bold);
    ui->captionBox->setFont(font);
    ui->captionBox->setText(caption);

    ui->gridLayout->setMargin(0);
    ui->gridLayout->setSpacing(0);

    connect(ui->btn_power,      SIGNAL(clicked(bool)),  this,   SLOT(btnOpenPortClicked()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(ui->BaudBox,     SIGNAL(currentIndexChanged(int)),  this,   SLOT(setBaudBox(int)));
    connect(ui->DataBitsBox, SIGNAL(currentIndexChanged(int)),  this,   SLOT(setDataBox(int)));
    connect(ui->ParityBox,   SIGNAL(currentIndexChanged(int)),  this,   SLOT(setParityBox(int)));
    connect(ui->StopBitsBox, SIGNAL(currentIndexChanged(int)),  this,   SLOT(setStopBox(int)));
    connect(ui->FlowBox,     SIGNAL(currentIndexChanged(int)),  this,   SLOT(setFlowBox(int)));

    connect(this, SIGNAL(output(QByteArray)), this, SLOT(drawData(QByteArray)));

#ifdef RS232_LOG
    logBox = new LogBox(o_name, this);
    ui->layout_right_LOG->addWidget(logBox);
#endif

#ifdef RS232_SEND
    sendBox5 = new SendBox5(this);
    connect(sendBox5, SIGNAL(sendData(QByteArray)), this, SLOT(sendData(QByteArray)));
    ui->layout_SEND->addWidget(sendBox5);
#endif

#ifndef RS232_NO_FRAME
    frame_ring = new QFrame(this);
    add_frame_text(frame_ring, tr("ring"));

    frame_dsr = new QFrame(this);
    add_frame_text(frame_dsr, tr("dsr"));

    frame_cts = new QFrame(this);
    add_frame_text(frame_cts, tr("cts"));

    ui->layout_status->addWidget(frame_ring);
    ui->layout_status->addWidget(frame_dsr);
    ui->layout_status->addWidget(frame_cts);
#endif
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox5::add_frame_text(QFrame *parent,
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
void SerialBox5::refresh(void)
{
    ui->PortBox->clear();
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts()) {
        ui->PortBox->addItem(p_info.portName());
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::initEnumerator(void)
{
    refresh();
    //---
    ui->BaudBox->clear();
    ui->BaudBox->addItem("Undefined Baud",  Qt::UserRole + QSerialPort::UnknownBaud);
    ui->BaudBox->addItem("1200 baud",       Qt::UserRole + QSerialPort::Baud1200);
    ui->BaudBox->addItem("2400 baud",       Qt::UserRole + QSerialPort::Baud2400);
    ui->BaudBox->addItem("4800 baud",       Qt::UserRole + QSerialPort::Baud4800);
    ui->BaudBox->addItem("9600 baud",       Qt::UserRole + QSerialPort::Baud9600);
    ui->BaudBox->addItem("19200 baud",      Qt::UserRole + QSerialPort::Baud19200);
    ui->BaudBox->addItem("38400 baud",      Qt::UserRole + QSerialPort::Baud38400);
    ui->BaudBox->addItem("57600 baud",      Qt::UserRole + QSerialPort::Baud57600);
    ui->BaudBox->addItem("115200 baud",     Qt::UserRole + QSerialPort::Baud115200);

    //---
    ui->DataBitsBox->clear();
    ui->DataBitsBox->addItem("Undefined data bits", Qt::UserRole + QSerialPort::UnknownDataBits);
    ui->DataBitsBox->addItem("5 bit",               Qt::UserRole + QSerialPort::Data5);
    ui->DataBitsBox->addItem("6 bit",               Qt::UserRole + QSerialPort::Data6);
    ui->DataBitsBox->addItem("7 bit",               Qt::UserRole + QSerialPort::Data7);
    ui->DataBitsBox->addItem("8 bit",               Qt::UserRole + QSerialPort::Data8);
    //---
    ui->ParityBox->clear();
    ui->ParityBox->addItem("Undefined parity",  Qt::UserRole + QSerialPort::UnknownParity);
    ui->ParityBox->addItem("None",              Qt::UserRole + QSerialPort::NoParity);
    ui->ParityBox->addItem("Even",              Qt::UserRole + QSerialPort::EvenParity);
    ui->ParityBox->addItem("Odd",               Qt::UserRole + QSerialPort::OddParity);
    ui->ParityBox->addItem("Space",             Qt::UserRole + QSerialPort::SpaceParity);
    ui->ParityBox->addItem("Mark",              Qt::UserRole + QSerialPort::MarkParity);
    //---
    ui->StopBitsBox->clear();
    ui->StopBitsBox->addItem("Undefined stop bits", Qt::UserRole + QSerialPort::UnknownStopBits);
    ui->StopBitsBox->addItem("1",                   Qt::UserRole + QSerialPort::OneStop);
    ui->StopBitsBox->addItem("1.5",                 Qt::UserRole + QSerialPort::OneAndHalfStop);
    ui->StopBitsBox->addItem("2",                   Qt::UserRole + QSerialPort::TwoStop);
    //---
    ui->FlowBox->clear();
    ui->FlowBox->addItem("Undefined flow",  Qt::UserRole + QSerialPort::UnknownFlowControl);
    ui->FlowBox->addItem("Disable",         Qt::UserRole + QSerialPort::NoFlowControl);
    ui->FlowBox->addItem("Hardware",        Qt::UserRole + QSerialPort::HardwareControl);
    ui->FlowBox->addItem("Software",        Qt::UserRole + QSerialPort::SoftwareControl);
}
//--------------------------------------------------------------------------------
void SerialBox5::initSerial(void)
{
    serial5 = new QSerialPort(this);

    //TODO
    timer = new QTimer();
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(timer_stop()));

    connect(serial5, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));
    connect(serial5, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serial5_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void SerialBox5::serial5_error(QSerialPort::SerialPortError err)
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
void SerialBox5::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));

    get_parameter();
}
//--------------------------------------------------------------------------------
void SerialBox5::setBaudBox(int index)
{
    bool ok = false;
    int value = ui->BaudBox->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setBaudRate(value);
    if(!ok) emit error("error set baud");
    else emit info(QString("set %1").arg(ui->BaudBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setDataBox(int index)
{
    bool ok = false;
    int value = ui->DataBitsBox->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setDataBits((QSerialPort::DataBits)value);
    if(!ok) emit error("error set data");
    else emit info(QString("set %1").arg(ui->DataBitsBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setParityBox(int index)
{
    bool ok = false;
    int value = ui->ParityBox->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setParity((QSerialPort::Parity)value);
    if(!ok) emit error("error set parity");
    else emit info(QString("set %1").arg(ui->ParityBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setStopBox(int index)
{
    bool ok = false;
    int value = ui->StopBitsBox->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setStopBits((QSerialPort::StopBits)value);
    if(!ok) emit error("error set stopbit");
    else emit info(QString("set %1").arg(ui->StopBitsBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setFlowBox(int index)
{
    bool ok = false;
    int value = ui->FlowBox->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setFlowControl((QSerialPort::FlowControl)value);
    if(!ok) emit error("error set flow");
    else emit info(QString("set %1").arg(ui->FlowBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setCloseState(void)
{
    ui->PortBox->setEnabled(true);
    ui->BaudBox->setEnabled(false);
    ui->DataBitsBox->setEnabled(false);
    ui->ParityBox->setEnabled(false);
    ui->StopBitsBox->setEnabled(false);
    ui->FlowBox->setEnabled(false);
    ui->btn_power->setChecked(false);
}
//--------------------------------------------------------------------------------
void SerialBox5::setOpenState()
{
    ui->PortBox->setEnabled(false);
    ui->BaudBox->setEnabled(true);
    ui->DataBitsBox->setEnabled(true);
    ui->ParityBox->setEnabled(true);
    ui->StopBitsBox->setEnabled(true);
    ui->FlowBox->setEnabled(true);
    ui->btn_power->setChecked(true);
}
//--------------------------------------------------------------------------------
void SerialBox5::btnOpenPortClicked()
{
    int idx;

    if (serial5)
    {
        bool result = serial5->isOpen();
        if (result)
        {
            serial5->close();
            result = false;
        }
        else
        {
            serial5->setPortName(ui->PortBox->currentText());
            result = serial5->open(QIODevice::ReadWrite);
            if(result)
            {
                idx = ui->BaudBox->findData(Qt::UserRole + serial5->baudRate());
                if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

                idx = ui->DataBitsBox->findData(Qt::UserRole + serial5->dataBits());
                if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

                idx = ui->ParityBox->findData(Qt::UserRole + serial5->parity());
                if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

                idx = ui->StopBitsBox->findData(Qt::UserRole + serial5->stopBits());
                if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

                idx = ui->FlowBox->findData(Qt::UserRole + serial5->flowControl());
                if (idx != -1) ui->FlowBox->setCurrentIndex(idx);

                get_parameter();
            }
            else
            {
                emit error(QString("ERROR: serial [%1] not open (%2)")
                           .arg(serial5->portName())
                           .arg(serial5->errorString()));
            }
        }

        (result) ? setOpenState() : setCloseState();
    }
}
//--------------------------------------------------------------------------------
int SerialBox5::input(const QByteArray &sending_data)
{
    // qDebug() << data;
    // emit debug(QString("send 0x%1").arg(sending_data.toHex().data()));
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
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
        //emit debug(QString("send 0x%1").arg(sending_data.toHex().data()));
        serial5->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5::input(const QString &data)
{
    qDebug() << data;
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
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
void SerialBox5::procSerialDataReceive(void)
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
void SerialBox5::timer_stop(void)
{
    emit output(serial5->readAll());
}
//--------------------------------------------------------------------------------
QString SerialBox5::ByteArrayToHex(const QByteArray &data)
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
void SerialBox5::sendData(const QByteArray &sending_data)
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
void SerialBox5::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
#ifdef RS232_LOG
    if(flag_in_hex)
    {
        logBox->append(ByteArrayToHex(data));
    }
    else
    {
        logBox->bappend(data.data());
    }
#else
    if(flag_in_hex)
    {
        emit info(ByteArrayToHex(data));
    }
    else
    {
        //emit info(data);
    }
#endif
}
//--------------------------------------------------------------------------------
void SerialBox5::updateText(void)
{
#ifdef RS232_LOG
    logBox->updateText();
#endif

#ifdef RS232_SEND
    sendBox5->updateText();
#endif
}
//--------------------------------------------------------------------------------
void SerialBox5::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        updateText();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
bool SerialBox5::isOpen(void)
{
    return serial5->isOpen();
}
//--------------------------------------------------------------------------------
bool SerialBox5::add_menu(int index)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return false;
    }

    QMenu *menu = new QMenu(caption);
    if(menu == nullptr)
    {
        return false;
    }

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
bool SerialBox5::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return false;
    }

    QMenu *menu = new QMenu(title);

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
void SerialBox5::set_flag_in_hex(bool state)
{
    // emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox5::set_flag_byte_by_byte(bool state)
{
    // emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox5::set_default(void)
{
    if(serial5->isOpen())
    {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        ui->BaudBox->setCurrentIndex(ui->BaudBox->findText("9600 baud"));
        ui->DataBitsBox->setCurrentIndex(ui->DataBitsBox->findText("8 bit"));
        ui->ParityBox->setCurrentIndex(ui->ParityBox->findText("None"));
        ui->StopBitsBox->setCurrentIndex(ui->StopBitsBox->findText("1"));
        ui->FlowBox->setCurrentIndex(ui->FlowBox->findText("Disable"));
#else
        ui->BaudBox->setCurrentText("9600 baud");
        ui->DataBitsBox->setCurrentText("8 bit");
        ui->ParityBox->setCurrentText("None");
        ui->StopBitsBox->setCurrentText("1");
        ui->FlowBox->setCurrentText("Disable");
#endif
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::get_parameter(void)
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
QPushButton *SerialBox5::add_QPushButton(const QString &title)
{
    QPushButton *btn = new QPushButton(title);
    ui->buttons_layout->addWidget(btn);
    return btn;
}
//--------------------------------------------------------------------------------
void SerialBox5::add_QHBoxLayout(QHBoxLayout * hbox)
{
    ui->buttons_layout->addLayout(hbox);
}
//--------------------------------------------------------------------------------
QByteArray SerialBox5::readAll(void)
{
    return serial5->readAll();
}
//--------------------------------------------------------------------------------
