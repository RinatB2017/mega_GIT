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
#include "select_hsv_color.hpp"
#include "ui_select_hsv_color.h"
//--------------------------------------------------------------------------------
#define MIN_H 0
#define MIN_S 0
#define MIN_V 0

#define MAX_H 359
#define MAX_S 255
#define MAX_V 255
//--------------------------------------------------------------------------------
Select_HSV_color::Select_HSV_color(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Select_HSV_color)
{
    init();
}
//--------------------------------------------------------------------------------
Select_HSV_color::~Select_HSV_color()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Select_HSV_color::init(void)
{
    ui->setupUi(this);

    ui->sl_H->setRange(MIN_H, MAX_H);
    ui->sl_S->setRange(MIN_S, MAX_S);
    ui->sl_V->setRange(MIN_V, MAX_V);

    ui->sl_R->setRange(0, 255);
    ui->sl_G->setRange(0, 255);
    ui->sl_B->setRange(0, 255);

    connect(ui->sl_H,   &QSlider::valueChanged, this,   &Select_HSV_color::show_H);
    connect(ui->sl_S,   &QSlider::valueChanged, this,   &Select_HSV_color::show_S);
    connect(ui->sl_V,   &QSlider::valueChanged, this,   &Select_HSV_color::show_V);

    connect(ui->sl_R,   &QSlider::valueChanged, this,   &Select_HSV_color::show_R);
    connect(ui->sl_G,   &QSlider::valueChanged, this,   &Select_HSV_color::show_G);
    connect(ui->sl_B,   &QSlider::valueChanged, this,   &Select_HSV_color::show_B);

    connect(ui->sl_H,   &QSlider::sliderMoved,  this,   &Select_HSV_color::update_color_HSV);
    connect(ui->sl_S,   &QSlider::sliderMoved,  this,   &Select_HSV_color::update_color_HSV);
    connect(ui->sl_V,   &QSlider::sliderMoved,  this,   &Select_HSV_color::update_color_HSV);

    connect(ui->sl_R,   &QSlider::sliderMoved,  this,   &Select_HSV_color::update_color_RGB);
    connect(ui->sl_G,   &QSlider::sliderMoved,  this,   &Select_HSV_color::update_color_RGB);
    connect(ui->sl_B,   &QSlider::sliderMoved,  this,   &Select_HSV_color::update_color_RGB);

    connect(ui->btn_ok,     &QPushButton::clicked,  this,   &Select_HSV_color::accept);
    connect(ui->btn_cancel, &QPushButton::clicked,  this,   &Select_HSV_color::reject);

    ui->btn_ok->setIcon(QIcon::fromTheme("dialog-ok"));
    ui->btn_cancel->setIcon(QIcon::fromTheme("dialog-cancel"));

    ui->color_label->installEventFilter(this);

    QList<QLCDNumber *> l_lcd = findChildren<QLCDNumber *>();
    foreach (QLCDNumber *lcd, l_lcd)
    {
        lcd->setStyleSheet("background:black; color:lightgreen;");
    }

    QTimer::singleShot(0, [this]{
        update_image();
        update_color_HSV();
    });
}
//--------------------------------------------------------------------------------
void Select_HSV_color::update_image(void)
{
    int max_x = ui->color_label->width();
    int max_y = ui->color_label->height();
    double corr = 359.0 / max_x;
    QImage *image = new QImage(max_x, max_y, QImage::Format_RGB32);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            int hue = x * corr; //TODO получаем корректный тон (ширина ведь может быть больше 359)
            QColor color = QColor::fromHsv(hue, 200, 200);
            image->setPixelColor(x, y, color);
        }
    }
    ui->color_label->setPixmap(QPixmap::fromImage(*image));
}
//--------------------------------------------------------------------------------
void Select_HSV_color::update_color_HSV(void)
{
    int h = ui->sl_H->value();
    int s = ui->sl_S->value();
    int v = ui->sl_V->value();

    QColor color = QColor::fromHsv(h, s, v);

    ui->color_widget->set_color(color);

    ui->sl_R->setValue(color.red());
    ui->sl_G->setValue(color.green());
    ui->sl_B->setValue(color.blue());
}
//--------------------------------------------------------------------------------
void Select_HSV_color::update_color_RGB(void)
{
    QColor color = QColor::fromRgb(ui->sl_R->value(),
                                   ui->sl_G->value(),
                                   ui->sl_B->value());

    ui->color_widget->set_color(color);

    int h = 0;
    int s = 0;
    int v = 0;
    color.getHsv(&h, &s, &v);

    ui->sl_H->setValue(h);
    ui->sl_S->setValue(s);
    ui->sl_V->setValue(v);
}
//--------------------------------------------------------------------------------
void Select_HSV_color::set_color(QColor color)
{
    int h = 0;
    int s = 0;
    int v = 0;
    color.getHsv(&h, &s, &v);

    ui->sl_H->setValue(h);
    ui->sl_S->setValue(s);
    ui->sl_V->setValue(v);

    ui->sl_R->setValue(color.red());
    ui->sl_G->setValue(color.green());
    ui->sl_B->setValue(color.blue());

    update_color_RGB();
}
//--------------------------------------------------------------------------------
QColor Select_HSV_color::get_color(void)
{
    return ui->color_widget->get_color();
}
//--------------------------------------------------------------------------------
void Select_HSV_color::show_H(int value)
{
    ui->lcd_H->display(value);
    ui->lcd_H_hex->display(QString("%1").arg(value, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
void Select_HSV_color::show_S(int value)
{
    ui->lcd_S->display(value);
    ui->lcd_S_hex->display(QString("%1").arg(value, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
void Select_HSV_color::show_V(int value)
{
    ui->lcd_V->display(value);
    ui->lcd_V_hex->display(QString("%1").arg(value, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
void Select_HSV_color::show_R(int value)
{
    ui->lcd_R->display(value);
    ui->lcd_R_hex->display(QString("%1").arg(value, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
void Select_HSV_color::show_G(int value)
{
    ui->lcd_G->display(value);
    ui->lcd_G_hex->display(QString("%1").arg(value, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
void Select_HSV_color::show_B(int value)
{
    ui->lcd_B->display(value);
    ui->lcd_B_hex->display(QString("%1").arg(value, 2, 16, QChar('0')));
}
//--------------------------------------------------------------------------------
bool Select_HSV_color::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->color_label)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            int x = ((QMouseEvent *)event)->position().x();
            int y = ((QMouseEvent *)event)->position().y();
#ifdef Q_OS_LINUX
            QImage image = ui->color_label->pixmap(Qt::ReturnByValue).toImage();
#endif
#ifdef Q_OS_WIN
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            const QImage image = ui->color_label->pixmap()->toImage();
#else
            const QImage image = ui->color_label->pixmap().toImage();
#endif
#endif
            if((x <= image.width() && (y <= image.height())))
            {
                QColor color = image.pixelColor(x, y);
                set_color(color);
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void Select_HSV_color::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Select_HSV_color::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Select_HSV_color::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Select_HSV_color::save_setting(void)
{

}
//--------------------------------------------------------------------------------
