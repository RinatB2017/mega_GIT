/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef KALEIDOSCOPE_WIDGET_HPP
#define KALEIDOSCOPE_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QPainterPath>
#include <QElapsedTimer>
#include <QTimerEvent>
#include <QPointer>
#include <QPainter>
#include <QBitmap>
#include <QWidget>
#include <QLabel>
#include <QtMath>
#include <QTimer>
#include <QtDebug>

#include "kaleidoscope_image.hpp"
//--------------------------------------------------------------------------------
//using namespace cv;
//--------------------------------------------------------------------------------
class KaleidoscopeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KaleidoscopeWidget(QWidget *parent = nullptr);
    virtual ~KaleidoscopeWidget();

    bool load_image(const QImage &new_image);
    void set_pos(int x, int y);
    void get_pos(int *x, int *y);

    void get_size(int *w, int *h);
    void set_size(int w, int h);

    QImage create_test_image(void);
    QImage get_image(void);

    int get_piece_w(void);
    int get_piece_h(void);
    void  set_piece_angle(qreal new_angle);
    qreal get_piece_angle(void);

    void f_move_lu(int inc = 1);
    void f_move_l(int inc = 1);
    void f_move_ld(int inc = 1);
    void f_move_u(int inc = 1);
    void f_move_d(int inc = 1);
    void f_move_ru(int inc = 1);
    void f_move_r(int inc = 1);
    void f_move_rd(int inc = 1);
    void f_move_cancel(void);
    void f_move_to(int x, int y);
    void f_update(void);

private:
    int widget_w = 600;
    int widget_h = 600;

    int pos_x = 0;
    int pos_y = 0;

    //qreal piece_angle = 15.0;
    qreal piece_angle = 30.0;
    //qreal piece_angle = 60.0;

#ifdef QT_DEBUG
    QLabel *label = nullptr;
#endif

    KaleidoscopeImage *image_widget = nullptr;
    QImage result_image;
    QPainter painter;

    void init(void);
    void init_kaleidoscope_widget(void);
};
//--------------------------------------------------------------------------------
#endif // KALEIDOSCOPE_WIDGET_HPP
