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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QFileDialog>
#   include <QVBoxLayout>
#   include <QPushButton>
#   include <QToolButton>
#   include <QToolBar>
#   include <QDebug>
#   include <QTimer>
#endif
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "oscilloscopebox.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
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

    createTestBar();

    index=0;
    main_timer = new QTimer(this);
    connect(main_timer, SIGNAL(timeout()), this, SLOT(update()));
    main_timer->start();

    QPushButton *btn_start = new QPushButton(this);
    QPushButton *btn_stop = new QPushButton(this);

    btn_start->setText("start");
    btn_stop->setText("stop");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(btn_start);
    hbox->addWidget(btn_stop);
    hbox->addStretch(1);

    connect(btn_start,  SIGNAL(clicked(bool)),  main_timer, SLOT(start()));
    connect(btn_stop,   SIGNAL(clicked(bool)),  main_timer, SLOT(stop()));

    oscilloscope = new OscilloscopeBox(this);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(oscilloscope);
    vbox->addLayout(hbox);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
#if 1
    for(int n=0; n<4; n++)
    {
        oscilloscope->add_curve_data(n, double(100.0)*qSin(double(index)*double(M_PI)/double(180.0)));
    }
    index++;
#else
    for(int i=0; i<10; i++)
    {
        for(int n=0; n<4; n++)
        {
            oscilloscope->add_curve_data(n, double(100.0)*qSin(double(index)*double(M_PI)/double(180.0)));
        }
        index++;
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("toolBar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::load(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
