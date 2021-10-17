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
#include "mainwindow.hpp"
#include "worker_fake.hpp"
#include "ui_worker_fake.h"
//--------------------------------------------------------------------------------
Worker_fake::Worker_fake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Worker_fake)
{
    init();
}
//--------------------------------------------------------------------------------
Worker_fake::~Worker_fake()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake::init(void)
{
    ui->setupUi(this);

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);
    }
#endif
}
//--------------------------------------------------------------------------------
void Worker_fake::input(QByteArray data)
{
    ui->log_widget->infoLog(data);
}
//--------------------------------------------------------------------------------
void Worker_fake::port_close(void)
{

}
//--------------------------------------------------------------------------------
