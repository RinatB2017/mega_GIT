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
#include <QCoreApplication>
#include <QProgressDialog>
#include <QFileInfo>
#include <QPainter>
#include <QPicture>
#include <QBitmap>
#include <QWidget>
#include <QtMath>
#include <QFile>
#include <QIcon>
#include <QRgb>
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include "viewer.hpp"
#include "defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "dat3.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Viewer::Viewer(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Viewer::init(void)
{
    dat3 = new DAT3();
    connect(dat3, SIGNAL(info(QString)),    this, SIGNAL(info(QString)));
    connect(dat3, SIGNAL(debug(QString)),   this, SIGNAL(debug(QString)));
    connect(dat3, SIGNAL(error(QString)),   this, SIGNAL(error(QString)));
    connect(dat3, SIGNAL(trace(QString)),   this, SIGNAL(trace(QString)));

    init_color_gradient();
    init_mono_gradient();

    //---
    colorMap = new QwtLinearColorMap();
    colorMap->setColorInterval(QColor(0,    0,      0),
                               QColor(255,  255,    255));

    double k = 0.5;

    colorMap->addColorStop(0.0,     QColor(0,   0,   0));
    colorMap->addColorStop(0.2 * k, QColor(85,  0,   170));
    colorMap->addColorStop(0.4 * k, QColor(170, 0,   170));
    colorMap->addColorStop(0.6 * k, QColor(255, 85,  0));
    colorMap->addColorStop(0.8 * k, QColor(255, 170, 0));
    colorMap->addColorStop(1.0,     QColor(255, 255, 255));
    //---

    clear_all();
}
//--------------------------------------------------------------------------------
QPixmap *Viewer::create_image(uint64_t maxCount,
                              PALETTE pal)
{
    uint8_t c = 0;
    this->maxCount = maxCount;
    this->maxValue = maxCount;

    //---
    emit debug(QString("floodmap_size_w %1").arg(floodmap_size_w));
    emit debug(QString("floodmap_size_h %1").arg(floodmap_size_h));
    //---
    int border = 50;

    int indent = 5;             // отступ от floodmap
    int dash = border - 30 - indent; // размер риски

    int border_scale_w = 100;
    int picture_size_w = floodmap_size_w + border_scale_w + border;
    int picture_size_h = floodmap_size_h + 2*border;
    int floodmap_x1 = border;
    int floodmap_y1 = border;
    int floodmap_x2 = border + floodmap_size_w;
    int floodmap_y2 = border + floodmap_size_w;

    // шкала
    int scale_w = 15;
    int scale_x = floodmap_x2+indent+dash+indent;
    int scale_y = floodmap_y1;
    int scale_h = floodmap_size_h;
    //---

    QPixmap *pixmap = new QPixmap(picture_size_w,
                                  picture_size_h);
    if(!pixmap)
    {
        MyWidget::messagebox_critical("Ошибка", "Не могу создать картинку");
        return NULL;
    }

    //---
    QPainter painter;
    painter.begin(pixmap);

    painter.setPen(Qt::black);
    painter.fillRect(0, 0,
                     picture_size_w, picture_size_h,
                     Qt::black);

#if 0
    painter.setPen(Qt::red);
    painter.drawRect(floodmap_x1, floodmap_y1,
                     floodmap_size_w, floodmap_size_h);
#endif

    painter.setPen(Qt::white);

    painter.drawLine(floodmap_x1-indent,        floodmap_y1,
                     floodmap_x1-indent-dash,   floodmap_y1);
    painter.drawLine(floodmap_x1-indent,        floodmap_y1+floodmap_size_h/2,
                     floodmap_x1-indent-dash,   floodmap_y1+floodmap_size_h/2);
    painter.drawLine(floodmap_x1-indent,        floodmap_y2,
                     floodmap_x1-indent-dash,   floodmap_y2);

    painter.drawLine(floodmap_x2+indent,        floodmap_y1,
                     floodmap_x2+indent+dash,   floodmap_y1);
    painter.drawLine(floodmap_x2+indent,        floodmap_y1+floodmap_size_h/2,
                     floodmap_x2+indent+dash,   floodmap_y1+floodmap_size_h/2);
    painter.drawLine(floodmap_x2+indent,        floodmap_y2,
                     floodmap_x2+indent+dash,   floodmap_y2);

    painter.drawLine(floodmap_x1,   floodmap_y1-indent,
                     floodmap_x1,   floodmap_y1-indent-dash);
    painter.drawLine(floodmap_x1+floodmap_size_w/2,   floodmap_y1-indent,
                     floodmap_x1+floodmap_size_w/2,   floodmap_y1-indent-dash);
    painter.drawLine(floodmap_x2,   floodmap_y1-indent,
                     floodmap_x2,   floodmap_y1-indent-dash);

    painter.drawLine(floodmap_x1,   floodmap_y2+indent,
                     floodmap_x1,   floodmap_y2+indent+dash);
    painter.drawLine(floodmap_x1+floodmap_size_w/2,   floodmap_y2+indent,
                     floodmap_x1+floodmap_size_w/2,   floodmap_y2+indent+dash);
    painter.drawLine(floodmap_x2,   floodmap_y2+indent,
                     floodmap_x2,   floodmap_y2+indent+dash);

    // рисуем шкалу палитры
    if(pal == COLOR)
    {
        painter.setBrush(*color_brush);
    }
    else
    {
        painter.setBrush(*mono_brush);
    }

    painter.drawRect(scale_x,   scale_y,
                     scale_w,   scale_h);

    double inc = floodmap_size_h / 20.0;
    for(int n=0; n<=20; n++)
    {
        painter.drawLine(scale_x+scale_w,       scale_y+n*inc,
                         scale_x+scale_w+10,    scale_y+n*inc);
    }

    int temp = (maxValue / 10) * 10;
    if(maxValue % 10)
    {
        temp += 10;
    }

    double inc2 = floodmap_size_h / 10.0;
    double inc_value = temp / 10.0;
    for(int n=0; n<=10; n++)
    {
        double value = abs(n-10)*inc_value;
        painter.drawText(scale_x+scale_w+20,
                         scale_y+n*inc2+5,
                         QString("%1").arg((int)value));
    }
    //---

    QwtInterval in(0.0f, 1.0f);
    double value = 0;
    for(int y=0; y<floodmap_size_h; y++)
    {
        for(int x=0; x<floodmap_size_w; x++)
        {
            switch(pal)
            {
            case GRAY:
                if(maxCount > 0)
                {
                    c = (uint8_t)(0xFF * array[x][y] / maxCount);
                    painter.setPen(qRgb(c,c,c));
                    painter.drawPoint(floodmap_x1+x, floodmap_y1+y);
                }
                break;

            case COLOR:
                if(maxCount > 0)
                {
                    value = (double)array[x][y] / (double)maxCount;
                }
                if(colorMap)
                {
                    painter.setPen(colorMap->color(in, value).rgb());
                    painter.drawPoint(floodmap_x1+x, floodmap_y1+y);
                }
                else
                {
                    QString temp_str = "error colorMap";
                    emit error(temp_str);
                    MyWidget::messagebox_critical("Ошибка", temp_str);
                    return NULL;
                }
                break;

            default:
                MyWidget::messagebox_critical("Ошибка", QString("Неизвестная палитра %1").arg(pal));
                return NULL;
            }
        }
    }
    return pixmap;
}
//--------------------------------------------------------------------------------
pet_event Viewer::get_point(uint8_t  address,
                            uint8_t  Flags,
                            uint16_t x,
                            uint16_t y,
                            quint64  ts)
{
    pet_event data;

    data.src    = address;
    data.flags  = Flags;
    data.ts = ts;

#if 1
    // ORIG
    data.xp = x;
    data.xn = 0xFFFF - x;
    data.yp = y;
    data.yn = 0xFFFF - y;
#else
    //TODO
    int temp = rand() % 2048;   //(0xFFFF / 32);
    //int temp = 2047;   //(0xFFFF / 32);

    data.xp = x + temp;
    data.xn = 0xFFFF - x + temp;
    data.yp = y + temp;
    data.yn = 0xFFFF - y + temp;
#endif

    return data;
}
//--------------------------------------------------------------------------------
pet_event Viewer::get_point(uint8_t  address,
                            uint8_t  Flags,
                            uint16_t x,
                            uint16_t y,
                            uint16_t energy,
                            quint64  ts)
{
    pet_event data;

    data.src    = address;
    data.flags  = Flags;
    data.ts = ts;

    data.xp = x + energy;
    data.xn = 0xFFFF - x + energy;
    data.yp = y + energy;
    data.yn = 0xFFFF - y + energy;

    return data;
}
//--------------------------------------------------------------------------------
void Viewer::generate_test_circle(int event_count)
{
    QProgressDialog progress("generate_test_circle", "Отмена", 0, event_count, this);
    progress.setFixedWidth(MESSAGEBOX_WIDTH);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    int index = 0;
    progress.setValue(index);
    int inc = event_count / 100;
    quint64 ts = 0;
    for(int n=0; n<event_count; n++)
    {
        if((index % inc) == 0)
        {
            progress.setValue(index);
        }
        if (progress.wasCanceled())
        {
            MyWidget::messagebox_info("Информация", "Анализ данных прерван");
            return;
        }
        index ++;

        double xx = 0x8000 + double(25000.0)*qCos(double(n)*double(M_PI)/double(180.0));
        double yy = 0x8000 + double(25000.0)*qSin(double(n)*double(M_PI)/double(180.0));
        xx += (rand() % 5000);
        yy += (rand() % 1000);

        //TODO dat3->get_event(get_point(0, 0, xx, yy, ts));
        ts += 10;
    }
    progress.setValue(event_count);
}
//--------------------------------------------------------------------------------
void Viewer::generate_test_square(int max_point,
                                  int event_count)
{
    if(max_point > MAX_POINT)
    {
        MyWidget::messagebox_critical("Ошибка", "Не могу создать картинку");
        return;
    }

    QProgressDialog progress("generate_test_circle", "Отмена", 0, event_count, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    int index = 0;
    int temp = 0xFFFF / max_point;
    progress.setValue(index);
    int inc = event_count / 100;
    quint64 ts = 0;
    while(index<event_count)
    {
        if((index % inc) == 0)
        {
            progress.setValue(index);
        }
        if (progress.wasCanceled())
        {
            MyWidget::messagebox_info("Информация", "generate_test_circle прерван");
            return;
        }
        index ++;

        for(int y=0; y<max_point; y++)
        {
            for(int x=0; x<max_point; x++)
            {
                int xx = x * temp + temp / 2;
                int yy = y * temp + temp / 2;

                xx += rand() % 100;
                yy += rand() % 100;

                //TODO dat3->get_event(get_point(0, 0, xx, yy, ts));
                ts += 10;
            }
        }
        index++;
    }
    progress.setValue(event_count);
}
//--------------------------------------------------------------------------------
bool Viewer::set_point(int x, int y, int value)
{
    if(x < 0)       return false;
    if(y < 0)       return false;
    if(x > floodmap_size_h)   return false;
    if(y > floodmap_size_h)   return false;

    array[x][y] = value;
    return true;
}
//--------------------------------------------------------------------------------
bool Viewer::set_floodmap_size(int value)
{
    emit debug(QString("set_floodmap_size(%1)").arg(value));
    floodmap_size_w = value;
    floodmap_size_h = value;

    init_color_gradient();
    init_mono_gradient();

    return true;
}
//--------------------------------------------------------------------------------
bool Viewer::clear_all(void)
{
    for(int y=0; y<MAX_SIZE_Y; y++)
    {
        for(int x=0; x<MAX_SIZE_X; x++)
        {
            array[x][y] = 0;
        }
    }

    floodmap_size_w = DEFAULT_SIZE_PICTURE;
    floodmap_size_h = DEFAULT_SIZE_PICTURE;

    return true;
}
//--------------------------------------------------------------------------------
void Viewer::init_color_gradient(void)
{
    color_gradient = new QLinearGradient(0, floodmap_size_h, 0, 0);

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
void Viewer::init_mono_gradient(void)
{
    mono_gradient = new QLinearGradient(0, floodmap_size_h, 0, 0);

    mono_brush = new QBrush(*mono_gradient);
}
//--------------------------------------------------------------------------------
