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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B588;
}
//--------------------------------------------------------------------------------
class Powersupply_B588;

class GrapherBox;

class QToolButton;
class QToolBar;
class QTimer;
//--------------------------------------------------------------------------------
class B588 : public MyWidget
{
    Q_OBJECT

public:
    B588(QWidget *parent);
    ~B588();

private slots:
    void test(void);

    void find_devices(void);

    int measuring(void);
    void auto_measuring(bool state);

    void rc_on(void);
    void rc_off(void);

private:
    Ui::B588 *ui = 0;

    bool is_blocked;

    Powersupply_B588 *powersupply = 0;

    QTimer *timer = 0;

    GrapherBox *grapher = 0;
    int curve_U_adc = 0;
    int curve_I_adc = 0;
    int curve_U3_adc = 0;

    void init(void);

    void createTestBar(void);

    void block_interface(bool state);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
