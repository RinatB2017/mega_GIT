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
#ifndef KALEIDOSCOPE_IMAGE_HPP
#define KALEIDOSCOPE_IMAGE_HPP
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QPainterPath>
#include <QPainter>
#include <QBitmap>
#include <QWidget>
#include <QDebug>
#include <QtMath>
//--------------------------------------------------------------------------------
#include <defines.hpp>
//--------------------------------------------------------------------------------
// возвращает маску или кусок пирога
//--------------------------------------------------------------------------------
class KaleidoscopeImage : public QWidget
{
    Q_OBJECT

public:
    explicit KaleidoscopeImage(QWidget *parent = nullptr);
    virtual ~KaleidoscopeImage();

    bool load_image(const QImage &new_image);
    QImage get_image(int pos_x, int pos_y);

    void set_param(int new_w, int new_h, int new_angle);

    int get_piece_w(void);
    int get_piece_h(void);
    void set_piece_w(int new_piece_w);
    void set_piece_h(int new_piece_h);

    QImage create_mask(void);
    QImage create_mask(int dir);
    void preparation_image(QImage image);

private:
    int image_w = 600;
    int image_h = 600;

    qreal piece_angle = 30.0;

    // горизонтальная маска
#ifdef H_MASH
    int piece_w = (qreal)image_w / 2.0 + 150.0;
    int piece_h = (qreal)piece_w * qTan(qDegreesToRadians(piece_angle / 2.0 + 0.5)) * 2.0;
#else
    int piece_h = (qreal)image_h / 2.0 + 150.0;
    int piece_w = (qreal)piece_h * qTan(qDegreesToRadians(piece_angle / 2.0 + 0.5)) * 2.0;
#endif

    QImage full_image;  // полная картинка
    QImage mask_image;  // маска
    QImage piece_image; // кусок для отображения
};
//--------------------------------------------------------------------------------
#endif // KALEIDOSCOPE_IMAGE_HPP
