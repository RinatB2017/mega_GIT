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
#include "ui_test_syslog_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_syslog_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "sender_syslog.hpp"
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

    createSenderBar();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
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

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, commands)
        {
            cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
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
void MainBox::createSenderBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        Sender_syslog *syslogBar_0 = new Sender_syslog("syslogBar_0", 0, LOG_EMERG);
        Sender_syslog *syslogBar_1 = new Sender_syslog("syslogBar_1", 1, LOG_ALERT);
        Sender_syslog *syslogBar_2 = new Sender_syslog("syslogBar_2", 2, LOG_CRIT);
        Sender_syslog *syslogBar_3 = new Sender_syslog("syslogBar_3", 3, LOG_ERR);
        Sender_syslog *syslogBar_4 = new Sender_syslog("syslogBar_4", 4, LOG_WARNING);
        Sender_syslog *syslogBar_5 = new Sender_syslog("syslogBar_5", 5, LOG_NOTICE);
        Sender_syslog *syslogBar_6 = new Sender_syslog("syslogBar_6", 6, LOG_INFO);
        Sender_syslog *syslogBar_7 = new Sender_syslog("syslogBar_7", 7, LOG_DEBUG);

        connect(syslogBar_0,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_1,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_2,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_3,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_4,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_5,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_6,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));
        connect(syslogBar_7,    SIGNAL(syslog(QDateTime,int,int,QString)),  mw, SIGNAL(syslog(QDateTime,int,int,QString)));

        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_0);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_1);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_2);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_3);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_4);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_5);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_6);
        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, syslogBar_7);
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
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
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
    emit info("Test()");

    emit info("info");
    emit debug("debug");
    emit error("error");
    emit trace("trace");

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
