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
#include "controlexitwidget.hpp"
#include "ui_controlexitwidget.h"
//--------------------------------------------------------------------------------
ControlExitWidget::ControlExitWidget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ControlExitWidget)
{
    init();
}
//--------------------------------------------------------------------------------
ControlExitWidget::~ControlExitWidget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void ControlExitWidget::init(void)
{
    ui->setupUi(this);

    QTimer::singleShot(100, [this]{
        ui->label->setText(objectName());
    });
}
//--------------------------------------------------------------------------------
void ControlExitWidget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool ControlExitWidget::programm_is_exit(void)
{
    bool is_exit = ui->cb_no_exit->isChecked();
    if(is_exit == false)
    {
        messagebox_info(objectName(), "Низзя");
    }
    return is_exit;
}
//--------------------------------------------------------------------------------
void ControlExitWidget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void ControlExitWidget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
