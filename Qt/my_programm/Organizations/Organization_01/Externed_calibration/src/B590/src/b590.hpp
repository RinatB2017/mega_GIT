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
#ifndef B590_HPP
#define B590_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class B590;
}
//--------------------------------------------------------------------------------
class Powersupply_B590;

class QToolButton;
class QToolBar;
class GrapherBox;

#ifdef USE_V764
    class Multimeter_V764;
#endif
#ifdef USE_V7642
    class Multimeter_V764_2;
#endif
//--------------------------------------------------------------------------------
class B590 : public MyWidget
{
    Q_OBJECT

public:
    B590(QWidget *parent = nullptr);
    ~B590();

    bool is_blocked_exit(void);

    int xxx(int x);

private slots:
    void init_multimeter(void);
    void find_devices(void);

    void test(void);
    void on(void);
    void off(void);

private:
    Ui::B590 *ui;

    Powersupply_B590  *powersupply;

#ifdef USE_V764
    Multimeter_V764 *multimeter;
#endif
#ifdef USE_V7642
    Multimeter_V764_2 *multimeter;
#endif

    GrapherBox *grapher;
    int index_progressbar = 0;

    int grapher_index = 0;

    int curve_B7642_U = 0;
    int curve_B7642_I = 0;

    int curve_U_parrot = 0;
    int curve_I_parrot = 0;

    void init(void);

    void createTestBar(void);

    void measuring_B590(void);
    void measuring_V7642(void);

    void wait_msec(int timeout_msec);

    void block_interface(bool state);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // B590_HPP
