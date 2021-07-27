/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "mouseposition.hpp"
#include "ui_mouseposition.h"
//--------------------------------------------------------------------------------
MousePosition::MousePosition(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MousePosition)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
MousePosition::~MousePosition()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MousePosition::init(void)
{

}
//--------------------------------------------------------------------------------
void MousePosition::mousePressEvent(QMouseEvent* event)
{
    emit info("press");
    QWidget::mousePressEvent(event);
}
//--------------------------------------------------------------------------------
void MousePosition::mouseMoveEvent(QMouseEvent* event)
{
    emit info("move");
    QWidget::mouseMoveEvent(event);
}
//--------------------------------------------------------------------------------
void MousePosition::mouseReleaseEvent(QMouseEvent* event)
{
    emit info("release");
    QWidget::mouseReleaseEvent(event);
}
//--------------------------------------------------------------------------------
void MousePosition::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MousePosition::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MousePosition::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MousePosition::save_setting(void)
{

}
//--------------------------------------------------------------------------------
