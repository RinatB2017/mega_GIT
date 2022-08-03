/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
// FAKE
//--------------------------------------------------------------------------------
#include "serialwidget_fake.hpp"
//--------------------------------------------------------------------------------
SerialWidget_fake::SerialWidget_fake(QWidget *parent) :
    SerialWidget(parent)
{
    worker_fake = new Worker_fake(this);

    QTimer::singleShot(0, [this]{
       worker_fake->setObjectName(objectName());
    });

// это шаблон, не надо его переписывать
    connect(worker_fake,    &Worker_fake::info,        this,           &SerialWidget::info);
    connect(worker_fake,    &Worker_fake::debug,       this,           &SerialWidget::debug);
    connect(worker_fake,    &Worker_fake::error,       this,           &SerialWidget::error);
    connect(worker_fake,    &Worker_fake::trace,       this,           &SerialWidget::trace);

    connect(worker_fake,    &Worker_fake::readyRead,   this,           &SerialWidget::readyRead);

    connect(worker_fake,    &Worker_fake::output,      this,           &SerialWidget::output);
    connect(worker_fake,    &Worker_fake::output,      this,           &SerialWidget::write_ba_output);

    connect(this,           &SerialWidget::port_open,       worker_fake,    &Worker_fake::port_open);
    connect(this,           &SerialWidget::port_close,      worker_fake,    &Worker_fake::port_close);

    worker_fake->show();
}
//--------------------------------------------------------------------------------
SerialWidget_fake::~SerialWidget_fake()
{
    if(worker_fake)
    {
        worker_fake->disconnect();
        delete worker_fake;
    }
}
//--------------------------------------------------------------------------------
