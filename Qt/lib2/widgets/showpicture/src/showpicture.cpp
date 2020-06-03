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
#include "showpicture.hpp"
#include "ui_showpicture.h"
//--------------------------------------------------------------------------------
ShowPicture::ShowPicture(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ShowPicture)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
ShowPicture::~ShowPicture()
{
    delete ui;
}
//--------------------------------------------------------------------------------
bool ShowPicture::show_picture(const QString &filename)
{
    if(filename.isEmpty())
    {
        emit error("filename.isEmpty()");
        return false;
    }
    ui->picture_label->setPixmap(QPixmap(filename));
    return true;
}
//--------------------------------------------------------------------------------
void ShowPicture::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool ShowPicture::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void ShowPicture::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void ShowPicture::save_setting(void)
{

}
//--------------------------------------------------------------------------------
