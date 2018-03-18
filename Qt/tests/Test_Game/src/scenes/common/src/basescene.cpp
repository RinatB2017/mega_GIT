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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "basescene.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
BaseScene::BaseScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setSceneRect(0, 0, WIDTH, HEIGHT);
    create_timer();
}
//--------------------------------------------------------------------------------
BaseScene::~BaseScene()
{
    if(timer)
    {
        timer->stop();
        timer->deleteLater();
    }
    if(background)
    {
        delete background;
    }
}
//--------------------------------------------------------------------------------
QGraphicsPixmapItem *BaseScene::get_item(QString filename)
{
    if(filename.isEmpty())
    {
        return 0;
    }
    QGraphicsPixmapItem *item = addPixmap(QPixmap(filename));
    item->setTransformOriginPoint(item->boundingRect().width()/2,
                                  item->boundingRect().height()/2);
    return item;
}
//--------------------------------------------------------------------------------
QGraphicsItemGroup *BaseScene::get_text_rect(int num,
                                             QColor text_color,
                                             QPen rect_border,
                                             QBrush rect_brush)
{
    QGraphicsItemGroup *test_group = new QGraphicsItemGroup;
    QGraphicsRectItem *rect = addRect(0, 0, 100, 20,
                                      rect_border,
                                      rect_brush);
    QGraphicsTextItem *text = addText(QString("%1").arg(num));
    text->setDefaultTextColor(text_color);
    text->setTransformOriginPoint(text->boundingRect().width()/2,
                                  text->boundingRect().height()/2);
    text->setPos(rect->boundingRect().width()/2 - text->boundingRect().width()/2,
                 rect->boundingRect().height()/2 - text->boundingRect().height()/2);
    //---
    test_group->addToGroup(rect);
    test_group->addToGroup(text);

    return test_group;
}
//--------------------------------------------------------------------------------
void BaseScene::add_background(QString filename)
{
    if(filename.isEmpty())
    {
        return;
    }
    background = addPixmap(QPixmap(filename));
}
//--------------------------------------------------------------------------------
void BaseScene::create_timer(void)
{
    //qDebug() << "BaseScene::create_timer";

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    start();
}
//--------------------------------------------------------------------------------
void BaseScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:    flag_up    = true;  break;
    case Qt::Key_Down:  flag_down  = true;  break;
    case Qt::Key_Left:  flag_left  = true;  break;
    case Qt::Key_Right: flag_right = true;  break;
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        BaseScene::keyPressEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
void BaseScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:    flag_up    = false;  break;
    case Qt::Key_Down:  flag_down  = false;  break;
    case Qt::Key_Left:  flag_left  = false;  break;
    case Qt::Key_Right: flag_right = false;  break;
    default:
        BaseScene::keyPressEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
void BaseScene::start(void)
{
    if(timer)
    {
        timer->start(DEFAULT_TIMEOUT);
    }
}
//--------------------------------------------------------------------------------
void BaseScene::stop(void)
{
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
