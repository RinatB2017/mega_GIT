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
#include "newmainwindow.hpp"
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
NewMainWindow::NewMainWindow(MainWindow *parent)
    : MainWindow(parent)
{
    add_dock_widget("RS232",  "serial",   Qt::LeftDockWidgetArea,  new SerialBox5(this));
    add_dock_widget("График", "grapher",  Qt::RightDockWidgetArea, new GrapherBox(this));
    add_dock_widget("SpinBox", "spinbox", Qt::RightDockWidgetArea, new QSpinBox(this));

    QPushButton *btn = new QPushButton(this);
    btn->setText("PUSH");
    connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(push()));
    add_dock_widget("PushButton", "pushbutton", Qt::RightDockWidgetArea,  btn);

    load_setting();
}
//--------------------------------------------------------------------------------
void NewMainWindow::push(void)
{
    emit info("push");
}
//--------------------------------------------------------------------------------
