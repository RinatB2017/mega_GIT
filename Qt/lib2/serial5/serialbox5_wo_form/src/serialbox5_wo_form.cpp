/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "serialbox5_wo_form.hpp"

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
SerialBox5_wo_form::SerialBox5_wo_form(QWidget *parent) :
    SerialWidget(parent),
    caption("no name")
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_wo_form::SerialBox5_wo_form(QWidget *parent,
                       const QString &caption,
                       const QString &o_name) :
    SerialWidget(parent),
    caption(caption),
    o_name(o_name)
{
    init();
}
//--------------------------------------------------------------------------------
SerialBox5_wo_form::~SerialBox5_wo_form()
{

}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::set_caption(const QString &value)
{
    caption = value;
    o_name = value;
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::init(void)
{
    setObjectName(o_name);

    initSerial();

    setCloseState();
    updateText();

    setFixedSize(1, 1);
    setVisible(false);
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::initSerial(void)
{
    connect(this,   &SerialWidget::s_baudRateChanged,    this,   &SerialBox5_wo_form::set_baudRate);
    connect(this,   &SerialWidget::s_dataBitsChanged,    this,   &SerialBox5_wo_form::set_dataBits);
    connect(this,   &SerialWidget::s_flowControlChanged, this,   &SerialBox5_wo_form::set_flowControl);
    connect(this,   &SerialWidget::s_parityChanged,      this,   &SerialBox5_wo_form::set_parity);
    connect(this,   &SerialWidget::s_stopBitsChanged,    this,   &SerialBox5_wo_form::set_stopBits);

    connect(this,   &SerialBox5_wo_form::port_close,    this,   &SerialBox5_wo_form::setCloseState);
    connect(this,   &SerialBox5_wo_form::port_open,     this,   &SerialBox5_wo_form::setOpenState);

    connect(this,   &SerialBox5_wo_form::output,        this,   &SerialBox5_wo_form::drawData);
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::getStatus(const QString &status, QDateTime current)
{
    emit info(QString("%1 %2")
              .arg(status)
              .arg(current.toString()));

    get_parameter();
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::setCloseState(void)
{
    emit port_is_active(false);
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::setOpenState()
{
    emit port_is_active(true);
}
//--------------------------------------------------------------------------------
int SerialBox5_wo_form::input(const QByteArray &sending_data)
{
    emit trace(QString("SerialBox5_wo_form::input [%1]").arg(sending_data.toHex().data()));
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
int SerialBox5_wo_form::input(const QString &data)
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
QString SerialBox5_wo_form::ByteArrayToHex(const QByteArray &data)
{
    return data.toHex();
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::sendData(const QByteArray &sending_data)
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
void SerialBox5_wo_form::drawData(const QByteArray &data)
{
    // qDebug() << "drawData[" << data << "]";
    Q_UNUSED(data)
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::save_setting(void)
{

}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::add_menu(int index)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    QMenu *menu = new QMenu(caption);
    Q_ASSERT(menu);

    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_byte_by_byte->setCheckable(true);
    action_flag_byte_by_byte->setText("byte to byte");
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_byte_by_byte,   &QAction::triggered,    this,   &SerialBox5_wo_form::set_flag_byte_by_byte);

    mw->add_optionsmenu_menu(index, menu);

    return false;
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::add_menu(int index, const QString &title)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    QMenu *menu = new QMenu(title);

    QAction *action_flag_byte_by_byte = new QAction(menu);

    action_flag_byte_by_byte->setCheckable(true);
    action_flag_byte_by_byte->setText("byte to byte");
    menu->addAction(action_flag_byte_by_byte);

    connect(action_flag_byte_by_byte, &QAction::triggered, this, &SerialBox5_wo_form::set_flag_byte_by_byte);

    mw->add_menu(index, menu);

    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::set_flag_byte_by_byte(bool state)
{
    emit debug(QString("state is %1").arg(state ? "true" : "false"));
    flag_byte_by_byte = state;
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::get_parameter(void)
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
    emit info(temp);
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::set_baudRate(qint32 value)
{
    return setBaudRate(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::set_dataBits(QSerialPort::DataBits value)
{
    return setDataBits(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::set_parity(QSerialPort::Parity value)
{
    return setParity(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::set_stopBits(QSerialPort::StopBits value)
{
    return setStopBits(value);
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::set_flowControl(QSerialPort::FlowControl value)
{
    return setFlowControl(value);
}
//--------------------------------------------------------------------------------
qint32 SerialBox5_wo_form::get_baudRate(void)
{
    return baudRate();
}
//--------------------------------------------------------------------------------
QSerialPort::DataBits       SerialBox5_wo_form::get_dataBits(void)
{
    return dataBits();
}
//--------------------------------------------------------------------------------
QSerialPort::Parity         SerialBox5_wo_form::get_parity(void)
{
    return parity();
}
//--------------------------------------------------------------------------------
QSerialPort::StopBits       SerialBox5_wo_form::get_stopBits(void)
{
    return stopBits();
}
//--------------------------------------------------------------------------------
QSerialPort::FlowControl    SerialBox5_wo_form::get_flowControl(void)
{
    return flowControl();
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::power_on(void)
{
    if(isOpen())
    {
        return false;
    }

    bool ok = false;
    ok = serial_open();

    return ok;
}
//--------------------------------------------------------------------------------
bool SerialBox5_wo_form::power_off(void)
{
    if(isOpen())
    {
        serial_close();
    }

    return true;
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::breakEnabledChanged(bool set)
{
    emit debug(QString("breakEnabledChanged %1").arg(set));
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::dataTerminalReadyChanged(bool set)
{
    emit debug(QString("dataTerminalReadyChanged %1").arg(set));
}
//--------------------------------------------------------------------------------
void SerialBox5_wo_form::requestToSendChanged(bool set)
{
    emit debug(QString("requestToSendChanged %1").arg(set));
}
//--------------------------------------------------------------------------------
