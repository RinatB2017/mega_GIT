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

    ui->spHueFrom->setRange(0, 0xFF);
    ui->spSaturationFrom->setRange(0, 0xFF);
    ui->spValueFrom->setRange(0, 0xFF);

    ui->slHueFrom->setRange(0, 0xFF);
    ui->slSaturationFrom->setRange(0, 0xFF);
    ui->slValueFrom->setRange(0, 0xFF);

    connect(ui->spHueFrom,          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slHueFrom,          &QSlider::setValue);
    connect(ui->spSaturationFrom,   static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slSaturationFrom,   &QSlider::setValue);
    connect(ui->spValueFrom,        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  ui->slValueFrom,        &QSlider::setValue);

    connect(ui->slHueFrom,          static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spHueFrom,          &QSpinBox::setValue);
    connect(ui->slSaturationFrom,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spSaturationFrom,   &QSpinBox::setValue);
    connect(ui->slValueFrom,        static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    ui->spValueFrom,        &QSpinBox::setValue);

    connect(ui->slHueFrom,          static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    this,                   &Show_HSV_color::update_color);
    connect(ui->slSaturationFrom,   static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    this,                   &Show_HSV_color::update_color);
    connect(ui->slValueFrom,        static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),    this,                   &Show_HSV_color::update_color);

    load_widgets();

    update_color();
}
//--------------------------------------------------------------------------------
void Show_HSV_color::update_color(int value)
{
    QColor color = QColor::fromHsv(ui->slHueFrom->value(),
                                   ui->slSaturationFrom->value(),
                                   ui->slValueFrom->value());

    ui->color_widget->set_color(color);
    Q_UNUSED(value);
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
