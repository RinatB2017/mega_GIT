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
#ifndef SCENE2_HPP
#define SCENE2_HPP
//--------------------------------------------------------------------------------
#include <QGraphicsScene>
//--------------------------------------------------------------------------------
#include "basescene.hpp"
//--------------------------------------------------------------------------------
class Scene2 : public BaseScene
{
    Q_OBJECT

public:
    Scene2(void);
    ~Scene2();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void update(void);

private:
    QGraphicsPixmapItem *item = 0;
    QGraphicsPixmapItem *tux = 0;
    QGraphicsItemGroup *group;

    void move(void);
};
//--------------------------------------------------------------------------------
#endif
