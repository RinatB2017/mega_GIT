/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "showpicture.hpp"
//--------------------------------------------------------------------------------
ShowPicture::ShowPicture(QWidget *parent) :
    QLabel(parent)
{
    init();
}
//--------------------------------------------------------------------------------
ShowPicture::~ShowPicture()
{

}
//--------------------------------------------------------------------------------
void ShowPicture::init(void)
{

}
//--------------------------------------------------------------------------------
void ShowPicture::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        //qDebug() << "delete";
        int x = event->x();
        int y = event->y();
        int index = 0;
        bool found = false;
        foreach (QRubberBand *band, l_bands)
        {
            int x1 = band->x();
            int y1 = band->y();
            int x2 = x1 + band->width();
            int y2 = y1 + band->height();

            if((x>=x1) && (x<=x2) && (y>=y1) && (y<=y2))
            {
                //qDebug() << "FOUND";
                found = true;
                band->hide();
                break;
            }

            //qDebug() << x1 << y1 << x2 << y2;
            index++;
        }
        if(found)
        {
            //qDebug() << "remove" << index;
            l_bands.removeAt(index);
        }
        //qDebug() << "count" << l_bands.count();
        return;
    }

    flag_clicked = true;
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

#if 0
    //FIXME в винде проблема с фоном резинки, если стиль приложения в винде отличен от windows
    // надо подумать, как корректно исправить
    // в linux проблем нет с любым стилем
    QPalette palette;

//    palette.setBrush(QPalette::WindowText, QBrush(Qt::white));

//    QColor color(Qt::blue);
//    color.setAlpha(80);
//    palette.setBrush(QPalette::Highlight, QBrush(color));

    rubberBand->setStyleSheet("background:red;");

    palette.setColor(QPalette::Background, Qt::transparent);

    rubberBand->setPalette(palette);
#endif


    origin = static_cast<QMouseEvent *>(event)->pos();
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}
//--------------------------------------------------------------------------------
void ShowPicture::mouseReleaseEvent(QMouseEvent *)
{
    if(flag_clicked)
    {
        flag_clicked = false;
        rubberBand->setProperty("ID", l_bands.count());
        l_bands.append(rubberBand);
    }
}
//--------------------------------------------------------------------------------
void ShowPicture::mouseMoveEvent(QMouseEvent *event)
{
    if(!flag_clicked) return;
    rubberBand->setGeometry(QRect(origin, static_cast<QMouseEvent *>(event)->pos()).normalized());
}
//--------------------------------------------------------------------------------
QList<QRubberBand *> ShowPicture::get_rects(void)
{
    return l_bands;
}
//--------------------------------------------------------------------------------
bool ShowPicture::get_pixmap(uint x, uint y, uint w, uint h, QPixmap *pixmap)
{
    if(w == 0)  return false;
    if(h == 0)  return false;

    (*pixmap) = grab(QRect(x, y, w, h));
    return true;
}
//--------------------------------------------------------------------------------
void ShowPicture::test(void)
{
    foreach (QRubberBand *band, l_bands)
    {
        int x = band->x();
        int y = band->y();
        int w = band->width() * 2;
        int h = band->height() * 2;
        band->setGeometry(x, y, w, h);
    }
}
//--------------------------------------------------------------------------------
bool ShowPicture::correct(int x, int y, QRect rect)
{
    int x1 = rect.x();
    int y1 = rect.y();
    int x2 = x1 + rect.width();
    int y2 = y1 + rect.height();

    foreach (QRubberBand *band, l_bands)
    {
        if((x>=x1) && (x<=x2) && (y>=y1) && (y<=y2))
        {
            band->setGeometry(rect.x(),
                              rect.y(),
                              rect.width(),
                              rect.height());
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool ShowPicture::correct(int id, QRect rect)
{
    foreach (QRubberBand *band, l_bands)
    {
        if(band->property("ID").toInt() == id)
        {
            qDebug() << id << "found";
            band->setGeometry(rect.x(),
                              rect.y(),
                              rect.width(),
                              rect.height());
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
