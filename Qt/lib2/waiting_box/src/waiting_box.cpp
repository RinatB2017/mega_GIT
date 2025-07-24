/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "waiting_box.hpp"
#include "ui_waiting_box.h"
//--------------------------------------------------------------------------------
Waiting_box::Waiting_box(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Waiting_box)
{
    ui->setupUi(this);

    setFixedSize(sizeHint());
    // setWindowModality(Qt::WindowModal);
    setWindowFlag(Qt::WindowStaysOnTopHint);    //TODO проверка абсолютной модальности
    setModal(true);
    // setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);   //убираем кнопку закрытия
}
//--------------------------------------------------------------------------------
Waiting_box::~Waiting_box()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Waiting_box::set_close_form(bool state)
{
    flag_close_form = state;
}
//--------------------------------------------------------------------------------
void Waiting_box::closeEvent(QCloseEvent *event)
{
    if(!flag_close_form)
    {
        // Игнорируем событие закрытия
        event->ignore();
    }
}
//--------------------------------------------------------------------------------

