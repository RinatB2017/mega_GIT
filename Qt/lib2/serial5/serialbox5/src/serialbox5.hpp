/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                      **
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
#ifndef SERIALBOX5_HPP
#define SERIALBOX5_HPP
//--------------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QPushButton>
//--------------------------------------------------------------------------------
#include "serialwidget.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class SendBox5;
class LogBox;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox5;
}
//--------------------------------------------------------------------------------
class SerialBox5 : public SerialWidget
{
    Q_OBJECT

public:
    enum
    {
        E_NO_ERROR = 0,
        E_PORT_NOT_INIT,
        E_PORT_NOT_OPEN
    };

    explicit SerialBox5(QWidget *parent,
               const QString &caption,
               const QString &o_name = "SerialBox5");
    explicit SerialBox5(QWidget *parent = nullptr);
    virtual ~SerialBox5();

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    QPushButton *add_QPushButton(const QString &title);
    void add_QHBoxLayout(QHBoxLayout *hbox);

    void set_caption(QString value);

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
    Ui::SerialBox5 *ui;
    QString caption;
    QString o_name;

    bool flag_byte_by_byte = false;

    QTimer *m_timer = nullptr;
    QStringList m_portNames;

#ifdef RS232_LOG
    LogBox  *logBox = nullptr;
#endif

#ifdef RS232_SEND
    SendBox5 *sendBox5 = nullptr;
#endif

    void init(void);
    void init_timer(void);
    void createWidgets(void);
    void initEnumerator(void);
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
    void btnOpenPortClicked(void);

    void refresh(void);

    void baudRateChanged(qint32 baudRate, QSerialPort::Directions);
    void breakEnabledChanged(bool set);
    void dataBitsChanged(QSerialPort::DataBits dataBits);
    void dataTerminalReadyChanged(bool set);
    void flowControlChanged(QSerialPort::FlowControl flow);
    void parityChanged(QSerialPort::Parity parity);
    void requestToSendChanged(bool set);
    void stopBitsChanged(QSerialPort::StopBits stopBits);

    void setBaudBox(int index);
    void setDataBox(int index);
    void setParityBox(int index);
    void setStopBox(int index);
    void setFlowBox(int index);

    void getStatus(const QString &status, QDateTime current);

    void get_parameter(void);

    void set_default(void);

    void checkPorts(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
