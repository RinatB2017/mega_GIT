/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
#ifndef SERIALBOX5_WO_FORM_HPP
#define SERIALBOX5_WO_FORM_HPP
//--------------------------------------------------------------------------------
#include "serialwidget.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class SerialBox5_wo_form : public SerialWidget
{
    Q_OBJECT

signals:
    void s_error(QSerialPort::SerialPortError);

public:
    enum
    {
        E_NO_ERROR = 0,
        E_PORT_NOT_INIT,
        E_PORT_NOT_OPEN
    };

    explicit SerialBox5_wo_form(QWidget *parent,
                                const QString &caption,
                                const QString &o_name = "SerialBox5_wo_form");
    explicit SerialBox5_wo_form(QWidget *parent = nullptr);
    virtual ~SerialBox5_wo_form();

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    QPushButton *add_QPushButton(const QString &title);
    void add_QHBoxLayout(QHBoxLayout *hbox);

    void set_caption(const QString &value);

    bool set_PortName(QString port_name);
    bool set_baudRate(qint32 value);
    bool set_dataBits(QSerialPort::DataBits value);
    bool set_parity(QSerialPort::Parity value);
    bool set_stopBits(QSerialPort::StopBits value);
    bool set_flowControl(QSerialPort::FlowControl value);

    qint32 get_baudRate(void);
    QSerialPort::DataBits       get_dataBits(void);
    QSerialPort::Parity         get_parity(void);
    QSerialPort::StopBits       get_stopBits(void);
    QSerialPort::FlowControl    get_flowControl(void);

    bool power_on(void);
    bool power_off(void);

private:
    QString caption;
    QString o_name;

    bool flag_byte_by_byte = false;

    void init(void);
    void initSerial(void);
    void setOpenState(void);
    void setCloseState(void);
    QString ByteArrayToHex(const QByteArray &data);

public slots:
    int input(const QByteArray &sending_data);
    int input(const QString &data);

    void set_flag_byte_by_byte(bool state);

private slots:
    void drawData(const QByteArray &data);
    void sendData(const QByteArray &sending_data);

    void breakEnabledChanged(bool set);
    void dataTerminalReadyChanged(bool set);
    void requestToSendChanged(bool set);

    void getStatus(const QString &status, QDateTime current);

    void get_parameter(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX5_WO_FORM_HPP
