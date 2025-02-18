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
#include "histogram_options.hpp"
#include "ui_histogram_options.h"
//--------------------------------------------------------------------------------
Histogram_Options::Histogram_Options(double min_axis_X,
                                       double max_axis_X,
                                       double min_axis_Y,
                                       double max_axis_Y,
                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram_Options),
    min_axis_X(min_axis_X),
    max_axis_X(max_axis_X),
    min_axis_Y(min_axis_Y),
    max_axis_Y(max_axis_Y)
{
    init();
}
//--------------------------------------------------------------------------------
Histogram_Options::~Histogram_Options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Histogram_Options::init(void)
{
    ui->setupUi(this);

    setWindowTitle(tr("options"));

    ui->sb_maxX->setMinimum(-INT_MAX);
    ui->sb_maxX->setMaximum(INT_MAX);
    ui->sb_maxY->setMinimum(-INT_MAX);
    ui->sb_maxY->setMaximum(INT_MAX);

    ui->sb_minX->setValue(min_axis_X);
    ui->sb_maxX->setValue(max_axis_X);
    ui->sb_minY->setValue(min_axis_Y);
    ui->sb_maxY->setValue(max_axis_Y);

    setFixedSize(sizeHint());

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
//--------------------------------------------------------------------------------
int Histogram_Options::get_min_axis_x(void)
{
    return ui->sb_minX->value();
}
//--------------------------------------------------------------------------------
int Histogram_Options::get_max_axis_x(void)
{
    return ui->sb_maxX->value();
}
//--------------------------------------------------------------------------------
int Histogram_Options::get_min_axis_y(void)
{
    return ui->sb_minY->value();
}
//--------------------------------------------------------------------------------
int Histogram_Options::get_max_axis_y(void)
{
    return ui->sb_maxY->value();
}
//--------------------------------------------------------------------------------
