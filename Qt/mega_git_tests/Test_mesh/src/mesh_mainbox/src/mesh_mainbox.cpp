/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "ui_mesh_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mesh_mainbox.hpp"
#include "mesh_control.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

#if 0
    add_serial0();
    add_serial1();
    add_serial2();
    add_serial3();

    setFixedSize(0, 0);
#else
    Mesh_control *control0 = new Mesh_control("control0", this);
    Mesh_control *control1 = new Mesh_control("control1", this);
    Mesh_control *control2 = new Mesh_control("control2", this);
    Mesh_control *control3 = new Mesh_control("control3", this);

    QSplitter *sp_0 = new QSplitter(Qt::Horizontal);
    QSplitter *sp_1 = new QSplitter(Qt::Horizontal);
    QSplitter *sp_2 = new QSplitter(Qt::Vertical);

    sp_0->addWidget(control0);
    sp_0->addWidget(control1);

    sp_1->addWidget(control2);
    sp_1->addWidget(control3);

    sp_2->addWidget(sp_0);
    sp_2->addWidget(sp_1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(sp_2);

    setLayout(vbox);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::add_serial0(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    Mesh_control *control0 = new Mesh_control("control0", this);
    mw->add_dock_widget("Serial0", "serial0", Qt::LeftDockWidgetArea,   reinterpret_cast<QWidget *>(control0));
}
//--------------------------------------------------------------------------------
void MainBox::add_serial1(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    Mesh_control *control1 = new Mesh_control("control1", this);
    mw->add_dock_widget("Serial1", "serial1", Qt::RightDockWidgetArea,   reinterpret_cast<QWidget *>(control1));
}
//--------------------------------------------------------------------------------
void MainBox::add_serial2(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    Mesh_control *control2 = new Mesh_control("control2", this);
    mw->add_dock_widget("Serial2", "serial2", Qt::LeftDockWidgetArea,   reinterpret_cast<QWidget *>(control2));
}
//--------------------------------------------------------------------------------
void MainBox::add_serial3(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    Mesh_control *control3 = new Mesh_control("control3", this);
    mw->add_dock_widget("Serial3", "serial3", Qt::RightDockWidgetArea,   reinterpret_cast<QWidget *>(control3));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
