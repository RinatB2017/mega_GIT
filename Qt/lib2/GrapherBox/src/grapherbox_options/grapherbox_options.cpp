/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <limits.h>
#include "grapherbox_options.hpp"
#include "ui_grapherbox_options.h"
#include "colorbutton.hpp"
//--------------------------------------------------------------------------------
GrapherBox_Options::GrapherBox_Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GrapherBox_Options)
{
    init();
}
//--------------------------------------------------------------------------------
GrapherBox_Options::~GrapherBox_Options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::init(void)
{
    ui->setupUi(this);

    setWindowTitle(tr("options"));

    ui->sb_maxX->setRange(-INT_MAX, INT_MAX);
    ui->sb_maxY->setRange(-INT_MAX, INT_MAX);

    //FIXME пока не сделано
    ui->rb_recorder->setEnabled(false);
    ui->rb_oscilloscope->setEnabled(false);
    ui->rb_recorder->setToolTip("Пока не сделано");
    ui->rb_oscilloscope->setToolTip("Пока не сделано");
    //---

    setFixedSize(sizeHint());

    connect(ui->buttonBox,  &QDialogButtonBox::accepted,
            this,           &GrapherBox_Options::accept);
    connect(ui->buttonBox,  &QDialogButtonBox::rejected,
            this,           &GrapherBox_Options::reject);
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::set_type_device(GrapherBox_Options::Device device)
{
    switch (device)
    {
    case GrapherBox_Options::RECORDER:
        ui->rb_recorder->setChecked(true);
        break;

    case GrapherBox_Options::OSCILLOSCOPE:
        ui->rb_oscilloscope->setChecked(true);
        break;
    }
}
//--------------------------------------------------------------------------------
GrapherBox_Options::Device GrapherBox_Options::get_type_device(void)
{
    if(ui->rb_recorder->isChecked())
    {
        return GrapherBox_Options::RECORDER;
    }
    if(ui->rb_oscilloscope->isChecked())
    {
        return GrapherBox_Options::OSCILLOSCOPE;
    }
    return GrapherBox_Options::RECORDER;
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::set_type_curve(GrapherBox_Options::TypeCurve type)
{
    switch (type)
    {
    case GrapherBox_Options::DOTS:
        ui->rb_dots->setChecked(true);
        break;

    case GrapherBox_Options::LINES:
        ui->rb_lines->setChecked(true);
        break;

    case GrapherBox_Options::SPLINE_LINES:
        ui->rb_spline_lines->setChecked(true);
        break;
    }
}
//--------------------------------------------------------------------------------
GrapherBox_Options::TypeCurve GrapherBox_Options::get_type_curve(void)
{
    if(ui->rb_dots->isChecked())
    {
        return GrapherBox_Options::DOTS;
    }
    if(ui->rb_lines->isChecked())
    {
        return GrapherBox_Options::LINES;
    }
    if(ui->rb_spline_lines->isChecked())
    {
        return GrapherBox_Options::SPLINE_LINES;
    }
    return GrapherBox_Options::DOTS;
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::set_min_axis_X(int value)
{
    ui->sb_minX->setValue(value);
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::set_max_axis_X(int value)
{
    ui->sb_maxX->setValue(value);
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::set_min_axis_Y(int value)
{
    ui->sb_minY->setValue(value);
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::set_max_axis_Y(int value)
{
    ui->sb_maxY->setValue(value);
}
//--------------------------------------------------------------------------------
int GrapherBox_Options::get_min_axis_x(void)
{
    return ui->sb_minX->value();
}
//--------------------------------------------------------------------------------
int GrapherBox_Options::get_max_axis_x(void)
{
    return ui->sb_maxX->value();
}
//--------------------------------------------------------------------------------
int GrapherBox_Options::get_min_axis_y(void)
{
    return ui->sb_minY->value();
}
//--------------------------------------------------------------------------------
int GrapherBox_Options::get_max_axis_y(void)
{
    return ui->sb_maxY->value();
}
//--------------------------------------------------------------------------------
void GrapherBox_Options::add_color_button(QColor color, QString text)
{
    ColorButton *btn = new ColorButton();
    Q_ASSERT(btn);

    btn->setColor(color);
    btn->setText(text);
    color_buttons.append(btn);

    ui->color_layout->addWidget(btn);
    cnt_channel++;
}
//--------------------------------------------------------------------------------
bool GrapherBox_Options::get_color(int channel, QColor *color)
{
    if(channel < 0)             return false;
    if(channel > cnt_channel)   return false;

    *color = color_buttons[channel]->getColor();
    return true;
}
//--------------------------------------------------------------------------------
bool GrapherBox_Options::get_text(int channel, QString *text)
{
    if(channel < 0)             return false;
    if(channel > cnt_channel)   return false;

    *text = color_buttons[channel]->getText();
    return true;
}
//--------------------------------------------------------------------------------
