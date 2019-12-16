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
#include "ui_test_OscilloscopeBox_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "oscilloscopebox.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_OscilloscopeBox_mainbox.hpp"
#include "defines.hpp"
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

    connect(ui->btn_start,  SIGNAL(clicked(bool)),  main_timer, SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  main_timer, SLOT(stop()));
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
#if 1
    for(int n=0; n<5; n++)
    {
        ui->oscilloscope_widget->add_curve_data(n, qreal(100.0)*qSin(qreal(index)*qreal(M_PI)/qreal(180.0)));
    }
    index++;
#else
    for(int i=0; i<10; i++)
    {
        for(int n=0; n<5; n++)
        {
            ui->oscilloscope_widget->add_curve_data(n, qreal(100.0)*qSin(qreal(index)*qreal(M_PI)/qreal(180.0)));
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

    int size = beginReadArray(CURVE_COLORS);
    endArray();
    emit info(QString("size %1").arg(size));

    //ui->oscilloscope_widget->set_curve_color(0, Qt::blue);
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
