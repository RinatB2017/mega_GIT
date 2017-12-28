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
#include <QInputDialog>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_b590.h"
//--------------------------------------------------------------------------------
#include "b590_powersupply.hpp"
#include "b590_powersupply.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "B590_fram.hpp"
#include "b590.hpp"
//--------------------------------------------------------------------------------
B590::B590(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B590),
    is_blocked(false)
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

    powersupply = new Powersupply_B590(this);
    connect(powersupply, SIGNAL(info(QString)),     topLevelWidget(), SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),    topLevelWidget(), SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),    topLevelWidget(), SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),    topLevelWidget(), SIGNAL(trace(QString)));

    grapher = new GrapherBox(this);
    curve_U = grapher->add_curve("U");
    curve_I = grapher->add_curve("I");
    curve_U_adc  = grapher->add_curve("U(adc)");
    curve_I_adc  = grapher->add_curve("I(adc)");
    curve_U3_adc = grapher->add_curve("U3(adc)");
    ui->grapher_layout->addWidget(grapher);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(measuring()));

    ui->sb_address->setMinimum(MIN_ADDRESS_B590);
    ui->sb_address->setMaximum(MAX_ADDRESS_B590);

    ui->sb_ref->setDecimals(4);
    ui->sb_shunt->setDecimals(6);

    ui->sb_address->setObjectName("sb_address");
    ui->sb_ref->setObjectName("sb_ref");
    ui->sb_shunt->setObjectName("sb_shunt");

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_FileDialogStart));

    connect(ui->btn_find_devices, SIGNAL(clicked()), this, SLOT(find_devices()));
    connect(ui->btn_run, SIGNAL(clicked()), this, SLOT(measuring()));
    connect(ui->btn_auto, SIGNAL(clicked(bool)), this, SLOT(auto_measuring(bool)));
    connect(ui->btn_on, SIGNAL(clicked()),  this, SLOT(rc_on()));
    connect(ui->btn_off, SIGNAL(clicked()), this, SLOT(rc_off()));

    setMinimumSize(1024, 600);
}
//--------------------------------------------------------------------------------
void B590::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

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
void B590::test(void)
{
    bool ok = false;
    double x = QInputDialog::getInt(this,
                                    tr("Измерение попугаев"),
                                    tr("Количество попугаев:"),
                                    1,
                                    1,
                                    0xFFFF,
                                    1,
                                    &ok);
    if(!ok) return;

    double vref = 2.5;
    double scale = qPow(2, 23);
    double res = (vref / scale) * x * 1e6;
    emit info(QString("x %1").arg(x));
    emit info(QString("res %1 uV").arg(res));
}
//--------------------------------------------------------------------------------
void B590::rc_on(void)
{
    powersupply->set_address(ui->sb_address->value());
    powersupply->rc_on();
}
//--------------------------------------------------------------------------------
void B590::rc_off(void)
{
    powersupply->set_address(ui->sb_address->value());
    powersupply->rc_off();
}
//--------------------------------------------------------------------------------
void B590::find_devices(void)
{
    emit info(tr("Поиск устройств начат"));
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
    emit info(tr("Поиск устройств завершен!"));
}
//--------------------------------------------------------------------------------
void B590::auto_measuring(bool state)
{
    if(state)
        timer->start(1000);
    else
        timer->stop();
}
//--------------------------------------------------------------------------------
uint16_t B590::convert_ADC_U_to_mV(unsigned long ADC_value)
{
#if 1
    double parrot_in_point = (double)(calib_U.Value_ADC_U[2] - calib_U.Value_ADC_U[1]) / (double)(POINT_U_2 - POINT_U_1);
    if(parrot_in_point < 1) parrot_in_point = (double)128;

    //ADC_value -= (calib_U.Value_ADC_U_zero - ZERO_ADC);
    uint16_t result = (uint16_t)((double)(ADC_value - ZERO_ADC) / (double)parrot_in_point + (double)0.5);
    //uint16_t result = (uint16_t)((double)(ADC_value - calib_U.Value_ADC_U_zero) / (double)parrot_in_point + (double)0.5);
    return result;
#else
    unsigned long result = 0;
    signed long temp = 0;
    signed long delta = 0;

    //---
    if(ADC_value <= calib_U.Value_ADC_U_0)
    {
        return 0;
    }
    //---
    if(ADC_value < calib_U.Value_ADC_U_1)
    {
        delta = ADC_value - calib_U.Value_ADC_U_0;
        temp = ((long long)(POINT_U_1 - POINT_U_0)) * delta / calib_U.Value_ADC_U_Delta_0_1;
        result = POINT_U_0 + temp;
        return result;
    }
    //---
    if(ADC_value < calib_U.Value_ADC_U_2)
    {
        delta = ADC_value - calib_U.Value_ADC_U_1;
        temp = ((long long)(POINT_U_2 - POINT_U_1)) * delta / calib_U.Value_ADC_U_Delta_1_2;
        result = POINT_U_1 + temp;
        return result;
    }
    //---
    if(ADC_value < calib_U.Value_ADC_U_3)
    {
        delta = ADC_value - calib_U.Value_ADC_U_2;
        temp = ((long long)(POINT_U_3 - POINT_U_2)) * delta / calib_U.Value_ADC_U_Delta_2_3;
        result = POINT_U_2 + temp;
        return result;
    }
    //---
    //delta = (signed long)(ADC_value - calib_U.Value_ADC_U_3);
    //temp = ((long long)(0xFFFF - POINT_U_3)) * delta / calib_U.Value_ADC_U_Delta_3_MAX;
    //result = POINT_U_3 + temp;
    //return result;

    return 0xFFFF;
#endif
}
//--------------------------------------------------------------------------------
uint16_t B590::convert_ADC_I_to_mA(unsigned long ADC_value)
{
#if 1
    double parrot_in_point = (double)(calib_I.Value_ADC_I[4] - calib_I.Value_ADC_I[1]) / (double)(POINT_I_4 - POINT_I_1);
    if(parrot_in_point < 1) parrot_in_point = (double)128;

    uint16_t result = (uint16_t)((double)(ADC_value - ZERO_ADC) / (double)parrot_in_point + (double)0.5);
    return result;
#else
    unsigned long result = 0;
    signed long temp = 0;
    signed long delta = 0;

    //---
    if(ADC_value < calib_I.Value_ADC_I_0)
    {
        return 0;
    }
    //---
    if(ADC_value < calib_I.Value_ADC_I_1)
    {
        delta = (signed long)(ADC_value - calib_I.Value_ADC_I_0);
        temp = ((long long)(POINT_I_1 - POINT_I_0)) * delta / calib_I.Value_ADC_I_Delta_0_1;
        result = POINT_I_0 + temp;
        return result;
    }
    //---
    if(ADC_value < calib_I.Value_ADC_I_2)
    {
        delta = (signed long)(ADC_value - calib_I.Value_ADC_I_1);
        temp = ((long long)(POINT_I_2 - POINT_I_1)) * delta / calib_I.Value_ADC_I_Delta_1_2;
        result = POINT_I_1 + temp;
        return result;
    }
    //---
    if(ADC_value < calib_I.Value_ADC_I_3)
    {
        delta = (signed long)(ADC_value - calib_I.Value_ADC_I_2);
        temp = ((long long)(POINT_I_3 - POINT_I_2)) * delta / calib_I.Value_ADC_I_Delta_2_3;
        result = POINT_I_2 + temp;
        return result;
    }
    //---
    if(ADC_value < calib_I.Value_ADC_I_4)
    {
        delta = (signed long)(ADC_value - calib_I.Value_ADC_I_3);
        temp = ((long long)(POINT_I_4 - POINT_I_3)) * delta / calib_I.Value_ADC_I_Delta_3_4;
        result = POINT_I_3 + temp;
        return result;
    }
    //---
    if(ADC_value < calib_I.Value_ADC_I_5)
    {
        delta = (signed long)(ADC_value - calib_I.Value_ADC_I_4);
        temp = ((long long)(POINT_I_5 - POINT_I_4)) * delta / calib_I.Value_ADC_I_Delta_4_5;
        result = POINT_I_4 + temp;
        return result;
    }
    //---
    //delta = (signed long)(ADC_value - calib_I.Value_ADC_I_5);
    //temp = ((long long)(0xFFFF - POINT_I_3)) * delta / calib_I.Value_ADC_I_Delta_5_MAX;
    //result = POINT_I_4 + temp;
    //return result;

    return 0xFFFF;
#endif
}
//--------------------------------------------------------------------------------
int B590::measuring(void)
{
    uint32_t U_parrot = 0;
    uint32_t I_parrot = 0;
    uint32_t U_input = 0;
    //int value_uV = 0;
    bool ok = false;

    powersupply->set_state_silence(true);

    powersupply->set_address(ui->sb_address->value());
    ok = powersupply->get_ADC(&U_parrot,
                              &I_parrot,
                              &U_input);
    if(!ok) return powersupply->print_last_error();

    ok = powersupply->get_calibration_zero(&calib_U, &calib_I);
    if(!ok) return powersupply->print_last_error();

    uint16_t U_real = convert_ADC_U_to_mV(U_parrot);
    uint16_t I_real = convert_ADC_I_to_mA(I_parrot);
    //emit info(QString("U %1 I %2").arg(qAbs(U_real)).arg(qAbs(I_real)));
    //emit info(QString("I через шунт %1 A").arg(I_real / ui->sb_shunt->value()));

    //ok = multimeter->get_value(&value_uV);
    //if(!ok) return multimeter->print_last_error();

    grapher->add_curve_data(curve_U_adc,  U_parrot);
    grapher->add_curve_data(curve_I_adc,  I_parrot);
    grapher->add_curve_data(curve_U3_adc, U_input);
    grapher->add_curve_data(curve_U, U_real);
    grapher->add_curve_data(curve_I, I_real);

    powersupply->set_state_silence(false);
    return 0;
}
//--------------------------------------------------------------------------------
void B590::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void B590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
