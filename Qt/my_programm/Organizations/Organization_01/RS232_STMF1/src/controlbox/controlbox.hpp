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

#include "mywidget.hpp"
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
class ControlBox : public MyWidget
{
    Q_OBJECT

public:
    ControlBox(QWidget *parent = 0);
    ~ControlBox();

    void print_last_error(void);

signals:
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
    Ui::ControlBox *ui;

#ifdef FAKE
    TestSerialPort serial;
#else
    QSerialPort serial;
#endif

    bool is_ready;
    QByteArray data_serial;

    GrapherBox *grapher;
    QLCDNumber *display;

    int value_index;

    int data_crc;
    int calc_crc;
    int last_error;

    double min_value;
    double max_value;
    double delta_value;
    double old_value;

    QTimer *timer;

    QSpinBox *sbTimer;
    QSpinBox *sbMaxTime;
    QCheckBox *cbUnlimTimer;
    QSpinBox *sb_timeout;
    int cnt_max_time = 0;

    QPushButton *btn_CMD_TEST;
    QPushButton *btn_CMD_INIT;
    QPushButton *btn_CMD_GET_DATA;

    QPushButton *btn_CALIBRATION_0;
    QPushButton *btn_CALIBRATION_1;
    QPushButton *btn_CALIBRATION_2;
    QPushButton *btn_CALIBRATION_3;
    QPushButton *btn_CALIBRATION_4;
    QPushButton *btn_CALIBRATION_5;

    QPushButton *btnStart;
    QPushButton *btnStop;

    void init(void);

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

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // CONTROLBOX_HPP
