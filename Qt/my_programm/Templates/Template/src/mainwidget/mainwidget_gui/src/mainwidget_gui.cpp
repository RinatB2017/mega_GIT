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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainWidget_GUI::MainWidget_GUI(QWidget *parent,
                               MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainWidget_GUI)
{
    init();
}
//--------------------------------------------------------------------------------
MainWidget_GUI::~MainWidget_GUI()
{
    save_widgets();
#ifdef QT_DEBUG
    qDebug() << "~MainWidget_GUI()";
#endif
    if(w) w->deleteLater();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    //---
    w = new MainWidget(this);
    Q_CHECK_PTR(w);

    connect(w,          SIGNAL(info(QString)),  this,       SIGNAL(info(QString)));
    connect(w,          SIGNAL(debug(QString)), this,       SIGNAL(debug(QString)));
    connect(w,          SIGNAL(error(QString)), this,       SIGNAL(error(QString)));
    connect(w,          SIGNAL(trace(QString)), this,       SIGNAL(trace(QString)));

    connect(ui->btn_0,  SIGNAL(clicked(bool)),  w,          SLOT(get()));
    connect(w,          SIGNAL(set(QString)),   ui->le_0,   SLOT(setText(QString)));
    //---
#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw != nullptr);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainWidget::test });

    testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
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
    Q_CHECK_PTR(btn_choice_test);
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block,   &QCheckBox::clicked,    cb_test,            &QToolButton::setDisabled);
    connect(cb_block,   &QCheckBox::clicked,    btn_choice_test,    &QToolButton::setDisabled);

#ifdef TOOLBAR_ORIENTATION
    testbar->adjustSize();
    testbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(testbar, &QToolBar::orientationChanged, [this]()
    {
        switch(testbar->orientation())
        {
        case Qt::Horizontal:
            testbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
            break;

        case Qt::Vertical:
            testbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            break;
        }
    });
#endif

}
//--------------------------------------------------------------------------------
void MainWidget_GUI::choice_test(void)
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
        typedef bool (MainWidget::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (w->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::updateText(void)
{
    ui->retranslateUi(this);
    if(testbar)
    {
        testbar->setWindowTitle(tr("testbar"));
    }
}
//--------------------------------------------------------------------------------
bool MainWidget_GUI::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::load_setting(void)
{
    if(cb_block)
    {
        bool block_is_checked = load_bool("cb_block");
        cb_block->clicked(block_is_checked);
        cb_block->setChecked(block_is_checked);
    }
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::save_setting(void)
{
    if(cb_block)
    {
        save_int("cb_block", cb_block->isChecked());
    }
}
//--------------------------------------------------------------------------------
