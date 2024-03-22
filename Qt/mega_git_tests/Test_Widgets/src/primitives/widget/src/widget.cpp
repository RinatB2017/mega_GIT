/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "widget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Widget::Widget(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Widget::init(void)
{
    label = new QLabel();
    label->setPixmap(QPixmap(ICON_PROGRAMM));

    QVBoxLayout *vbox = new QVBoxLayout();
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    vbox->setMargin(0);
#endif
    vbox->setSpacing(0);
    vbox->addWidget(label);
    setLayout(vbox);

    show();
}
//--------------------------------------------------------------------------------
void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        raise();
        lastPoint = event->pos();
        b_move = true;
    }
}
//--------------------------------------------------------------------------------
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - lastPoint));
    }
}
//--------------------------------------------------------------------------------
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && b_move)
    {
        b_move = false;
    }
}
//--------------------------------------------------------------------------------
void Widget::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
