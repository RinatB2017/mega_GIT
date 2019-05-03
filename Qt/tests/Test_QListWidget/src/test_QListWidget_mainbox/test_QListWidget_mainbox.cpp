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
#include "ui_test_QListWidget_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QListWidget_mainbox.hpp"
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

    //createTestBar();

    QListWidget *lstWgt = new QListWidget;
    QLayout *l = new QVBoxLayout();
    l->addWidget(lstWgt);
    setLayout(l);

    // Добавим в список 10 элементов
    for(int i=0; i<10; ++i)
    {
        makeItem(lstWgt);
    }
}
//--------------------------------------------------------------------------------
void MainBox::onBtnClicked(void)
{
    if(QPushButton* btn = qobject_cast< QPushButton* >( sender()))
    {
        if(QLineEdit* e = btn->parent()->findChild< QLineEdit* >())
        {
            QMessageBox::information(this, "Button was clicked!", e->text());
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::makeItem(QListWidget* lstWgt)
{
    QWidget* wgt = new QWidget;
    QLayout* l = new QHBoxLayout();
    l->addWidget(new QLineEdit );
    QPushButton* btn = new QPushButton("Click me");
    connect(btn, SIGNAL(clicked()), SLOT( onBtnClicked()));
    l->addWidget(btn);
    wgt->setLayout(l);

    QListWidgetItem* item = new QListWidgetItem(lstWgt);
    item->setSizeHint(wgt->sizeHint());
    lstWgt->setItemWidget(item, wgt);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    btn_test->setObjectName("btn_test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test()");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
