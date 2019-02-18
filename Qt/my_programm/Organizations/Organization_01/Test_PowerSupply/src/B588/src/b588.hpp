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
#include <QFrame>
//--------------------------------------------------------------------------------
#include "defines.hpp"

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
#define VALUE_IN_REAL   0
#define VALUE_IN_REAL_TEXT   tr("в мВ")

#define VALUE_IN_PARROT 1
#define VALUE_IN_PARROT_TEXT tr("в попугаях")
//--------------------------------------------------------------------------------
class Powersupply_B588;

class QToolButton;
class QToolBar;
class GrapherBox;

#ifdef USE_V764
    class Multimeter_V764;
#endif
#ifdef USE_V7642
    class Multimeter_V764_2;
#endif

class Multimeter_V786_2;
//--------------------------------------------------------------------------------
class B588 : public MyWidget
{
    Q_OBJECT

public:
    B588(QWidget *parent = nullptr);
    ~B588();

    bool is_blocked_exit(void);

private slots:
    void init_multimeter(void);

    void test_triangle_U(void);
    void test_triangle_I(void);
    void test_triangle_UI(void);
    void test_stability(void);

    void stop_test_triangle(void);
    void stop_test_stability(void);

    void find_devices(void);

    void correct_V7642_value(void);
    void correct_V7862_value(void);

    void active_V7642(bool state);
    void active_V7862(bool state);

    int test(void);

private:
    Ui::B588 *ui;

    Powersupply_B588  *powersupply;

#ifdef USE_V764
    Multimeter_V764 *multimeter;
#endif
#ifdef USE_V7642
    Multimeter_V764_2 *multimeter;
#endif

    Multimeter_V786_2 *multimeter_V7862;

    GrapherBox *grapher;
    int index_progressbar;

    int grapher_index;

    int curve_powersuply = 0;
    int curve_B7642 = 0;
    int curve_B7862 = 0;

    int curve_U_parrot = 0;
    int curve_I_parrot = 0;
    int curve_U_input_parrot = 0;

    bool flag_stop_test_triangle;
    bool flag_stop_test_stability;

    void init(void);

    void createTestBar(void);

    int test_triangle_b588_U(void);
    int test_stability_b588(void);
    int test_triangle_b588_I(void);
    int test_triangle_b588_UI(void);
    void measuring_B588(void);

    int min_to_max_b588_U(int min_U, int max_U, int step_U, int current);
    int min_to_max_b588_I(int min_I, int max_I, int step_I, int voltage);
    int max_to_min_b588_U(int min_U, int max_U, int step_U, int current);
    int max_to_min_b588_I(int min_I, int max_I, int step_I, int voltage);
    int min_to_max_b588_UI(int min_value, int max_value, int step);
    int max_to_min_b588_UI(int min_value, int max_value, int step);

    void measuring_V7642(void);
    void measuring_V7862(void);

    bool test_in_parrot(void);
    bool stability_in_parrot(void);

    void wait_msec(int timeout_msec);

    void block_interface(bool state);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // B588_HPP
