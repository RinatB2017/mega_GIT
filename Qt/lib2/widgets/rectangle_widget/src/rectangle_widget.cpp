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
#include "rectangle_widget.hpp"
//--------------------------------------------------------------------------------
RectangeWidget::RectangeWidget(QWidget *parent) :
    QFrame(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

    setWindowFlags(Qt::SubWindow);
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    sizeGrip = new QSizeGrip(this);
    layout->addWidget(sizeGrip, 0, 0, 1, 1, Qt::AlignBottom | Qt::AlignRight);
}
//--------------------------------------------------------------------------------
RectangeWidget::~RectangeWidget()
{
    delete sizeGrip;
}
//--------------------------------------------------------------------------------
void RectangeWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        flag_move = true;
        dx = event->position().x();
        dy = event->position().y();
    }
}
//--------------------------------------------------------------------------------
void RectangeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        flag_move = false;
    }
}
//--------------------------------------------------------------------------------
void RectangeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() && Qt::LeftButton)
    {
        if(flag_move)
        {
            setGeometry(pos().x() + (event->x() - dx),
                        pos().y() + (event->y() - dy),
                        width(),
                        height());
        }
    }
}
//--------------------------------------------------------------------------------
