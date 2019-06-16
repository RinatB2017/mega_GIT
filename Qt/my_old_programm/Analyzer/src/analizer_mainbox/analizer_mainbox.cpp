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
#include <QVBoxLayout>
#include <QToolButton>
#include <QToolBar>
//--------------------------------------------------------------------------------
#include "ui_analizer_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "analizer_mainbox.hpp"

#include "ft2232h_thread.hpp"
#include "grapherbox.hpp"
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
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(grapher) delete grapher;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    QVBoxLayout *vbox = new QVBoxLayout();
    create_grapher();
    vbox->addWidget(grapher);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::create_grapher(void)
{
    grapher = new GrapherBox(this);
    grapher->set_title("Анализатор");
    grapher->set_title_axis_X("Время");
    grapher->set_title_axis_Y("Каналы");
    grapher->set_axis_scale_x(0, 1024);
    grapher->set_axis_scale_y(0, DELTA_Y*MAX_CHANNEL);

    for(int n=0; n<MAX_CHANNEL; n++)
    {
        grapher->add_curve(QString("%1").arg(n, 0, 16).toUpper());
    }

    for(int n=0; n<MAX_CHANNEL; n++)
        old_values[n] = 0;
}
//--------------------------------------------------------------------------------
void MainBox::append_data(int x,
                          unsigned short data)
{
    //QString temp;
    for(int n=(MAX_CHANNEL-1); n>=0; n--)
    {
        if(data & 0x8000)
        {
            //temp.append("1");
            if(!old_values[n])
                grapher->add_curve_data(n, x, (DELTA_Y*n));
            grapher->add_curve_data(n, x, (DELTA_Y*n)+(DELTA_Y/2));
            old_values[n] = 1;
        }
        else
        {
            //temp.append("0");
            if(old_values[n])
                grapher->add_curve_data(n, x, (DELTA_Y*n)+(DELTA_Y/2));
            grapher->add_curve_data(n, x, (DELTA_Y*n));
            old_values[n] = 0;
        }
        data <<= 1;
    }
    //emit debug(temp);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
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
    emit info(tr("test"));

#if 0
    for(int x=0; x<1024; x++)
    {
        append_data(x, x);
        //append_data(x, rand() % 0xFFFF);
    }
#endif

    FT2232H_thread *thread;
    thread = new FT2232H_thread(this);

    connect(thread, SIGNAL(info(QString)),  this, SIGNAL(info(QString)));
    connect(thread, SIGNAL(debug(QString)), this, SIGNAL(debug(QString)));
    connect(thread, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(thread, SIGNAL(send_value(int, unsigned short)), this, SLOT(draw_value(int,unsigned short)));

    thread->start();

    connect(thread, SIGNAL(started()),  this, SLOT(measuring_started()));
    connect(thread, SIGNAL(finished()), this, SLOT(measuring_finished()));
}
//--------------------------------------------------------------------------------
void MainBox::draw_value(int x, unsigned short data)
{
    append_data(x, data);
}
//--------------------------------------------------------------------------------
void MainBox::measuring_started(void)
{
    emit info("started");
}
//--------------------------------------------------------------------------------
void MainBox::measuring_finished(void)
{
    emit info("finished");
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
