/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "lcd_widget.hpp"
#include "ui_lcd_widget.h"
//--------------------------------------------------------------------------------
LCD_widget::LCD_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LCD_widget)
{
    init();;
}
//--------------------------------------------------------------------------------
LCD_widget::~LCD_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void LCD_widget::init(void)
{
    ui->setupUi(this);

    QList<QLCDNumber *> allobj = findChildren<QLCDNumber *>();
    foreach (QLCDNumber *obj, allobj)
    {
        obj->setFixedSize(220, 48);
        obj->setDigitCount(6);

        l_lcd.append(obj);
    }
}
//--------------------------------------------------------------------------------
bool LCD_widget::display(QString object_name, qreal value)
{
    foreach (QLCDNumber *obj, l_lcd)
    {
        if(obj->objectName() == object_name)
        {
            obj->display(value);
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
