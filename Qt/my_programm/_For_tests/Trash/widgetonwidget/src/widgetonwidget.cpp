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
#include "widgetonwidget.hpp"
#include "ui_widgetonwidget.h"
//--------------------------------------------------------------------------------
WidgetOnWidget::WidgetOnWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetOnWidget)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
WidgetOnWidget::~WidgetOnWidget()
{
    if(my_button)
    {
        my_button->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void WidgetOnWidget::init(void)
{
    my_button = new QPushButton(this);
    my_button->setText("test");
    my_button->adjustSize();
}
//--------------------------------------------------------------------------------
void WidgetOnWidget::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width();
    //int h = event->size().height();

    int pos_x = w - my_button->width() - 10;
    //int pos_y = h - my_button->height() - 10;
    int pos_y = 10;

    my_button->move(pos_x, pos_y);
}
//--------------------------------------------------------------------------------
