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
#include "ui_test_logboxs_mainbox.h"
//--------------------------------------------------------------------------------
#include "test_logboxs_mainbox.hpp"
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    connect(this,    SIGNAL(info(QString)),  ui->widget,    SLOT(infoLog(QString)));
    connect(this,    SIGNAL(debug(QString)), ui->widget,    SLOT(debugLog(QString)));
    connect(this,    SIGNAL(error(QString)), ui->widget,    SLOT(errorLog(QString)));
    connect(this,    SIGNAL(trace(QString)), ui->widget,    SLOT(traceLog(QString)));

    connect(this,    SIGNAL(info(QString)),  ui->widget_2,  SLOT(infoLog(QString)));
    connect(this,    SIGNAL(debug(QString)), ui->widget_2,  SLOT(debugLog(QString)));
    connect(this,    SIGNAL(error(QString)), ui->widget_2,  SLOT(errorLog(QString)));
    connect(this,    SIGNAL(trace(QString)), ui->widget_2,  SLOT(traceLog(QString)));

    connect(this,    SIGNAL(info(QString)),  ui->widget_3,  SLOT(infoLog(QString)));
    connect(this,    SIGNAL(debug(QString)), ui->widget_3,  SLOT(debugLog(QString)));
    connect(this,    SIGNAL(error(QString)), ui->widget_3,  SLOT(errorLog(QString)));
    connect(this,    SIGNAL(trace(QString)), ui->widget_3,  SLOT(traceLog(QString)));

    load_widgets();

#ifdef USE_DOCK_WIDGETS
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("log",  "log",  Qt::BottomDockWidgetArea, ui->widget);
        mw->add_dock_widget("log2", "log2", Qt::BottomDockWidgetArea, ui->widget_2);
        mw->add_dock_widget("log3", "log3", Qt::BottomDockWidgetArea, ui->widget_3);

        setVisible(false);
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",     &MainBox::test });

    testbar = new QToolBar("testbar");
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

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
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
    emit trace(Q_FUNC_INFO);

    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");

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
    if(cb_block)
    {
        bool is_checked = load_bool("cb_block");
        cb_block->setChecked(is_checked);
        cb_block->clicked(is_checked);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    if(cb_block)
    {
        save_int("cb_block", cb_block->isChecked());
    }
}
//--------------------------------------------------------------------------------
