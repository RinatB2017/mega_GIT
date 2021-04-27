/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "test_ui_mainbox.hpp"
#include "ui_mainbox.hpp"
//--------------------------------------------------------------------------------
UI_MainBox::UI_MainBox(QWidget *parent,
                       MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash)
//    ui(new Ui::MainBox)
{
//    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
UI_MainBox::~UI_MainBox()
{
//    delete ui;
}
//--------------------------------------------------------------------------------
void UI_MainBox::set_value_range(int min, int max)
{

}
//--------------------------------------------------------------------------------
void UI_MainBox::set_value(int value)
{

}
//--------------------------------------------------------------------------------
int UI_MainBox::get_value(void)
{

}
//--------------------------------------------------------------------------------
