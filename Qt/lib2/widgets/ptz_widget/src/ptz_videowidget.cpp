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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QVideoWidget>
//--------------------------------------------------------------------------------
#include "ptz_videowidget.hpp"
//--------------------------------------------------------------------------------
PTZ_VideoWidget::PTZ_VideoWidget(QWidget *parent) :
    QVideoWidget(parent)
{
    btn_lu = new QToolButton(this);
    btn_ru = new QToolButton(this);

    btn_u  = new QToolButton(this);
    btn_d  = new QToolButton(this);
    btn_l  = new QToolButton(this);
    btn_r  = new QToolButton(this);

    btn_ld = new QToolButton(this);
    btn_rd = new QToolButton(this);

    btn_lu->setIcon(QIcon(":/arrows/up_left.png"));
    btn_ru->setIcon(QIcon(":/arrows/up_right.png"));

    btn_u->setIcon(QIcon(":/arrows/up.png"));
    btn_d->setIcon(QIcon(":/arrows/down.png"));
    btn_l->setIcon(QIcon(":/arrows/left.png"));
    btn_r->setIcon(QIcon(":/arrows/right.png"));

    btn_ld->setIcon(QIcon(":/arrows/down_left.png"));
    btn_rd->setIcon(QIcon(":/arrows/down_right.png"));
}
//--------------------------------------------------------------------------------
void PTZ_VideoWidget::resizeEvent(QResizeEvent *)
{
    int x1 = btn_lu->width();
    int y1 = btn_lu->height();

    int x2 = width() - btn_ru->width() * 2.0;
    int y2 = btn_ru->height();

    int x3 = btn_ld->width();
    int y3 = height() - btn_ld->height() * 5.0;

    int x4 = width() - btn_rd->width() * 2.0;
    int y4 = height() - btn_rd->height() * 5.0;

//    btn_lu->move(x1, y1);
//    btn_ru->move(x2, y2);
//    btn_ld->move(x3, y3);
//    btn_rd->move(x4, y4);
}
//--------------------------------------------------------------------------------
