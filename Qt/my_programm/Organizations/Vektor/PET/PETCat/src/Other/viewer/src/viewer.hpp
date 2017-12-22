/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef VIEWER_HPP
#define VIEWER_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QString>
#include <QWidget>
#include <QImage>
//--------------------------------------------------------------------------------
#include <qwt_color_map.h>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class DAT3;
//--------------------------------------------------------------------------------
class Viewer : public MyWidget
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = 0);

    bool set_floodmap_size(int value);
    bool clear_all(void);

    bool set_point(int x, int y, int value);

    QPixmap *create_image(uint64_t maxCount,
                          PALETTE pal = GRAY);
    pet_event get_point(uint8_t  address,
                        uint8_t  Flags,
                        uint16_t x,
                        uint16_t y,
                        quint64  ts);
    pet_event get_point(uint8_t  address,
                        uint8_t  Flags,
                        uint16_t x,
                        uint16_t y,
                        uint16_t energy,
                        quint64  ts);

private:
    void init(void);

    DAT3 *dat3 = 0;
    QString filename;

    int maxCount = 0;
    int maxValue = 100;

    QLinearGradient *color_gradient = 0;
    QBrush *color_brush = 0;

    QLinearGradient *mono_gradient = 0;
    QBrush *mono_brush = 0;

    QwtLinearColorMap *colorMap = 0;

    QColor borderPenColor = Qt::white;
    QColor backgroundColor = Qt::black;

    int floodmap_size_w = DEFAULT_SIZE_PICTURE;
    int floodmap_size_h = DEFAULT_SIZE_PICTURE;

    uint64_t array[MAX_SIZE_Y][MAX_SIZE_X];

    void init_color_gradient(void);
    void init_mono_gradient(void);

    void generate_test_circle(int event_count);
    void generate_test_square(int max_point, int event_count);
};
//--------------------------------------------------------------------------------
#endif
