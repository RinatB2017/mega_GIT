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
#ifndef WIDGET_HPP
#define WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QMouseEvent>
#include <QWidget>
#include <QPixmap>
#include <QPoint>
//--------------------------------------------------------------------------------
class QLabel;
//--------------------------------------------------------------------------------
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    void connect_log(void);
    void init(void);

    QLabel *label;
    QPoint lastPoint;
    bool b_move;

    int pos_x;
    int pos_y;
    int width;
    int height;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif
