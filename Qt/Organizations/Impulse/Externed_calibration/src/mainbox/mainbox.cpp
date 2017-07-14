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
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QComboBox>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//#include "ui_mainbox_new.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    QWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_setting();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::connect_log(void)
{
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),    parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parentWidget(), SIGNAL(message(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::log(const QString &text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    connect_log();

    createTestBar();

    load_setting();
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = (MainWindow *)parentWidget();
    if(!mw) return;

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->addItem(TEST_0_TEXT, QVariant(Qt::UserRole + TEST_0));
    cb_test->addItem(TEST_1_TEXT, QVariant(Qt::UserRole + TEST_1));
    cb_test->addItem(TEST_2_TEXT, QVariant(Qt::UserRole + TEST_2));
    cb_test->addItem(TEST_3_TEXT, QVariant(Qt::UserRole + TEST_3));

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    switch (cmd)
    {
    case TEST_0: test_0(); break;
    case TEST_1: test_1(); break;
    case TEST_2: test_2(); break;
    case TEST_3: test_3(); break;
    default: break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    emit info("Test_0()");
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QPushButton *pb=qobject_cast<QPushButton *>(btn))
    {
        pb->setDisabled(state);
    }
    if (QToolButton *tb=qobject_cast<QToolButton *>(btn))
    {
        tb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MainBox::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
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
void MainBox::load_setting(void)
{
    qDebug() << "MainBox::load_setting";
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    qDebug() << "MainBox::save_setting";
}
//--------------------------------------------------------------------------------
int MainBox::xxx(int x)
{
    return x+5;
}
//--------------------------------------------------------------------------------
