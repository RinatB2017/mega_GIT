/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QMouseEvent>
#include <QVariant>
//--------------------------------------------------------------------------------
#include "diod.hpp"
//--------------------------------------------------------------------------------
Diod::Diod(QWidget *parent) :
    QToolButton(parent)
{
    setFixedSize(WIDTH, HEIGHT);
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

    setStyleSheet(QString("background:rgb(%1,%2,%3)")
                  .arg(R_color)
                  .arg(G_color)
                  .arg(B_color));
}
//--------------------------------------------------------------------------------
QRgb Diod::get_color(void)
{
    return qRgba(R_color, G_color, B_color, 0);
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
void Diod::mousePressEvent(QMouseEvent *event)
{
    QColorDialog *dlg = 0;

    switch(event->button())
    {
    case Qt::LeftButton:
        dlg = new QColorDialog(QColor(R_color, G_color, B_color));
        if(dlg->exec() == QColorDialog::Accepted)
        {
            set_color(dlg->selectedColor());
        }
        dlg->deleteLater();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
