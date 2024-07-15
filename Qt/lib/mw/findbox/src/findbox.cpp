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
#include "findbox.hpp"
#include "ui_findbox.h"
//--------------------------------------------------------------------------------
FindBox::FindBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindBox)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
FindBox::~FindBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void FindBox::init(void)
{
    ui->btn_close->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->btn_prev->setIcon(qApp->style()->standardIcon(QStyle::SP_ArrowUp));
    ui->btn_next->setIcon(qApp->style()->standardIcon(QStyle::SP_ArrowDown));

    connect(ui->btn_close,  &QToolButton::clicked,  this,   &FindBox::f_hide);
    connect(ui->btn_prev,   &QToolButton::clicked,  this,   &FindBox::f_prev);
    connect(ui->btn_next,   &QToolButton::clicked,  this,   &FindBox::f_next);

    connect(ui->le_find_str,    &QLineEdit::editingFinished,    this,   &FindBox::f_next);
}
//--------------------------------------------------------------------------------
void FindBox::f_hide(void)
{
    hide();
}
//--------------------------------------------------------------------------------
void FindBox::f_prev(void)
{
    QString text = ui->le_find_str->text();
    if(text.isEmpty())  return;

    emit find_prev(text);
}
//--------------------------------------------------------------------------------
void FindBox::f_next(void)
{
    QString text = ui->le_find_str->text();
    if(text.isEmpty())  return;

    emit find_next(text);
}
//--------------------------------------------------------------------------------
