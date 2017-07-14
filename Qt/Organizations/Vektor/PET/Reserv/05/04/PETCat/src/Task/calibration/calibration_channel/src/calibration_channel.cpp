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
#include <QPushButton>
//--------------------------------------------------------------------------------
#include "calibration_channel.hpp"
#include "ui_calibration_channel.h"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Calibration_channel::Calibration_channel(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Calibration_channel)
{
    init();
}
//--------------------------------------------------------------------------------
Calibration_channel::~Calibration_channel()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Calibration_channel::init(void)
{
    ui->setupUi(this);

    ui->sb_K_xp->setRange(0, 0xFFFF);
    ui->sb_K_xn->setRange(0, 0xFFFF);
    ui->sb_K_yp->setRange(0, 0xFFFF);
    ui->sb_K_yn->setRange(0, 0xFFFF);
    ui->sb_B_xp->setRange(0, 0xFFFF);
    ui->sb_B_xn->setRange(0, 0xFFFF);
    ui->sb_B_yp->setRange(0, 0xFFFF);
    ui->sb_B_yn->setRange(0, 0xFFFF);

    //---
    QList<QPushButton *> all_pb = findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pb)
    {
        obj->setDisabled(true);
        obj->setToolTip(TODODO);
    }
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        obj->setDisabled(true);
        obj->setToolTip(TODODO);
    }
    //---
}
//--------------------------------------------------------------------------------
