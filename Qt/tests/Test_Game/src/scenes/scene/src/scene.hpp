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
#ifndef SCENE_HPP
#define SCENE_HPP
//--------------------------------------------------------------------------------
#include <QGraphicsScene>
//--------------------------------------------------------------------------------
#include "basescene.hpp"
//--------------------------------------------------------------------------------
class Scene : public BaseScene
{
    Q_OBJECT

public:
    Scene(void);
    ~Scene();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void update(void);

private:
    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT
    };

    QPixmap *pix;
    QGraphicsPixmapItem *item;
    qreal x = 0;
    qreal y = 0;
    qreal delta = 0.05;
    int direction = LEFT;
};
//--------------------------------------------------------------------------------
#endif
