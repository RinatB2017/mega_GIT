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
#include "ui_get_acceleration_data.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mainwindow.hpp"
#include "get_acceleration_data.hpp"
#include "defines.hpp"

#include "serialbox5_fix_baudrate.hpp"
#include "glwidget.h"
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
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    init_serial();
    init_grapher();
    init_gl_widget();

#if 0
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->load_setting();
    }
#endif
    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_serial(void)
{
    ui->serial_widget->set_fix_baudrate(115200);
    ui->serial_widget->add_menu(2);
    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::init_grapher(void)
{
    curve_valuesAccel_x = ui->grapher_widget->add_curve("Accel_x");
    curve_valuesAccel_y = ui->grapher_widget->add_curve("Accel_y");
    curve_valuesAccel_z = ui->grapher_widget->add_curve("Accel_z");
    curve_valuesResult_x = ui->grapher_widget->add_curve("Result_x");
    curve_valuesResult_y = ui->grapher_widget->add_curve("Result_y");
    curve_valuesResult_z = ui->grapher_widget->add_curve("Result_z");
    curve_valuesAccelMotion_x = ui->grapher_widget->add_curve("AccelMotion_x");
    curve_valuesAccelMotion_y = ui->grapher_widget->add_curve("AccelMotion_y");
    curve_valuesAccelMotion_z = ui->grapher_widget->add_curve("AccelMotion_z");
    curve_valuesAccelGravity_x = ui->grapher_widget->add_curve("AccelGravity_x");
    curve_valuesAccelGravity_y = ui->grapher_widget->add_curve("AccelGravity_y");
    curve_valuesAccelGravity_z = ui->grapher_widget->add_curve("AccelGravity_z");
    curve_valuesLinAccel_x = ui->grapher_widget->add_curve("LinAccel_x");
    curve_valuesLinAccel_y = ui->grapher_widget->add_curve("LinAccel_y");
    curve_valuesLinAccel_z = ui->grapher_widget->add_curve("LinAccel_z");
    curve_valuesGravity_x = ui->grapher_widget->add_curve("Gravity_x");
    curve_valuesGravity_y = ui->grapher_widget->add_curve("Gravity_y");
    curve_valuesGravity_z = ui->grapher_widget->add_curve("Gravity_z");
    curve_valuesMagnet_x = ui->grapher_widget->add_curve("Magnet_x");
    curve_valuesMagnet_y = ui->grapher_widget->add_curve("Magnet_y");
    curve_valuesMagnet_z = ui->grapher_widget->add_curve("Magnet_z");

    ui->grapher_widget->set_title_axis_X("time");
    ui->grapher_widget->set_title_axis_Y("value");

    ui->grapher_widget->set_axis_scale_x(0, 100);
    ui->grapher_widget->set_axis_scale_y(-100, 100);
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
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray data)
{
    emit debug(data);

    QString text;
    text.append(data);

    text = text.replace(',', '.');  //TODO надо поправить в исходнике на android

    QStringList sl = text.split(';');
    //emit info(QString("count %1").arg(sl.count()));

    if(sl.count() != 21)
    {
        //emit error("sl.count() != 21");
        return;
    }

    int index = 0;
    qreal valuesAccel_x = sl.at(index++).toDouble();
    qreal valuesAccel_y = sl.at(index++).toDouble();
    qreal valuesAccel_z = sl.at(index++).toDouble();

    qreal valuesResult_x = sl.at(index++).toDouble();
    qreal valuesResult_y = sl.at(index++).toDouble();
    qreal valuesResult_z = sl.at(index++).toDouble();

    qreal valuesAccelMotion_x = sl.at(index++).toDouble();
    qreal valuesAccelMotion_y = sl.at(index++).toDouble();
    qreal valuesAccelMotion_z = sl.at(index++).toDouble();

    qreal valuesAccelGravity_x = sl.at(index++).toDouble();
    qreal valuesAccelGravity_y = sl.at(index++).toDouble();
    qreal valuesAccelGravity_z = sl.at(index++).toDouble();

    qreal valuesLinAccel_x = sl.at(index++).toDouble();
    qreal valuesLinAccel_y = sl.at(index++).toDouble();
    qreal valuesLinAccel_z = sl.at(index++).toDouble();

    qreal valuesGravity_x = sl.at(index++).toDouble();
    qreal valuesGravity_y = sl.at(index++).toDouble();
    qreal valuesGravity_z = sl.at(index++).toDouble();

    qreal valuesMagnet_x = sl.at(index++).toDouble();
    qreal valuesMagnet_y = sl.at(index++).toDouble();
    qreal valuesMagnet_z = sl.at(index++).toDouble();

    //---
    ui->grapher_widget->add_curve_data(curve_valuesAccel_x, valuesAccel_x);
    ui->grapher_widget->add_curve_data(curve_valuesAccel_y, valuesAccel_y);
    ui->grapher_widget->add_curve_data(curve_valuesAccel_z, valuesAccel_y);
    ui->grapher_widget->add_curve_data(curve_valuesResult_x, valuesResult_x);
    ui->grapher_widget->add_curve_data(curve_valuesResult_y, valuesResult_y);
    ui->grapher_widget->add_curve_data(curve_valuesResult_z, valuesResult_z);
    ui->grapher_widget->add_curve_data(curve_valuesAccelMotion_x, valuesAccelMotion_x);
    ui->grapher_widget->add_curve_data(curve_valuesAccelMotion_y, valuesAccelMotion_y);
    ui->grapher_widget->add_curve_data(curve_valuesAccelMotion_z, valuesAccelMotion_z);
    ui->grapher_widget->add_curve_data(curve_valuesAccelGravity_x, valuesAccelGravity_x);
    ui->grapher_widget->add_curve_data(curve_valuesAccelGravity_y, valuesAccelGravity_y);
    ui->grapher_widget->add_curve_data(curve_valuesAccelGravity_z, valuesAccelGravity_z);
    ui->grapher_widget->add_curve_data(curve_valuesLinAccel_x, valuesLinAccel_x);
    ui->grapher_widget->add_curve_data(curve_valuesLinAccel_y, valuesLinAccel_y);
    ui->grapher_widget->add_curve_data(curve_valuesLinAccel_z, valuesLinAccel_z);
    ui->grapher_widget->add_curve_data(curve_valuesGravity_x, valuesGravity_x);
    ui->grapher_widget->add_curve_data(curve_valuesGravity_y, valuesGravity_y);
    ui->grapher_widget->add_curve_data(curve_valuesGravity_z, valuesGravity_z);
    ui->grapher_widget->add_curve_data(curve_valuesMagnet_x, valuesMagnet_x);
    ui->grapher_widget->add_curve_data(curve_valuesMagnet_y, valuesMagnet_y);
    ui->grapher_widget->add_curve_data(curve_valuesMagnet_z, valuesMagnet_z);
    //---
    
    //---
    ui->lcd_widget->display("lcd_accel_x", valuesAccel_x);
    ui->lcd_widget->display("lcd_accel_y", valuesAccel_y);
    ui->lcd_widget->display("lcd_accel_z", valuesAccel_z);
    ui->lcd_widget->display("lcd_result_x", valuesResult_x);
    ui->lcd_widget->display("lcd_result_y", valuesResult_y);
    ui->lcd_widget->display("lcd_result_z", valuesResult_z);
    ui->lcd_widget->display("lcd_accelmotion_x", valuesAccelMotion_x);
    ui->lcd_widget->display("lcd_accelmotion_y", valuesAccelMotion_y);
    ui->lcd_widget->display("lcd_accelmotion_z", valuesAccelMotion_z);
    ui->lcd_widget->display("lcd_accelgravity_x", valuesAccelGravity_x);
    ui->lcd_widget->display("lcd_accelgravity_y", valuesAccelGravity_y);
    ui->lcd_widget->display("lcd_accelgravity_z", valuesAccelGravity_z);
    ui->lcd_widget->display("lcd_linaccel_x", valuesLinAccel_x);
    ui->lcd_widget->display("lcd_linaccel_y", valuesLinAccel_y);
    ui->lcd_widget->display("lcd_linaccel_z", valuesLinAccel_z);
    ui->lcd_widget->display("lcd_gravity_x", valuesGravity_x);
    ui->lcd_widget->display("lcd_gravity_y", valuesGravity_y);
    ui->lcd_widget->display("lcd_gravity_z", valuesGravity_z);
    ui->lcd_widget->display("lcd_magnet_x", valuesMagnet_x);
    ui->lcd_widget->display("lcd_magnet_y", valuesMagnet_y);
    ui->lcd_widget->display("lcd_magnet_z", valuesMagnet_z);
    //---

    //---
    if(valuesResult_x < 0) valuesResult_x += 360.0;
    if(valuesResult_y < 0) valuesResult_y += 360.0;
    if(valuesResult_z < 0) valuesResult_z += 360.0;

    ui->sl_X->setValue(static_cast<int>(valuesResult_x));
    ui->sl_Y->setValue(static_cast<int>(valuesResult_y));
    ui->sl_Z->setValue(static_cast<int>(valuesResult_z));
    //---
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
