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
#include <QRadialGradient>
#include <QPainter>
//--------------------------------------------------------------------------------
#include "floodmap_viewer.hpp"
#include "ui_floodmap_viewer.h"
//--------------------------------------------------------------------------------
FloodMap_viewer::FloodMap_viewer(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::FloodMap_viewer)
{
    init();
}
//--------------------------------------------------------------------------------
FloodMap_viewer::~FloodMap_viewer()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void FloodMap_viewer::init(void)
{
    ui->setupUi(this);

    init_color_gradient();
    init_mono_gradient();

    update_widget_size();
}
//--------------------------------------------------------------------------------
void FloodMap_viewer::init_color_gradient(void)
{
    color_gradient = new QLinearGradient(0,floodmapSize,0,0);

    double k = 0.5;
    color_gradient->setColorAt(0.0,     QColor(0,   0,   0));
    color_gradient->setColorAt(0.2 * k, QColor(85,  0,   170));
    color_gradient->setColorAt(0.4 * k, QColor(170, 0,   170));
    color_gradient->setColorAt(0.6 * k, QColor(255, 85,  0));
    color_gradient->setColorAt(0.8 * k, QColor(255, 170, 0));
    color_gradient->setColorAt(1.0,     QColor(255, 255, 255));

    color_brush = new QBrush(*color_gradient);
}
//--------------------------------------------------------------------------------
void FloodMap_viewer::init_mono_gradient(void)
{
    mono_gradient = new QLinearGradient(0,floodmapSize,0,0);

    mono_brush = new QBrush(*mono_gradient);
}
//--------------------------------------------------------------------------------
#include <QDebug>
void FloodMap_viewer::draw(QImage image, int maxValue, bool state)
{
    if(image.isNull())
    {
        return;
    }
    if(image.width() != image.height())
    {
        return;
    }
    floodmapSize = image.width();
    init_color_gradient();
    init_mono_gradient();

    this->is_color = state;
    this->maxValue = maxValue;
    update_widget_size();

    //---
#if 0
    qDebug() << "TEST";
    QPainter *painter = new QPainter(this);
    painter->begin(this);
    painter->setPen(Qt::white);
    painter->drawLine(0, 0, width(), height());
    painter->end();
#endif
    //---

    floodmap = image.copy(0, 0, image.width(), image.height());
}
//--------------------------------------------------------------------------------
void FloodMap_viewer::update_widget_size(void)
{
    color_gradient->setStart(0,floodmapSize);
    setFixedSize(floodmapSize+(2 * borderSize + 100),
                 floodmapSize+(2 * borderSize));
}
//--------------------------------------------------------------------------------
void FloodMap_viewer::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), backgroundColor);

    p.setPen(QPen(borderPenColor, 1, Qt::SolidLine));

    int x1 = borderSize;
    int y1 = borderSize;
    int x2 = borderSize + floodmapSize;
    int y2 = borderSize + floodmapSize;

#if 0
    //рисуем рамку
    p.drawRect(borderSize, borderSize,
               floodmapSize, floodmapSize);
#endif

    p.drawLine(x1-lineSize-10, y1,
               x1-10, y1);
    p.drawLine(x2+lineSize+10, y1,
               x2+10, y1);

    p.drawLine(x1-lineSize-10, y2,
               x1-10, y2);
    p.drawLine(x2+lineSize+10, y2,
               x2+10, y2);

    p.drawLine(x1, y1-lineSize-10,
               x1, y1-10);
    p.drawLine(x1, y2+10,
               x1, y2+lineSize+10);

    p.drawLine(x2, y1-lineSize-10,
               x2, y1-10);
    p.drawLine(x2, y2+10,
               x2, y2+lineSize+10);

    p.drawLine(x1 + (x2 - x1) / 2, y1-lineSize-10,
               x1 + (x2 - x1) / 2, y1-10);
    p.drawLine(x1 + (x2 - x1) / 2, y2+10,
               x1 + (x2 - x1) / 2, y2+lineSize+10);

    p.drawLine(x1-lineSize-10, y1 + (x2 - x1) / 2,
               x1-10, y1 + (x2 - x1) / 2);
    p.drawLine(x2+lineSize+10, y1 + (x2 - x1) / 2,
               x2+10, y1 + (x2 - x1) / 2);

    //---
    if(is_color)
    {
        p.setBrush(*color_brush);
    }
    else
    {
        p.setBrush(*mono_brush);
    }
    p.drawRect(x2+30, y1,
               20, floodmapSize);
    //---
    double inc = floodmapSize / 20.0;
    for(int n=0; n<=20; n++)
    {
        p.drawLine(x2+50, y1+n*inc,
                   x2+60, y1+n*inc);
    }
    //---
#if 1
    int temp = (maxValue / 10) * 10;
    if(maxValue % 10)
    {
        temp += 10;
    }
#else
    int temp = maxValue;
#endif
    //---
    double inc2 = floodmapSize / 10.0;
    double inc_value = temp / 10.0;
    for(int n=0; n<=10; n++)
    {
        double value = abs(n-10)*inc_value;
        p.drawText(x2+70, y1+n*inc2+5, QString("%1").arg((int)value));
    }
    //---

    if(floodmap.isNull() == false)
    {
        p.drawImage(borderSize, borderSize, floodmap);
    }

    update();
}
//--------------------------------------------------------------------------------
