/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox4.hpp"
#include "ui_serialbox4.h"
//--------------------------------------------------------------------------------
#include "serialdeviceenumerator.h"
#include "abstractserial.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#   include "sendbox4.hpp"
#endif
#include "logbox.hpp"
//--------------------------------------------------------------------------------
SerialBox4::SerialBox4(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SerialBox4),
    parent(parent),
    serial(0),
    caption("no name"),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox4::SerialBox4(QWidget *parent, const QString &caption) :
    QFrame(parent),
    ui(new Ui::SerialBox4),
    parent(parent),
    caption(caption),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox4::~SerialBox4()
{
#ifdef RS232_LOG
    delete logBox;
#endif

#ifdef RS232_SEND
    delete sendBox;
#endif

    delete enumerator;
    delete serial;

    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox4::connect_log(void)
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
void SerialBox4::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void SerialBox4::init(void)
{
    ui->setupUi(this);
    connect_log();

    createWidgets();
    initEnumerator();
    initSerial();
    initBoxs();
    setCloseState();
#ifdef RS232_FIXED_SIZE
    setFixedSize(sizeHint());
#endif

    connect(ui->btn_default, SIGNAL(clicked()), this, SLOT(set_default()));
}
//--------------------------------------------------------------------------------
void SerialBox4::createWidgets(void)
{
    QFont font("Liberation Sans", 8, QFont::Bold);
    ui->captionBox->setFont(font);
    ui->captionBox->setText(caption);

    ui->gridLayout->setMargin(0);
    ui->gridLayout->setSpacing(0);

    connect(ui->btn_power, SIGNAL(clicked()), this, SLOT(btnOpenPortClicked()));

    connect(ui->BaudBox,     SIGNAL(currentIndexChanged(QString)),  this, SLOT(setBaudBox(QString)));
    connect(ui->DataBitsBox, SIGNAL(currentIndexChanged(QString)),  this, SLOT(setDataBox(QString)));
    connect(ui->ParityBox,   SIGNAL(currentIndexChanged(QString)),  this, SLOT(setParityBox(QString)));
    connect(ui->StopBitsBox, SIGNAL(currentIndexChanged(QString)),  this, SLOT(setStopBox(QString)));
    connect(ui->FlowBox,     SIGNAL(currentIndexChanged(QString)),  this, SLOT(setFlowBox(QString)));

    connect(this, SIGNAL(output(QByteArray)), this, SLOT(drawData(QByteArray)));

#ifdef RS232_LOG
    logBox = new LogBox("RS232", this);
    ui->layout_right_LOG->addWidget(logBox);
#endif

#ifdef RS232_SEND
    sendBox = new SendBox4(this);
    connect(sendBox, SIGNAL(sendData(QByteArray)), this, SLOT(sendData(QByteArray)));
    ui->layout_SEND->addWidget(sendBox);
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
void SerialBox4::add_frame_text(QFrame *parent,
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
void SerialBox4::initBoxs()
{
    ui->BaudBox->addItems(serial->listBaudRate());
    ui->DataBitsBox->addItems(serial->listDataBits());
    ui->ParityBox->addItems(serial->listParity());
    ui->StopBitsBox->addItems(serial->listStopBits());
    ui->FlowBox->addItems(serial->listFlowControl());
#ifdef RS232_LOG
    logBox->clear();
#endif
}
//--------------------------------------------------------------------------------
void SerialBox4::initEnumerator()
{
    enumerator = new SerialDeviceEnumerator(this);
    connect(enumerator, SIGNAL(hasChanged(QStringList)), this, SLOT(procEnumerate(QStringList)));
    enumerator->setEnabled(true);
}
//--------------------------------------------------------------------------------
void SerialBox4::procEnumerate(const QStringList &l)
{
    ui->PortBox->clear();
    ui->PortBox->addItems(l);
}
//--------------------------------------------------------------------------------
void SerialBox4::initSerial()
{
    serial = new AbstractSerial(this);

    connect(serial, SIGNAL(readyRead()),                        this, SLOT(procSerialDataReceive()));
    connect(serial, SIGNAL(signalStatus(QString, QDateTime)),   this, SLOT(getStatus(QString, QDateTime)));
    connect(serial, SIGNAL(readChannelFinished()),              this, SIGNAL(readChannelFinished()));

#ifndef RS232_NO_FRAME
    connect(serial, SIGNAL(ringChanged(bool)), this, SIGNAL(procRingChanged(bool)));
    connect(serial, SIGNAL(ctsChanged(bool)),  this, SIGNAL(procCtsChanged(bool)));
    connect(serial, SIGNAL(dsrChanged(bool)),  this, SIGNAL(procDsrChanged(bool)));

    connect(serial, SIGNAL(ringChanged(bool)), this, SLOT(ring_changed(bool)));
    connect(serial, SIGNAL(ctsChanged(bool)),  this, SLOT(cts_changed(bool)));
    connect(serial, SIGNAL(dsrChanged(bool)),  this, SLOT(dsr_changed(bool)));
#endif
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox4::ring_changed(bool state)
{
    state ? frame_ring->setStyleSheet("background:green") : frame_ring->setStyleSheet("");
}
#endif
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox4::dsr_changed(bool state)
{
    state ? frame_dsr->setStyleSheet("background:green") : frame_dsr->setStyleSheet("");
}
#endif
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox4::cts_changed(bool state)
{
    state ? frame_cts->setStyleSheet("background:green") : frame_cts->setStyleSheet("");
}
#endif
//--------------------------------------------------------------------------------
void SerialBox4::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));

    get_parameter();
}
//--------------------------------------------------------------------------------
void SerialBox4::setBaudBox(const QString &text)
{
    serial->setBaudRate(text);
    parameter_baud = text;
}
//--------------------------------------------------------------------------------
void SerialBox4::setDataBox(const QString &text)
{
    serial->setDataBits(text);
    parameter_data = text;
}
//--------------------------------------------------------------------------------
void SerialBox4::setParityBox(const QString &text)
{
    serial->setParity(text);
    parameter_parity = text;
}
//--------------------------------------------------------------------------------
void SerialBox4::setStopBox(const QString &text)
{
    serial->setStopBits(text);
    parameter_stop = text;
}
//--------------------------------------------------------------------------------
void SerialBox4::setFlowBox(const QString &text)
{
    serial->setFlowControl(text);
    parameter_flow = text;
}
//--------------------------------------------------------------------------------
void SerialBox4::setCloseState()
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
void SerialBox4::setOpenState()
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
void SerialBox4::btnOpenPortClicked()
{
    int idx;

    if (serial)
    {
        bool result = serial->isOpen();
        if (result)
        {
            serial->close();
            serial->enableEmitStatus(false);
            //emit error(QString("ERROR: %1").arg(serial->errorString()));
            result = false;
        }
        else
        {
            serial->setDeviceName(ui->PortBox->currentText());
            result = serial->open(QIODevice::ReadWrite);
            if(result)
            {
                serial->enableEmitStatus(true);

                idx = ui->BaudBox->findText(serial->baudRate());
                if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

                idx = ui->DataBitsBox->findText(serial->dataBits());
                if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

                idx = ui->ParityBox->findText(serial->parity());
                if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

                idx = ui->StopBitsBox->findText(serial->stopBits());
                if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

                idx = ui->FlowBox->findText(serial->flowControl());
                if (idx != -1) ui->FlowBox->setCurrentIndex(idx);

                get_parameter();
            }
            else
            {
                emit error(QString("ERROR: serial [%1] not open").arg(serial->deviceName()));
                emit error(QString("errorString(%1)").arg(serial->errorString()));
            }
        }

        (result) ? setOpenState() : setCloseState();
    }
}
//--------------------------------------------------------------------------------
int SerialBox4::input(const QByteArray &sending_data)
{
    // qDebug() << data;
    // emit debug(QString("send 0x%1").arg(sending_data.toHex().data()));
    if(!serial)
    {
        return E_PORT_NOT_INIT;
    }
    if(!serial->isOpen())
    {
        return E_PORT_NOT_OPEN;
    }
    if(flag_byte_by_byte)
    {
        emit debug("flag_byte_by_byte");
        for(int n=0; n<sending_data.length(); n++)
            serial->write(sending_data.constData()+n, 1);
    }
    else
    {
        //emit debug(QString("send 0x%1").arg(sending_data.toHex().data()));
        serial->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox4::input(const QString &data)
{
    if(data.isEmpty() == false)
    {
        qDebug() << data;
    }
    if(!serial)
    {
        return E_PORT_NOT_INIT;
    }
    if(!serial->isOpen())
    {
        return E_PORT_NOT_OPEN;
    }
    QByteArray sending_data;
    sending_data.clear();
    sending_data.append(data);
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
            serial->write(sending_data.constData()+n, 1);
    }
    else
    {
        serial->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void SerialBox4::procSerialDataReceive()
{
    if(!serial)
    {
        return;
    }
    if(!serial->isOpen())
    {
        return;
    }
    emit output(serial->readAll());
}
//--------------------------------------------------------------------------------
QString SerialBox4::ByteArrayToHex(const QByteArray &data)
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
void SerialBox4::sendData(const QByteArray &sending_data)
{
    //qDebug() << "sendData[" << sending_data.toHex() << "]";
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            //qDebug() << "sending_data.constData()+n" << sending_data.constData()+n;
            serial->write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        //emit debug(QString("send 0x%1").arg(sending_data.toHex().data()));
        serial->write(sending_data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox4::drawData(const QByteArray &data)
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
void SerialBox4::updateText(void)
{
#ifdef RS232_LOG
    logBox->updateText();
#endif

#ifdef RS232_SEND
    sendBox->updateText();
#endif
}
//--------------------------------------------------------------------------------
void SerialBox4::changeEvent(QEvent *e)
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
bool SerialBox4::isOpen(void)
{
    return serial->isOpen();
}
//--------------------------------------------------------------------------------
bool SerialBox4::add_menu(int index)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return false;
    }

    QMenu *menu = new QMenu(tr("Настройка RS-232"));

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
bool SerialBox4::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
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
void SerialBox4::set_flag_in_hex(bool state)
{
    // emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox4::set_flag_byte_by_byte(bool state)
{
    // emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox4::set_default(void)
{
    if(serial->isOpen())
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
void SerialBox4::get_parameter(void)
{
    if(serial->isOpen() == false)
    {
        return;
    }
    parameter_baud = ui->BaudBox->currentText();
    parameter_data = ui->DataBitsBox->currentText();
    parameter_parity = ui->ParityBox->currentText();
    parameter_stop = ui->StopBitsBox->currentText();
    parameter_flow = ui->FlowBox->currentText();

    QString temp;
    temp.clear();
    temp = QString("%1, %2, %3, %4, %5")
            .arg(parameter_baud)
            .arg(parameter_data)
            .arg(parameter_parity)
            .arg(parameter_stop)
            .arg(parameter_flow);

    emit info(temp);
}
//--------------------------------------------------------------------------------
QPushButton *SerialBox4::add_QPushButton(const QString &title)
{
    QPushButton *btn = new QPushButton(title);
    ui->buttons_layout->addWidget(btn);
    return btn;
}
//--------------------------------------------------------------------------------
void SerialBox4::add_QHBoxLayout(QHBoxLayout * hbox)
{
    ui->buttons_layout->addLayout(hbox);
}
//--------------------------------------------------------------------------------
QByteArray SerialBox4::readAll(void)
{
    return serial->readAll();
}
//--------------------------------------------------------------------------------
