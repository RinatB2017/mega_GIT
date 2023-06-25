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
#include "kaleidoscope_image.hpp"
//--------------------------------------------------------------------------------
KaleidoscopeImage::KaleidoscopeImage(QWidget *parent) :
    QWidget(parent)
{
    mask_image = create_mask();
#ifdef QT_DEBUG
    mask_image.save("/dev/shm/mask_image.png", "PNG");
#endif

    piece_image = QImage(piece_w, piece_h, QImage::Format_ARGB32);;
}
//--------------------------------------------------------------------------------
KaleidoscopeImage::~KaleidoscopeImage()
{

}
//--------------------------------------------------------------------------------
void KaleidoscopeImage::set_param(int new_w, int new_h, int new_angle)
{
    image_w = new_w;
    image_h = new_h;
    piece_angle = new_angle;

#ifdef H_MASH
    piece_w = (qreal)image_w / 2.0 + 150.0;
    piece_h = (qreal)piece_w * qTan(qDegreesToRadians(piece_angle / 2.0 + 0.5)) * 2.0;
#else
    piece_h = (qreal)image_h / 2.0 + 150.0;
    piece_w = (qreal)piece_h * qTan(qDegreesToRadians(piece_angle / 2.0 + 0.5)) * 2.0;
#endif

    qDebug() << "piece_w:" << piece_w;
    qDebug() << "piece_h:" << piece_h;

    piece_image = QImage(piece_w, piece_h, QImage::Format_ARGB32);;

    mask_image = create_mask();
#ifdef QT_DEBUG
    mask_image.save("/dev/shm/mask_image.png", "PNG");
#endif
}
//--------------------------------------------------------------------------------
#if 1
void KaleidoscopeImage::preparation_image(QImage image)
{
    Q_ASSERT(!image.isNull());
    Q_ASSERT(!mask_image.isNull());
    Q_ASSERT(piece_w);
    Q_ASSERT(piece_h);

    //QImage result(piece_w, piece_h, QImage::Format_ARGB32);
#if 0
    //TODO пробуем быстрее сделать
    uchar *mask_bits   = mask_image.bits();
    uchar *image_bits  = image.bits();
    uchar *result_bits = piece_image.bits();

    int max_n = piece_w * piece_h * 4;
    for(int n=0; n<max_n; n++)
    {
        *result_bits++ = *image_bits++ & *mask_bits++;
    }
#endif

    for(int y=0; y<piece_h; y++)
    {
        for(int x=0; x<piece_w; x++)
        {
            Q_ASSERT(mask_image.valid(x, y));
            if(mask_image.valid(x, y))
            {
                uint rgb = mask_image.pixel(x, y);
                if(rgb & 0xFF)
                {
                    piece_image.setPixel(x, y, 0);
                }
                else
                {
                    piece_image.setPixel(x, y, image.pixel(x, y));
                }
            }
        }
    }
}
#else
#include <QDebug>
void KaleidoscopeImage::preparation_image(QImage image)
{
    Q_ASSERT(!image.isNull());
    Q_ASSERT(!mask_image.isNull());
    Q_ASSERT(piece_w);
    Q_ASSERT(piece_h);

    QPainterPath m_path_1;
    QPainterPath m_path_2;
#ifdef H_MASH
    m_path_1.moveTo(0, piece_h / 2);
    m_path_1.lineTo(piece_w, piece_h);
    m_path_1.lineTo(piece_w, 0);
    m_path_1.lineTo(0, piece_h / 2);

    m_path_2.moveTo(0, piece_h / 2);
    m_path_2.lineTo(piece_w, piece_h);
    m_path_2.lineTo(piece_w, 0);
    m_path_2.lineTo(0, piece_h / 2);
#else
    m_path_1.moveTo(piece_w / 2, piece_h);
    m_path_1.lineTo(piece_w, piece_h);
    m_path_1.lineTo(piece_w, 0);
    m_path_1.lineTo(piece_w / 2, piece_h);

    m_path_2.moveTo(piece_w / 2, piece_h);
    m_path_2.lineTo(0, 0);
    m_path_2.lineTo(0, piece_h);
    m_path_2.lineTo(piece_w / 2, piece_h);
#endif
    piece_image = image;
    QPainter p(&piece_image);

    // так почему то не работает
    p.fillPath(m_path_1, QBrush(QColor(Qt::transparent)));
    p.fillPath(m_path_2, QBrush(QColor(Qt::transparent)));

    image.save("/dev/shm/_image.png");
    mask_image.save("/dev/shm/_mask_image.png");
    piece_image.save("/dev/shm/_piece_image.png");

    qDebug() << "xxx";
}
#endif
//--------------------------------------------------------------------------------
bool KaleidoscopeImage::load_image(const QImage &new_image)
{
    Q_ASSERT(!new_image.isNull());

    full_image = new_image;
#ifdef QT_DEBUG
    full_image.save("/dev/shm/full_image.png", "PNG");
#endif

    return true;
}
//--------------------------------------------------------------------------------
QImage KaleidoscopeImage::create_mask(void)
{
    //создаем маску
    QPainterPath m_path;
#ifdef H_MASH
    m_path.moveTo(0, piece_h / 2);
    m_path.lineTo(piece_w, piece_h);
    m_path.lineTo(piece_w, 0);
    m_path.lineTo(0, piece_h / 2);
#else
    m_path.moveTo(piece_w / 2, 0);
    m_path.lineTo(0, piece_h);
    m_path.lineTo(piece_w, piece_h);
    m_path.lineTo(piece_w / 2, 0);
#endif

    // маска
    QImage mask(piece_w, piece_h, QImage::Format_ARGB32);
    QPainter p(&mask);
    p.fillRect(0, 0, piece_w, piece_h, QColor(Qt::color0));
    p.fillPath(m_path, QBrush(Qt::color1));

#ifdef QT_DEBUG
    mask_image.save("/dev/shm/mask_image.png", "PNG");
#endif

    return mask;
}
//--------------------------------------------------------------------------------
QImage KaleidoscopeImage::create_mask(int dir)
{
    //создаем маску
    QPainterPath m_path;

    switch (dir)
    {
    case 0:     // left
        m_path.moveTo(0, piece_h / 2);
        m_path.lineTo(piece_w, piece_h);
        m_path.lineTo(piece_w, 0);
        m_path.lineTo(0, piece_h / 2);
        break;

    case 1:     // right
        m_path.moveTo(0, 0);
        m_path.lineTo(0, piece_h);
        m_path.lineTo(piece_w, piece_h / 2);
        m_path.lineTo(0, 0);
        break;

    case 2:     // up
        m_path.moveTo(piece_w / 2, 0);
        m_path.lineTo(0, piece_h);
        m_path.lineTo(piece_w, piece_h);
        m_path.lineTo(piece_w / 2, 0);
        break;

    case 3:     // down
        m_path.moveTo(piece_w / 2, piece_h);
        m_path.lineTo(0, 0);
        m_path.lineTo(piece_w, 0);
        m_path.lineTo(piece_w / 2, piece_h);
        break;

    default:
        return QImage();
    }

    // маска
    QImage mask(piece_w, piece_h, QImage::Format_ARGB32);
    QPainter p(&mask);
    p.fillRect(0, 0, piece_w, piece_h, QColor(Qt::color0));
    p.fillPath(m_path, QBrush(Qt::color1));

#ifdef QT_DEBUG
    mask_image.save("/dev/shm/mask_image.png", "PNG");
#endif

    return mask;
}
//--------------------------------------------------------------------------------
int KaleidoscopeImage::get_piece_w(void)
{
    return piece_w;
}
//--------------------------------------------------------------------------------
int KaleidoscopeImage::get_piece_h(void)
{
    return piece_h;
}
//--------------------------------------------------------------------------------
void KaleidoscopeImage::set_piece_w(int new_piece_w)
{
    piece_w = new_piece_w;
}
//--------------------------------------------------------------------------------
void KaleidoscopeImage::set_piece_h(int new_piece_h)
{
    piece_h = new_piece_h;
}
//--------------------------------------------------------------------------------
#include <QDebug>

QImage KaleidoscopeImage::get_image(int pos_x, int pos_y)
{
    //QTransform tr;
    //tr.rotate(-90.0);

    preparation_image(full_image.copy(pos_x, pos_y, piece_w, piece_h));
    return piece_image;
}
//--------------------------------------------------------------------------------
