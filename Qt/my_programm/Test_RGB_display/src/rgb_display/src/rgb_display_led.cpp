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
#include "rgb_display_led.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
RGB_dislpay_led::RGB_dislpay_led(QWidget *parent) :
    QToolButton(parent)
{
    pixelPerMm = QApplication::screens().at(0)->logicalDotsPerInch()/2.54/10;
    setFixedSize(static_cast<int>(full_width * pixelPerMm),
                 static_cast<int>(full_height * pixelPerMm));
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
            color_R = static_cast<uint8_t>(color.red());
            color_G = static_cast<uint8_t>(color.green());
            color_B = static_cast<uint8_t>(color.blue());
        }
    }
    QToolButton::mouseReleaseEvent(event);
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_R(int value)
{
    color_R = static_cast<uint8_t>(value);
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_G(int value)
{
    color_G = static_cast<uint8_t>(value);
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::set_B(int value)
{
    color_B = static_cast<uint8_t>(value);
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
double RGB_dislpay_led::get_width(void)
{
    return full_width;
}
//--------------------------------------------------------------------------------
double RGB_dislpay_led::get_height(void)
{
    return full_height;
}
//--------------------------------------------------------------------------------
bool RGB_dislpay_led::set_size(double w_size_mm,
                               double h_size_mm,
                               double l_border_mm,
                               double u_border_mm)
{
    if((w_size_mm <= 0) || (h_size_mm <= 0))
    {
        return false;
    }
    if(l_border_mm < 0) l_border_mm = 0;
    if(u_border_mm < 0) u_border_mm = 0;

    led_width = w_size_mm;
    led_heigth = h_size_mm;
    left_border_width = l_border_mm;
    up_border_height = u_border_mm;

    full_width = led_width + left_border_width * 2;
    full_height = led_heigth + up_border_height * 2;

    setFixedSize(static_cast<int>(full_width * pixelPerMm),
                 static_cast<int>(full_height * pixelPerMm));
    return true;
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::get_size(double *w_size_mm,
                               double *h_size_mm,
                               double *l_border_mm,
                               double *u_border_mm)
{
    *w_size_mm = led_width;
    *h_size_mm = led_heigth;
    *l_border_mm = left_border_width;
    *u_border_mm = up_border_height;
}
//--------------------------------------------------------------------------------
void RGB_dislpay_led::paintEvent(QPaintEvent *)
{
    QColor color;
    color.setRed(color_R);
    color.setGreen(color_G);
    color.setBlue(color_B);

    QPainter p(this);
    p.fillRect(0, 0,
               static_cast<int>(full_width * pixelPerMm),
               static_cast<int>(full_height * pixelPerMm),
               QColor(Qt::gray));
    p.fillRect(static_cast<int>(left_border_width * pixelPerMm),
               static_cast<int>(up_border_height * pixelPerMm),
               static_cast<int>(led_width * pixelPerMm),
               static_cast<int>(led_heigth * pixelPerMm),
               color);
}
//--------------------------------------------------------------------------------
