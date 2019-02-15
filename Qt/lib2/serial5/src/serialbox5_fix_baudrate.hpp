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
    class SerialBox5_fix_baudrate;
}
//--------------------------------------------------------------------------------
class SerialBox5_fix_baudrate : public MyWidget
{
    Q_OBJECT

    Q_PROPERTY(bool p_test      READ get_test       WRITE set_test)

public:
    SerialBox5_fix_baudrate(QWidget *parent,
                    const QString &caption,
                    const QString &o_name);
    SerialBox5_fix_baudrate(QWidget *parent = nullptr);
    ~SerialBox5_fix_baudrate();

    bool isOpen(void);

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    QByteArray readAll(void);

    void set_caption(QString value);
    bool set_fix_baudrate(int value);

    qint64 bytesAvailable(void);
    qint64 write ( const char *data );

private:
    Ui::SerialBox5_fix_baudrate *ui;
    QWidget *parent;
    QSerialPort *serial5;
    QString caption;
    QString o_name;

    bool flag_in_hex = false;
    bool flag_byte_by_byte = false;

    QTimer *timer;

    int fix_baudrate = 9600;

    //TODO
    bool p_test = false;
    void set_test(bool value);
    bool get_test(void);

#ifndef RS232_NO_FRAME
    void add_frame_text(QFrame *parent,
                        const QString &text);
#endif

    void init(void);
    void createWidgets(void);
    void initSerial(void);
    void setOpenState(void);
    void setCloseState(void);
    QString ByteArrayToHex(const QByteArray &data);

signals:
    void readyRead(void);
    void readChannelFinished(void);

    void output(const QByteArray &data);

    void is_open(void);
    void is_close(void);

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
    void getStatus(const QString &status, QDateTime current);
    void serial5_error(QSerialPort::SerialPortError err);
    void get_parameter(void);
    void timer_stop(void);

    void change_icon(bool state);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
