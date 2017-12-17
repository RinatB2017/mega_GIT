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
#ifndef B588_HPP
#define B588_HPP
//--------------------------------------------------------------------------------
#include <inttypes.h>
#include <QList>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
struct Point3 {
    int number_point;
    unsigned int PWM;
    int set_voltage_mV;
    int get_voltage_mV;
    int result_mV;
    int max_deviation_mV;
};
//--------------------------------------------------------------------------------
#ifdef V764
    class Multimeter_V764;
#endif
#ifdef V7642
    class Multimeter_V764_2;
#endif
//--------------------------------------------------------------------------------
class Powersupply_B588;

class QSpinBox;
class QToolButton;
class QToolBar;

class QtVariantProperty;
class QStandardItemModel;
//--------------------------------------------------------------------------------
class B588 : public MyWidget
{
    Q_OBJECT

public:
    explicit B588(QWidget *parent = 0);
    ~B588();

private slots:
    void log(const QString &data);
    void test(void);

    void calibration_ADC_U(void);
    void calibration_DAC_U(void);
    void calibration_ADC_I(void);
    void calibration_DAC_I(void);

    bool process_calibration_ADC_U(void);
    bool process_calibration_DAC_U(void);
    bool process_calibration_ADC_I(void);
    bool process_calibration_DAC_I(void);

    void find_devices(void);

    void check_control_point_U(void);
    void check_control_point_I(void);

    void check_3_channel(void);
    void check_user_point(void);
    void save_data(void);

    void stop_calibration_ADC_U(void);
    void stop_calibration_DAC_U(void);
    void stop_calibration_ADC_I(void);
    void stop_calibration_DAC_I(void);

    void stop_check_U(void);
    void stop_check_I(void);

    void set_UI_parrot(int U, int I);
    void save_report(void);

private:
    Ui::B588 *ui = 0;

#ifdef V764
    Multimeter_V764 *multimeter = 0;
#endif
#ifdef V7642
    Multimeter_V764_2 *multimeter = 0;
#endif

    Powersupply_B588  *powersupply = 0;

    QList<Point3> list;

    bool flag_stop_calibration_ADC_U;
    bool flag_stop_calibration_DAC_U;
    bool flag_stop_calibration_ADC_I;
    bool flag_stop_calibration_DAC_I;

    bool flag_stop_check_U;
    bool flag_stop_check_I;

#ifndef USER_CALIBRATIONS
    //---
    QtVariantProperty *item_U_point_1 = 0;
    QtVariantProperty *item_U_max_deviation_point_1 = 0;
    QtVariantProperty *item_U_begin_DAC_point_1 = 0;

    QtVariantProperty *item_U_point_2 = 0;
    QtVariantProperty *item_U_max_deviation_point_2 = 0;
    QtVariantProperty *item_U_begin_DAC_point_2 = 0;

    QtVariantProperty *item_I_point_1 = 0;
    QtVariantProperty *item_I_max_deviation_point_1 = 0;
    QtVariantProperty *item_I_begin_DAC_point_1 = 0;

    QtVariantProperty *item_I_point_2 = 0;
    QtVariantProperty *item_I_max_deviation_point_2 = 0;
    QtVariantProperty *item_I_begin_DAC_point_2 = 0;

    QtVariantProperty *item_I_point_3 = 0;
    QtVariantProperty *item_I_max_deviation_point_3 = 0;
    QtVariantProperty *item_I_begin_DAC_point_3 = 0;
    //---
#endif

#ifdef FAKE
    void generate_fake_data(void);
#endif

    void createTestBar(void);
    void createParrotBar(void);
    void createPowerSupplyBar(void);
    void createPropertyBox(void);

    void init();
    void connect_log();
    void block_interface(bool state);

    bool measuring(int value_uV, int *measuring_value_uV);
    bool find_U(uint16_t value_mV,
                uint16_t max_delta_mV);
    bool find_I(uint16_t begin_parrot_I,
                uint16_t value_mA,
                uint16_t max_delta_uV);

    unsigned short points_U[61];
    unsigned short points_I[61];

    QStandardItemModel *model_U = 0;
    QStandardItemModel *model_I = 0;

    bool Calibration_remote_control_on(void);

    bool Calibration_ADC_U_begin(void);
    bool Calibration_ADC_U_find_point_1(void);
    bool Calibration_ADC_U_find_point_2(void);
    bool Calibration_DAC_U_begin(void);
    bool Calibration_DAC_U_show_process(void);
    bool Calibration_DAC_U_check_points(void);

    bool Calibration_press_enter(void);


    bool Calibration_ADC_I_begin(void);
    bool Calibration_ADC_I_find_point_1(void);
    bool Calibration_ADC_I_find_point_2(void);
    bool Calibration_ADC_I_find_point_3(void);
    bool Calibration_DAC_I_begin(void);
    bool Calibration_DAC_I_show_process(void);
    bool Calibration_DAC_I_check_points(void);

    bool wait_hot_temp(unsigned char begin_temp);
    bool wait_temp_30(unsigned char *current_temp);

    bool Calibration_remote_control_off(void);
};
//--------------------------------------------------------------------------------
#endif
