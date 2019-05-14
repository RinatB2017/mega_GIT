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
#include "test_widget.hpp"
//--------------------------------------------------------------------------------
TestWidget::TestWidget(int number,
                       int size,
                       int font_size,
                       QWidget *parent)
    :QWidget(parent)
{
    this->number = number;
    this->width = size;
    this->height = size;
    this->font_size = font_size;
    this->border = width / 5;
    setFixedSize(width, height);
}
//--------------------------------------------------------------------------------
void TestWidget::save(QString filename)
{
    QPixmap *pixmap = new QPixmap(this->size());
    this->render(pixmap);
    pixmap->save(filename);
}
//--------------------------------------------------------------------------------
void TestWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(begin_color));
    painter.drawRect(0, 0, width, height);

    painter.setBrush(QBrush(end_color));
    painter.drawRect(border -1, border - 1, width - border, height - border);

    //---
    QLinearGradient shader_left(QPointF(0, height / 2.0),
                                QPointF(border, height / 2.0));
    shader_left.setColorAt(0, begin_border_color);
    shader_left.setColorAt(1, end_border_color);
    //---
    QLinearGradient shader_up(QPointF(width / 2.0, 0),
                              QPointF(width / 2.0, border));
    shader_up.setColorAt(0, begin_border_color);
    shader_up.setColorAt(1, end_border_color);
    //---
    QLinearGradient shader_right(QPointF(width, height / 2.0),
                                 QPointF(width - border, height / 2.0));
    shader_right.setColorAt(0, begin_border_color);
    shader_right.setColorAt(1, end_border_color);
    //---
    QLinearGradient shader_down(QPointF(width / 2.0, height),
                                QPointF(width / 2.0, height - border));
    shader_down.setColorAt(0, begin_border_color);
    shader_down.setColorAt(1, end_border_color);
    //---

    QPainterPath path;
    //---
    // left
    path = QPainterPath();
    path.moveTo(0, 0);
    path.lineTo(border, border);
    path.lineTo(border, height - border);
    path.lineTo(0, height);
    painter.fillPath(path, shader_left);
    //---
    // right
    path = QPainterPath();
    path.moveTo(width, 0);
    path.lineTo(width - border, border);
    path.lineTo(width - border, height - border);
    path.lineTo(width, height);
    painter.fillPath(path, shader_right);
    //---
    // up
    path = QPainterPath();
    path.moveTo(0, 0);
    path.lineTo(border, border);
    path.lineTo(width - border, border);
    path.lineTo(width, 0);
    painter.fillPath(path, shader_up);
    //---
    // down
    path = QPainterPath();
    path.moveTo(0, height);
    path.lineTo(border, height - border);
    path.lineTo(width - border, height - border);
    path.lineTo(width, height);
    painter.fillPath(path, shader_down);
    //---
    QFont font("Courier", font_size);
    painter.setFont(font);
    painter.setPen(number_color);
    QRectF rect = QRectF(0, 0, width, height);
    painter.drawText(rect,
                     Qt::AlignCenter,
                     QString("%1").arg(number));
    //---
}
//--------------------------------------------------------------------------------
