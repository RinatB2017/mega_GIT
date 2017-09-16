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
#include <QByteArray>
#include <QDateTime>
#include <QWidget>
#include <QString>
#include <QFrame>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
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
class SerialBox5 : public QFrame
{
    Q_OBJECT

public:
    explicit SerialBox5(QWidget *parent,
                        const QString &caption,
                        const QString &o_name);
    explicit SerialBox5(QWidget *parent = 0);
    ~SerialBox5();

    bool isOpen(void);
    void updateText(void);

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    QPushButton *add_QPushButton(const QString &title);
    void add_QHBoxLayout(QHBoxLayout *hbox);

    QByteArray readAll(void);

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

    void connect_log(void);
    void init(void);
    void createWidgets(void);
    void initEnumerator(void);
    void initSerial(void);
    void setOpenState(void);
    void setCloseState(void);
    QString ByteArrayToHex(const QByteArray &data);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void output(const QByteArray &data);

public slots:
    int input(const QByteArray &sending_data);
    int input(const QString &data);

    void set_flag_in_hex(bool state);
    void set_flag_byte_by_byte(bool state);

private slots:
    void log(const QString &data);
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

    void getStatus(const QString &status, QDateTime current);

    void serial5_error(QSerialPort::SerialPortError err);

    void get_parameter(void);

    void set_default(void);

    void timer_stop(void);

protected:
    void changeEvent(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
