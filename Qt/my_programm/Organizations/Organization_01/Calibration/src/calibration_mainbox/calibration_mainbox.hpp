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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QVector>
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
//--------------------------------------------------------------------------------
enum
{
    CABLE_IN = 0,
    CABLE_A,
    CABLE_mA
};
enum
{
    H4_6 = 6,
    H4_7 = 7
};

#define H4_6_KEY_0    "Y8"
#define H4_6_KEY_1    "X0"
#define H4_6_KEY_2    "X1"
#define H4_6_KEY_3    "X2"
#define H4_6_KEY_4    "X3"
#define H4_6_KEY_5    "X4"
#define H4_6_KEY_6    "X5"
#define H4_6_KEY_7    "X6"
#define H4_6_KEY_8    "X7"
#define H4_6_KEY_9    "X8"
#define H4_6_KEY_R    "X0"
#define H4_6_KEY_V    "X1"
#define H4_6_KEY_I    "X2"
#define H4_6_KEY_LEFT     "Y7"
#define H4_6_KEY_RIGHT    "Y6"
#define H4_6_KEY_ENTER_VI "Z7"
#define H4_6_KEY_ENTER_F  "Z6"
#define H4_6_KEY_OFF      "X8"
#define H4_6_KEY_Hz       "X4"
#define H4_6_KEY_kHz      "X3"
#define H4_6_KEY_PLUS_MINUS "Y0"

#define H4_7_KEY_0    "Z7"
#define H4_7_KEY_1    "X0"
#define H4_7_KEY_2    "X1"
#define H4_7_KEY_3    "X2"
#define H4_7_KEY_4    "X3"
#define H4_7_KEY_5    "X4"
#define H4_7_KEY_6    "X5"
#define H4_7_KEY_7    "X6"
#define H4_7_KEY_8    "X7"
#define H4_7_KEY_9    "Y7"
#define H4_7_KEY_R    "X4"
#define H4_7_KEY_V    "X2"
#define H4_7_KEY_I    "X3"
#define H4_7_KEY_LEFT     "Y1"
#define H4_7_KEY_RIGHT    "Y0"
#define H4_7_KEY_DOT      "Y6"
#define H4_7_KEY_ENTER_VI "Y2"
#define H4_7_KEY_ENTER_F  "Y3"
#define H4_7_KEY_OFF      "Z7"
#define H4_7_KEY_Hz       "X1"
#define H4_7_KEY_kHz      "X0"
#define H4_7_KEY_PLUS_MINUS "Y5"
//--------------------------------------------------------------------------------
struct CURRENT_STATE
{
    int cable_in;
    bool need_amplifier_U;
    bool need_amplifier_I_DC;
    bool need_amplifier_I_AC;
};
//--------------------------------------------------------------------------------
struct questions_data
{
    QString calibrator_question_before;
    QString calibrator_question;
    QString limit_name;

    int cmd;

    int cable_in;
    int calibrator_type;
    bool calibrator_step_by_step; // поклавишно на калибратор
    bool need_amplifier_U;
    bool need_amplifier_I_AC;
    bool need_amplifier_I_DC;
    int step_a;
    int step_b;
    int step_c;

    bool last_point;
};
//--------------------------------------------------------------------------------
struct log_data
{
    QString step;
    int time;
};
//--------------------------------------------------------------------------------
namespace Ui {
class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class MySplashScreen;
class QTreeWidgetItem;
class QLineEdit;
class QPushButton;
class QToolButton;
class QComboBox;
class QToolBar;
class QSpinBox;

class Calibrator;
class QSerialPort;
class Multimeter_V786_2;

class ControlLog;
class MyReport;

class Frame_8bit;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    enum
    {
        E_NO_ERROR = 0x1000,
        E_ERROR_MULTIMETR,
        E_ERROR_INDEX_NOT_CORRECT,
        E_ERROR_PORT_NOT_OPEN,
        E_ERROR_BREAK,
        E_ERROR_MAX_REPEAT,
        E_ERROR_NO_ANSWER,
        E_ERROR_BIG_ANSWER,
        E_ERROR_SMALL_ANSWER,
        E_ERROR_SMALL_PACKET,
        E_ERROR_BAD_SERNO,
        E_ERROR_BAD_ADDRESS,
        E_ERROR_BAD_CRC,
        E_ERROR_BAD_CMD,
        E_ERROR_BAD_LEN,
        E_ERROR_UNKNOWN_CMD,
        E_ERROR_SIZE_NOT_CORRECT,
        E_UNKNOWN_ERROR,
        E_INVALID_FLOAT,
        E_INVALID_LIMIT,
        E_INVALID_DELTA,
        E_BREAK_MEASURING,
    };
    enum {
        TEST_CABLE_OFF = 0,
        TEST_CABLE_IN,
        TEST_CABLE_IN_UDC,
        TEST_CABLE_IN_UAC,
        TEST_CABLE_mA,
        TEST_CABLE_mA_IDC,
        TEST_CABLE_mA_IAC,
        TEST_CABLE_A,
        TEST_CABLE_A_IDC,
        TEST_CABLE_A_IAC
    };

    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void item_clicked(QTreeWidgetItem *item, int column);

    void set_calibration_point_color(const QString &name,
                                     const QBrush foreground,
                                     const QBrush background);
    void clr_calibration_point_color(const QString &name);
    void clr_calibration_point_all(void);

    void start_calibration(void);

    QString get_port(const QString &busy_port, bool *ok);
    bool find_one_device(void);
    bool find_two_device(void);
    bool search_devices(void);

    bool check_calibrator(void);
    void check_cable(int index, bool send_UD0);

    void set_limits_all_on(void);
    void set_limits_all_off(void);

    void set_controls_all_on(void);
    void set_controls_all_off(void);

    void test_connect_v786(void);
    void test_connect_calibrator(void);
    void test(void);

    void test_measuring_3500(bool state);
    void test_calibration_3500(bool state);

    void check_multemeter(bool state);

    void type_selected(int index);

#ifdef CALIBRATION_WIRED
    void wired_off(void);
#endif

    bool choice_calibration(void);

    void report(void);
    void clear_report_database(void);
    void update_text_type_calibrator(void);

#ifdef FAKE
    void fake(void);
#endif

private:
    Ui::MainBox *ui;

    ControlLog *control_log;

    int type_calibrator;

    QPushButton *btn_find_devices;

    MySplashScreen *splash;

    CURRENT_STATE current_state;

    bool flag_is_measuring;

    QTreeWidgetItem *item_UDC;
    QTreeWidgetItem *item_UAC;
    QTreeWidgetItem *item_IDC;
    QTreeWidgetItem *item_IAC;
#ifdef R_CALIBRATION_ENABLED
    QTreeWidgetItem *item_R;
#endif

    QTreeWidgetItem *item_limit_UDC_200mV;
    QTreeWidgetItem *item_limit_UDC_2V;
    QTreeWidgetItem *item_limit_UDC_20V;
    QTreeWidgetItem *item_limit_UDC_200V;
    QTreeWidgetItem *item_limit_UDC_1000V;

    QTreeWidgetItem *item_limit_UAC_200mV;
    QTreeWidgetItem *item_limit_UAC_2V;
    QTreeWidgetItem *item_limit_UAC_20V;
    QTreeWidgetItem *item_limit_UAC_200V;
    QTreeWidgetItem *item_limit_UAC_1000V;

    QTreeWidgetItem *item_limit_IDC_200uA;
    QTreeWidgetItem *item_limit_IDC_2mA;
    QTreeWidgetItem *item_limit_IDC_20mA;
    QTreeWidgetItem *item_limit_IDC_200mA;
    QTreeWidgetItem *item_limit_IDC_2000mA;
    QTreeWidgetItem *item_limit_IDC_20A;

    QTreeWidgetItem *item_limit_IAC_2mA;
    QTreeWidgetItem *item_limit_IAC_20mA;
    QTreeWidgetItem *item_limit_IAC_200mA;
    QTreeWidgetItem *item_limit_IAC_2000mA;
    QTreeWidgetItem *item_limit_IAC_20A;

#ifdef R_CALIBRATION_ENABLED
    QTreeWidgetItem *item_limit_R_200;
    QTreeWidgetItem *item_limit_R_2K;
    QTreeWidgetItem *item_limit_R_20K;
    QTreeWidgetItem *item_limit_R_200K;
    QTreeWidgetItem *item_limit_R_2M;
#endif

#ifdef CALIBRATOR_DEBUG
    QComboBox *cb_Cmd;
    QSpinBox *sb_data_len;
    Frame_8bit *frame_8bit;
    QSpinBox *sb_data_1;

    QLineEdit *le_calibrator;
#endif

    Calibrator *calibrator;
    Multimeter_V786_2 *multimeter;

    QVector <questions_data> answers;
    bool is_worked = false;

    QSpinBox *sb_time_calibration;
    QSpinBox *sb_repeat;
    QSpinBox *sb_delay;

    QVector<log_data> time_log;

    void set_item_param(QTreeWidgetItem *item,
                        Qt::ItemFlags flags,
                        const QString &caption,
                        Qt::CheckState state);

    void init(void);
    void init_tree_widgets(void);
    void init_serialport_widgets(void);

    bool calibration(void);

    void createOptionsBar(void);

#ifdef CALIBRATOR_DEBUG
    void createTestBar(void);
#endif

#ifdef FAKE
    void createFakeBar(void);
#endif

    float return_freq(unsigned int step);

#ifdef NEW_POINTS
    //добавлено 22.09.14
    float return_freq_new_points(unsigned int step);
#endif

    QString return_freq_string_H4_6(float freq, bool append_key_Hz);
    QString return_freq_string_H4_7(float freq);

    void prepare_steps(void);

    QString prepare_string(const QString &prefix,
                           float value,
                           float limit,
                           int step);

#ifdef NEW_POINTS
    //добавлено 22.09.14
    QString prepare_string_new_points(const QString &prefix,
                                      float value,
                                      float limit,
                                      int step);
#endif

    QString prepare_string_adv(const QString &prefix,
                               float value,
                               float limit,
                               int step);

    //---
    struct STEP {
        int step_a;
        int step_b;
        int step_c;
    };
    STEP old_step;
    STEP new_step;
    //---

    void prepare_step_0_0(void);
    void prepare_step_0_1(void);
    void prepare_step_0_2(void);
    void prepare_step_0_3(void);
    void prepare_step_0_4(void);

    void prepare_step_1_0(void);
    void prepare_step_1_1(void);
    void prepare_step_1_2(void);
    void prepare_step_1_3(void);
    void prepare_step_1_4(void);

    void prepare_step_2_0(void);
    void prepare_step_2_1(void);
    void prepare_step_2_2(void);
    void prepare_step_2_3(void);
    void prepare_step_2_4(void);
    void prepare_step_2_5(void);

    void prepare_step_3_0(void);
    void prepare_step_3_1(void);
    void prepare_step_3_2(void);
    void prepare_step_3_3(void);
    void prepare_step_3_4(void);

    void prepare_step_4_0(void);
    void prepare_step_4_1(void);
    void prepare_step_4_2(void);
    void prepare_step_4_3(void);
    void prepare_step_4_4(void);

    int work(int index, int max_repeat);

    void msgInfo(const QString &message);

    void controls_enabled(bool state);

    void spike_nail_1000V(void);
    void spike_nail_minus_1000V(void);

    //---
    int check_DC_V(float  value_V,
                   float max_delta_mV,
                   unsigned char limit,
                   float *result_delta_mV,
                   bool is_auto_check);
    int check_DC_mV(float value_mV,
                    float max_delta_mV,
                    unsigned char limit,
                    float *result_delta_mV,
                    bool is_auto_check);
    //---
    int check_DC_A(float value_A,
                   float max_delta_A,
                   unsigned char limit,
                   float *result_delta_A,
                   bool is_auto_check);
    int check_DC_mA(float value_mA,
                    float max_delta_mA,
                    unsigned char limit,
                    float *result_delta_mA,
                    bool is_auto_check);
    int check_DC_mkA(float value_mkA,
                     float max_delta_mkA,
                     unsigned char limit,
                     float *result_delta_mkA,
                     bool is_auto_check);
    //---
    int check_AC_V(float value_V,
                   float max_delta_mV,
                   float freq_kHz,
                   unsigned char limit,
                   float *result_delta_mV,
                   bool is_auto_check);
    int check_AC_mV(float value_mV,
                    float max_delta_mV,
                    float freq_kHz,
                    unsigned char limit,
                    float *result_delta_mV,
                    bool is_auto_check);
    //---
    int check_AC_A(float value_A,
                   float max_delta_mA,
                   float freq_kHz,
                   unsigned char limit,
                   float *result_delta_mA,
                   bool is_auto_check);
    int check_AC_mA(float value_mA,
                    float max_delta_mA,
                    float freq_kHz,
                    unsigned char limit,
                    float *result_delta_mA,
                    bool is_auto_check);
    //---
    int check_other_measuring(const QString &prefix,
                              unsigned int cmd,
                              double measuring_value, const
                              QString &measure,
                              float max_delta,
                              unsigned char limit,
                              float *result_delta,
                              bool is_auto_check);
    //---
    int cnt_error;
    int cnt_test;
    int cable_in;

    int control_UDC_10mV(MyReport *report);
    int control_UDC_minus_10mV(MyReport *report);
    int control_UDC_199mV(MyReport *report);
    int control_UDC_minus_199mV(MyReport *report);
    int control_UDC_0_5V(MyReport *report);
    int control_UDC_minus_0_5V(MyReport *report);
    int control_UDC_1V(MyReport *report);
    int control_UDC_1_5V(MyReport *report);
    int control_UDC_1_99V(MyReport *report);
    int control_UDC_minus_1_99V(MyReport *report);
    int control_UDC_19_9V(MyReport *report);
    int control_UDC_199V(MyReport *report);
    int control_UDC_1000V(MyReport *report);
    int control_UDC_minus_1000V(MyReport *report);

    int control_UAC_19_9mV_20kHz(MyReport *report);
    int control_UAC_190mV_1kHz(MyReport *report);
    int control_UAC_190mV_50kHz(MyReport *report);
    int control_UAC_190mV_100kHz(MyReport *report);
    int control_UAC_0_5V_1kHz(MyReport *report);
    int control_UAC_1V_20Hz(MyReport *report);
    int control_UAC_1V_1kHz(MyReport *report);
    int control_UAC_1V_20kHz(MyReport *report);
    int control_UAC_1V_50kHz(MyReport *report);
    int control_UAC_1V_100kHz(MyReport *report);
    int control_UAC_1_99V_1kHz(MyReport *report);
    int control_UAC_10V_1kHz(MyReport *report);
    int control_UAC_10V_20kHz(MyReport *report);
    int control_UAC_10V_50kHz(MyReport *report);
    int control_UAC_10V_100kHz(MyReport *report);
    int control_UAC_100V_1kHz(MyReport *report);
    int control_UAC_100V_20kHz(MyReport *report);
    int control_UAC_100V_100kHz(MyReport *report);

    //нужен усилитель
    int control_UAC_199V_1kHz(MyReport *report);
    int control_UAC_400V_1kHz(MyReport *report);

    int control_IDC_0_5mkA(MyReport *report);
    int control_IDC_100mkA(MyReport *report);
    int control_IDC_minus_100mkA(MyReport *report);
    int control_IDC_1mA(MyReport *report);
    int control_IDC_minus_1mA(MyReport *report);
    int control_IDC_10mA(MyReport *report);
    int control_IDC_minus_10mA(MyReport *report);
    int control_IDC_100mA(MyReport *report);
    int control_IDC_1000mA(MyReport *report);
    int control_IDC_minus_1000mA(MyReport *report);
    int control_IDC_19A(MyReport *report);
    int control_IDC_minus_19A(MyReport *report);

    int control_IAC_1mA_1kHz(MyReport *report);
    int control_IAC_10mA_1kHz(MyReport *report);
    int control_IAC_100mA_1kHz(MyReport *report);
    int control_IAC_1000mA_1kHz(MyReport *report);
    int control_IAC_10A_1kHz(MyReport *report);

    int control_R_0oM(MyReport *report);
    int control_R_10oM(MyReport *report);
    int control_R_100oM(MyReport *report);
    int control_R_190oM(MyReport *report);
    int control_R_1koM(MyReport *report);
    int control_R_1_9koM(MyReport *report);
    int control_R_10koM(MyReport *report);
    int control_R_100koM(MyReport *report);
    int control_R_1MoM(MyReport *report);

    int control_C_0pF(MyReport *report);
    int control_C_100pF(MyReport *report);
    int control_C_1000pF(MyReport *report);
    int control_C_10000pF(MyReport *report);
    int control_C_0_1mkF(MyReport *report);
    int control_C_1mkF(MyReport *report);
    int control_C_10mkF(MyReport *report);
    int control_C_1000mkF(MyReport *report);

    int control_L_0mH(MyReport *report);
    int control_L_1mH(MyReport *report);
    int control_L_10mH(MyReport *report);
    int control_L_100mH(MyReport *report);

    //повышенное напряжение
    int control_hi_UDC_1V(MyReport *report);
    int control_hi_UDC_10V(MyReport *report);
    int control_hi_UAC_1V_1kHz(MyReport *report);
    int control_hi_UAC_10V_1kHz(MyReport *report);

    int control_hi_R_1koM(MyReport *report);
    int control_hi_R_100koM(MyReport *report);

    int control_hi_IDC_10A(MyReport *report);
    int control_hi_FREQ_1MHz(MyReport *report);
    int control_hi_C_1mkF(MyReport *report);
    int control_hi_L_10mH(MyReport *report);

    //пониженное напряжение
    int control_lo_UDC_1V(MyReport *report);
    int control_lo_UDC_10V(MyReport *report);
    int control_lo_UAC_1V_1kHz(MyReport *report);
    int control_lo_UAC_10V_1kHz(MyReport *report);

    int control_lo_R_1koM(MyReport *report);
    int control_lo_R_100koM(MyReport *report);

    int control_lo_IDC_10A(MyReport *report);
    int control_lo_FREQ_1MHz(MyReport *report);
    int control_lo_C_1mkF(MyReport *report);
    int control_lo_L_10mH(MyReport *report);

    int check_treeWidget_controls_U(MyReport *report);
    int check_treeWidget_controls_I(MyReport *report);
    int check_treeWidget_controls_Other(MyReport *report);
    int check_treeWidget_controls_Hi_Low_U(MyReport *report);

    int message_question(QWidget *parent,
                         const QString &title,
                         const QString& text);
    int message_information(QWidget *parent,
                            const QString &title,
                            const QString& text);
    int message_critical(QWidget *parent,
                         const QString &title,
                         const QString& text);

    void updateText(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
