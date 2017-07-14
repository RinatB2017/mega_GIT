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
#include <QSpinBox>
//--------------------------------------------------------------------------------
#include "calibration_param.hpp"
#include "ui_calibration_param.h"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Calibration_param::Calibration_param(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Calibration_param)
{
    init();
}
//--------------------------------------------------------------------------------
Calibration_param::~Calibration_param()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Calibration_param::init(void)
{
    ui->setupUi(this);

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
