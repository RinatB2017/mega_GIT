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
#include "timer_messagebox.hpp"
#include "ui_timer_messagebox.h"
//--------------------------------------------------------------------------------
Timer_messagebox::Timer_messagebox(const QString &message, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Timer_messagebox)
{
    ui->setupUi(this);

    QIcon icon = style()->standardIcon(QStyle::SP_MessageBoxWarning);
    QPixmap pixmap = icon.pixmap(QSize(60, 60));
    ui->label->setPixmap(pixmap);

    ui->le_message->setText(message);
    ui->le_message->setReadOnly(true);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(ui->buttonBox,  &QDialogButtonBox::accepted,
            this,           &Timer_messagebox::close);
}
//--------------------------------------------------------------------------------
Timer_messagebox::~Timer_messagebox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
