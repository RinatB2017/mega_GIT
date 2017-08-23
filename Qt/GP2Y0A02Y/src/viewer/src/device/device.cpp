/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "device.hpp"
#include "ui_device.h"
//--------------------------------------------------------------------------------
Device::Device(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Device)
{
    init();
}
//--------------------------------------------------------------------------------
Device::~Device()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Device::init(void)
{
    ui->setupUi(this);

    //show_value(-1);
    setFixedWidth(100);
}
//--------------------------------------------------------------------------------
void Device::show_value(int value)
{
    this->value = value;

    ui->label->setText(QString("%1").arg(this->value));

    /*
    0xFFFF  255
    x       y
    */

#if 1
    float max_value = 500;
    uint8_t R = (float)value * 255.0 / max_value + 0.5;
#else
    uint8_t R = value;
#endif
    uint8_t G = 0;
    uint8_t B = 0;
    ui->label->setStyleSheet(QString("background:#%1%2%3")
            .arg(R, 2, 16, QChar('0'))
            .arg(G, 2, 16, QChar('0'))
            .arg(B, 2, 16, QChar('0')));
    ui->label->setToolTip(QString("%1").arg(value));
}
//--------------------------------------------------------------------------------
