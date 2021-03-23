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
#include "hsv_color_dialog.hpp"
#include "ui_hsv_color_dialog.h"
//--------------------------------------------------------------------------------
HSV_color_dialog::HSV_color_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HSV_color_dialog)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
HSV_color_dialog::~HSV_color_dialog()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void HSV_color_dialog::set_color(QColor color)
{
    ui->HSV_widget->set_color(color);
}
//--------------------------------------------------------------------------------
QColor HSV_color_dialog::get_color(void)
{
    return ui->HSV_widget->get_color();
}
//--------------------------------------------------------------------------------
