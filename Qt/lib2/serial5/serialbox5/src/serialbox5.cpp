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
#include "mainwindow.hpp"
#include "serialbox5.hpp"

#include "mywidget.hpp"
#include "ui_serialbox5.h"
//--------------------------------------------------------------------------------
#ifdef RS232_SEND
#   include "sendbox5.hpp"
#endif
//--------------------------------------------------------------------------------
#   include "logbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
SerialBox5::SerialBox5(QWidget *parent) :
    SerialWidget(parent),
    ui(new Ui::SerialBox5),
    caption("no name")
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5::SerialBox5(QWidget *parent,
                       const QString &caption,
                       const QString &o_name) :
    SerialWidget(parent),
    ui(new Ui::SerialBox5),
    caption(caption),
    o_name(o_name)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5::~SerialBox5()
{
    if(m_timer)
    {
        delete m_timer;
    }

#ifdef RS232_LOG
    if(logBox)
    {
        disconnect(this,   &SerialBox5::info,  logBox, &LogBox::infoLog);
        disconnect(this,   &SerialBox5::debug, logBox, &LogBox::debugLog);
        disconnect(this,   &SerialBox5::error, logBox, &LogBox::errorLog);
        disconnect(this,   &SerialBox5::trace, logBox, &LogBox::traceLog);

        logBox->close();
        delete logBox;
    }
#endif

#ifdef RS232_SEND
    if(sendBox5)
    {
        disconnect(sendBox5,   &SendBox5::sendData, this, &SerialBox5::sendData);

        sendBox5->close();
        delete sendBox5;
    }
#endif
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
    init_timer();

#ifdef RS232_FIXED_SIZE
    setFixedSize(sizeHint());
#endif

    connect(ui->btn_default,    &QPushButton::clicked,  this,   &SerialBox5::set_default);

    QTimer::singleShot(100, [this]{
        setCloseState();
    });
    updateText();
}
//--------------------------------------------------------------------------------
void SerialBox5::init_timer(void)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerialBox5::checkPorts);
    m_timer->start(5000);
}
//--------------------------------------------------------------------------------
void SerialBox5::checkPorts(void)
{
    QStringList currentPortNames;
    QList<QSerialPortInfo> currentPorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : currentPorts)
    {
        currentPortNames << port.portName();
    }

    // Сравнение с предыдущим состоянием
    if (currentPortNames != m_portNames)
    {
        m_portNames = currentPortNames;

        QList<QSerialPortInfo> currentPorts = QSerialPortInfo::availablePorts();
        QStringList currentPortNames;
        for (const QSerialPortInfo &port : currentPorts)
        {
            currentPortNames << port.portName();
        }

        // Обновление QComboBox
        QString current_text = ui->PortBox->currentText();

        ui->PortBox->clear();
        ui->PortBox->addItems(currentPortNames);

        ui->PortBox->setCurrentText(current_text);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::createWidgets(void)
{
    QFont font("Liberation Sans", 8, QFont::Bold);
    ui->captionBox->setFont(font);
    ui->captionBox->setText(caption);

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    ui->gridLayout->setMargin(0);
#endif
    ui->gridLayout->setSpacing(0);
}
//--------------------------------------------------------------------------------
void SerialBox5::refresh(void)
{
    ui->PortBox->clear();
    ui->PortBox->addItems(get_port_names());
}
//--------------------------------------------------------------------------------
void SerialBox5::initEnumerator(void)
{
    refresh();
    //---
    ui->BaudBox->clear();
#ifdef Q_OS_LINUX
    // ui->BaudBox->addItem("Undefined Baud",  QSerialPort::UnknownBaud);
#endif
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
#ifdef Q_OS_LINUX
    // ui->DataBitsBox->addItem("Undefined data bits", QSerialPort::UnknownDataBits);
#endif
    ui->DataBitsBox->addItem("5 bit",               QSerialPort::Data5);
    ui->DataBitsBox->addItem("6 bit",               QSerialPort::Data6);
    ui->DataBitsBox->addItem("7 bit",               QSerialPort::Data7);
    ui->DataBitsBox->addItem("8 bit",               QSerialPort::Data8);
    //---
    ui->ParityBox->clear();
#ifdef Q_OS_LINUX
    // ui->ParityBox->addItem("Undefined parity",  QSerialPort::UnknownParity);
#endif
    ui->ParityBox->addItem("None",              QSerialPort::NoParity);
    ui->ParityBox->addItem("Even",              QSerialPort::EvenParity);
    ui->ParityBox->addItem("Odd",               QSerialPort::OddParity);
    ui->ParityBox->addItem("Space",             QSerialPort::SpaceParity);
    ui->ParityBox->addItem("Mark",              QSerialPort::MarkParity);
    //---
    ui->StopBitsBox->clear();
#ifdef Q_OS_LINUX
    // ui->StopBitsBox->addItem("Undefined stop bits", QSerialPort::UnknownStopBits);
#endif
    ui->StopBitsBox->addItem("1",                   QSerialPort::OneStop);
    ui->StopBitsBox->addItem("1.5",                 QSerialPort::OneAndHalfStop);
    ui->StopBitsBox->addItem("2",                   QSerialPort::TwoStop);
    //---
    ui->FlowBox->clear();
#ifdef Q_OS_LINUX
    // ui->FlowBox->addItem("Undefined flow",  QSerialPort::UnknownFlowControl);
#endif
    ui->FlowBox->addItem("Disable",         QSerialPort::NoFlowControl);
    ui->FlowBox->addItem("Hardware",        QSerialPort::HardwareControl);
    ui->FlowBox->addItem("Software",        QSerialPort::SoftwareControl);
}
//--------------------------------------------------------------------------------
void SerialBox5::initSerial(void)
{
    ui->PortBox->setProperty(NO_SAVE, true);
    ui->BaudBox->setProperty(NO_SAVE, true);
    ui->FlowBox->setProperty(NO_SAVE, true);
    ui->ParityBox->setProperty(NO_SAVE, true);
    ui->DataBitsBox->setProperty(NO_SAVE, true);
    ui->StopBitsBox->setProperty(NO_SAVE, true);
    ui->btn_power->setProperty(NO_SAVE, true);
    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->btn_refresh->setToolTip("Обновить список портов");

#ifdef RS232_LOG
    logBox = new LogBox(o_name, this);
    ui->layout_right_LOG->addWidget(logBox);

    connect(this,   &SerialBox5::info,  logBox, &LogBox::infoLog);
    connect(this,   &SerialBox5::debug, logBox, &LogBox::debugLog);
    connect(this,   &SerialBox5::error, logBox, &LogBox::errorLog);
    connect(this,   &SerialBox5::trace, logBox, &LogBox::traceLog);
#endif

#ifdef RS232_SEND
    sendBox5 = new SendBox5(this);
    Q_ASSERT(sendBox5);
    connect(sendBox5,   &SendBox5::sendData, this, &SerialBox5::sendData);
    ui->layout_other->addWidget(sendBox5);
#endif

    connect(this,   &SerialWidget::s_baudRateChanged,    this,   &SerialBox5::set_baudRate);
    connect(this,   &SerialWidget::s_dataBitsChanged,    this,   &SerialBox5::set_dataBits);
    connect(this,   &SerialWidget::s_flowControlChanged, this,   &SerialBox5::set_flowControl);
    connect(this,   &SerialWidget::s_parityChanged,      this,   &SerialBox5::set_parity);
    connect(this,   &SerialWidget::s_stopBitsChanged,    this,   &SerialBox5::set_stopBits);

    connect(this,   &SerialBox5::port_close,    this,   &SerialBox5::setCloseState);
    connect(this,   &SerialBox5::port_open,     this,   &SerialBox5::setOpenState);

    connect(ui->btn_power,      &QPushButton::clicked,  this,   &SerialBox5::btnOpenPortClicked);
    connect(ui->btn_refresh,    &QPushButton::clicked,  this,   &SerialBox5::refresh);

    connect(ui->BaudBox,     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5::setBaudBox);
    connect(ui->DataBitsBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5::setDataBox);
    connect(ui->ParityBox,   static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5::setParityBox);
    connect(ui->StopBitsBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5::setStopBox);
    connect(ui->FlowBox,     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  this,   &SerialBox5::setFlowBox);

    connect(this,   &SerialBox5::output, this,   &SerialBox5::drawData);
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
    emit port_is_active(false);
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
    emit port_is_active(true);
}
//--------------------------------------------------------------------------------
void SerialBox5::btnOpenPortClicked()
{
    bool result = isOpen();
    if (result)
    {
        serial_close();
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

            idx = ui->DataBitsBox->findData(dataBits());
            if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

            idx = ui->ParityBox->findData(parity());
            if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

            idx = ui->StopBitsBox->findData(stopBits());
            if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

            idx = ui->FlowBox->findData(flowControl());
            if (idx != -1) ui->FlowBox->setCurrentIndex(idx);

            get_parameter();
            emit port_is_active(true);
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
int SerialBox5::input(const QByteArray &sending_data)
{
    emit trace(QString("SerialBox5::input [%1]").arg(sending_data.toHex().data()));
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
        QString temp = sending_data;
        bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
        if(containsNonASCII == false)
        {
            if(temp.isEmpty() == false)
            {
                QString x_str = temp.remove('\r').remove('\n');
                emit debug(QString("send: %1").arg(x_str));
            }
        }
        else
        {
            emit debug(QString("send [%1]").arg(sending_data.toHex().toUpper().data()));
        }
        write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5::input(const QString &data)
{
#ifdef QT_DEBUG
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
            write(sending_data.constData()+n, 1);
        }
    }
    else
    {
        write(sending_data);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
#ifdef RS232_LOG
    logBox->infoLog(data.data());
#else
    Q_UNUSED(data)
#endif
}
//--------------------------------------------------------------------------------
void SerialBox5::setBaudBox(int index)
{
    bool ok = false;
    int value = ui->BaudBox->itemData(index, Qt::UserRole).toInt(&ok);
    if(!ok) return;
    if(value < 0) return;

    ok = setBaudRate(value);
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

    ok = setDataBits(static_cast<QSerialPort::DataBits>(value));
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

    ok = setParity(static_cast<QSerialPort::Parity>(value));
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

    ok = setStopBits(static_cast<QSerialPort::StopBits>(value));
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

    ok = setFlowControl(static_cast<QSerialPort::FlowControl>(value));
    if(!ok) emit error("error set flow");
    else emit info(QString("set %1").arg(ui->FlowBox->currentText()));
}
//--------------------------------------------------------------------------------
void SerialBox5::updateText(void)
{
    ui->retranslateUi(this);

#ifdef RS232_SEND
    sendBox5->updateText();
#endif
}
//--------------------------------------------------------------------------------
bool SerialBox5::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5::save_setting(void)
{

}
//--------------------------------------------------------------------------------
bool SerialBox5::add_menu(int index)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    QMenu *menu = new QMenu(caption);
    Q_ASSERT(menu);

    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_byte_by_byte->setCheckable(true);
    action_flag_byte_by_byte->setText("byte to byte");
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_byte_by_byte,   &QAction::triggered,    this,   &SerialBox5::set_flag_byte_by_byte);

    mw->add_optionsmenu_menu(index, menu);

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5::add_menu(int index, const QString &title)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    QMenu *menu = new QMenu(title);

    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_byte_by_byte->setCheckable(true);
    action_flag_byte_by_byte->setText("byte to byte");
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_byte_by_byte, &QAction::triggered, this, &SerialBox5::set_flag_byte_by_byte);

    mw->add_menu(index, menu);

    return true;
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
    if(isOpen())
    {
        setBaudRate(9600);
        setDataBits(QSerialPort::Data8);
        setParity(QSerialPort::NoParity);
        setStopBits(QSerialPort::OneStop);
        setFlowControl(QSerialPort::NoFlowControl);

        int idx = 0;

        idx = ui->BaudBox->findData(baudRate());
        if (idx != -1) ui->BaudBox->setCurrentIndex(idx);

        idx = ui->DataBitsBox->findData(dataBits());
        if (idx != -1) ui->DataBitsBox->setCurrentIndex(idx);

        idx = ui->ParityBox->findData(parity());
        if (idx != -1) ui->ParityBox->setCurrentIndex(idx);

        idx = ui->StopBitsBox->findData(stopBits());
        if (idx != -1) ui->StopBitsBox->setCurrentIndex(idx);

        idx = ui->FlowBox->findData(flowControl());
        if (idx != -1) ui->FlowBox->setCurrentIndex(idx);
    }
}
//--------------------------------------------------------------------------------
void SerialBox5::get_parameter(void)
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
QPushButton *SerialBox5::add_QPushButton(const QString &title)
{
    QPushButton *btn = new QPushButton(title);
    Q_ASSERT(btn);
    ui->layout_other->addWidget(btn);
    return btn;
}
//--------------------------------------------------------------------------------
void SerialBox5::add_QHBoxLayout(QHBoxLayout * hbox)
{
    ui->layout_other->addLayout(hbox, 0, 0);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_baudRate(qint32 value)
{
    return setBaudRate(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_dataBits(QSerialPort::DataBits value)
{
    return setDataBits(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_parity(QSerialPort::Parity value)
{
    return setParity(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_stopBits(QSerialPort::StopBits value)
{
    return setStopBits(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5::set_flowControl(QSerialPort::FlowControl value)
{
    return setFlowControl(value);
}
//--------------------------------------------------------------------------------
qint32 SerialBox5::get_baudRate(void)
{
    return baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits       SerialBox5::get_dataBits(void)
{
    return dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity         SerialBox5::get_parity(void)
{
    return parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits       SerialBox5::get_stopBits(void)
{
    return stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl    SerialBox5::get_flowControl(void)
{
    return flowControl();
}
//--------------------------------------------------------------------------------
bool SerialBox5::power_on(void)
{
    if(isOpen())
    {
        return false;
    }

    bool ok = false;
    ok = serial_open();
    btnOpenPortClicked();

    return ok;
}
//--------------------------------------------------------------------------------
bool SerialBox5::power_off(void)
{
    if(isOpen())
    {
        serial_close();
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
