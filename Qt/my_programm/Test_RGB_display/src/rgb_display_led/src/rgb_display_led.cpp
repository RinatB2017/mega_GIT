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
#include <QPainter>
//--------------------------------------------------------------------------------
#include "rgb_display_led.hpp"
//--------------------------------------------------------------------------------
RGB_dislpay_led::RGB_dislpay_led(int fix_width,
                                 int fix_heigth,
                                 QWidget *parent) :
    QToolButton(parent)
{
    this->fix_width = fix_width;
    this->fix_heigth = fix_heigth;
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

        QColor color;
        color.setRed(color_R);
        color.setGreen(color_G);
        color.setBlue(color_B);

        QColorDialog *dialog = new QColorDialog(this);
        dialog->setCurrentColor(color);
        int btn = dialog->exec();
        if(btn == QColorDialog::Accepted)
        {
            emit info("OK");
            QColor color = dialog->selectedColor();
            color_R = color.red();
            color_G = color.green();
            color_B = color.blue();
        }
    }
    QToolButton::mouseReleaseEvent(event);
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_R(uint8_t value)
{
    color_R = value;
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_G(uint8_t value)
{
    color_G = value;
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_B(uint8_t value)
{
    color_B = value;
}
//--------------------------------------------------------------------------------
uint8_t RGB_dislpay_led::get_R(void)
{
    return color_R;
}
//--------------------------------------------------------------------------------
uint8_t RGB_dislpay_led::get_G(void)
{
    return color_G;
}
//--------------------------------------------------------------------------------
uint8_t RGB_dislpay_led::get_B(void)
{
    return color_B;
}
//--------------------------------------------------------------------------------
int RGB_dislpay_led::get_width(void)
{
    return fix_width;
}
//--------------------------------------------------------------------------------
int RGB_dislpay_led::get_height(void)
{
    return fix_heigth;
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::paintEvent(QPaintEvent *)
{
    QColor color;
    color.setRed(color_R);
    color.setGreen(color_G);
    color.setBlue(color_B);

    QPainter p(this);
    p.fillRect(0, 0, width(), height(), color);
}
//--------------------------------------------------------------------------------
