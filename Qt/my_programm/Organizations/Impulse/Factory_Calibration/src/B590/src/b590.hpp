/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef B590_HPP
#define B590_HPP
//--------------------------------------------------------------------------------
//#include <inttypes.h>
#include <QList>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "b590_powersupply.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B590;
}
//--------------------------------------------------------------------------------
struct DATA_U {
    uint16_t value_mV;
    double max_delta_mV;
    uint16_t begin_DAC_U;
    uint16_t end_DAC_U;
    int counter;
    uint16_t result_dac_value;
    int result_value_mV;
};
struct DATA_I {
    uint16_t value_mA;
    double max_delta_mA;
    uint16_t begin_DAC_I;
    uint16_t end_DAC_I;
    int counter;
    uint16_t result_dac_value;
    int result_value_mA;
};
//--------------------------------------------------------------------------------
#ifdef V7642
    class Multimeter_V764_2;
#endif
#ifdef V764
    class Multimeter_V764;
#endif
#ifdef V780
    class Multimeter_V780;
#endif
//--------------------------------------------------------------------------------
class Powersupply_B590;

class QPushButton;
class QToolButton;
class QToolBar;
class QComboBox;

class QtVariantProperty;
class QStandardItemModel;
//--------------------------------------------------------------------------------
class B590 : public MyWidget
{
    Q_OBJECT

public:
    B590(QWidget *parent = 0);
    ~B590();

private slots:
    void clear(void);
    void test(void);

    void show_tables(void);

    void new_calibration_U(void);

    void choice_test(void);

    void find_1V(void);
    void find_59V(void);

    void find_50mA(void);
    void find_5A(void);
    void find_25A(void);
    void find_48A(void);

    void find_devices(void);

    void calibration_ADC_U(void);
    void calibration_DAC_U(void);

    void calibration_ADC_I(void);
    void calibration_DAC_I(void);

    bool process_calibration_ADC_U(void);
    bool process_calibration_DAC_U(void);

    bool process_calibration_ADC_I(void);
    bool process_calibration_DAC_I(void);

    void check_control_point_U(void);
    void check_control_point_I(void);

    void check_user_point(void);
    void save_data(void);

    void stop_calibration_ADC_U(void);
    void stop_calibration_DAC_U(void);
    void stop_calibration_ADC_I(void);
    void stop_calibration_DAC_I(void);

    void stop_check_U(void);
    void stop_check_I(void);

    void set_UI_parrot(int U, int I);

    void check(void);
    void save_report(void);

    void save_setting(void);

    void set_max_U(void);
    void set_max_I(void);
    void set_clear_UI(void);

    void save_table_U(void);
    void save_table_I(void);

private:
    Ui::B590 *ui = 0;

#ifdef V764
    Multimeter_V764 *multimeter = 0;
#endif
#ifdef V7642
    Multimeter_V764_2 *multimeter = 0;
#endif
#ifdef V780
    Multimeter_V780 *multimeter = 0;
#endif

    Powersupply_B590  *powersupply = 0;

    QComboBox *cb_test = 0;

    bool flag_stop_calibration_ADC_U;
    bool flag_stop_calibration_DAC_U;
    bool flag_stop_calibration_ADC_I;
    bool flag_stop_calibration_DAC_I;
    bool flag_stop_check_U;
    bool flag_stop_check_I;

#ifndef USER_CALIBRATIONS
    //---
    QtVariantProperty *item_U_point_0 = 0;
    QtVariantProperty *item_U_max_deviation_point_0 = 0;
    QtVariantProperty *item_U_begin_DAC_point_0 = 0;

    QtVariantProperty *item_U_point_1 = 0;
    QtVariantProperty *item_U_max_deviation_point_1 = 0;
    QtVariantProperty *item_U_begin_DAC_point_1 = 0;

    QtVariantProperty *item_U_point_2 = 0;
    QtVariantProperty *item_U_max_deviation_point_2 = 0;
    QtVariantProperty *item_U_begin_DAC_point_2 = 0;

    QtVariantProperty *item_U_point_3 = 0;
    QtVariantProperty *item_U_max_deviation_point_3 = 0;
    QtVariantProperty *item_U_begin_DAC_point_3 = 0;

    //---
    QtVariantProperty *item_I_point_0 = 0;
    QtVariantProperty *item_I_max_deviation_point_0 = 0;
    QtVariantProperty *item_I_begin_DAC_point_0 = 0;
    QtVariantProperty *item_I_end_DAC_point_0 = 0;

    QtVariantProperty *item_I_point_1 = 0;
    QtVariantProperty *item_I_max_deviation_point_1 = 0;
    QtVariantProperty *item_I_begin_DAC_point_1 = 0;

    QtVariantProperty *item_I_point_2 = 0;
    QtVariantProperty *item_I_max_deviation_point_2 = 0;
    QtVariantProperty *item_I_begin_DAC_point_2 = 0;

    QtVariantProperty *item_I_point_3 = 0;
    QtVariantProperty *item_I_max_deviation_point_3 = 0;
    QtVariantProperty *item_I_begin_DAC_point_3 = 0;

    QtVariantProperty *item_I_point_4 = 0;
    QtVariantProperty *item_I_max_deviation_point_4 = 0;
    QtVariantProperty *item_I_begin_DAC_point_4 = 0;

    QtVariantProperty *item_I_point_5 = 0;
    QtVariantProperty *item_I_max_deviation_point_5 = 0;
    QtVariantProperty *item_I_begin_DAC_point_5 = 0;

    QtVariantProperty *item_I_point_6 = 0;
    QtVariantProperty *item_I_max_deviation_point_6 = 0;
    QtVariantProperty *item_I_begin_DAC_point_6 = 0;

    QtVariantProperty *item_I_point_7 = 0;
    QtVariantProperty *item_I_max_deviation_point_7 = 0;
    QtVariantProperty *item_I_begin_DAC_point_7 = 0;

    QtVariantProperty *item_I_point_8 = 0;
    QtVariantProperty *item_I_max_deviation_point_8 = 0;
    QtVariantProperty *item_I_begin_DAC_point_8 = 0;

    QtVariantProperty *item_I_point_9 = 0;
    QtVariantProperty *item_I_max_deviation_point_9 = 0;
    QtVariantProperty *item_I_begin_DAC_point_9 = 0;

    QtVariantProperty *item_I_point_10 = 0;
    QtVariantProperty *item_I_max_deviation_point_10 = 0;
    QtVariantProperty *item_I_begin_DAC_point_10 = 0;

    QtVariantProperty *item_I_point_11 = 0;
    QtVariantProperty *item_I_max_deviation_point_11 = 0;
    QtVariantProperty *item_I_begin_DAC_point_11 = 0;

    QtVariantProperty *item_I_point_12 = 0;
    QtVariantProperty *item_I_max_deviation_point_12 = 0;
    QtVariantProperty *item_I_begin_DAC_point_12 = 0;
#endif

#ifdef FAKE
    void generate_fake_data(void);
#endif

    //---
    bool get_value_setting(const QString  &name,
                           QVariant *value,
                           QVariant *max_deviation,
                           QVariant *begin_DAC,
                           QVariant *end_DAC);
    bool create_setting(const QString  &name,
                        const QVariant &value,
                        const QVariant &max_deviation,
                        const QVariant &begin_DAC,
                        const QVariant &end_DAC);
    //---

    void createTestBar(void);
    void createParrotBar(void);
    void createPowerSupplyBar(void);
    void createPropertyBox(void);
    void createInfoBar(void);

    void init();
    void block_interface(bool state);

    void load_setting(void);

    bool measuring_U(int value_mV,
                     double max_delta_mV,
                     int *measuring_value_mV);
    bool measuring_I(int value_mA,
                     double R_om,
                     double max_delta_mA,
                     int *measuring_value_mA);
    bool find_U(DATA_U *data);
    bool find_I(DATA_I *data);

    unsigned short points_U[MAX_CALIBRATION_POINTS_B590_U];
    unsigned short points_I[MAX_CALIBRATION_POINTS_B590_I];

    QStandardItemModel *model_U = 0;
    QStandardItemModel *model_I = 0;

    QPushButton *btn_set_max_U = 0;
    QPushButton *btn_set_max_I = 0;
    QPushButton *btn_clear_UI = 0;

    bool Calibration_remote_control_on(void);

    bool Calibration_ADC_U_begin(void);
    bool Calibration_ADC_U_find_point_0(void);
    bool Calibration_ADC_U_find_point_1(void);
    bool Calibration_ADC_U_find_point_2(void);
    bool Calibration_ADC_U_find_point_3(void);
    bool Calibration_DAC_U_begin(void);
    bool Calibration_DAC_U_show_process(void);
    bool Calibration_DAC_U_check_points(void);

    bool check_point_U(int value_mV,
                       int *result_uV);
    bool check_point_I(int value_mA,
                       int *result_uV);

    bool Calibration_press_enter(void);

    bool Calibration_ADC_I_begin(void);
#if 1
    bool Calibration_ADC_I_find_zero_point(void);
#else
    bool Calibration_ADC_I_find_point_0(void);
#endif
    bool Calibration_ADC_I_find_point_1(void);
    bool Calibration_ADC_I_find_point_2(void);
    bool Calibration_ADC_I_find_point_3(void);
    bool Calibration_ADC_I_find_point_4(void);
    bool Calibration_ADC_I_find_point_5(void);
    bool Calibration_ADC_I_find_point_6(void);
    bool Calibration_ADC_I_find_point_7(void);
    bool Calibration_ADC_I_find_point_8(void);
    bool Calibration_ADC_I_find_point_9(void);
    bool Calibration_ADC_I_find_point_10(void);
    bool Calibration_ADC_I_find_point_11(void);
    bool Calibration_ADC_I_find_point_11_HOT(void);
    bool Calibration_ADC_I_find_point_12(void);
    bool Calibration_DAC_I_begin(void);
    bool Calibration_DAC_I_show_process(void);
    bool Calibration_DAC_I_check_points(void);

    bool wait_hot_temp(unsigned char begin_temp);
    bool wait_temp_30(void);

    bool Calibration_remote_control_off(void);

    bool measuring_V764(int *value_uV);
    bool measuring_V7642(int *value_uV);
    bool measuring_V780(int *value_uV);
    bool measuring(int *value_uV);

    double convert_uA_to_uV(double value_uA, double R_om);
    double convert_uV_to_uA(double value_uV, double R_om);
};
//--------------------------------------------------------------------------------
#endif
