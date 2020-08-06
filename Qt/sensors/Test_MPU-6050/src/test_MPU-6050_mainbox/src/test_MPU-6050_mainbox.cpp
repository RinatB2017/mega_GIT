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

    createTestBar();

    init_grapher_widget();
    init_gl_widget();
    init_serial_widget();

#if 1
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    mw->add_dock_widget("GL widget",
                        "gl_widget",
                        Qt::RightDockWidgetArea,
                        reinterpret_cast<QWidget *>(ui->gl_frame));
    mw->add_dock_widget("Grapher widget",
                        "grapher_widget",
                        Qt::RightDockWidgetArea,
                        reinterpret_cast<QWidget *>(ui->grapher_widget));
#endif

//#ifdef Q_OS_WIN
//    setFixedSize(sizeHint());
//#endif
}
//--------------------------------------------------------------------------------
void MainBox::init_gl_widget(void)
{
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
}
//--------------------------------------------------------------------------------
void MainBox::init_serial_widget(void)
{
    ui->serial_widget->set_fix_baudrate(BAUDRATE);
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)),  this,   SLOT(data_mpu6050(QByteArray)));
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

    curve_x_accel       = ui->grapher_widget->add_curve("x_accel");
    curve_y_accel       = ui->grapher_widget->add_curve("y_accel");
    curve_z_accel       = ui->grapher_widget->add_curve("z_accel");
    curve_temperature   = ui->grapher_widget->add_curve("temperature");
    curve_x_gyro        = ui->grapher_widget->add_curve("x_gyro");
    curve_y_gyro        = ui->grapher_widget->add_curve("y_gyro");
    curve_z_gyro        = ui->grapher_widget->add_curve("z_gyro");

    curve_x_angle        = ui->grapher_widget->add_curve("x_angle");
    curve_y_angle        = ui->grapher_widget->add_curve("y_angle");
    curve_z_angle        = ui->grapher_widget->add_curve("z_angle");
#else
    ui->grapher_widget->setVisible(false);
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
        //emit error(QString("sl.count = %1").arg(sl.count()));
        dirty_array.clear();
        return;
    }

    int index = 0;
    int err             = sl.at(index++).toInt();
    qreal x_accel       = sl.at(index++).toDouble();
    qreal y_accel       = sl.at(index++).toDouble();
    qreal z_accel       = sl.at(index++).toDouble();
    qreal temperature   = sl.at(index++).toDouble();
    qreal x_gyro        = sl.at(index++).toDouble();
    qreal y_gyro        = sl.at(index++).toDouble();
    qreal z_gyro        = sl.at(index++).toDouble();
    qreal x_angle       = sl.at(index++).toDouble();
    qreal y_angle       = sl.at(index++).toDouble();
    qreal z_angle       = sl.at(index++).toDouble();

    if(x_angle < 0) x_angle += 360.0;
    if(y_angle < 0) y_angle += 360.0;
    if(z_angle < 0) z_angle += 360.0;

    //---
    ui->gl_widget->setXRotation(static_cast<int>(x_angle));
    ui->gl_widget->setYRotation(static_cast<int>(y_angle));
    ui->gl_widget->setZRotation(static_cast<int>(z_angle));
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

    ui->grapher_widget->add_curve_data(curve_x_angle,       x_angle);
    ui->grapher_widget->add_curve_data(curve_y_angle,       y_angle);
    ui->grapher_widget->add_curve_data(curve_z_angle,       z_angle);
#endif

    ui->lcd_widget->display("display_error", err);
    ui->lcd_widget->display("display_x_accel", convert(x_accel));
    ui->lcd_widget->display("display_y_accel", convert(y_accel));
    ui->lcd_widget->display("display_z_accel", convert(z_accel));
    ui->lcd_widget->display("display_x_gyro", convert(x_gyro));
    ui->lcd_widget->display("display_y_gyro", convert(y_gyro));
    ui->lcd_widget->display("display_z_gyro", convert(z_gyro));

    //    16384.0 - 9.8
    //    x       - y

    //    y = x * 9.8 / 16384.0;

    if(x_accel > 16384.0) x_accel -= 16384.0;
    if(y_accel > 16384.0) y_accel -= 16384.0;
    if(z_accel > 16384.0) z_accel -= 16384.0;

    if(x_accel < -16384.0) x_accel += 16384.0;
    if(y_accel < -16384.0) y_accel += 16384.0;
    if(z_accel < -16384.0) z_accel += 16384.0;

    ui->lcd_widget->display("display_x_angle", QString("%1").arg(x_angle, 0, 'f', 2));
    ui->lcd_widget->display("display_y_angle", QString("%1").arg(y_angle, 0, 'f', 2));
    ui->lcd_widget->display("display_z_angle", QString("%1").arg(z_angle, 0, 'f', 2));
    ui->lcd_widget->display("display_temperature", QString("%1").arg(temperature, 0, 'f', 2));

    //https://electronics.stackexchange.com/questions/39714/how-to-read-a-gyro-accelerometer
#if 1
    ui->lcd_widget->display("display_x_real_accel", QString("%1").arg(x_accel / 16384.0, 0, 'f', 2));
    ui->lcd_widget->display("display_y_real_accel", QString("%1").arg(y_accel / 16384.0, 0, 'f', 2));
    ui->lcd_widget->display("display_z_real_accel", QString("%1").arg(z_accel / 16384.0, 0, 'f', 2));
#else
    ui->lcd_widget->display("display_x_real_accel", QString("%1").arg(x_accel * 9.8 / 16384.0, 0, 'f', 2));
    ui->lcd_widget->display("display_y_real_accel", QString("%1").arg(y_accel * 9.8 / 16384.0, 0, 'f', 2));
    ui->lcd_widget->display("display_z_real_accel", QString("%1").arg(z_accel * 9.8 / 16384.0, 0, 'f', 2));
#endif

    dirty_array.clear();
}
//--------------------------------------------------------------------------------
#include "qwt_legend_label.h"
#include "test_glwidget.hpp"

void MainBox::test(void)
{
    emit info("test");

#if 1
    Test_GLWidget *tw = new Test_GLWidget();
    tw->show();
#endif

#if 0
    ui->grapher_widget->test2();
#endif

#if 0
    foreach (QLCDNumber *display, display_widgets)
    {
        display->display(-16384.99);
    }
#endif
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
