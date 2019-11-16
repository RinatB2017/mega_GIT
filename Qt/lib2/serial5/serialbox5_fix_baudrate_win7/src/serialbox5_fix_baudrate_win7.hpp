/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                      **
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
#ifndef SERIALBOX5_FIX_BAUDRATE_WIN7_HPP
#define SERIALBOX5_FIX_BAUDRATE_WIN7_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef FAKE
#   include "fake_serialbox5.hpp"
#endif
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "serialbox5_thread.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QHBoxLayout;
class QPushButton;
class LogBox;
class SendBox5;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox5_fix_baudrate_win7;
}
//--------------------------------------------------------------------------------
class SerialBox5_fix_baudrate_win7 : public MyWidget
{
    Q_OBJECT

public:
    SerialBox5_fix_baudrate_win7(QWidget *parent,
                    const QString &caption,
                    const QString &o_name);
    SerialBox5_fix_baudrate_win7(QWidget *parent = nullptr);
    ~SerialBox5_fix_baudrate_win7();

    bool isOpen(void);

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    void set_caption(QString value);

    void set_fix_baudrate(int value);
    void write(const char *data);
    QByteArray readAll(void);

signals:
    void readyRead(void);
    void readChannelFinished(void);

    void output(const QByteArray &data);

    void state(bool);

    //---
signals:
    void port_open(void);
    void port_close(void);
    void port_set_name(QString name);
    void port_set_baudrate(qint32 baudrate);
    void port_set_bits(QSerialPort::DataBits);
    void port_set_stop_bits(QSerialPort::StopBits);
    void port_set_parity(QSerialPort::Parity);
    void port_set_flow_control(QSerialPort::FlowControl);
    void port_write(const char *data);
    void port_write(const char *data, qint64 maxSize);

private slots:
    void port_bytes_avialable(void);
    void port_get_state(bool state);
    void port_get_name(QString port_name);
    void port_get_baudrate(qint32 value);
    void port_getbits(QSerialPort::DataBits value);
    void port_get_stop_bits(QSerialPort::StopBits value);
    void port_get_patity(QSerialPort::Parity value);
    void port_get_flow_control(QSerialPort::FlowControl value);
    void port_read_all(QByteArray ba);
    void port_ready_read(bool state);
    void port_error(QSerialPort::SerialPortError err);
    //---

public slots:
    int input(const QByteArray &sending_data);
    int input(const QString &data);

    void set_flag_in_hex(bool state);
    void set_flag_byte_by_byte(bool state);

private:
    Ui::SerialBox5_fix_baudrate_win7 *ui;

    QString caption;
    QString o_name;

    bool flag_in_hex = false;
    bool flag_byte_by_byte = false;

    int fix_baudrate = 57600;
    bool is_open = false;
    QByteArray serial_data;
    void initThread(void);

#ifndef RS232_NO_FRAME
    void add_frame_text(QFrame *parent,
                        const QString &text);
#endif

#ifdef RS232_SEND
    SendBox5 *sendBox5;
#endif

    void init(void);
    void createWidgets(void);
    void setOpenState(void);
    void setCloseState(void);
    QString ByteArrayToHex(const QByteArray &data);

private slots:
    void drawData(const QByteArray &data);
    void sendData(const QByteArray &sending_data);
    void btnOpenPortClicked(void);
    void refresh(void);
    void procSerialDataReceive(void);
    void getStatus(const QString &status, QDateTime current);
    void get_parameter(void);
    void timer_stop(void);

    //---
    void thread_is_finished(void);
    //---

    void change_icon(bool state);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
