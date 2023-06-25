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
#include "kaleidoscope_widget.hpp"
//--------------------------------------------------------------------------------
KaleidoscopeWidget::KaleidoscopeWidget(QWidget *parent) :
    QWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
KaleidoscopeWidget::~KaleidoscopeWidget()
{
    if(image_widget)
    {
        delete image_widget;
    }

#ifdef QT_DEBUG
    if(label)
    {
        delete label;
    }
#endif
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::init(void)
{
    init_kaleidoscope_widget();
    //f_test();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::init_kaleidoscope_widget(void)
{
    QImage test_image = create_test_image();
    Q_ASSERT(!test_image.isNull());
#ifdef QT_DEBUG
    test_image.save("/dev/shm/image.png", "PNG");
#endif

#if 0
    label = new QLabel();
    label->setPixmap(QPixmap::fromImage(test_image));
    label->setFixedSize(test_image.width(), test_image.height());
    label->show();
#endif

    image_widget = new KaleidoscopeImage(this);
    image_widget->set_param(widget_w, widget_h, piece_angle);
    image_widget->load_image(test_image);
#ifdef QT_DEBUG
    image_widget->get_image(0, 0).save("/dev/shm/image_widget.png", "PNG");
#endif

    result_image = QImage(widget_w, widget_h, QImage::Format_ARGB32);
    //painter.begin(&result_image);
}
//--------------------------------------------------------------------------------
QImage KaleidoscopeWidget::create_test_image(void)
{
    //TODO test
    QImage image(widget_w, widget_h, QImage::Format_ARGB32);
    QPainter p(&image);
    p.fillRect(0, 0, widget_w, widget_h, QColor(Qt::white));

    bool flag = false;
    int pos_x = 0;
    int d_color = 10;

    int pos_y = 0;
    while(pos_x < widget_h)
    {
        flag = !flag;
        if(flag)
            p.fillRect(pos_x, pos_y, d_color, widget_w, QBrush(Qt::white));
        else
            p.fillRect(pos_x, pos_y, d_color, widget_w, QBrush(Qt::black));

        pos_x += d_color;
    }

#ifdef QT_DEBUG
    image.save("/dev/shm/test_image.png", "PNG");
#endif

    return image;
}

//--------------------------------------------------------------------------------
bool KaleidoscopeWidget::load_image(const QImage &new_image)
{
    Q_ASSERT(!new_image.isNull());
    qInfo() << "load_image" << new_image.width() << new_image.height();

    return image_widget->load_image(new_image);
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::set_pos(int x, int y)
{
    pos_x = x;
    pos_y = y;
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::get_pos(int *x, int *y)
{
    *x = pos_x;
    *y = pos_y;
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::get_size(int *w, int *h)
{
    *w = widget_w;
    *h = widget_h;
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::set_size(int w, int h)
{
    widget_w = w;
    widget_h = h;

    image_widget->set_param(widget_w, widget_h, piece_angle);

    result_image = QImage(widget_w, widget_h, QImage::Format_ARGB32);
    painter.begin(&result_image);
}
//--------------------------------------------------------------------------------
QImage KaleidoscopeWidget::get_image(void)
{
    return result_image;
}
//--------------------------------------------------------------------------------
int KaleidoscopeWidget::get_piece_w(void)
{
    return image_widget->get_piece_w();
}
//--------------------------------------------------------------------------------
int KaleidoscopeWidget::get_piece_h(void)
{
    return image_widget->get_piece_h();
}
//--------------------------------------------------------------------------------
qreal KaleidoscopeWidget::get_piece_angle(void)
{
    return piece_angle;
}
//--------------------------------------------------------------------------------
void  KaleidoscopeWidget::set_piece_angle(qreal new_angle)
{
    piece_angle = new_angle;
    image_widget->set_param(widget_w, widget_h, piece_angle);
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_to(int x, int y)
{
    pos_x = x;
    pos_y = y;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_update(void)
{
    QCoreApplication::processEvents();

    //TODO background
    painter.fillRect(0, 0, widget_w, widget_w, QColor(Qt::transparent));
    //painter.fillRect(0, 0, widget_w, widget_w, QColor(Qt::white));

    painter.save();
    painter.translate(widget_w / 2.0, widget_w / 2.0);
    QImage piece_image = image_widget->get_image(pos_x, pos_y);
#ifdef H_MASH
    int piece_h = image_widget->get_piece_h();
#else
    int piece_w = image_widget->get_piece_w();
#endif
    painter.rotate(piece_angle / 2.0);
    for(int n=0; n<(360.0 / piece_angle); n++)
    {
#ifdef H_MASH
        painter.drawImage(0, -(piece_h / 2), piece_image);
#else
        painter.drawImage(-(piece_w / 2), 0, piece_image);
#endif
        painter.rotate(piece_angle);
    }
    painter.restore();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_lu(int inc)
{
    if(pos_x > inc)   pos_x-=inc;
    pos_y+=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_l(int inc)
{
    if(pos_x > inc)   pos_x-=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_ld(int inc)
{
    if(pos_x > inc)   pos_x-=inc;
    if(pos_y > inc)   pos_y-=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_u(int inc)
{
    if(pos_y > inc)   pos_y-=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_d(int inc)
{
    pos_y+=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_ru(int inc)
{
    pos_x+=inc;
    pos_y+=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_r(int inc)
{
    pos_x+=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_rd(int inc)
{
    pos_x+=inc;
    if(pos_y > inc)   pos_y-=inc;

    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeWidget::f_move_cancel(void)
{
    qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------
