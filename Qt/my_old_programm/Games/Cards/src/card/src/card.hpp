/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef CARD_HPP
#define CARD_HPP
//--------------------------------------------------------------------------------
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>
#include <QPoint>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QSvgRenderer;
class QLabel;
//--------------------------------------------------------------------------------
class Card : public MyWidget
{
    Q_OBJECT
public:
    Card(QString name,
         int pos_x,
         int pos_y,
         int card_width,
         int card_height,
         QSvgRenderer *renderer,
         QWidget *parent);

private:
    void init(void);

    QLabel *label = 0;
    QPoint lastPoint;
    bool b_move;

    int pos_x;
    int pos_y;
    int card_width;
    int card_height;
    QSvgRenderer *re = 0;

    QString card_name;

    int get_corr_x1(QPixmap *pix);
    int get_corr_y1(QPixmap *pix);
    int get_corr_x2(QPixmap *pix);
    int get_corr_y2(QPixmap *pix);

    void updateText(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif
