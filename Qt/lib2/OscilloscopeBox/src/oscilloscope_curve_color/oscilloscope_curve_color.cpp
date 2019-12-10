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
#include "oscilloscope_curve_color.hpp"
#include "ui_oscilloscope_curve_color.h"
//--------------------------------------------------------------------------------
Oscilloscope_curve_color::Oscilloscope_curve_color(int ID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Oscilloscope_curve_color),
    id(ID)
{
    init();
}
//--------------------------------------------------------------------------------
Oscilloscope_curve_color::~Oscilloscope_curve_color()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Oscilloscope_curve_color::init(void)
{
    ui->setupUi(this);

    ui->btn_push->setText("curve");
    connect(ui->btn_color,  SIGNAL(clicked()),  this,   SLOT(set_color()));
}
//--------------------------------------------------------------------------------
void Oscilloscope_curve_color::set_color(QColor color)
{
    this->color = color;
    ui->btn_color->setStyleSheet(QString("background:rgb(%1,%2,%3);")
                                 .arg(color.red())
                                 .arg(color.green())
                                 .arg(color.blue()));
}
//--------------------------------------------------------------------------------
void Oscilloscope_curve_color::set_color(void)
{
    QColorDialog *dlg = new QColorDialog();
    dlg->setCurrentColor(color);

    int btn = dlg->exec();
    if(btn == QColorDialog::Accepted)
    {

    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_curve_color::set_button_text(QString text)
{
    if(!text.isEmpty())
    {
        ui->btn_push->setText(text);
    }
}
//--------------------------------------------------------------------------------
