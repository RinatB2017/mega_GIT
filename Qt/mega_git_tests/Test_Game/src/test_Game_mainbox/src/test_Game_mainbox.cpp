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
#include "ui_test_Game_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_Game_mainbox.hpp"
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
    if(scene)
    {
        delete scene;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    //scene = new Scene();
    //scene = new Scene2();
    scene = new Scene3();

    connect(scene, SIGNAL(info(QString)),    this, SIGNAL(info(QString)));
    connect(scene, SIGNAL(debug(QString)),   this, SIGNAL(debug(QString)));
    connect(scene, SIGNAL(error(QString)),   this, SIGNAL(error(QString)));
    connect(scene, SIGNAL(trace(QString)),   this, SIGNAL(trace(QString)));

    ui->graphicsView->setScene(scene);
    int border = 5;
    ui->graphicsView->setFixedSize(WIDTH+border,
                                   HEIGHT+border);

    installEventFilter(this);

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw != nullptr);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

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

    QPushButton *btn_timer_start = new QPushButton;
    btn_timer_start->setObjectName("btn_timer_start");
    btn_timer_start->setText("START");

    QPushButton *btn_timer_stop = new QPushButton;
    btn_timer_stop->setObjectName("btn_timer_stop");
    btn_timer_stop->setText("STOP");

    QPushButton *btn_update = new QPushButton;
    btn_update->setObjectName("btn_update");
    btn_update->setText("UPDATE");

    testbar->addWidget(btn_timer_start);
    testbar->addWidget(btn_timer_stop);
    testbar->addWidget(btn_update);

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(btn_timer_start,    SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(btn_timer_stop,     SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    connect(btn_update,         SIGNAL(clicked(bool)),  this,   SLOT(update()));
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
void MainBox::start(void)
{
    if(scene)
    {
        scene->start();
    }
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    if(scene)
    {
        scene->stop();
    }
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
    if(scene)
    {
        scene->update();
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

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
bool MainBox::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    if(mouseEvent == nullptr)
    {
        return false;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::MouseButtonPress)
    {
        emit info(QString("%1 %2")
                  .arg(mouseEvent->pos().x())
                  .arg(mouseEvent->pos().y()));
        return true;
    }
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
