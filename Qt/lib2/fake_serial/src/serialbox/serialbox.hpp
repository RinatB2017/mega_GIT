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
// FAKE_SERIAL
//--------------------------------------------------------------------------------
#ifndef SERIALBOX_HPP
#define SERIALBOX_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QDateTime>
#include <QWidget>
#include <QString>
#include <QFrame>
//--------------------------------------------------------------------------------
enum Flow
{
    FlowControlUndefined = -1,  /*!< \~english Flow control undefined. */

    FlowControlOff,         /*!< \~english Flow control "Off". */
    FlowControlHardware,    /*!< \~english Flow control "Hardware". */
    FlowControlXonXoff,     /*!< \~english Flow control "Xon/Xoff". */
};
//--------------------------------------------------------------------------------
class FakeEnumerator;
class FakeSerial;

class LogBox;
class SendBox;
//--------------------------------------------------------------------------------
namespace Ui
{
    class SerialBox;
}
//--------------------------------------------------------------------------------
class SerialBox : public QFrame
{
    Q_OBJECT

public:
    explicit SerialBox(QWidget *parent, const QString &caption);
    explicit SerialBox(QWidget *parent = nullptr);
    ~SerialBox();

    bool isOpen(void);
    void updateText(void);

private:
    Ui::SerialBox *ui;
    QWidget *parent;
    FakeEnumerator *enumerator;
    FakeSerial *serial;
    QString caption;

    bool flag_in_hex;
    bool flag_byte_by_byte;

#ifdef RS232_LOG
    LogBox  *logBox;
#endif

#ifdef RS232_SEND
    SendBox *sendBox;
#endif

#ifndef RS232_NO_FRAME
    QFrame *frame_ring;
    QFrame *frame_dsr;
    QFrame *frame_cts;
    void add_frame_text(QFrame *parent,
                        const QString &text);
#endif

    void connect_log(void);
    void init();
    void createWidgets();
    void initEnumerator();
    void initSerial();
    void initBoxs();
    void setOpenState();
    void setCloseState();
    QString ByteArrayToHex(const QByteArray &data);

signals:
    void output(const QByteArray &data);

    void procCtsChanged(bool);
    void procDsrChanged(bool);
    void procRingChanged(bool);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

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
