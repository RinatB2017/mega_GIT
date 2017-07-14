/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef CONTROLBOX_HPP
#define CONTROLBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFrame>
//--------------------------------------------------------------------------------
#include <QSerialPort>
//--------------------------------------------------------------------------------
#ifdef FAKE
    #include "testserialport.hpp"
#endif
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class ControlBox;
}
//--------------------------------------------------------------------------------
class GrapherBox;
class QRadioButton;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class GrapherBox;
class QLCDNumber;
class QGroupBox;
class QCheckBox;
class QSpinBox;
//--------------------------------------------------------------------------------
class ControlBox : public QFrame
{
    Q_OBJECT

public:
    explicit ControlBox(QWidget *parent = 0);
    ~ControlBox();

    void print_last_error(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void send_to_grapher(const QByteArray &);
    void send_to_serial(const QByteArray &);

public slots:
    bool cmd_TEST(void);
    bool cmd_INIT(void);
    bool cmd_GET_DATA(void);
    bool cmd_calibration_0(void);
    bool cmd_calibration_1(void);
    bool cmd_calibration_2(void);
    bool cmd_calibration_3(void);
    bool cmd_calibration_4(void);
    bool cmd_calibration_5(void);

    void reset(void);

private slots:
    bool find_device(void);

    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

    void update(void);

    void measure_start(void);
    void measure_stop(void);

    void timer_unlimited(bool state);

private:
    Ui::ControlBox *ui = 0;

#ifdef FAKE
    TestSerialPort serial;
#else
    QSerialPort serial;
#endif

    bool is_ready;
    QByteArray data_serial;

    GrapherBox *grapher = 0;
    QLCDNumber *display = 0;

    int value_index;

    int data_crc;
    int calc_crc;
    int last_error;

    double min_value;
    double max_value;
    double delta_value;
    double old_value;

    QTimer *timer = 0;

    QSpinBox *sbTimer = 0;
    QSpinBox *sbMaxTime = 0;
    QCheckBox *cbUnlimTimer = 0;
    QSpinBox *sb_timeout = 0;
    int cnt_max_time;

    QPushButton *btn_CMD_TEST = 0;
    QPushButton *btn_CMD_INIT = 0;
    QPushButton *btn_CMD_GET_DATA = 0;

    QPushButton *btn_CALIBRATION_0 = 0;
    QPushButton *btn_CALIBRATION_1 = 0;
    QPushButton *btn_CALIBRATION_2 = 0;
    QPushButton *btn_CALIBRATION_3 = 0;
    QPushButton *btn_CALIBRATION_4 = 0;
    QPushButton *btn_CALIBRATION_5 = 0;

    QPushButton *btnStart = 0;
    QPushButton *btnStop = 0;

    void init(void);
    void connect_log(void);

    QGroupBox *add_measure_group(void);
    QHBoxLayout *add_display_layout(void);

    void wait(int max_time_ms);
    void send_data_to_grapherBox(double value);

    int send_check_cmd(unsigned char cmd,
                       qint64 *value);

    void wait_msec(int timeout_msec);

    void draw_value(double value);
    void set_enabled(bool state);

    void create_timer(void);
    void create_serial_widgets(void);

    bool get_data(qint64 *data);

    bool check_stm(void);

    QByteArray convert_ascii_to_data(QString input, bool *ok);
    QByteArray convert_data_to_ascii(QByteArray data);

    int check_len_and_crc8(int len);
};
//--------------------------------------------------------------------------------
#endif // CONTROLBOX_HPP
