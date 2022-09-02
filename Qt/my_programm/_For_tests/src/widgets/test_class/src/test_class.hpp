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
        bool (Test_class::*check_box)(void);
        void (Test_class::*func)(void);
    } CMD;
    QList<CMD> commands;

    Ui::Test_class *ui;

    void init(void);
    void test(void);

    bool get_0(void);
    bool get_1(void);
    bool get_2(void);
    bool get_3(void);
    bool get_4(void);

    void f_0(void);
    void f_1(void);
    void f_2(void);
    void f_3(void);
    void f_4(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // TEST_CLASS_HPP
