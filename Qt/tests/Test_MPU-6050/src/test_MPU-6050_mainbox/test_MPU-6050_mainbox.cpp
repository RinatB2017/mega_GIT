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
#include "ui_test_MPU-6050_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_MPU-6050_mainbox.hpp"
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

    ui->grapher_widget->set_visible_btn_Options(false);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);

    curve_x_accel       = ui->grapher_widget->add_curve("x_accel");
    curve_y_accel       = ui->grapher_widget->add_curve("y_accel");
    curve_z_accel       = ui->grapher_widget->add_curve("z_accel");
    curve_temperature   = ui->grapher_widget->add_curve("temperature");
    curve_x_gyro        = ui->grapher_widget->add_curve("x_gyro");
    curve_y_gyro        = ui->grapher_widget->add_curve("y_gyro");
    curve_z_gyro        = ui->grapher_widget->add_curve("z_gyro");

#else
    ui->grapher_widget->setVisible(false);
#endif

#if 1
    ui->sl_X->setRange(0, 359);
    ui->sl_Y->setRange(0, 359);
    ui->sl_Z->setRange(0, 359);

    ui->sb_X->setRange(0, 359);
    ui->sb_Y->setRange(0, 359);
    ui->sb_Z->setRange(0, 359);

    connect(ui->sl_X,   SIGNAL(valueChanged(int)),  ui->sb_X,   SLOT(setValue(int)));
    connect(ui->sl_Y,   SIGNAL(valueChanged(int)),  ui->sb_Y,   SLOT(setValue(int)));
    connect(ui->sl_Z,   SIGNAL(valueChanged(int)),  ui->sb_Z,   SLOT(setValue(int)));

    connect(ui->sb_X,   SIGNAL(valueChanged(int)),  ui->sl_X,   SLOT(setValue(int)));
    connect(ui->sb_Y,   SIGNAL(valueChanged(int)),  ui->sl_Y,   SLOT(setValue(int)));
    connect(ui->sb_Z,   SIGNAL(valueChanged(int)),  ui->sl_Z,   SLOT(setValue(int)));

    connect(ui->sb_X,   SIGNAL(valueChanged(int)),  ui->gl_widget,  SLOT(setXRotation(int)));
    connect(ui->sb_Y,   SIGNAL(valueChanged(int)),  ui->gl_widget,  SLOT(setYRotation(int)));
    connect(ui->sb_Z,   SIGNAL(valueChanged(int)),  ui->gl_widget,  SLOT(setZRotation(int)));
#endif

    ui->serial_widget->set_fix_baudrate(BAUDRATE);

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_mpu6050(QByteArray)));

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
void MainBox::data_mpu6050(QByteArray data)
{
    if(data.isEmpty())
    {
        return;
    }

    dirty_array.append(data);
    bool found = false;
    for(int n=0; n<dirty_array.length(); n++)
    {
        if(dirty_array[n] == '\n')
        {
            found = true;
        }
    }
    if(!found)
    {
        return;
    }

    QString temp = dirty_array;
    emit debug(temp);
    QStringList sl = temp.split("|");
    if(sl.count() != (9 + 3))
    {
        emit error(QString("sl.count = %1").arg(sl.count()));
        dirty_array.clear();
        return;
    }

    int err             = sl.at(0).toInt();
    qreal x_accel       = sl.at(1).toDouble();
    qreal y_accel       = sl.at(2).toDouble();
    qreal z_accel       = sl.at(3).toDouble();
    qreal temperature   = sl.at(4).toDouble();
    qreal x_gyro        = sl.at(5).toDouble();
    qreal y_gyro        = sl.at(6).toDouble();
    qreal z_gyro        = sl.at(7).toDouble();

    qreal s_x_gyro      = sl.at(8).toDouble();
    qreal s_y_gyro      = sl.at(9).toDouble();
    qreal s_z_gyro      = sl.at(10).toDouble();

    //---
    ui->display_s_x_gyro->display(s_x_gyro);
    ui->display_s_y_gyro->display(s_y_gyro);
    ui->display_s_z_gyro->display(s_z_gyro);
    //---

    if(err != 0)
    {
        emit error(QString("Error: %1").arg(err));
    }

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curve_x_accel,       x_accel);
    ui->grapher_widget->add_curve_data(curve_y_accel,       y_accel);
    ui->grapher_widget->add_curve_data(curve_z_accel,       z_accel);
    ui->grapher_widget->add_curve_data(curve_temperature,   temperature);
    ui->grapher_widget->add_curve_data(curve_x_gyro,        x_gyro);
    ui->grapher_widget->add_curve_data(curve_y_gyro,        y_gyro);
    ui->grapher_widget->add_curve_data(curve_z_gyro,        z_gyro);
#endif

    ui->display_error->display(err);
    ui->display_x_accel->display(x_accel);
    ui->display_y_accel->display(y_accel);
    ui->display_z_accel->display(z_accel);
    ui->display_temperature->display(temperature);
    ui->display_x_gyro->display(x_gyro);
    ui->display_y_gyro->display(y_gyro);
    ui->display_z_gyro->display(z_gyro);

    dirty_array.clear();
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
