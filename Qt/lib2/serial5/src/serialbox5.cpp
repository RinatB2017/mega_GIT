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
#include "mainwindow.hpp"
#include "serialbox5.hpp"

#include "mywidget.hpp"
#include "ui_serialbox5.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#   include "sendbox5.hpp"
#endif
#   include "logbox.hpp"
//--------------------------------------------------------------------------------
#define MAX_TIME_MSEC   100
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
SerialBox5::SerialBox5(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::SerialBox5),
    parent(parent),
    caption("no name")
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5::SerialBox5(QWidget *parent,
                       const QString &caption,
                       const QString &o_name) :
    MyWidget(parent),
    ui(new Ui::SerialBox5),
    parent(parent),
    caption(caption),
    o_name(o_name)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5::~SerialBox5()
{
#ifdef RS232_LOG
    if(logBox)
    {
        logBox->disconnect();
        logBox->close();
        logBox->deleteLater();
    }
#endif

#ifdef RS232_SEND
    if(sendBox5)
    {
        sendBox5->disconnect();
        sendBox5->close();
        sendBox5->deleteLater();
    }
#endif

    if(serial5)
    {
        serial5->disconnect();
        serial5->close();
        serial5->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5::set_caption(QString value)
{
    caption = value;
    o_name = value;
    if(value.isEmpty() == false)
    {
        ui->captionBox->setText(value);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::init(void)
{
    ui->setupUi(this);

    setObjectName(o_name);

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

    ui->btn_refresh->setToolTip("Обновить список портов");

    connect(ui->btn_power,      SIGNAL(clicked(bool)),  this,   SLOT(btnOpenPortClicked()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(ui->BaudBox,     SIGNAL(currentIndexChanged(int)),  this,   SLOT(setBaudBox(int)));
    connect(ui->DataBitsBox, SIGNAL(currentIndexChanged(int)),  this,   SLOT(setDataBox(int)));
    connect(ui->ParityBox,   SIGNAL(currentIndexChanged(int)),  this,   SLOT(setParityBox(int)));
    connect(ui->StopBitsBox, SIGNAL(currentIndexChanged(int)),  this,   SLOT(setStopBox(int)));
    connect(ui->FlowBox,     SIGNAL(currentIndexChanged(int)),  this,   SLOT(setFlowBox(int)));

    connect(this,   SIGNAL(output(QByteArray)), this,   SLOT(drawData(QByteArray)));

#ifdef RS232_LOG
    logBox = new LogBox(o_name, this);
    ui->layout_right_LOG->addWidget(logBox);

    connect(this,   SIGNAL(info(QString)),  logBox, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)), logBox, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)), logBox, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)), logBox, SLOT(traceLog(QString)));
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
    foreach (QSerialPortInfo p_info, QSerialPortInfo::availablePorts())
    {
        ui->PortBox->addItem(p_info.portName());
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::initEnumerator(void)
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

    //---
    ui->DataBitsBox->clear();
    ui->DataBitsBox->addItem("Undefined data bits", QSerialPort::UnknownDataBits);
    ui->DataBitsBox->addItem("5 bit",               QSerialPort::Data5);
    ui->DataBitsBox->addItem("6 bit",               QSerialPort::Data6);
    ui->DataBitsBox->addItem("7 bit",               QSerialPort::Data7);
    ui->DataBitsBox->addItem("8 bit",               QSerialPort::Data8);
    //---
    ui->ParityBox->clear();
    ui->ParityBox->addItem("Undefined parity",  QSerialPort::UnknownParity);
    ui->ParityBox->addItem("None",              QSerialPort::NoParity);
    ui->ParityBox->addItem("Even",              QSerialPort::EvenParity);
    ui->ParityBox->addItem("Odd",               QSerialPort::OddParity);
    ui->ParityBox->addItem("Space",             QSerialPort::SpaceParity);
    ui->ParityBox->addItem("Mark",              QSerialPort::MarkParity);
    //---
    ui->StopBitsBox->clear();
    ui->StopBitsBox->addItem("Undefined stop bits", QSerialPort::UnknownStopBits);
    ui->StopBitsBox->addItem("1",                   QSerialPort::OneStop);
    ui->StopBitsBox->addItem("1.5",                 QSerialPort::OneAndHalfStop);
    ui->StopBitsBox->addItem("2",                   QSerialPort::TwoStop);
    //---
    ui->FlowBox->clear();
    ui->FlowBox->addItem("Undefined flow",  QSerialPort::UnknownFlowControl);
    ui->FlowBox->addItem("Disable",         QSerialPort::NoFlowControl);
    ui->FlowBox->addItem("Hardware",        QSerialPort::HardwareControl);
    ui->FlowBox->addItem("Software",        QSerialPort::SoftwareControl);
}
//--------------------------------------------------------------------------------
void SerialBox5::initSerial(void)
{
    serial5 = new QSerialPort(this);

    //TODO
    timer = new QTimer();
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(timer_stop()));

    connect(serial5, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));
    connect(serial5, SIGNAL(error(QSerialPort::SerialPortError)), this,   SLOT(serial5_error(QSerialPort::SerialPortError)));
    //connect(serial5, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this,   SLOT(errorOccurred(QSerialPort::SerialPortError)));

    connect(serial5, SIGNAL(baudRateChanged(qint32,QSerialPort::Directions)),   this,   SLOT(baudRateChanged(qint32,QSerialPort::Directions)));
    connect(serial5, SIGNAL(breakEnabledChanged(bool)),                         this,   SLOT(breakEnabledChanged(bool)));
    connect(serial5, SIGNAL(dataBitsChanged(QSerialPort::DataBits)),            this,   SLOT(dataBitsChanged(QSerialPort::DataBits)));
    connect(serial5, SIGNAL(dataTerminalReadyChanged(bool)),                    this,   SLOT(dataTerminalReadyChanged(bool)));
    connect(serial5, SIGNAL(flowControlChanged(QSerialPort::FlowControl)),      this,   SLOT(flowControlChanged(QSerialPort::FlowControl)));
    connect(serial5, SIGNAL(parityChanged(QSerialPort::Parity)),                this,   SLOT(parityChanged(QSerialPort::Parity)));
    connect(serial5, SIGNAL(requestToSendChanged(bool)),                        this,   SLOT(requestToSendChanged(bool)));
    connect(serial5, SIGNAL(stopBitsChanged(QSerialPort::StopBits)),            this,   SLOT(stopBitsChanged(QSerialPort::StopBits)));
}
//--------------------------------------------------------------------------------
void SerialBox5::serial5_error(QSerialPort::SerialPortError err)
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

    emit not_working();
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
    int value = ui->BaudBox->itemData(index, Qt::UserRole).toInt(&ok);
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
    int value = ui->DataBitsBox->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setDataBits(static_cast<QSerialPort::DataBits>(value));
    if(!ok) emit error("error set data");
    else emit info(QString("set %1").arg(ui->DataBitsBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setParityBox(int index)
{
    bool ok = false;
    int value = ui->ParityBox->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setParity(static_cast<QSerialPort::Parity>(value));
    if(!ok) emit error("error set parity");
    else emit info(QString("set %1").arg(ui->ParityBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setStopBox(int index)
{
    bool ok = false;
    int value = ui->StopBitsBox->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setStopBits(static_cast<QSerialPort::StopBits>(value));
    if(!ok) emit error("error set stopbit");
    else emit info(QString("set %1").arg(ui->StopBitsBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setFlowBox(int index)
{
    bool ok = false;
    int value = ui->FlowBox->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;
    if(value < 0) return;

    ok = serial5->setFlowControl(static_cast<QSerialPort::FlowControl>(value));
    if(!ok) emit error("error set flow");
    else emit info(QString("set %1").arg(ui->FlowBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::setCloseState(void)
{
    ui->PortBox->setEnabled(true);
    ui->btn_refresh->setEnabled(true);
    ui->BaudBox->setEnabled(false);
    ui->DataBitsBox->setEnabled(false);
    ui->ParityBox->setEnabled(false);
    ui->StopBitsBox->setEnabled(false);
    ui->FlowBox->setEnabled(false);
    ui->btn_power->setChecked(false);

    ui->btn_default->setEnabled(false);

#ifdef RS232_SEND
    sendBox5->block_interface(true);
#endif
    ui->btn_power->setToolTip("Старт");
}
//--------------------------------------------------------------------------------
void SerialBox5::setOpenState()
{
    ui->PortBox->setEnabled(false);
    ui->btn_refresh->setEnabled(false);
    ui->BaudBox->setEnabled(true);
    ui->DataBitsBox->setEnabled(true);
    ui->ParityBox->setEnabled(true);
    ui->StopBitsBox->setEnabled(true);
    ui->FlowBox->setEnabled(true);
    ui->btn_power->setChecked(true);

    ui->btn_default->setEnabled(true);

#ifdef RS232_SEND
    sendBox5->block_interface(false);
#endif
    ui->btn_power->setToolTip("Стоп");
}
//--------------------------------------------------------------------------------
void SerialBox5::btnOpenPortClicked()
{
    int idx = 0;

    bool result = serial5->isOpen();
    if (result)
    {
        serial5->close();
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
            idx = ui->BaudBox->findData(serial5->baudRate());
            if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

            idx = ui->DataBitsBox->findData(serial5->dataBits());
            if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

            idx = ui->ParityBox->findData(serial5->parity());
            if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

            idx = ui->StopBitsBox->findData(serial5->stopBits());
            if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

            idx = ui->FlowBox->findData(serial5->flowControl());
            if (idx != -1) ui->FlowBox->setCurrentIndex(idx);

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
//--------------------------------------------------------------------------------
int SerialBox5::input(const QByteArray &sending_data)
{
    emit trace(QString("SerialBox5::input [%1]").arg(sending_data.toHex().data()));
    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        emit is_close();
        emit not_working();
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit is_close();
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
int SerialBox5::input(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#endif

    if(!serial5)
    {
        emit error("E_PORT_NOT_INIT");
        emit is_close();
        emit not_working();
        return E_PORT_NOT_INIT;
    }
    if(!serial5->isOpen())
    {
        emit error("E_PORT_NOT_OPEN");
        emit is_close();
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
    return data.toHex();
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
void SerialBox5::updateText(void)
{
    ui->retranslateUi(this);

#ifdef RS232_LOG
    logBox->updateText();
#endif

#ifdef RS232_SEND
    sendBox5->updateText();
#endif
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

    mw->add_optionsmenu_menu(index, menu);

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

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
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox5::set_flag_byte_by_byte(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox5::set_default(void)
{
    if(serial5->isOpen())
    {
        serial5->setBaudRate(9600);
        serial5->setDataBits(QSerialPort::Data8);
        serial5->setParity(QSerialPort::NoParity);
        serial5->setStopBits(QSerialPort::OneStop);
        serial5->setFlowControl(QSerialPort::NoFlowControl);

        int idx = 0;

        idx = ui->BaudBox->findData(serial5->baudRate());
        if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

        idx = ui->DataBitsBox->findData(serial5->dataBits());
        if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

        idx = ui->ParityBox->findData(serial5->parity());
        if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

        idx = ui->StopBitsBox->findData(serial5->stopBits());
        if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

        idx = ui->FlowBox->findData(serial5->flowControl());
        if (idx != -1) ui->FlowBox->setCurrentIndex(idx);
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
qint64 SerialBox5::bytesAvailable(void)
{
    return serial5->bytesAvailable();
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_baudRate(qint32 value)
{
    return serial5->setBaudRate(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_dataBits(QSerialPort::DataBits value)
{
    return serial5->setDataBits(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_parity(QSerialPort::Parity value)
{
    return serial5->setParity(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_stopBits(QSerialPort::StopBits value)
{
    return serial5->setStopBits(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_flowControl(QSerialPort::FlowControl value)
{
    return serial5->setFlowControl(value);
}
//--------------------------------------------------------------------------------
qint32 SerialBox5::get_baudRate(void)
{
    return serial5->baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits       SerialBox5::get_dataBits(void)
{
    return serial5->dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity         SerialBox5::get_parity(void)
{
    return serial5->parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits       SerialBox5::get_stopBits(void)
{
    return serial5->stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl    SerialBox5::get_flowControl(void)
{
    return serial5->flowControl();
}
//--------------------------------------------------------------------------------
bool SerialBox5::power_on(void)
{
    if(serial5->isOpen())
    {
        return false;
    }

    bool ok = false;
    ok = serial5->open(QIODevice::ReadWrite);
    btnOpenPortClicked();

    return ok;
}
//--------------------------------------------------------------------------------
bool SerialBox5::power_off(void)
{
    if(serial5->isOpen())
    {
        serial5->close();
    }
    btnOpenPortClicked();

    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5::baudRateChanged(qint32 baudRate, QSerialPort::Directions)
{
    int idx = ui->BaudBox->findData(baudRate);
    if (idx != -1) ui->BaudBox->setCurrentIndex(idx);
}
//--------------------------------------------------------------------------------
void SerialBox5::breakEnabledChanged(bool set)
{
    emit debug(QString("breakEnabledChanged %1").arg(set));
}
//--------------------------------------------------------------------------------
void SerialBox5::dataBitsChanged(QSerialPort::DataBits dataBits)
{
    int idx = ui->DataBitsBox->findData(dataBits);
    if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);
}
//--------------------------------------------------------------------------------
void SerialBox5::dataTerminalReadyChanged(bool set)
{
    emit debug(QString("dataTerminalReadyChanged %1").arg(set));
}
//--------------------------------------------------------------------------------
void SerialBox5::errorOccurred(QSerialPort::SerialPortError error)
{
    serial5_error(error);
}
//--------------------------------------------------------------------------------
void SerialBox5::flowControlChanged(QSerialPort::FlowControl flow)
{
    int idx = ui->FlowBox->findData(flow);
    if (idx != -1) ui->FlowBox->setCurrentIndex(idx);
}
//--------------------------------------------------------------------------------
void SerialBox5::parityChanged(QSerialPort::Parity parity)
{
    int idx = ui->ParityBox->findData(parity);
    if (idx != -1) ui->ParityBox->setCurrentIndex(idx);
}
//--------------------------------------------------------------------------------
void SerialBox5::requestToSendChanged(bool set)
{
    emit debug(QString("requestToSendChanged %1").arg(set));
}
//--------------------------------------------------------------------------------
void SerialBox5::stopBitsChanged(QSerialPort::StopBits stopBits)
{
    int idx = ui->StopBitsBox->findData(stopBits);
    if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);
}
//--------------------------------------------------------------------------------
