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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QByteArray>
#   include <QDateTime>
#   include <QWidget>
#   include <QString>
#   include <QFrame>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QHBoxLayout;
class QPushButton;
class LogBox;
class SendBox5;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox5;
}
//--------------------------------------------------------------------------------
class SerialBox5 : public MyWidget
{
    Q_OBJECT

public:
    enum
    {
        E_NO_ERROR = 0,
        E_PORT_NOT_INIT,
        E_PORT_NOT_OPEN
    };

    SerialBox5(QWidget *parent,
               const QString &caption,
               const QString &o_name = "SerialBox5");
    SerialBox5(QWidget *parent = 0);
    ~SerialBox5();

    bool isOpen(void);

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    QPushButton *add_QPushButton(const QString &title);
    void add_QHBoxLayout(QHBoxLayout *hbox);

    QByteArray readAll(void);

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

    qint64 bytesAvailable(void);

private:
    Ui::SerialBox5 *ui = 0;
    QWidget *parent = 0;
    QSerialPort *serial5 = 0;
    QString caption;
    QString o_name;

    bool flag_in_hex = false;
    bool flag_byte_by_byte = false;

#ifdef RS232_LOG
    LogBox  *logBox = 0;
#endif

#ifdef RS232_SEND
    SendBox5 *sendBox5 = 0;
#endif

    QTimer *timer = 0;

#ifndef RS232_NO_FRAME
    QFrame *frame_ring = 0;
    QFrame *frame_dsr = 0;
    QFrame *frame_cts = 0;
    void add_frame_text(QFrame *parent,
                        const QString &text);
#endif

    void init(void);
    void createWidgets(void);
    void initEnumerator(void);
    void initSerial(void);
    void setOpenState(void);
    void setCloseState(void);
    QString ByteArrayToHex(const QByteArray &data);

signals:
    void readyRead(void);
    void readChannelFinished(void);

    void not_working(void);
    void output(const QByteArray &data);

public slots:
    int input(const QByteArray &sending_data);
    int input(const QString &data);

    void set_flag_in_hex(bool state);
    void set_flag_byte_by_byte(bool state);

private slots:
    void drawData(const QByteArray &data);
    void sendData(const QByteArray &sending_data);
    void btnOpenPortClicked(void);

    void refresh(void);

    void procSerialDataReceive(void);

    void setBaudBox(int index);
    void setDataBox(int index);
    void setParityBox(int index);
    void setStopBox(int index);
    void setFlowBox(int index);

    void baudRateChanged(qint32 baudRate, QSerialPort::Directions);
    void breakEnabledChanged(bool set);
    void dataBitsChanged(QSerialPort::DataBits dataBits);
    void dataTerminalReadyChanged(bool set);
    void errorOccurred(QSerialPort::SerialPortError error);
    void flowControlChanged(QSerialPort::FlowControl flow);
    void parityChanged(QSerialPort::Parity parity);
    void requestToSendChanged(bool set);
    void stopBitsChanged(QSerialPort::StopBits stopBits);

    void getStatus(const QString &status, QDateTime current);

    void serial5_error(QSerialPort::SerialPortError err);

    void get_parameter(void);

    void set_default(void);

    void timer_stop(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
