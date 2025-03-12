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
#ifndef SERIALBOX5_LITE_HPP
#define SERIALBOX5_LITE_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
//--------------------------------------------------------------------------------
#include "serialwidget.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define SERIALBOX5_LITE_PORTNAME    "SerialBox5_lite_portname"
#define SERIALBOX5_LITE_BAUDRATE    "SerialBox5_lite_baudrate"
//--------------------------------------------------------------------------------
class LogBox;
class SendBox5;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox5_lite;
}
//--------------------------------------------------------------------------------
class SerialBox5_lite : public SerialWidget
{
    Q_OBJECT

    Q_PROPERTY(bool p_test      READ get_test       WRITE set_test)

public:
    explicit SerialBox5_lite(QWidget *parent,
                             const QString &caption,
                             const QString &o_name);
    explicit SerialBox5_lite(QWidget *parent = nullptr);
    virtual ~SerialBox5_lite();

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    bool set_baudRate(int value);
    qint32 get_baudRate(void);

    QSerialPort::DataBits       get_dataBits(void);
    QSerialPort::Parity         get_parity(void);
    QSerialPort::StopBits       get_stopBits(void);
    QSerialPort::FlowControl    get_flowControl(void);

    void set_caption(const QString &value);

private:
    Ui::SerialBox5_lite *ui;
    QWidget *parent;
    QString caption;
    QString o_name;

    bool flag_in_hex = false;
    bool flag_byte_by_byte = false;

    QTimer *m_timer = nullptr;
    QStringList m_portNames;

    //TODO
    bool p_test = false;
    void set_test(bool value);
    bool get_test(void);

#ifndef RS232_NO_FRAME
    void add_frame_text(QFrame *parent,
                        const QString &text);
#endif

#ifdef RS232_SEND
    SendBox5 *sendBox5;
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

    void set_flag_in_hex(bool state);
    void set_flag_byte_by_byte(bool state);

private slots:
    void drawData(const QByteArray &data);
    void sendData(const QByteArray &sending_data);
    void btnOpenPortClicked(void);
    void refresh(void);
    void setBaudBox(int index);
    void getStatus(const QString &status, QDateTime current);
    void get_parameter(void);
    void set_default(void);

    void checkPorts(void);

    void change_icon(bool state);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
