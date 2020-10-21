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
#include "mycombobox.hpp"
#include "ui_mycombobox.h"
//--------------------------------------------------------------------------------
MyComboBox::MyComboBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MyComboBox)
{
    init();
}
//--------------------------------------------------------------------------------
MyComboBox::~MyComboBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MyComboBox::init(void)
{
    ui->setupUi(this);

    connect(ui->btn_clear, &QToolButton::clicked, [this]() {
        ui->cb_command->removeItem(ui->cb_command->currentIndex());
    });
    connect(ui->btn_send,   &QToolButton::clicked,  this,   &MyComboBox::send_command);

    ui->btn_clear->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->btn_send->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
}
//--------------------------------------------------------------------------------
QString MyComboBox::get_command(void)
{
    return ui->cb_command->currentText();
}
//--------------------------------------------------------------------------------
void MyComboBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MyComboBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MyComboBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MyComboBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
