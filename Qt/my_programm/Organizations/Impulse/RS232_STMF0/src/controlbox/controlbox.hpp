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

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class ControlBox;
}
//--------------------------------------------------------------------------------
enum {
    CMD_TEST		= 0,
    CMD_INIT		= 1,

    CMD_GET_DATA	= 2,
    CMD_GET_DIRTY_DATA	= 3,

    GMD_GET_GAIN		= 4,

    CMD_SET_GAIN_010	= 5,
    CMD_SET_GAIN_1		= 6,
    CMD_SET_GAIN_5		= 7,
    CMD_SET_GAIN_8		= 8,
    CMD_SET_GAIN_16		= 9,
    CMD_SET_GAIN_32		= 10,
    CMD_SET_GAIN_40		= 11,
    CMD_SET_GAIN_64		= 12,
    CMD_SET_GAIN_80		= 13,
    CMD_SET_GAIN_128	= 14,
    CMD_SET_GAIN_160	= 15,
    CMD_SET_GAIN_320	= 16,
    CMD_SET_GAIN_640	= 17,

    CMD_CALIBRATION_NULL	= 18,

    CMD_CALIBRATION_MAX_010	= 19,
    CMD_CALIBRATION_MAX_1	= 20,
    CMD_CALIBRATION_MAX_5	= 21,
    CMD_CALIBRATION_MAX_8	= 22,
    CMD_CALIBRATION_MAX_16	= 23,
    CMD_CALIBRATION_MAX_32	= 24,
    CMD_CALIBRATION_MAX_40	= 25,
    CMD_CALIBRATION_MAX_64	= 26,
    CMD_CALIBRATION_MAX_80	= 27,
    CMD_CALIBRATION_MAX_128	= 28,
    CMD_CALIBRATION_MAX_160	= 29,
    CMD_CALIBRATION_MAX_320	= 30,
    CMD_CALIBRATION_MAX_640	= 31
};
enum {
    CORRECTION_INDEX_010	= 0,
    CORRECTION_INDEX_1		= 1,
    CORRECTION_INDEX_5		= 2,
    CORRECTION_INDEX_8		= 3,
    CORRECTION_INDEX_16		= 4,
    CORRECTION_INDEX_32		= 5,
    CORRECTION_INDEX_40		= 6,
    CORRECTION_INDEX_64		= 7,
    CORRECTION_INDEX_80		= 8,
    CORRECTION_INDEX_128	= 9,
    CORRECTION_INDEX_160	= 10,
    CORRECTION_INDEX_320	= 11,
    CORRECTION_INDEX_640	= 12
};

#define CMD_OK      42
#define CMD_FAIL    13
//--------------------------------------------------------------------------------
class GrapherBox;
class QRadioButton;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class GrapherBox;
class SerialBox5;
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

    void test(void);

signals:
    void send_to_grapher(const QByteArray &);
    void send_to_serial(const QByteArray &);

private slots:
    void read_data_serialBox(const QByteArray &data);
    void read_data_calibratorBox(const QByteArray &data);

    void update(void);

    void measure_start(void);
    void measure_stop(void);

    void calibrate(void);

    void reset(void);

    bool cmd_TEST(void);
    bool cmd_INIT(void);
    bool cmd_GET_DATA(void);
    bool cmd_GET_DIRTY_DATA(void);
    int  cmd_GET_GAIN(bool *ok);

    bool cmd_SET_GAIN_010(void);
    bool cmd_SET_GAIN_1(void);
    bool cmd_SET_GAIN_5(void);
    bool cmd_SET_GAIN_8(void);
    bool cmd_SET_GAIN_16(void);
    bool cmd_SET_GAIN_32(void);
    bool cmd_SET_GAIN_40(void);
    bool cmd_SET_GAIN_64(void);
    bool cmd_SET_GAIN_80(void);
    bool cmd_SET_GAIN_128(void);
    bool cmd_SET_GAIN_160(void);
    bool cmd_SET_GAIN_320(void);
    bool cmd_SET_GAIN_640(void);

    bool cmd_calibration_null(void);
    bool cmd_calibration_010(void);
    bool cmd_calibration_1(void);
    bool cmd_calibration_5(void);
    bool cmd_calibration_8(void);
    bool cmd_calibration_16(void);
    bool cmd_calibration_32(void);
    bool cmd_calibration_40(void);
    bool cmd_calibration_64(void);
    bool cmd_calibration_80(void);
    bool cmd_calibration_128(void);
    bool cmd_calibration_160(void);
    bool cmd_calibration_320(void);
    bool cmd_calibration_640(void);

    void timer_unlimited(bool state);

    void set_gain(void);
    void get_gain(void);

private:
    Ui::ControlBox *ui = 0;
    QWidget *parent = 0;

    GrapherBox *grapher = 0;
    SerialBox5 *stm32 = 0;
    SerialBox5 *calibrator = 0;

    QLCDNumber *display = 0;

    bool data_stm32_is_ready;
    bool data_calibrator_is_ready;

    int value_index;

    double min_value;
    double max_value;
    double delta_value;

    double old_value;

    QByteArray data_stm32;
    QByteArray data_calibrator;
    QTimer *timer = 0;

    float gain;
    float correct_value;
    bool calibration_in_progress;

    QPushButton *btnStart = 0;
    QPushButton *btnStop = 0;
    QPushButton *btnCalibrate = 0;

    QSpinBox *sbTimer = 0;
    QSpinBox *sbMaxTime = 0;
    QCheckBox *cbUnlimTimer = 0;
    int cnt_max_time;

    QPushButton *btn_CMD_TEST = 0;
    QPushButton *btn_CMD_INIT = 0;
    QPushButton *btn_CMD_GET_DATA = 0;
    QPushButton *btn_CMD_GET_DIRTY_DATA = 0;

    QRadioButton *btn_gain_010 = 0;
    QRadioButton *btn_gain_1 = 0;
    QRadioButton *btn_gain_5 = 0;
    QRadioButton *btn_gain_8 = 0;
    QRadioButton *btn_gain_16 = 0;
    QRadioButton *btn_gain_32 = 0;
    QRadioButton *btn_gain_40 = 0;
    QRadioButton *btn_gain_64 = 0;
    QRadioButton *btn_gain_80 = 0;
    QRadioButton *btn_gain_128 = 0;
    QRadioButton *btn_gain_160 = 0;
    QRadioButton *btn_gain_320 = 0;
    QRadioButton *btn_gain_640 = 0;

    QPushButton *btn_CALIBRATION_NULL = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_010 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_1 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_5 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_8 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_16 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_32 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_40 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_64 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_80 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_128 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_160 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_320 = 0;
    QPushButton *btn_CALIBRATION_GAIN_MAX_640 = 0;

    void init(void);

    QFrame *add_other_widget(void);
    QGroupBox *add_measure_group(void);
    QGroupBox *add_calibration_widget(void);
    QHBoxLayout *add_display_layout(void);
    QHBoxLayout *add_gain_group(void);
    QGroupBox *add_mul_group(void);
    QGroupBox *add_div_group(void);

    void wait(int max_time_ms);
    void send_data_to_grapherBox(double dirty_value, double clean_value);

    bool send_check_cmd(unsigned char cmd,
                        int timeout_msec,
                        bool silence = false);
    void wait_msec(int timeout_msec, int count_break);

    void draw_value(double value);
    void set_enabled(bool state);

    void create_timer(void);
    void create_serial_widgets(void);

    double get_float_data(int timeout_msec, bool *ok);

    int get_gain(int timeout_msec, bool *ok);
    double get_data(int timeout_msec, bool *ok);
    double get_dirty_data(int timeout_msec, bool *ok);

    double get_average_value(const QString &calibration_string,
                             int max_timeout,
                             int max_err_count,
                             int max_count,
                             bool *result_ok);
    float get_data_value(void);
};
//--------------------------------------------------------------------------------
#endif // CONTROLBOX_HPP
