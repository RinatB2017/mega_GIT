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
#include "show_hsv_color.hpp"
#include "ui_show_hsv_color.h"
//--------------------------------------------------------------------------------
Show_HSV_color::Show_HSV_color(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Show_HSV_color)
{
    init();
}
//--------------------------------------------------------------------------------
Show_HSV_color::~Show_HSV_color()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void Show_HSV_color::init(void)
{
    ui->setupUi(this);

    ui->sl_H->setRange(0, 255);
    ui->sl_S->setRange(0, 255);
    ui->sl_V->setRange(0, 255);

    ui->sl_R->setRange(0, 255);
    ui->sl_G->setRange(0, 255);
    ui->sl_B->setRange(0, 255);

    connect(ui->sl_H,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->lcd_H,  static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));
    connect(ui->sl_S,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->lcd_S,  static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));
    connect(ui->sl_V,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->lcd_V,  static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    connect(ui->sl_R,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->lcd_R,  static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));
    connect(ui->sl_G,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->lcd_G,  static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));
    connect(ui->sl_B,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->lcd_B,  static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    connect(ui->sl_H,   &QSlider::sliderMoved, this,   &Show_HSV_color::update_color_HSV);
    connect(ui->sl_S,   &QSlider::sliderMoved, this,   &Show_HSV_color::update_color_HSV);
    connect(ui->sl_V,   &QSlider::sliderMoved, this,   &Show_HSV_color::update_color_HSV);

    connect(ui->sl_R,   &QSlider::sliderMoved, this,   &Show_HSV_color::update_color_RGB);
    connect(ui->sl_G,   &QSlider::sliderMoved, this,   &Show_HSV_color::update_color_RGB);
    connect(ui->sl_B,   &QSlider::sliderMoved, this,   &Show_HSV_color::update_color_RGB);

    //---
#if 1
    QList<QLCDNumber *> l_lcd = findChildren<QLCDNumber *>();
    foreach (QLCDNumber *lcd, l_lcd)
    {
        lcd->setStyleSheet("background:black; color:lightgreen;");
    }
#endif
    //---

    load_widgets();

    update_color_HSV();
}
//--------------------------------------------------------------------------------
void Show_HSV_color::update_color_HSV(void)
{
    int h = ui->sl_H->value();
    int s = ui->sl_S->value();
    int v = ui->sl_V->value();

    QColor color = QColor::fromHsv(h,
                                   s,
                                   v);

    ui->color_widget->set_color(color);

    ui->sl_R->setValue(color.red());
    ui->sl_G->setValue(color.green());
    ui->sl_B->setValue(color.blue());
}
//--------------------------------------------------------------------------------
void Show_HSV_color::update_color_RGB(void)
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
void Show_HSV_color::set_color(QColor color)
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
}
//--------------------------------------------------------------------------------
void Show_HSV_color::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Show_HSV_color::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Show_HSV_color::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Show_HSV_color::save_setting(void)
{

}
//--------------------------------------------------------------------------------
