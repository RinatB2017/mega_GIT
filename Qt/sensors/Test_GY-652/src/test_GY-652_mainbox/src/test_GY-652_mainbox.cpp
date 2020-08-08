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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_GY-652_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_GY-652_mainbox.hpp"
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

#ifdef QT_DEBUG
    createTestBar();
#endif

#ifndef NO_GRAPHER
    ui->grapher_widget->setObjectName("GrapherBox");

    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 100);

    ui->grapher_widget->set_visible_btn_Options(false);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);

    curve_temperature   = ui->grapher_widget->add_curve("temperature");
    curve_pressure      = ui->grapher_widget->add_curve("pressure");
    curve_atm           = ui->grapher_widget->add_curve("atm");
    curve_altitude      = ui->grapher_widget->add_curve("altitude");

#else
    ui->grapher_widget->setVisible(false);
#endif
    ui->serial_widget->set_fix_baudrate(57600);

    //---
#ifdef NO_SHOW_TEMPERATURE
    ui->label_temp->setVisible(false);
    ui->display_temperature->setVisible(false);
#endif

#ifdef NO_SHOW_PRESSURE
    ui->label_press->setVisible(false);
    ui->display_pressure->setVisible(false);
#endif

#ifdef NO_SHOW_ATM
    ui->label_atm->setVisible(false);
    ui->display_atm->setVisible(false);
#endif

#ifdef NO_SHOW_ALTITUDE
    ui->label_alt->setVisible(false);
    ui->display_altitude->setVisible(false);
#endif
    //---

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_gy652(QByteArray)));

#ifdef Q_OS_WIN
    setFixedSize(sizeHint());
#endif
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
    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
QString MainBox::convert(qreal value)
{
    return QString("%1").arg(value, 0, 'f', 2);
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

    qreal temperature = sl.at(0).toDouble();
    qreal pressure = sl.at(1).toDouble();
    qreal atm = sl.at(2).toDouble();
    qreal altitude = sl.at(3).toDouble();

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curve_temperature,   static_cast<double>(temperature));
    ui->grapher_widget->add_curve_data(curve_pressure,      static_cast<double>(pressure));
    ui->grapher_widget->add_curve_data(curve_atm,           static_cast<double>(atm));
    ui->grapher_widget->add_curve_data(curve_altitude,      static_cast<double>(altitude));
#endif

    ui->display_temperature->display(convert(temperature));
    ui->display_pressure->display(convert(pressure * 0.75006375541921 / 100.0));
    ui->display_atm->display(convert(atm));
    ui->display_altitude->display(convert(altitude));

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
