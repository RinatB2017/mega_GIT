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
#include "viewer.hpp"
#include "ui_viewer.h"
//--------------------------------------------------------------------------------
Viewer::Viewer(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Viewer)
{
    init();
}
//--------------------------------------------------------------------------------
Viewer::~Viewer()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Viewer::init(void)
{
    ui->setupUi(this);

    devices.append(ui->frame_0);
    devices.append(ui->frame_1);
    devices.append(ui->frame_2);
    devices.append(ui->frame_3);
    devices.append(ui->frame_4);
    devices.append(ui->frame_5);
    devices.append(ui->frame_6);
    devices.append(ui->frame_7);

    emit info("Viewer starting");
}
//--------------------------------------------------------------------------------
bool Viewer::set_value(int channel, int value)
{
    emit debug("Viewer: set_value");

    if(channel < 0)
    {
        emit error("Viewer: channel < 0");
        return false;
    }
    if(channel > MAX_CHANNEL)
    {
        emit error(QString("Viewer: channel > MAX_CHANNEL (%1)").arg(channel));
        return false;
    }

    devices[channel]->show_value(value);
    return true;
}
//--------------------------------------------------------------------------------
void Viewer::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
