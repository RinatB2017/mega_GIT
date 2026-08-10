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
// FAKE_SERIAL
//--------------------------------------------------------------------------------
#include <QDateTime>
#include <QWidget>
#include <QEvent>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "fakeenumerator.hpp"
#include "fakeserial.hpp"

#include "serialbox.hpp"
#include "ui_serialbox.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#include "sendbox.hpp"
#endif
#include "logbox.hpp"
//--------------------------------------------------------------------------------
typedef enum
{
    E_NO_ERROR = 0,
    E_PORT_NOT_INIT,
    E_PORT_NOT_OPEN
} ERRORS;
//--------------------------------------------------------------------------------
SerialBox::SerialBox(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SerialBox),
    parent(parent),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    ui->setupUi(this);

    this->caption = "FAKE: no name";
    init();
}
//--------------------------------------------------------------------------------
SerialBox::SerialBox(QWidget *parent, const QString &caption) :
    QFrame(parent),
    ui(new Ui::SerialBox),
    parent(parent),
    flag_in_hex(false),
    flag_byte_by_byte(false)
{
    ui->setupUi(this);

    this->caption = QString("FAKE: %1").arg(caption);
    init();
}
//--------------------------------------------------------------------------------
SerialBox::~SerialBox()
{
#ifdef RS232_LOG
    delete logBox;
#endif

#ifdef RS232_SEND
    delete sendBox;
#endif

    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void SerialBox::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void SerialBox::init()
{
    connect_log();

    createWidgets();
    initEnumerator();
    initSerial();
    initBoxs();
    setCloseState();
#ifdef RS232_FIXED_SIZE
    setFixedSize(sizeHint());
#endif
}
//--------------------------------------------------------------------------------
void SerialBox::createWidgets()
{
    QFont font("Liberation Sans", 8, QFont::Bold);
    ui->captionBox->setFont(font);
    ui->captionBox->setText(caption);
    ui->captionBox->setStyleSheet("background:red;");

    ui->gridLayout->setMargin(0);
    ui->gridLayout->setSpacing(0);

    connect(ui->PowerBtn, SIGNAL(clicked()), this, SLOT(btnOpenPortClicked()));

    connect(ui->BaudBox, SIGNAL(currentIndexChanged(QString)),     this, SLOT(setBaudBox(QString)));
    connect(ui->DataBitsBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setDataBox(QString)));
    connect(ui->ParityBox, SIGNAL(currentIndexChanged(QString)),   this, SLOT(setParityBox(QString)));
    connect(ui->StopBitsBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setStopBox(QString)));
    connect(ui->FlowBox, SIGNAL(currentIndexChanged(QString)),     this, SLOT(setFlowBox(QString)));

    connect(this, SIGNAL(output(QByteArray)), this, SLOT(drawData(QByteArray)));

#ifdef RS232_LOG
    logBox = new LogBox(this);
    ui->layout_right_LOG->addWidget(logBox);
#endif

#ifdef RS232_SEND
    sendBox = new SendBox(this);
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
void SerialBox::add_frame_text(QFrame *parent,
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
void SerialBox::initBoxs()
{
    ui->BaudBox->addItems(this->serial->listBaudRate());
    ui->DataBitsBox->addItems(this->serial->listDataBits());
    ui->ParityBox->addItems(this->serial->listParity());
    ui->StopBitsBox->addItems(this->serial->listStopBits());
    ui->FlowBox->addItems(this->serial->listFlowControl());
#ifdef RS232_LOG
    logBox->clear();
#endif
}
//--------------------------------------------------------------------------------
void SerialBox::initEnumerator()
{
    enumerator = new FakeEnumerator(this);
    connect(enumerator, SIGNAL(hasChanged(QStringList)), this, SLOT(procEnumerate(QStringList)));
    enumerator->setEnabled(true);
}
//--------------------------------------------------------------------------------
void SerialBox::procEnumerate(const QStringList &l)
{
    ui->PortBox->clear();
    ui->PortBox->addItems(l);
}
//--------------------------------------------------------------------------------
void SerialBox::initSerial()
{
    this->serial = new FakeSerial(this);
    connect(this->serial, SIGNAL(ctsChanged(bool)),  this, SIGNAL(procCtsChanged(bool)));
    connect(this->serial, SIGNAL(dsrChanged(bool)),  this, SIGNAL(procDsrChanged(bool)));
    connect(this->serial, SIGNAL(ringChanged(bool)), this, SIGNAL(procRingChanged(bool)));

    connect(this->serial, SIGNAL(readyRead()), this, SLOT(procSerialDataReceive()));

    connect(this->serial, SIGNAL(signalStatus(QString, QDateTime)), this, SLOT(getStatus(QString, QDateTime)));

#ifndef RS232_NO_FRAME
    connect(this->serial, SIGNAL(ringChanged(bool)), this, SLOT(ring_changed(bool)));
    connect(this->serial, SIGNAL(ctsChanged(bool)),  this, SLOT(cts_changed(bool)));
    connect(this->serial, SIGNAL(dsrChanged(bool)),  this, SLOT(dsr_changed(bool)));
#endif
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox::ring_changed(bool state)
{
    state ? frame_ring->setStyleSheet("background:green") : frame_ring->setStyleSheet("");
}
#endif
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox::dsr_changed(bool state)
{
    state ? frame_dsr->setStyleSheet("background:green") : frame_dsr->setStyleSheet("");
}
#endif
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox::cts_changed(bool state)
{
    state ? frame_cts->setStyleSheet("background:green") : frame_cts->setStyleSheet("");
}
#endif
//--------------------------------------------------------------------------------
void SerialBox::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));
}
//--------------------------------------------------------------------------------
void SerialBox::setBaudBox(const QString &text)
{
    this->serial->setBaudRate(text);
}
//--------------------------------------------------------------------------------
void SerialBox::setDataBox(const QString &text)
{
    this->serial->setDataBits(text);
}
//--------------------------------------------------------------------------------
void SerialBox::setParityBox(const QString &text)
{
    this->serial->setParity(text);
}
//--------------------------------------------------------------------------------
void SerialBox::setStopBox(const QString &text)
{
    this->serial->setStopBits(text);
}
//--------------------------------------------------------------------------------
void SerialBox::setFlowBox(const QString &text)
{
    this->serial->setFlowControl(text);
}
//--------------------------------------------------------------------------------
void SerialBox::setCloseState()
{
    ui->PortBox->setEnabled(true);
    ui->BaudBox->setEnabled(false);
    ui->DataBitsBox->setEnabled(false);
    ui->ParityBox->setEnabled(false);
    ui->StopBitsBox->setEnabled(false);
    ui->FlowBox->setEnabled(false);
    ui->PowerBtn->setChecked(false);
}
//--------------------------------------------------------------------------------
void SerialBox::setOpenState()
{
    ui->PortBox->setEnabled(false);
    ui->BaudBox->setEnabled(true);
    ui->DataBitsBox->setEnabled(true);
    ui->ParityBox->setEnabled(true);
    ui->StopBitsBox->setEnabled(true);
    ui->FlowBox->setEnabled(true);
    ui->PowerBtn->setChecked(true);
}
//--------------------------------------------------------------------------------
void SerialBox::btnOpenPortClicked()
{
    int idx;

    if (this->serial)
    {
        bool result = this->serial->isOpen();
        if (result)
        {
            this->serial->close();
            this->serial->enableEmitStatus(false);
            result = false;
        }
        else
        {
            this->serial->setDeviceName(ui->PortBox->currentText());
            result = this->serial->open(QIODevice::ReadWrite);

            this->serial->enableEmitStatus(true);

            idx = ui->BaudBox->findText(this->serial->baudRate());
            if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

            idx = ui->DataBitsBox->findText(this->serial->dataBits());
            if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

            idx = ui->ParityBox->findText(this->serial->parity());
            if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

            idx = ui->StopBitsBox->findText(this->serial->stopBits());
            if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

            idx = ui->FlowBox->findText(this->serial->flowControl());
            if (idx != -1) ui->FlowBox->setCurrentIndex(idx);
        }

        (result) ? this->setOpenState() : this->setCloseState();
    }
}
//--------------------------------------------------------------------------------
int SerialBox::input(const QByteArray &sending_data)
{
    // qDebug() << data;
    emit debug(sending_data.toHex());
    if(!this->serial)
    {
        return E_PORT_NOT_INIT;
    }
    if(!this->serial->isOpen())
    {
        return E_PORT_NOT_OPEN;
    }
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
            this->serial->write(sending_data.constData()+n, 1);
    }
    else
    {
        this->serial->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox::input(const QString &data)
{
    // qDebug() << data;
    if(!this->serial)
    {
        return E_PORT_NOT_INIT;
    }
    if(!this->serial->isOpen())
    {
        return E_PORT_NOT_OPEN;
    }
    QByteArray sending_data;
    sending_data.clear();
    sending_data.append(data);
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
            this->serial->write(sending_data.constData()+n, 1);
    }
    else
    {
        this->serial->write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void SerialBox::procSerialDataReceive()
{
    if(!this->serial)
    {
        return;
    }
    if(!this->serial->isOpen())
    {
        return;
    }
    emit output(this->serial->readAll());
}
//--------------------------------------------------------------------------------
QString SerialBox::ByteArrayToHex(const QByteArray &data)
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
void SerialBox::sendData(const QByteArray &sending_data)
{
    //qDebug() << "sendData[" << sending_data.toHex() << "]";
    if(flag_byte_by_byte)
    {
        for(int n=0; n<sending_data.length(); n++)
        {
            //qDebug() << "sending_data.constData()+n" << sending_data.constData()+n;
            this->serial->write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        this->serial->write(sending_data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox::drawData(const QByteArray &data)
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
        emit info(data);
    }
#endif
}
//--------------------------------------------------------------------------------
void SerialBox::updateText(void)
{
#ifdef RS232_LOG
    logBox->updateText();
#endif

#ifdef RS232_SEND
    sendBox->updateText();
#endif
}
//--------------------------------------------------------------------------------
void SerialBox::changeEvent(QEvent *e)
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
bool SerialBox::isOpen(void)
{
    return serial->isOpen();
}
//--------------------------------------------------------------------------------
void SerialBox::set_flag_in_hex(bool state)
{
    flag_in_hex = state;
}
//--------------------------------------------------------------------------------
void SerialBox::set_flag_byte_by_byte(bool state)
{
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
