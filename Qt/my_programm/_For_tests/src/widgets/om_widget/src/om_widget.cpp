/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "om_widget.hpp"
//--------------------------------------------------------------------------------
OM_widget::OM_widget(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
OM_widget::~OM_widget()
{

}
//--------------------------------------------------------------------------------
void OM_widget::init(void)
{

}
//--------------------------------------------------------------------------------
void OM_widget::paintEvent(QPaintEvent *)
{
    QPainter painter;

    painter.begin(this);
    //painter.setPen(Qt::red);
    //painter.drawLine(0, 0, 200, 200);

    //painter.drawPixmap(QPointF(0, 0), QPixmap(":/om/kisspng-om-symbol-clip-art-5ada.png"));
    //painter.drawPixmap(0, 0, width(), height(), QPixmap(":/om/kisspng-om-symbol-clip-art-5ada.png"));
    painter.drawPixmap(0, 0, QPixmap(":/om/kisspng-om-symbol-clip-art-5ada.png").scaled(width(), height(), Qt::KeepAspectRatio));

    painter.end();
}
//--------------------------------------------------------------------------------
