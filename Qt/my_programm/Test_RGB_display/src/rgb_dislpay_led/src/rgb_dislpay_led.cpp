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
#include <QColorDialog>
//--------------------------------------------------------------------------------
#include "rgb_dislpay_led.hpp"
//--------------------------------------------------------------------------------
RGB_dislpay_led::RGB_dislpay_led(int fix_width,
                                 int fix_heigth,
                                 QWidget *parent) :
    QToolButton(parent)
{
    setFixedSize(fix_width, fix_heigth);
}
//--------------------------------------------------------------------------------
RGB_dislpay_led::~RGB_dislpay_led()
{

}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit info(QString("%1:%2")
                  .arg(property("property_col").toInt())
                  .arg(property("property_row").toInt()));

        QColorDialog *dialog = new QColorDialog(this);
        int btn = dialog->exec();
        if(btn == QColorDialog::Accepted)
        {
            emit info("OK");
            QColor color = dialog->selectedColor();
            color_R = color.red();
            color_G = color.green();
            color_B = color.blue();

            set_color();
        }
    }
    QToolButton::mouseReleaseEvent(event);
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_R(int value)
{
    color_R = value;
    set_color();
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_G(int value)
{
    color_G = value;
    set_color();
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_B(int value)
{
    color_B = value;
    set_color();
}
//--------------------------------------------------------------------------------
int RGB_dislpay_led::get_R(void)
{
    return color_R;
}
//--------------------------------------------------------------------------------
int RGB_dislpay_led::get_G(void)
{
    return color_G;
}
//--------------------------------------------------------------------------------
int RGB_dislpay_led::get_B(void)
{
    return color_B;
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_color(void)
{
    setStyleSheet(QString("background-color:rgb(%1, %2, %3);")
                  .arg(color_R)
                  .arg(color_G)
                  .arg(color_B));
}
//--------------------------------------------------------------------------------
