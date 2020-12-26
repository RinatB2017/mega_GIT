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

    ui->spHue->setRange(0, 0xFF);
    ui->spSaturation->setRange(0, 0xFF);
    ui->spValue->setRange(0, 0xFF);

    ui->slHue->setRange(0, 0xFF);
    ui->slSaturation->setRange(0, 0xFF);
    ui->slValue->setRange(0, 0xFF);

    connect(ui->spHue,          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slHue,          &QSlider::setValue);
    connect(ui->spSaturation,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slSaturation,   &QSlider::setValue);
    connect(ui->spValue,        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slValue,        &QSlider::setValue);

    connect(ui->slHue,          static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spHue,          &QSpinBox::setValue);
    connect(ui->slSaturation,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spSaturation,   &QSpinBox::setValue);
    connect(ui->slValue,        static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spValue,        &QSpinBox::setValue);

    connect(ui->slHue,          static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    this,                   &Show_HSV_color::update_color);
    connect(ui->slSaturation,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    this,                   &Show_HSV_color::update_color);
    connect(ui->slValue,        static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    this,                   &Show_HSV_color::update_color);

    load_widgets();

    update_color();
}
//--------------------------------------------------------------------------------
void Show_HSV_color::update_color(int value)
{
    QColor color = QColor::fromHsv(ui->slHue->value(),
                                   ui->slSaturation->value(),
                                   ui->slValue->value());

    ui->color_widget->set_color(color);
    Q_UNUSED(value);
}
//--------------------------------------------------------------------------------
void Show_HSV_color::set_color(QColor color)
{
    int h = 0;
    int s = 0;
    int v = 0;
    color.getHsv(&h, &s, &v);

    ui->slHue->setValue(h);
    ui->slSaturation->setValue(s);
    ui->slValue->setValue(v);
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
