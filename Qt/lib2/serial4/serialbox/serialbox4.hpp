/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef SERIALBOX4_HPP
#define SERIALBOX4_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QDateTime>
#include <QWidget>
#include <QString>
#include <QFrame>
//--------------------------------------------------------------------------------
class SerialDeviceEnumerator;
class AbstractSerial;
class QHBoxLayout;
class QPushButton;
class LogBox;
class SendBox4;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox4;
}
//--------------------------------------------------------------------------------
class SerialBox4 : public QFrame
{
    Q_OBJECT

public:
    enum
    {
        E_NO_ERROR = 0,
        E_PORT_NOT_INIT,
        E_PORT_NOT_OPEN
    };

    explicit SerialBox4(QWidget *parent, const QString &caption);
    explicit SerialBox4(QWidget *parent = 0);
    ~SerialBox4();

    bool isOpen(void);
    void updateText(void);

    bool add_menu(int index);
    bool add_menu(int index, const QString &title);

    QPushButton *add_QPushButton(const QString &title);
    void add_QHBoxLayout(QHBoxLayout *hbox);

    QByteArray readAll(void);

private:
    Ui::SerialBox4 *ui = 0;
    QWidget *parent = 0;
    SerialDeviceEnumerator *enumerator = 0;
    AbstractSerial *serial = 0;
    QString caption;

    bool flag_in_hex = false;
    bool flag_byte_by_byte = false;

    QString parameter_baud;
    QString parameter_data;
    QString parameter_parity;
    QString parameter_stop;
    QString parameter_flow;

#ifdef RS232_LOG
    LogBox  *logBox = 0;
#endif

#ifdef RS232_SEND
    SendBox4 *sendBox = 0;
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
    void initBoxs(void);
    void setOpenState(void);
    void setCloseState(void);
    QString ByteArrayToHex(const QByteArray &data);

signals:
    void output(const QByteArray &data);

    void procCtsChanged(bool);
    void procDsrChanged(bool);
    void procRingChanged(bool);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void readChannelFinished(void);

public slots:
    int input(const QByteArray &sending_data);
    int input(const QString &data);

    void set_flag_in_hex(bool state);
    void set_flag_byte_by_byte(bool state);

private slots:
    void log(const QString &data);
    void drawData(const QByteArray &data);
    void sendData(const QByteArray &sending_data);
    void procEnumerate(const QStringList &l);
    void btnOpenPortClicked();

    void procSerialDataReceive();

    void setBaudBox(const QString &text);
    void setDataBox(const QString &text);
    void setParityBox(const QString &text);
    void setStopBox(const QString &text);
    void setFlowBox(const QString &text);

    void getStatus(const QString &status, QDateTime current);

    void get_parameter(void);

    void timer_stop(void);

    void set_default(void);

#ifndef RS232_NO_FRAME
    void ring_changed(bool state);
    void dsr_changed(bool state);
    void cts_changed(bool state);
#endif

protected:
    void changeEvent(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif // SERIALBOX_HPP
