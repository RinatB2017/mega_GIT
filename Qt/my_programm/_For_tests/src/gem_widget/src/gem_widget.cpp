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
#include "gem_widget.hpp"
//--------------------------------------------------------------------------------
Gem_widget::Gem_widget(qreal angle,
                       QWidget *parent)
    :QWidget(parent)
{
    //TODO начальный угол gem_green = -40
    this->angle = angle + 40;
    if(this->angle >= 360)
    {
        this->angle -= 360;
    }

    setFixedSize(256, 256);
}
//--------------------------------------------------------------------------------
void Gem_widget::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(t_update()));
    timer->start(10);
}
//--------------------------------------------------------------------------------
void Gem_widget::t_update(void)
{
    angle++;
    if(angle >= 360) angle = 0;
    update();
}
//--------------------------------------------------------------------------------
void Gem_widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/gem_green.png");

    QSize size = pixmap.size();
    painter.translate(size.height()/2, size.height()/2);
    painter.rotate(angle);
    painter.translate(-size.height()/2, -size.height()/2);

    painter.drawPixmap(0, 0, pixmap, 0, 0, 256, 256);
}
//--------------------------------------------------------------------------------
