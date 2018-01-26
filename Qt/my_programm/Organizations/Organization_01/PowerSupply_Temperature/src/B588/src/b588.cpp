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
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QComboBox>
#include <QTimer>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_b588.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "b588.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "b588_powersupply.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
B588::B588(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B588),
    is_blocked(false),
    timer(0)
{
    init();
}
//--------------------------------------------------------------------------------
B588::~B588()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void B588::init(void)
{
    ui->setupUi(this);

    createTestBar();

#ifndef NO_GRAPH
    //grapher = new GrapherBox(0, 1000, 0, qPow(2, 24), "parrots", "time", "value", this);
    grapher = new GrapherBox(this);
    curve_1 = grapher->add_curve("channel 1");
    curve_2 = grapher->add_curve("channel 2");
    curve_3 = grapher->add_curve("channel 3");
    curve_temp = grapher->add_curve("temp");

    ui->grapher_layout->addWidget(grapher);
#endif

    powersupply = new Powersupply_B588(this);
    connect(powersupply, SIGNAL(info(QString)),     topLevelWidget(), SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),    topLevelWidget(), SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),    topLevelWidget(), SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),  topLevelWidget(), SIGNAL(trace(QString)));

    ui->sb_address->setMinimum(1);
    ui->sb_address->setMaximum(251);

    ui->sb_interval->setMinimum(1000);
    ui->sb_interval->setMaximum(0xFFFF);

    //ui->lcd_temperatute->setStyleSheet("background: black; color: red;");

    ui->btn_find_device->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lcd_temperatute->display("--.-- C");

    ui->le_channel_1->setReadOnly(true);
    ui->le_channel_2->setReadOnly(true);
    ui->le_channel_3->setReadOnly(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui->btn_power, SIGNAL(toggled(bool)), this, SLOT(measuring(bool)));
    connect(ui->btn_find_device, SIGNAL(clicked()), SLOT(find_device()));
}
//--------------------------------------------------------------------------------
void B588::measuring(bool state)
{
    if(state)
    {
        timer->start(ui->sb_interval->value());
    }
    else
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void B588::find_device(void)
{
    bool ok = false;
    powersupply->set_ignore_bad_cmd(true);
    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        powersupply->print_last_error();
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void B588::update(void)
{
    //emit info("update");
    //unsigned char temp = 0;
    double temp = 0;
    bool ok = false;
    uint32_t U_parrot = 0;
    uint32_t I_parrot = 0;
    uint32_t U_input_parrot = 0;

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_state_silence(true);
    //ok = powersupply->get_temperature(&temp);
    ok = powersupply->get_accurate_temperature(&temp);
    //emit info(QString("temp %1").arg(temp));
    powersupply->set_state_silence(false);
    if(ok)
    {
        //ui->lcd_temperatute->display(QString("%1 C").arg(temp));
        ui->lcd_temperatute->display(QString("%1 C").arg(temp, 0, 'f', 2));
        correct_temperature();
    }
    else
    {
        powersupply->print_last_error();
        ui->btn_power->setChecked(false);
        return;
    }
    powersupply->set_state_silence(true);
    ok = powersupply->get_adv_ADC(&U_parrot, &I_parrot, &U_input_parrot);
    powersupply->set_state_silence(false);
    if(ok)
    {
        ui->le_channel_1->setText(QString("%1").arg(U_parrot));
        ui->le_channel_2->setText(QString("%1").arg(I_parrot));
        ui->le_channel_3->setText(QString("%1").arg(U_input_parrot));

#ifndef NO_GRAPH
        grapher->add_curve_data(curve_1, U_parrot);
        grapher->add_curve_data(curve_2, I_parrot);
        grapher->add_curve_data(curve_3, U_input_parrot);
        grapher->add_curve_data(curve_temp, temp);
#endif
    }
    else
    {
        powersupply->print_last_error();
        ui->btn_power->setChecked(false);
        return;
    }
}
//--------------------------------------------------------------------------------
void B588::correct_temperature(void)
{
    if(!ui->cb_correct_temperature->isChecked()) return;
    if(ui->sb_max_temp->value() <= ui->sb_min_temp->value()) return;

    bool ok = false;
    unsigned char temp = 0;
    unsigned int value = 0;

    ok = powersupply->get_temperature(&temp);
    if(ok)
    {
        value = (unsigned int)((float)(temp * MAGIC_VENTILATOR) / (float)(abs(ui->sb_max_temp->value() - ui->sb_min_temp->value())) + 0.5f);
        emit info(QString("speed %1").arg(value));
        ok = powersupply->set_vent_speed(value);
        if(!ok)
        {
            powersupply->print_last_error();
        }
    }
}
//--------------------------------------------------------------------------------
void B588::createTestBar(void)
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
    
    btn_test->setObjectName("btn_test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void B588::test(void)
{
    //QFrame *frame = powersupply->add_frame_find_powersupply();
    //frame->show();
}
//--------------------------------------------------------------------------------
void B588::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
