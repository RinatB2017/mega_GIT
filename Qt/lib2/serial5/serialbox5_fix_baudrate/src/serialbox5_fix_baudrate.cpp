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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5_fix_baudrate.hpp"
#include "ui_serialbox5_fix_baudrate.h"
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
    SerialWidget(parent),
    ui(new Ui::SerialBox5_fix_baudrate)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate::SerialBox5_fix_baudrate(QWidget *parent,
                                                 const QString &caption,
                                                 const QString &o_name) :
    SerialWidget(parent),
    ui(new Ui::SerialBox5_fix_baudrate)
{
    this->caption = caption;
    this->o_name  = o_name;

    init();
}
//--------------------------------------------------------------------------------
SerialBox5_fix_baudrate::~SerialBox5_fix_baudrate()
{
    if(m_timer)
    {
        delete m_timer;
    }

#ifdef RS232_SEND
    if(sendBox5)
    {
        disconnect(sendBox5, &SendBox5::sendData, this, &SerialBox5_fix_baudrate::sendData);

        sendBox5->close();
        delete sendBox5;
    }
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::set_caption(const QString &value)
{
    caption = value;
    o_name = value;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::set_fix_baudrate(int value)
{
    fix_baudrate = value;
    return setBaudRate(value);
}
//--------------------------------------------------------------------------------
qint32 SerialBox5_fix_baudrate::get_baudRate(void)
{
    return baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits       SerialBox5_fix_baudrate::get_dataBits(void)
{
    return dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity         SerialBox5_fix_baudrate::get_parity(void)
{
    return parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits       SerialBox5_fix_baudrate::get_stopBits(void)
{
    return stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl    SerialBox5_fix_baudrate::get_flowControl(void)
{
    return flowControl();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::init(void)
{
    ui->setupUi(this);

    createWidgets();
    initSerial();
    init_timer();

    ui->cb_PortBox->setMinimumWidth(150);

    ui->btn_power->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay)));
    ui->btn_refresh->setToolTip("Обновить список портов");

    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedWidth(sizeHint().width());
    //setFixedHeight(sizeHint().height());

    QTimer::singleShot(100, [this]{
        setCloseState();
    });
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::init_timer(void)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerialBox5_fix_baudrate::checkPorts);
    m_timer->start(5000);
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::checkPorts(void)
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
void SerialBox5_fix_baudrate::createWidgets(void)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    ui->gridLayout->setMargin(0);
#endif
    ui->gridLayout->setSpacing(0);

    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->cb_PortBox->setProperty(NO_BLOCK, true);

    connect(ui->btn_power,      &QPushButton::clicked,  this,   &SerialBox5_fix_baudrate::btnOpenPortClicked);
    connect(ui->btn_refresh,    &QToolButton::clicked,  this,   &SerialBox5_fix_baudrate::refresh);

    connect(this, &SerialBox5_fix_baudrate::output, this, &SerialBox5_fix_baudrate::drawData);

#ifdef RS232_SEND
    sendBox5 = new SendBox5(this);
    connect(sendBox5, &SendBox5::sendData, this, &SerialBox5_fix_baudrate::sendData);
    ui->layout_SEND->addWidget(sendBox5);
#endif
}
//--------------------------------------------------------------------------------
#ifndef RS232_NO_FRAME
void SerialBox5_fix_baudrate::add_frame_text(QFrame *parent,
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
void SerialBox5_fix_baudrate::refresh(void)
{
    ui->cb_PortBox->clear();
    ui->cb_PortBox->addItems(get_port_names());
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::initSerial(void)
{
    ui->cb_PortBox->setProperty(NO_SAVE, true);
    ui->btn_power->setProperty(NO_SAVE, true);
    ui->btn_refresh->setProperty(NO_BLOCK, true);
    ui->btn_refresh->setToolTip("Обновить список портов");

    connect(ui->btn_power,  &QToolButton::toggled,  this,   &SerialBox5_fix_baudrate::change_icon);

    connect(this,   &SerialBox5_fix_baudrate::port_close,    this,   &SerialBox5_fix_baudrate::setCloseState);
    connect(this,   &SerialBox5_fix_baudrate::port_open,     this,   &SerialBox5_fix_baudrate::setOpenState);
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
    ui->btn_refresh->setEnabled(true);
    ui->cb_PortBox->setEnabled(true);
    ui->btn_power->setChecked(false);
#ifdef RS232_SEND
    sendBox5->block_interface(true);
#endif
    ui->btn_power->setToolTip("Старт");
    emit port_is_active(false);
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::setOpenState()
{
    ui->btn_refresh->setEnabled(false);
    ui->cb_PortBox->setEnabled(false);
    ui->btn_power->setChecked(true);
#ifdef RS232_SEND
    sendBox5->block_interface(false);
#endif
    ui->btn_power->setToolTip("Стоп");
    emit port_is_active(true);
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::btnOpenPortClicked()
{
    bool result = isOpen();
    if (result)
    {
        serial_close();
        result = false;
    }
    else
    {
        QString text = ui->cb_PortBox->currentText();
        if(text.isEmpty())
        {
            if(isOpen())
            {
                port_close();
            }
            setCloseState();
            return;
        }
        setPortName(text);
        result = serial_open();
        if(result)
        {
            bool ok = setBaudRate(fix_baudrate);
            if(!ok)
            {
                emit error(QString("Не удалось установить baudrate %1").arg(fix_baudrate));
            }
            get_parameter();
        }
        else
        {
            emit error(QString("ERROR: serial [%1] not open (%2)")
                       .arg(portName())
                       .arg(errorString()));
        }
    }

    (result) ? setOpenState() : setCloseState();
}
//--------------------------------------------------------------------------------
int SerialBox5_fix_baudrate::input(const QByteArray &sending_data)
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
        QString temp = QString(sending_data);

        //if(sending_data.isLetterOrNumber)
        //bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
        //bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^\\x{0020}-\\x{007F}]")));
        //bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^[:print:]]")));

        bool containsNonASCII = temp.contains(QRegularExpression(QStringLiteral("[^\\w\\d\\s]")));
        if(containsNonASCII == false)
        {
            if(temp.isEmpty() == false)
            {
                QString x_str = temp.remove('\r').remove('\n');
                emit debug(QString("send TXT: %1").arg(x_str));
            }
        }
        else
        {
            emit debug(QString("send HEX: [%1]").arg(sending_data.toHex().toUpper().data()));
        }
        write(sending_data);
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int SerialBox5_fix_baudrate::input(const QString &data)
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
    if(flag_byte_by_byte)
    {
        int len = data.length();
        for(int n=0; n<len; n++)
        {
            write(data.toLatin1().constData()+n, 1);
        }
    }
    else
    {
        write(data.toLatin1());
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
QString SerialBox5_fix_baudrate::ByteArrayToHex(const QByteArray &data)
{
    return data.toHex();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::sendData(const QByteArray &sending_data)
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
void SerialBox5_fix_baudrate::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
    Q_UNUSED(data)
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::add_menu(int index)
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

        connect(action_flag_in_hex,         &QAction::triggered,    this,   &SerialBox5_fix_baudrate::set_flag_in_hex);
        connect(action_flag_byte_by_byte,   &QAction::triggered,    this,   &SerialBox5_fix_baudrate::set_flag_byte_by_byte);
    }
    else
    {
        emit error("mw not found");
    }

    Q_UNUSED(index);

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::add_menu(int index, const QString &title)
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

        connect(action_flag_in_hex, &QAction::triggered, this, &SerialBox5_fix_baudrate::set_flag_in_hex);
        connect(action_flag_byte_by_byte, &QAction::triggered, this, &SerialBox5_fix_baudrate::set_flag_byte_by_byte);
    }
    else
    {
        emit error("mw not found");
    }

    Q_UNUSED(index);

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
void SerialBox5_fix_baudrate::set_portname(const QString &portname)
{
    for(int n=0; n<ui->cb_PortBox->count(); n++)
    {
        ui->cb_PortBox->setCurrentIndex(n);
        if(ui->cb_PortBox->currentText() == portname)
        {
            return;
        }
    }
}
//--------------------------------------------------------------------------------
QString SerialBox5_fix_baudrate::get_portname(void)
{
    return ui->cb_PortBox->currentText();
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::updateText(void)
{
#ifdef RS232_SEND
    sendBox5->updateText();
#endif
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool SerialBox5_fix_baudrate::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_fix_baudrate::save_setting(void)
{

}
//--------------------------------------------------------------------------------
