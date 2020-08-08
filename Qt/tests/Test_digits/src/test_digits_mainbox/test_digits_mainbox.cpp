/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include "ui_test_digits_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_digits_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    is_blocked(false)
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

    QHBoxLayout *red_hbox = new QHBoxLayout();
    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setCheckable(true);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(QIcon(QString(":/red/%1.png").arg(n)));
        red_hbox->addWidget(btn);
    }
    QHBoxLayout *black_hbox = new QHBoxLayout();
    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setCheckable(true);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(QIcon(QString(":/black/%1.png").arg(n)));
        black_hbox->addWidget(btn);
    }
    QHBoxLayout *white_hbox = new QHBoxLayout();
    for(int n=0; n<10; n++)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setCheckable(true);
        btn->setIconSize(QSize(32, 32));
        btn->setIcon(QIcon(QString(":/white/%1.png").arg(n)));
        white_hbox->addWidget(btn);
    }
    ui->digits_layout->addLayout(red_hbox);
    ui->digits_layout->addLayout(white_hbox);
    ui->digits_layout->addLayout(black_hbox);
    ui->digits_layout->addStretch(1);

    setFixedSize(sizeHint());
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
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
