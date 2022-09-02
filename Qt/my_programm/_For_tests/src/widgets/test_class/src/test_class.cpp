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
#include "test_class.hpp"
#include "ui_test_class.h"
//--------------------------------------------------------------------------------
Test_class::Test_class(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Test_class)
{
    init();
}
//--------------------------------------------------------------------------------
Test_class::~Test_class()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Test_class::init(void)
{
    ui->setupUi(this);

    commands.append({ &Test_class::get_0, &Test_class::f_0});
    commands.append({ &Test_class::get_1, &Test_class::f_1});
    commands.append({ &Test_class::get_2, &Test_class::f_2});
    commands.append({ &Test_class::get_3, &Test_class::f_3});
    commands.append({ &Test_class::get_4, &Test_class::f_4});

    connect(ui->btn_test,   &QPushButton::clicked,
            this,           &Test_class::test);
}
//--------------------------------------------------------------------------------
void Test_class::test(void)
{
    typedef bool (Test_class::*check_box)(void);
    typedef void (Test_class::*func)(void);

    foreach (CMD cmd, commands)
    {
        check_box cb = cmd.check_box;
        func res = cmd.func;

        bool x = (this->*cb)();
        if(x)
        {
            (this->*res)();
        }
    }
}
//--------------------------------------------------------------------------------
bool Test_class::get_0(void)
{
    return ui->cb_0->isChecked();
}
//--------------------------------------------------------------------------------
bool Test_class::get_1(void)
{
    return ui->cb_1->isChecked();
}
//--------------------------------------------------------------------------------
bool Test_class::get_2(void)
{
    return ui->cb_2->isChecked();
}
//--------------------------------------------------------------------------------
bool Test_class::get_3(void)
{
    return ui->cb_3->isChecked();
}
//--------------------------------------------------------------------------------
bool Test_class::get_4(void)
{
    return ui->cb_4->isChecked();
}
//--------------------------------------------------------------------------------
void Test_class::f_0(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Test_class::f_1(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Test_class::f_2(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Test_class::f_3(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Test_class::f_4(void)
{
    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Test_class::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Test_class::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Test_class::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Test_class::save_setting(void)
{

}
//--------------------------------------------------------------------------------
