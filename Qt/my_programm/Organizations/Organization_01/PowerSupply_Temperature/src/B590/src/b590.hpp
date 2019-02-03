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
#ifndef B590_HPP
#define B590_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include "B590_fram.hpp"

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B590;
}
//--------------------------------------------------------------------------------
class QToolButton;
class QToolBar;
class QComboBox;
class QTimer;

class GrapherBox;
class Powersupply_B590;
//--------------------------------------------------------------------------------
class B590 : public MyWidget
{
    Q_OBJECT

public:
    B590(QWidget *parent);
    ~B590();

private slots:
    void find_device(void);
    void measuring(bool state);
    void update(void);
    void test(void);

    void vent_off(void);
    void vent_auto(void);

private:
    Ui::B590 *ui;

    bool is_blocked;
    QComboBox *cb_test;

    void createTestBar(void);

#ifndef NO_GRAPH
    GrapherBox *grapher;
    int curve_1 = 0;
    int curve_2 = 0;
    int curve_3 = 0;
    int curve_temp = 0;
#endif

    Powersupply_B590 *powersupply;

    QTimer *timer;

    s_zero_b590_U calib_U;
    s_zero_b590_I calib_I;

    void init(void);

    uint16_t convert_ADC_U_to_mV(int32_t ADC_value);
    uint16_t convert_ADC_I_to_mA(int32_t ADC_value);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif
