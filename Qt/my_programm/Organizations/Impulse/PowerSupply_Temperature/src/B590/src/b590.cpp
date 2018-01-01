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
#include <QDebug>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_b590.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "b590.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#ifdef USE_B588
#include "b588_powersupply.hpp"
#endif
#ifdef USE_B590
#include "b590_powersupply.hpp"
#endif
//--------------------------------------------------------------------------------
B590::B590(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B590),
    is_blocked(false),
    timer(0)
{
    init();
}
//--------------------------------------------------------------------------------
B590::~B590()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void B590::init(void)
{
    ui->setupUi(this);

    createTestBar();

#ifndef NO_GRAPH
    grapher = new GrapherBox(this);
    grapher->set_title("parrots");
    grapher->set_title_axis_X("time");
    grapher->set_title_axis_Y("value");
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(0, qPow(2, 24));

    curve_1 = grapher->add_curve("channel 1");
    curve_2 = grapher->add_curve("channel 2");
    curve_3 = grapher->add_curve("channel 3");
    curve_temp = grapher->add_curve("temp");

    ui->grapher_layout->addWidget(grapher);
#endif

    powersupply = new Powersupply_B590(this);
    connect(powersupply, SIGNAL(info(QString)),     topLevelWidget(), SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),    topLevelWidget(), SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),    topLevelWidget(), SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),  topLevelWidget(), SIGNAL(trace(QString)));

    ui->sb_address->setMinimum(1);
    ui->sb_address->setMaximum(251);

    ui->sb_interval->setMinimum(1000);
    ui->sb_interval->setMaximum(0xFFFF);

    ui->sb_temp_vent->setMinimum(20);
    ui->sb_temp_vent->setMaximum(75);

    //ui->lcd_temperatute->setStyleSheet("background: black; color: red;");

    ui->btn_find_device->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lcd_temperatute->display("--.-- C");

    ui->le_channel_1->setReadOnly(true);
    ui->le_channel_2->setReadOnly(true);
    ui->le_channel_3->setReadOnly(true);
    ui->le_voltage->setReadOnly(true);
    ui->le_current->setReadOnly(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui->btn_power, SIGNAL(toggled(bool)), this, SLOT(measuring(bool)));
    connect(ui->btn_find_device, SIGNAL(clicked()), SLOT(find_device()));
}
//--------------------------------------------------------------------------------
void B590::measuring(bool state)
{
    if(state)
    {
        bool ok = powersupply->get_calibration_zero(&calib_U, &calib_I);
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
        timer->start(ui->sb_interval->value());
    }
    else
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void B590::find_device(void)
{
    bool ok = false;
    powersupply->set_ignore_bad_cmd(true);
    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::update(void)
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

    if(ui->cb_temp_vent->isChecked())
    {
        if(temp > ui->sb_temp_vent->value())
        {
            vent_auto();
        }
    }

    powersupply->set_state_silence(false);
    if(ok)
    {
        //ui->lcd_temperatute->display(QString("%1 C").arg(temp));
        ui->lcd_temperatute->display(QString("%1 C").arg(temp, 0, 'f', 2));
    }
    else
    {
        powersupply->print_last_error();
        ui->btn_power->setChecked(false);
        return;
    }
    powersupply->set_state_silence(true);
    ok = powersupply->get_ADC(&U_parrot, &I_parrot, &U_input_parrot);
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
        uint16_t U = convert_ADC_U_to_mV(U_parrot);
        uint16_t I = convert_ADC_I_to_mA(I_parrot);

        ui->le_voltage->setText(QString("%1").arg(U));
        ui->le_current->setText(QString("%1").arg(I));
    }
    else
    {
        powersupply->print_last_error();
        ui->btn_power->setChecked(false);
        return;
    }
}
//--------------------------------------------------------------------------------
void B590::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    testbar->addWidget(new QLabel("Ventilator"));
    QToolButton *btn_vent_off = add_button(testbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_MediaPause),
                                           "off",
                                           "off");
    QToolButton *btn_vent_auto = add_button(testbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                           "auto",
                                           "auto");
    btn_vent_off->setObjectName("btn_off");
    btn_vent_auto->setObjectName("btn_auto");

    connect(btn_vent_off,   SIGNAL(clicked()), this, SLOT(vent_off()));
    connect(btn_vent_auto,  SIGNAL(clicked()), this, SLOT(vent_auto()));
}
//--------------------------------------------------------------------------------
void B590::vent_off(void)
{
    bool ok = powersupply->set_vent_speed(0);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::vent_auto(void)
{
    bool ok = powersupply->set_vent_speed(MAGIC_VENTILATOR);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::test(void)
{
    QWidget *frame = powersupply->add_frame_find_powersupply();
    frame->show();
}
//--------------------------------------------------------------------------------
uint16_t B590::convert_ADC_U_to_mV(int32_t ADC_value)
{
#if 0
    double parrot_in_point = (double)(calib_U.Value_ADC_U_2 - calib_U.Value_ADC_U_1) / (double)(POINT_U_2 - POINT_U_1);
    if(parrot_in_point < 1) parrot_in_point = (double)128;

    //ADC_value -= (calib_U.Value_ADC_U_zero - ZERO_ADC);
    uint16_t result = (uint16_t)((double)(ADC_value - ZERO_ADC) / (double)parrot_in_point + (double)0.5);
    //uint16_t result = (uint16_t)((double)(ADC_value - calib_U.Value_ADC_U_zero) / (double)parrot_in_point + (double)0.5);
    return result;
#else
    unsigned long result = 0;
    double A = 0;
    double B = 0;
    double C = 0;
    double Uoi = 0;
    double Uoi1 = 0;
    double Moi = 0;
    double Moi1 = 0;
    double dNoi = 0;

    if(ADC_value <= calib_U.Value_ADC_U[0])
    {
        return 0;
    }
    for(int index=0; index<CALIBRATION_POINTS_U; index++)
    {
        if(ADC_value < calib_U.Value_ADC_U[index+1])
        {
            Moi  = calib_U.Value_ADC_U[index];
            Moi1 = calib_U.Value_ADC_U[index+1];
            Uoi  = calib_U.POINT_U[index];
            Uoi1 = calib_U.POINT_U[index+1];
            dNoi = ADC_value - calib_U.Value_ADC_U[index];

            A = calib_U.POINT_U[index];
            B = (double)(Uoi1 - Uoi) / (double)(Moi1 - Moi);
            C = dNoi;
            result = (uint16_t)(A + B * C);
            return result;        }
    }
    return 0xFFFF;
#endif
}
//--------------------------------------------------------------------------------
uint16_t B590::convert_ADC_I_to_mA(int32_t ADC_value)
{
#if 0
    double parrot_in_point = (double)(calib_I.Value_ADC_I_4 - calib_I.Value_ADC_I_1) / (double)(POINT_I_4 - POINT_I_1);
    if(parrot_in_point < 1) parrot_in_point = (double)128;

    uint16_t result = (uint16_t)((double)(ADC_value - ZERO_ADC) / (double)parrot_in_point + (double)0.5);
    return result;
#else
    unsigned long result = 0;
    double A = 0;
    double B = 0;
    double C = 0;
    double Ioi = 0;
    double Ioi1 = 0;
    double Noi  = 0;
    double Noi1 = 0;
    double dNoi = 0;

    if(ADC_value < calib_I.Value_ADC_I[0])
    {
        return 0;
    }
    for(int index=0; index<CALIBRATION_POINTS_I; index++)
    {
        if(ADC_value < calib_I.Value_ADC_I[index+1])
        {
            Ioi  = calib_I.POINT_I[index];
            Ioi1 = calib_I.POINT_I[index+1];
            Noi  = calib_I.Value_ADC_I[index];
            Noi1 = calib_I.Value_ADC_I[index+1];
            dNoi = ADC_value - calib_I.Value_ADC_I[index];

            A = Ioi;
            B = (double)(Ioi1 - Ioi) / (double)(Noi1 - Noi);
            C = dNoi;
            result = (uint16_t)(A + B * C);
            return result;
        }
    }
    return 0xFFFF;
#endif
}
//--------------------------------------------------------------------------------
void B590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
