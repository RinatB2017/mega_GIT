/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef TEST_CLASS_HPP
#define TEST_CLASS_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Test_class;
}
//--------------------------------------------------------------------------------
class Test_class : public MyWidget
{
    Q_OBJECT

public:
    explicit Test_class(QWidget *parent = nullptr);
    ~Test_class();

private:
    typedef struct
    {
        bool (Test_class::*check_box)();
        void (Test_class::*func)();
    } CMD;
    QList<CMD> commands;

    Ui::Test_class *ui;

    void init();
    void test();

    bool get_0();
    bool get_1();
    bool get_2();
    bool get_3();
    bool get_4();

    void f_0();
    void f_1();
    void f_2();
    void f_3();
    void f_4();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // TEST_CLASS_HPP
