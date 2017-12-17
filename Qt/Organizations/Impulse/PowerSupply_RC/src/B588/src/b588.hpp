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
#ifndef B588_HPP
#define B588_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//#include <stdint.h>

#include "b588_powersupply.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
class Powersupply_B588;
class GrapherBox;
class QSpinBox;
class QTimer;

class MySplashScreen;
class QToolButton;
class QToolBar;
class QTimer;

class QwtChartZoom;
class QAxisZoomSvc;

class HexSpinBox;
//--------------------------------------------------------------------------------
class B588 : public MyWidget
{
    Q_OBJECT

public:
    B588(QWidget *parent);
    ~B588();

private slots:
    void inc_U(void);
    void inc_I(void);
    void dec_U(void);
    void dec_I(void);

    bool search_power_supply(void);
    void get_UI(void);

    void timer_start(void);
    void timer_stop(void);

    void find_max_power_start(void);
    void find_max_power_stop(void);

    void test_start(void);
    void test_stop(void);

    void set_UI(void);

    bool rc_on(void);
    bool rc_off(void);

    void find_max_power(void);
    void extreme(void);

    void read_calibration_point_B588(void);

    int test_U(void);
    int test_I(void);

    void read(void);

    int show_temp(void);

    void corridor(bool state);
    int corridor_on(void);
    int corridor_off(void);

    void ignore_bad_cmd(void);

    void change_type_powersupply(int);
    void change_state_powersupply(int);

    int send_0_0(void);

    int set_vent_speed(void);
    int set_vent_speed_0(void);
    int set_vent_speed_max(void);
    int set_vent_speed_auto(void);

    int set_UI_parrot(int U, int I);

    void set_manual_U(double value);
    void set_manual_I(double value);
    void set_manual_U(int value);
    void set_manual_I(int value);

private:
    MySplashScreen *splash = 0;
    Ui::B588 *ui = 0;

    Powersupply_B588  *powersupply = 0;
    GrapherBox *grapher = 0;
    QTimer *timer = 0;

    //HexSpinBox *sb_begin = 0;
    //HexSpinBox *sb_end = 0;

    QSpinBox *sb_begin = 0;
    QSpinBox *sb_end = 0;

    bool flag_find_stop;
    bool flag_test_stop;

    unsigned int curve_U = 0;
    unsigned int curve_I = 0;
    unsigned int curve_R = 0;
    unsigned int curve_W = 0;
    unsigned int curve_T = 0;

    void init(void);
    void connect_log(void);

    void createGrapherBox(void);
    void createTimer(void);
    void createWidgets(void);
    void createPowersupply(void);

    void createTestBar(void);
    void createParrotBar(void);

    void block_interface(bool state);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif
