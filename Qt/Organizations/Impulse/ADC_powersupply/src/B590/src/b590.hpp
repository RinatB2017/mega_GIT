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
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "B590_fram.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B590;
}
//--------------------------------------------------------------------------------
class GrapherBox;

class Powersupply_B590;

class QToolButton;
class QToolBar;
class QTimer;
//--------------------------------------------------------------------------------
class B590 : public MyWidget
{
    Q_OBJECT

public:
    explicit B590(QWidget *parent);
    ~B590();

private slots:
    void test(void);

    void find_devices(void);

    int measuring(void);
    void auto_measuring(bool state);

    void rc_on(void);
    void rc_off(void);

private:
    Ui::B590 *ui = 0;

    Powersupply_B590 *powersupply = 0;

    bool is_blocked;

    QTimer *timer = 0;

    s_zero_b590_U calib_U;
    s_zero_b590_I calib_I;

    GrapherBox *grapher = 0;
    int curve_U = 0;
    int curve_I = 0;
    int curve_U_adc = 0;
    int curve_I_adc = 0;
    int curve_U3_adc = 0;

    void init(void);
    void connect_log(void);

    void createTestBar(void);

    void block_interface(bool state);

    uint16_t convert_ADC_U_to_mV(unsigned long ADC_value);
    uint16_t convert_ADC_I_to_mA(unsigned long ADC_value);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif
