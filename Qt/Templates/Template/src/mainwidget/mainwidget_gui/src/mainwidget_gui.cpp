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
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainwidget_gui.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainwidget_gui.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "mainwidget.hpp"
//--------------------------------------------------------------------------------
MainWidget_GUI::MainWidget_GUI(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainWidget_GUI)
{
    init();
    load_config();
}
//--------------------------------------------------------------------------------
MainWidget_GUI::~MainWidget_GUI()
{
    qDebug() << "~MainWidget_GUI()";

    save_config();
    if(w) w->deleteLater();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::init(void)
{
    ui->setupUi(this);

    createTestBar();

    //---
    w = new MainWidget(this);
    if(w)
    {
        connect(w,          SIGNAL(info(QString)),  this,       SIGNAL(info(QString)));
        connect(w,          SIGNAL(debug(QString)), this,       SIGNAL(debug(QString)));
        connect(w,          SIGNAL(error(QString)), this,       SIGNAL(error(QString)));
        connect(w,          SIGNAL(trace(QString)), this,       SIGNAL(trace(QString)));

        connect(ui->btn_0,  SIGNAL(clicked(bool)),  w,          SLOT(get()));
        connect(w,          SIGNAL(set(QString)),   ui->le_0,   SLOT(setText(QString)));
    }
    //---

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainWidget::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainWidget::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainWidget::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainWidget::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainWidget::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainWidget::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *toolBar = new QToolBar("testbar");
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QCheckBox *cb_block = new QCheckBox("block");
    toolBar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainWidget::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (w->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
