/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "knob_lcd.hpp"
#include "ui_knob_lcd.h"
//--------------------------------------------------------------------------------
Knob_lcd::Knob_lcd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Knob_lcd)
{
    init();
}
//--------------------------------------------------------------------------------
Knob_lcd::~Knob_lcd()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Knob_lcd::init(void)
{
    ui->setupUi(this);

    connect(ui->knob,   SIGNAL(valueChanged(double)),
            ui->dislay, SLOT(display(double)));

#if 0
    connect(ui->knob,   &QwtKnob::valueChanged,
            ui->dislay, &QLCDNumber::display);
#endif
}
//--------------------------------------------------------------------------------
void Knob_lcd::resizeEvent(QResizeEvent *event)
{
#if 1
    int w = event->size().width();
    int h = event->size().height();

    int d_h = h / 3;

    ui->dislay->resize(w, d_h);
    ui->knob->resize(h - d_h, h - d_h);
    ui->knob->move(w / 2 - (h - d_h) / 2, d_h);
#endif
}
//--------------------------------------------------------------------------------
