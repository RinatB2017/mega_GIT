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
#include "myqdockwidget.hpp"
//--------------------------------------------------------------------------------
MyQDockWidget::MyQDockWidget(QWidget *parent) :
    QDockWidget(parent)
{

}
//--------------------------------------------------------------------------------
MyQDockWidget::~MyQDockWidget()
{

}
//--------------------------------------------------------------------------------
void MyQDockWidget::set_dock_position_widget(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    dock_position_widget = widget;
}
//--------------------------------------------------------------------------------
void MyQDockWidget::resizeEvent(QResizeEvent *event)
{
    if(dock_position_widget)
    {
        int w = event->size().width();
        //int h = event->size().height();

#if 0
        int pos_x = w - dock_position_widget->width() + 12;
        //int pos_y = h - dock_position_widget->height() - 10;
        int pos_y = 40;
#else
        int pos_x = w - dock_position_widget->width() - 2;
        //int pos_y = h - dock_position_widget->height() - 10;
        int pos_y = 4;
#endif

        dock_position_widget->move(pos_x, pos_y);
    }
}
//--------------------------------------------------------------------------------
