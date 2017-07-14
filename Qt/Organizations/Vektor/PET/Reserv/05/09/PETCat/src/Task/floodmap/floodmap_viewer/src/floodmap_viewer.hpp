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
#ifndef FLOODMAP_VIEWER_HPP
#define FLOODMAP_VIEWER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class FloodMap_viewer;
}
//--------------------------------------------------------------------------------
class FloodMap_viewer : public MyWidget
{
    Q_OBJECT

public:
    explicit FloodMap_viewer(QWidget *parent);
    ~FloodMap_viewer();

    void draw(QImage image, int maxValue, bool state);

private:
    Ui::FloodMap_viewer *ui;

    int floodmapSize = 512;
    int borderSize = floodmapSize / 16;
    int lineSize = borderSize / 3;
    QColor borderPenColor = Qt::white;
    QColor backgroundColor = Qt::black;

    int maxValue = 100;

    QImage floodmap;

    QLinearGradient *color_gradient = 0;
    QBrush *color_brush = 0;

    QLinearGradient *mono_gradient = 0;
    QBrush *mono_brush = 0;

    // true COLOR
    // false MONO
    bool is_color = true;

    void init(void);
    void update_widget_size(void);

    void init_color_gradient(void);
    void init_mono_gradient(void);

protected:
    void paintEvent(QPaintEvent *);

};
//--------------------------------------------------------------------------------
#endif // FLOODMAP_VIEWER_HPP
