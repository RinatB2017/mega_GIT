/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "mainbox.hpp"
#include "defines.hpp"
#include "mouse.hpp"
//--------------------------------------------------------------------------------
#include "ui_mouse.h"
//--------------------------------------------------------------------------------
Mouse::Mouse(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Mouse)
{
    init();
}
//--------------------------------------------------------------------------------
Mouse::~Mouse()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Mouse::init(void)
{
    ui->setupUi(this);

    ui->w_minimap->set(0, 0, SPACE_ID);
    ui->w_minimap->set(2, 0, SPACE_ID);
    ui->w_minimap->set(0, 2, SPACE_ID);
    ui->w_minimap->set(2, 2, SPACE_ID);
}
//--------------------------------------------------------------------------------
bool Mouse::set(int x, int y, int id)
{
    return ui->w_minimap->set(x,y,id);
}
//--------------------------------------------------------------------------------
bool Mouse::get(int x, int y, int *id)
{
    return ui->w_minimap->get(x,y,id);
}
//--------------------------------------------------------------------------------
void Mouse::updateText(void)
{

}
//--------------------------------------------------------------------------------
