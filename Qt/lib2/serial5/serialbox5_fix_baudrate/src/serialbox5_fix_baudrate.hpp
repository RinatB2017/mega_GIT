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
#ifndef SERIALBOX5_FIX_BAUDRATE_HPP
#define SERIALBOX5_FIX_BAUDRATE_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "serialwidget.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class LogBox;
class SendBox5;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox5_fix_baudrate;
}
//--------------------------------------------------------------------------------
class SerialBox5_fix_baudrate : public SerialWidget
{
    Q_OBJECT

public:
    explicit SerialBox5_fix_baudrate(QWidget *parent,
                                     const QString &caption,
                                     const QString &o_name);
    explicit SerialBox5_fix_baudrate(QWidget *parent = nullptr);
    ~SerialBox5_fix_baudrate();

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    void set_caption(const QString &value);

    bool set_fix_baudrate(int value);

    qint32 get_baudRate(void);
    QSerialPort::DataBits       get_dataBits(void);
    QSerialPort::Parity         get_parity(void);
    QSerialPort::StopBits       get_stopBits(void);
    QSerialPort::FlowControl    get_flowControl(void);

private:
    Ui::SerialBox5_fix_baudrate *ui;
    QWidget *parent;

    QString caption;
    QString o_name;

    bool flag_in_hex = false;
    bool flag_byte_by_byte = false;

    int fix_baudrate = 9600;

#ifndef RS232_NO_FRAME
    void add_frame_text(QFrame *parent,
                        const QString &text);
#endif

#ifdef RS232_SEND
    SendBox5 *sendBox5;
#endif

    void init(void);
    void createWidgets(void);
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
    void getStatus(const QString &status, QDateTime current);
    void get_parameter(void);

    void change_icon(bool state);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
