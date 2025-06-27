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
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QStateMachine>
//--------------------------------------------------------------------------------
#include "ui_test_QStateMachine_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QStateMachine_mainbox.hpp"
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
    create_test_bar();
#endif

    QStateMachine *machine = new QStateMachine(this);

    QState *state1 = new QState(machine);
    state1->assignProperty(ui->btn_animated, "geometry", QRect(0, 0, 100, 100));

    QState *state2 = new QState(machine);
    state2->assignProperty(ui->btn_animated, "geometry", QRect(200, 0, 100, 100));

    QState *state3 = new QState(machine);
    state3->assignProperty(ui->btn_animated, "geometry", QRect(200, 200, 100, 100));

    QState *state4 = new QState(machine);
    state4->assignProperty(ui->btn_animated, "geometry", QRect(0, 200, 100, 100));

    machine->setInitialState(state1);

    QSignalTransition *transition1 = state1->addTransition(ui->btn_animated,    &QPushButton::clicked,  state2);
    QSignalTransition *transition2 = state2->addTransition(ui->btn_animated,    &QPushButton::clicked,  state3);
    QSignalTransition *transition3 = state3->addTransition(ui->btn_animated,    &QPushButton::clicked,  state4);
    QSignalTransition *transition4 = state4->addTransition(ui->btn_animated,    &QPushButton::clicked,  state1);

    QPropertyAnimation *anim = new QPropertyAnimation(ui->btn_animated, "geometry");
    anim->setDuration(1000);

    transition1->addAnimation(anim);
    transition2->addAnimation(anim);
    transition3->addAnimation(anim);
    transition4->addAnimation(anim);

    machine->start();
}
//--------------------------------------------------------------------------------
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test", &MainBox::test });

        QToolBar *testbar = new QToolBar(tr("testbar"));
        testbar->setObjectName("testbar");

        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_block = new QCheckBox("block", this);
        testbar->addWidget(cb_block);

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
    }
    else
    {
        emit error("mw not found!");
    }
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
