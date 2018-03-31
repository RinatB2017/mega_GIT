/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QMainWindow>
#include <QMenuBar>
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "../src/applications/gqrx/mainwindow.h"
#include "scan_tooldock.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MyMainWindow::MyMainWindow(const QString cfgfile,
                           bool edit_conf,
                           QWidget *parent) :
    MainWindow(cfgfile, edit_conf, parent)
{
#if 0
    Scan_ToolBar *toolBar = new Scan_ToolBar(this);
    toolBar->setObjectName("toolBar");
    addToolBar(Qt::TopToolBarArea, toolBar);
    //addToolBar(Qt::LeftToolBarArea, toolBar);
#else
    Scan_ToolDock *md = new Scan_ToolDock(this);
    md->setWindowTitle("scan");
    md->setObjectName("md");
    md->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, md);

    QMenuBar *mb = menuBar();
    if(mb)
    {
        mb->addAction("XXX");
        mb->addAction(md->toggleViewAction());
    }
    //ui->menu_View->addAction(md->toggleViewAction());
#endif
}
//--------------------------------------------------------------------------------
MyMainWindow::~MyMainWindow()
{
#ifdef QT_DEBUG
    qDebug() << "~NyMainWindow()";
#endif
}
//--------------------------------------------------------------------------------
