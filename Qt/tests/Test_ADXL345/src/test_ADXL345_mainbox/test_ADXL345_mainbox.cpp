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
#include "ui_test_ADXL345_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "test_ADXL345_mainbox.hpp"
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

    createTestBar();

    init_grapher_widget();
    init_serial_widget();
    init_display_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_serial_widget(void)
{
    ui->serial_widget->set_fix_baudrate(BAUDRATE);
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_adxl345(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::init_grapher_widget(void)
{
#ifndef NO_GRAPHER
    ui->grapher_widget->setObjectName("GrapherBox");

    ui->grapher_widget->set_title("тест");
    ui->grapher_widget->set_title_axis_X("X");
    ui->grapher_widget->set_title_axis_Y("Y");
    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(0, 100);

    ui->grapher_widget->set_visible_btn_Options(true);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);

    curve_x_raw_accel       = ui->grapher_widget->add_curve("raw x_accel");
    curve_y_raw_accel       = ui->grapher_widget->add_curve("raw y_accel");
    curve_z_raw_accel       = ui->grapher_widget->add_curve("raw z_accel");

    curve_x_norm_accel       = ui->grapher_widget->add_curve("norm x_accel");
    curve_y_norm_accel       = ui->grapher_widget->add_curve("norm y_accel");
    curve_z_norm_accel       = ui->grapher_widget->add_curve("norm z_accel");

#else
    ui->grapher_widget->setVisible(false);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::init_display_widgets(void)
{
    display_widgets.append(ui->display_x_raw_accel);
    display_widgets.append(ui->display_y_raw_accel);
    display_widgets.append(ui->display_z_raw_accel);
    display_widgets.append(ui->display_x_norm_accel);
    display_widgets.append(ui->display_y_norm_accel);
    display_widgets.append(ui->display_z_norm_accel);

    foreach (QLCDNumber *display, display_widgets)
    {
        display->setFixedSize(220, 48);
        display->setDigitCount(9);
    }
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
void MainBox::data_adxl345(QByteArray data)
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
    if(sl.count() != 6)
    {
        //emit error(QString("sl.count = %1").arg(sl.count()));
        dirty_array.clear();
        return;
    }

    qreal x_raw_accel       = sl.at(0).toDouble();
    qreal y_raw_accel       = sl.at(1).toDouble();
    qreal z_raw_accel       = sl.at(2).toDouble();
    qreal x_norm_accel      = sl.at(3).toDouble();
    qreal y_norm_accel      = sl.at(4).toDouble();
    qreal z_norm_accel      = sl.at(5).toDouble();

#ifndef NO_GRAPHER
    ui->grapher_widget->add_curve_data(curve_x_raw_accel,       x_raw_accel);
    ui->grapher_widget->add_curve_data(curve_y_raw_accel,       y_raw_accel);
    ui->grapher_widget->add_curve_data(curve_z_raw_accel,       z_raw_accel);

    ui->grapher_widget->add_curve_data(curve_x_norm_accel,      x_norm_accel);
    ui->grapher_widget->add_curve_data(curve_y_norm_accel,      y_norm_accel);
    ui->grapher_widget->add_curve_data(curve_z_norm_accel,      z_norm_accel);
#endif

    ui->display_x_raw_accel->display(x_raw_accel);
    ui->display_y_raw_accel->display(y_raw_accel);
    ui->display_z_raw_accel->display(z_raw_accel);

    ui->display_x_norm_accel->display(x_norm_accel);
    ui->display_y_norm_accel->display(y_norm_accel);
    ui->display_z_norm_accel->display(z_norm_accel);

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
