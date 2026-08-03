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
#include "mainwindow.hpp"
#include "mainwidget.hpp"
#include "menudock.hpp"
#include "logdock.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    create_menu();
    create_toolbars();
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
void MainWindow::create_menu()
{
    QMenu *f_menu = menuBar()->addMenu("File");

    QAction *a_test = new QAction("test");
    connect(a_test, &QAction::triggered,    this,   &MainWindow::test);

    QAction *a_exit = new QAction("exit");
    connect(a_exit, &QAction::triggered,    this,   &MainWindow::close);

    f_menu->addAction(a_test);
    f_menu->addSeparator();
    f_menu->addAction(a_exit);
}
//--------------------------------------------------------------------------------
void MainWindow::create_toolbars()
{
    QToolBar *toolbar = new QToolBar("toolbar", this);
    Q_ASSERT(toolbar);
    if(toolbar == nullptr)
    {
        return;
    }

    toolbar->setObjectName("toolbar");

    QToolButton *btn_exit = new QToolButton(this);
    btn_exit->setIcon(QIcon(":/images/computer.ico"));
    connect(btn_exit,   &QToolButton::clicked,  this,   &MainWindow::close);

    toolbar->addWidget(btn_exit);

    addToolBar(Qt::TopToolBarArea, toolbar);
}
//--------------------------------------------------------------------------------
void MainWindow::create_central_widget()
{
    MainWidget *mw = new MainWidget(this);

    connect(mw, &MainWidget::info,  this,   &MainWindow::info);
    connect(mw, &MainWidget::debug, this,   &MainWindow::debug);
    connect(mw, &MainWidget::error, this,   &MainWindow::error);
    connect(mw, &MainWidget::trace, this,   &MainWindow::trace);

    setCentralWidget(mw);
}
//--------------------------------------------------------------------------------
void MainWindow::create_menu_dock()
{
    MenuDock *md = new MenuDock(this);
    md->setObjectName("md");

    connect(md, &MenuDock::info,    this,   &MainWindow::info);
    connect(md, &MenuDock::debug,   this,   &MainWindow::debug);
    connect(md, &MenuDock::error,   this,   &MainWindow::error);
    connect(md, &MenuDock::trace,   this,   &MainWindow::trace);

    md->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, md);
}
//--------------------------------------------------------------------------------
void MainWindow::create_log_dock()
{
    LogDock *ld = new LogDock(this);
    ld->setObjectName("ld");

    connect(this,   &MainWindow::info,      ld, &LogDock::infoLog);
    connect(this,   &MainWindow::debug,     ld, &LogDock::debugLog);
    connect(this,   &MainWindow::error,     ld, &LogDock::errorLog);
    connect(this,   &MainWindow::trace,     ld, &LogDock::traceLog);

    ld->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
//--------------------------------------------------------------------------------
void MainWindow::test()
{
    emit info("test");
    emit debug("test");
    emit error("test");
    emit trace("test");
}
//--------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    int btn =  QMessageBox::question(this,
                                     "Выход из программы",
                                     "Вы уверены, что хотите выйти из программы?",
                                     QMessageBox::Yes | QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
//--------------------------------------------------------------------------------
