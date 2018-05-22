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
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_Test_GY-652_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "Test_GY-652_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifndef NO_GRAPHER
#   include "grapherbox.hpp"
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

#ifndef NO_GRAPHER
    ui->grapher_widget->setObjectName("GrapherBox");

    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 100);

    curve_temperature   = ui->grapher_widget->add_curve("temperature");
    curve_pressure      = ui->grapher_widget->add_curve("pressure");
    curve_atm           = ui->grapher_widget->add_curve("atm");
    curve_altitude      = ui->grapher_widget->add_curve("altitude");

#else
    ui->grapher_widget->setVisible(false);
    setFixedSize(sizeHint());
#endif
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_gy652(QByteArray)));
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

    connect(btn_test,   SIGNAL(clicked()),  this,   SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::data_gy652(QByteArray data)
{
    QString temp = data;
    if(temp.isEmpty())
    {
        return;
    }
    QStringList sl = temp.split("|");
    if(sl.count() != 4)
    {
        return;
    }

    float temperature = sl.at(0).toFloat();
    float pressure = sl.at(1).toFloat();
    float atm = sl.at(2).toFloat();
    float altitude = sl.at(3).toFloat();

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curve_temperature,   temperature);
    ui->grapher_widget->add_curve_data(curve_pressure,      pressure);
    ui->grapher_widget->add_curve_data(curve_atm,           atm);
    ui->grapher_widget->add_curve_data(curve_altitude,      altitude);
#endif

    ui->display_temperature->display(temperature);
    ui->display_pressure->display(pressure * 0.75006375541921f / 100.0f);
    ui->display_atm->display(atm);
    ui->display_altitude->display(altitude);

    //emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
