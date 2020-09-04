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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
void MainWindow::create_menu(void)
{
    QMenu *f_menu = menuBar()->addMenu("File");

    QAction *a_test = new QAction("test");
    connect(a_test, SIGNAL(triggered(bool)),    this,   SLOT(test()));

    QAction *a_exit = new QAction("exit");
    connect(a_exit, SIGNAL(triggered(bool)),    this,   SLOT(close()));

    f_menu->addAction(a_test);
    f_menu->addSeparator();
    f_menu->addAction(a_exit);
}
//--------------------------------------------------------------------------------
void MainWindow::create_toolbars(void)
{
    QToolBar *toolbar = new QToolBar("toolbar", this);
    Q_CHECK_PTR(toolbar);
    if(toolbar == nullptr)
    {
        return;
    }

    toolbar->setObjectName("toolbar");

    QToolButton *btn_exit = new QToolButton(this);
    btn_exit->setIcon(QIcon(":/images/computer.ico"));
    connect(btn_exit,   SIGNAL(clicked(bool)),  this,   SLOT(close()));

    toolbar->addWidget(btn_exit);

    addToolBar(Qt::TopToolBarArea, toolbar);
}
//--------------------------------------------------------------------------------
void MainWindow::create_central_widget(void)
{
    MainWidget *mw = new MainWidget(this);

    connect(mw, SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(mw, SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(mw, SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(mw, SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    setCentralWidget(mw);
}
//--------------------------------------------------------------------------------
void MainWindow::create_menu_dock(void)
{
    MenuDock *md = new MenuDock(this);
    md->setObjectName("md");

    connect(md, SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(md, SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    md->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, md);
}
//--------------------------------------------------------------------------------
void MainWindow::create_log_dock(void)
{
    LogDock *ld = new LogDock(this);
    ld->setObjectName("ld");

    connect(this,   SIGNAL(info(QString)),      ld, SLOT(infoLog(QString)));
    connect(this,   SIGNAL(debug(QString)),     ld, SLOT(debugLog(QString)));
    connect(this,   SIGNAL(error(QString)),     ld, SLOT(errorLog(QString)));
    connect(this,   SIGNAL(trace(QString)),     ld, SLOT(traceLog(QString)));

    ld->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, ld);
}
//--------------------------------------------------------------------------------
void MainWindow::test(void)
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
