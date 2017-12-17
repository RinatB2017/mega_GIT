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
#include <QWidget>
#include <QFrame>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class B590;
}
//--------------------------------------------------------------------------------
class Powersupply_B590;
class QSpinBox;
class QTimer;

class GrapherBox;

class QStandardItemModel;

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
    void inc_U(void);
    void inc_I(void);
    void dec_U(void);
    void dec_I(void);

    bool search_power_supply(void);

    void get_UI(void);
    void set_UI(void);

    int show_temp(void);

    int send_0_0(void);

    void show_U(double value);
    void show_I(double value);

    void test(void);

    int  get_current_profile(void);
    void read_profile(int number_profile);
    void load_profile(void);
    void save_profile(void);
    void run_profile(void);

    void add_point(void);
    void del_point(void);

    void clear_profile(void);

private:
    Ui::B590 *ui = 0;

    Powersupply_B590  *powersupply = 0;
    QStandardItemModel *model_profiles = 0;

#ifdef USE_GRAPHER
    GrapherBox *grapher = 0;
    int curve_U = 0;
    int curve_I = 0;
#endif

    void init(void);

    void createWidgets(void);
    void createPowersupply(void);

    void createTestBar(void);
    void createParrotBar(void);

    void block_interface(bool state);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
