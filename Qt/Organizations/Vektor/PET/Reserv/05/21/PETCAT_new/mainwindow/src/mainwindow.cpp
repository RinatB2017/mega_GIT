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
#include <QMenuBar>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainwidget.hpp"
#include "menudock.hpp"
#include "logdock.hpp"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    create_menu();
    create_central_widget();
    create_menu_dock();
    create_log_dock();

    setMinimumSize(800, 600);
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{

}
//--------------------------------------------------------------------------------
void MainWindow::create_menu(void)
{
    QMenu *f_menu = menuBar()->addMenu("File");
    f_menu->addAction(new QAction("test"));
}
//--------------------------------------------------------------------------------
void MainWindow::create_central_widget(void)
{
    MainWidget *mw = new MainWidget(this);
    setCentralWidget(mw);
}
//--------------------------------------------------------------------------------
void MainWindow::create_menu_dock(void)
{
    MenuDock *md = new MenuDock(this);
    md->setObjectName("md");
    md->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, md);
}
//--------------------------------------------------------------------------------
void MainWindow::create_log_dock(void)
{
    LogDock *ld = new LogDock(this);
    ld->setObjectName("ld");
    ld->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
//--------------------------------------------------------------------------------
