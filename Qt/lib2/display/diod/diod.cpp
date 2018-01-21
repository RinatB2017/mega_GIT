/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QApplication>
#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QCursor>
#include <QPixmap>
#include <QImage>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "diod.hpp"
//--------------------------------------------------------------------------------
Diod::Diod(int led_width,
           int led_height,
           QWidget *parent) :
    QToolButton(parent)
{
    if(led_width < 0)   led_width = MAX_LED_SIZE_W;
    if(led_width > MAX_LED_SIZE_W)   led_width = MAX_LED_SIZE_W;
    if(led_height < 0)   led_height = MAX_LED_SIZE_H;
    if(led_height > MAX_LED_SIZE_H)   led_height = MAX_LED_SIZE_H;

    setFixedSize(led_width,
                 led_height);
}
//--------------------------------------------------------------------------------
void Diod::set_color(QColor color)
{
    set_color(color.red(),
              color.green(),
              color.blue());
}
//--------------------------------------------------------------------------------
void Diod::set_color(uint8_t R_value,
                     uint8_t G_value,
                     uint8_t B_value)
{
    R_color = R_value;
    G_color = G_value;
    B_color = B_value;

#if 0
    setStyleSheet(QString("background:rgb(%1,%2,%3);")
                  .arg(R_color)
                  .arg(G_color)
                  .arg(B_color));
#endif
}
//--------------------------------------------------------------------------------
QRgb Diod::get_color(void)
{
    return qRgba(R_color, G_color, B_color, 255);
}
//--------------------------------------------------------------------------------
uint8_t Diod::get_R(void)
{
    return R_color;
}
//--------------------------------------------------------------------------------
uint8_t Diod::get_G(void)
{
    return G_color;
}
//--------------------------------------------------------------------------------
uint8_t Diod::get_B(void)
{
    return B_color;
}
//--------------------------------------------------------------------------------
void Diod::set_cursor(void)
{
    QPixmap *pixmap = new QPixmap(MAX_LED_SIZE_W,
                                  MAX_LED_SIZE_H);
    Q_CHECK_PTR(pixmap);

    pixmap->fill(QColor(0, 0, 0, 0));

    QPainter painter;
    painter.begin(pixmap);

    painter.setPen(QPen(QColor(R_color, G_color, B_color), 1, Qt::SolidLine));
    painter.setBrush(QBrush(QColor(R_color, G_color, B_color)));
    painter.drawEllipse(QPoint(width() / 2, height() / 2), width() / 2 - 2, height() / 2 - 2);

    painter.end();

    QCursor cursor(*pixmap);
    topLevelWidget()->setCursor(cursor);
}
//--------------------------------------------------------------------------------
void Diod::set_left_btn_active(bool value)
{
    active_left_btn = value;
}
//--------------------------------------------------------------------------------
void Diod::set_right_btn_active(bool value)
{
    active_right_btn = value;
}
//--------------------------------------------------------------------------------
void Diod::set_flag_is_palette(bool value)
{
    is_palette = value;
}
//--------------------------------------------------------------------------------
bool Diod::resize(int w, int h)
{
    if(w <= 0)  return false;
    if(h <= 0)  return false;
    if(w > MAX_LED_SIZE_W)  return false;
    if(h > MAX_LED_SIZE_H)  return false;

    setFixedSize(w, h);

    return true;
}
//--------------------------------------------------------------------------------
void Diod::mousePressEvent(QMouseEvent *event)
{
    QColorDialog *dlg = 0;
    QImage *img = 0;

    switch(event->button())
    {
    case Qt::LeftButton:
        if(active_left_btn)
        {
            img = new QImage(cursor().pixmap().toImage());
            if(is_palette == false)
            {
                if((img->width() >= MAX_LED_SIZE_W / 2) && (img->height() >= MAX_LED_SIZE_H / 2))
                {
                    set_color(img->pixelColor(MAX_LED_SIZE_W / 2, MAX_LED_SIZE_H / 2));
                }
            }
            else
            {
                set_cursor();
            }
        }
        break;

    case Qt::RightButton:
        if(active_right_btn)
        {
            dlg = new QColorDialog(QColor(R_color, G_color, B_color));
            if(dlg->exec() == QColorDialog::Accepted)
            {
                set_color(dlg->selectedColor());
                set_cursor();
            }
            dlg->deleteLater();
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void Diod::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    painter.setPen(QPen(QColor(R_color, G_color, B_color), 1, Qt::SolidLine));
    painter.setBrush(QBrush(QColor(R_color, G_color, B_color)));
    painter.drawEllipse(QPoint(width() / 2, height() / 2), width() / 2 - 2, height() / 2 - 2);

    painter.end();
}
//--------------------------------------------------------------------------------
