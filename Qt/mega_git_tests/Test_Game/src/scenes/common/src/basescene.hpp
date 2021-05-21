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
#ifndef BASE_SCENE_HPP
#define BASE_SCENE_HPP
//--------------------------------------------------------------------------------
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QKeyEvent>
#include <QObject>
#include <QTimer>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class BaseScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BaseScene(QObject *parent = nullptr);
    virtual ~BaseScene();

    QGraphicsPixmapItem *get_item(QString filename);
    QGraphicsItemGroup *get_text_rect(int num,
                                      QColor text_color = QColor(Qt::black),
                                      QPen rect_border = QPen(Qt::black),
                                      QBrush rect_brush = QBrush(Qt::NoBrush));
    void add_background(QString filename);

    void start(void);
    void stop(void);

    qreal min_x = 0;
    qreal max_x = 0;
    qreal min_y = 0;
    qreal max_y = 0;

    qreal pos_x = 0;
    qreal pos_y = 0;

    bool flag_up = false;
    bool flag_down = false;
    bool flag_left = false;
    bool flag_right = false;

public slots:
    virtual void update(void) = 0;

private:
    QTimer *timer = nullptr;
    QGraphicsPixmapItem *background = nullptr;

    void create_timer(void);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

};
//--------------------------------------------------------------------------------
#endif
