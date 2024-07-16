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
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QBitmap>
#include <QPointer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Card : public MyWidget
{
    Q_OBJECT
public:
    Card(QWidget *parent);

    void create_card(QString name,
                     int pos_x,
                     int pos_y,
                     int card_width,
                     int card_height,
                     QSvgRenderer *renderer);

private:
    void init(void);

    QPointer<QLabel> label;
    QPoint lastPoint;
    bool b_move = 0;

    int pos_x = 0;
    int pos_y = 0;
    int card_width = 0;
    int card_height = 0;
    QSvgRenderer *re = nullptr;

    QString card_name;

    int get_corr_x1(QPixmap *pix);
    int get_corr_y1(QPixmap *pix);
    int get_corr_x2(QPixmap *pix);
    int get_corr_y2(QPixmap *pix);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif
