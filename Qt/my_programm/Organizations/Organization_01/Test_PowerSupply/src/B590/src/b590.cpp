/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QCoreApplication>
#include <QProgressBar>
#include <QTableWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QDebug>
#include <QMenu>
#include <QTime>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "ui_b590.h"
//--------------------------------------------------------------------------------
#include "b590.hpp"

#include "b590_powersupply.hpp"
#include "b590_toolbar.hpp"

#include "sleeper.h"
#include "mainwindow.hpp"
#include "grapherbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
//--------------------------------------------------------------------------------
#ifdef USE_V764
    #include "v764_multimeter.hpp"
#endif
#ifdef USE_V7642
    #include "v764_2_multimeter.hpp"
#endif
//--------------------------------------------------------------------------------
B590::B590(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B590)
{
    init();
}
//--------------------------------------------------------------------------------
B590::~B590()
{
    qDebug() << "~B590()";

    if(multimeter) multimeter->deleteLater();
    if(multimeter_V7862) multimeter_V7862->deleteLater();

    if(powersupply) powersupply->deleteLater();

    if(grapher) grapher->deleteLater();
}
//--------------------------------------------------------------------------------
void B590::init(void)
{
    ui->setupUi(this);

    createTestBar();

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(MIN_VOLTAGE_B590, MAX_VOLTAGE_B590);

    curve_powersuply = grapher->add_curve(tr("Источник"));
    curve_B7642 = grapher->add_curve(tr("В7-64/2"));
    curve_B7862 = grapher->add_curve(tr("В7-86/2"));

    curve_U_parrot = grapher->add_curve(tr("U parrot"));
    curve_I_parrot = grapher->add_curve(tr("I parrot"));
    curve_U_input_parrot = grapher->add_curve(tr("Uin parrot"));

    grapher->setMinimumWidth(640);
    ui->grapher_layout->addWidget(grapher);

    flag_stop_test_triangle  = false;
    flag_stop_test_stability = false;

    ui->cb_in_parrot->clear();
    ui->cb_in_parrot->addItem(VALUE_IN_PARROT_TEXT, QVariant(Qt::UserRole + VALUE_IN_PARROT));
    ui->cb_in_parrot->addItem(VALUE_IN_REAL_TEXT, QVariant(Qt::UserRole + VALUE_IN_REAL));

    ui->btn_start_test_triangle_U->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_start_test_triangle_I->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_start_test_triangle_UI->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_start_test_stability->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    ui->btn_stop_test_triangle_U->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_stop_test_triangle_I->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_stop_test_triangle_UI->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_stop_test_stability->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    ui->progressbar_U->setValue(0);
    ui->progressbar_I->setValue(0);
    ui->progressbar_UI->setValue(0);

    ui->sb_min_value_U->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_min_value_U->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_min_value_I->setMinimum(MIN_CURRENT_B590);
    ui->sb_min_value_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_min_value_UI->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_min_value_UI->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_min_value_UI->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_min_value_UI->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_value_U->setMinimum(1);
    ui->sb_value_U->setMaximum(MAX_CURRENT_B590);

    ui->sb_value_I->setMinimum(1);
    ui->sb_value_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_max_value_U->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_max_value_U->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_max_value_I->setMinimum(MIN_CURRENT_B590);
    ui->sb_max_value_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_max_value_UI->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_max_value_UI->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_max_value_UI->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_max_value_UI->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_step_U->setMinimum(1);
    ui->sb_step_U->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_step_I->setMinimum(1);
    ui->sb_step_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_step_UI->setMinimum(1);
    ui->sb_step_UI->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_address->setMinimum(MIN_ADDRESS_B590);
    ui->sb_address->setMaximum(MAX_ADDRESS_B590);

    ui->sb_stability_value_U->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_stability_value_U->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_stability_value_I->setMinimum(0);
    ui->sb_stability_value_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_timeout_U->setMinimum(1);
    ui->sb_timeout_U->setMaximum(0xFFFF);

    ui->sb_timeout_I->setMinimum(1);
    ui->sb_timeout_I->setMaximum(0xFFFF);

    ui->sb_timeout_UI->setMinimum(1);
    ui->sb_timeout_UI->setMaximum(0xFFFF);

    ui->sb_factor_V7642->setMinimum(0.0001f);
    ui->sb_factor_V7642->setMaximum(100.0f);
    ui->sb_factor_V7642->setDecimals(4);

    ui->sb_factor_V7862->setMinimum(0.0001f);
    ui->sb_factor_V7862->setMaximum(100.0f);
    ui->sb_factor_V7862->setDecimals(4);
    //---
    ui->sb_address_V7862->setMinimum(0);
    ui->sb_address_V7862->setMaximum(0xFF);

    ui->sb_serno_V7862->setMinimum(0);
    ui->sb_serno_V7862->setMaximum(9999);

    ui->sb_year_V7862->setMinimum(2000);
    ui->sb_year_V7862->setMaximum(2016);

    ui->sb_channel_V7862->setMinimum(0);
    ui->sb_channel_V7862->setMaximum(15);

    ui->sb_timeout_V7862->setMinimum(0);
    ui->sb_timeout_V7862->setMaximum(0xFFFF);
    //

    powersupply = new Powersupply_B590();
    connect(powersupply, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),  this, SIGNAL(trace(QString)));

#ifdef ADD_TOOLBAR
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        mw->addToolBarBreak();
        B590_toolbar *psBar_b590 = new B590_toolbar(tr("powersupply B5-90"), this);
        psBar_b590->setObjectName("ps_B590");
        connect(psBar_b590, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
        mw->addToolBar(Qt::TopToolBarArea, psBar_b590);
    }
#endif

#ifdef USE_V764
    multimeter = new Multimeter_V764();
#endif
#ifdef USE_V7642
    multimeter = new Multimeter_V764_2();
#endif

    connect(multimeter, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(multimeter, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(multimeter, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(multimeter, SIGNAL(trace(QString)),  this, SIGNAL(trace(QString)));

    multimeter_V7862 = new Multimeter_V786_2();
    connect(multimeter_V7862, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(multimeter_V7862, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(multimeter_V7862, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(multimeter_V7862, SIGNAL(trace(QString)),  this, SIGNAL(trace(QString)));

    connect(ui->btn_start_test_triangle_U, SIGNAL(clicked()), this, SLOT(test_triangle_U()));
    connect(ui->btn_stop_test_triangle_U,  SIGNAL(clicked()), this, SLOT(stop_test_triangle()));

    connect(ui->btn_start_test_triangle_I, SIGNAL(clicked()), this, SLOT(test_triangle_I()));
    connect(ui->btn_stop_test_triangle_I,  SIGNAL(clicked()), this, SLOT(stop_test_triangle()));

    connect(ui->btn_start_test_triangle_UI, SIGNAL(clicked()), this, SLOT(test_triangle_UI()));
    connect(ui->btn_stop_test_triangle_UI,  SIGNAL(clicked()), this, SLOT(stop_test_triangle()));

    connect(ui->btn_start_test_stability, SIGNAL(clicked()), this, SLOT(test_stability()));
    connect(ui->btn_stop_test_stability,  SIGNAL(clicked()), this, SLOT(stop_test_stability()));

    connect(ui->btn_correct_V7642_value, SIGNAL(clicked()), this, SLOT(correct_V7642_value()));
    connect(ui->btn_correct_V7862_value, SIGNAL(clicked()), this, SLOT(correct_V7862_value()));

    connect(ui->cb_active_V7642, SIGNAL(clicked(bool)), this, SLOT(active_V7642(bool)));
    connect(ui->cb_active_V7862, SIGNAL(clicked(bool)), this, SLOT(active_V7862(bool)));

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    connect(ui->btn_find_devices, SIGNAL(clicked()), this, SLOT(find_devices()));

    ui->cb_active_V7642->setChecked(true);
    ui->cb_active_V7862->setChecked(true);
}
//--------------------------------------------------------------------------------
void B590::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       tr("test_sinus"),
                                       tr("test_sinus"));
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
int B590::test(void)
{
    //grapher->test_sinus();

    int   measuring_U = 0;
    unsigned short parrot_I = 9980;
    unsigned short x = parrot_I;
    double shunt = 0.009992f;

    powersupply->set_address(ui->sb_address->value());
    if(!powersupply->rc_on())
    {
        return powersupply->print_last_error();
    }

    //---
    //for(int i=1; i<50; i++)
    for(float i=0.5; i<1.0; i+=0.01)
    {
        double U = shunt * 1e6 * i; //A
        int delta = 10;
        while((U - measuring_U) > delta)
        {
            parrot_I = x;
            if(!powersupply->set_UI_parrot(3000,
                                                parrot_I))
            {
                return powersupply->print_last_error();
            }

            if(!multimeter->get_value(&measuring_U))
            {
                return multimeter->print_last_error();
            }
            x = (unsigned short)((double)(parrot_I * U) /(double)measuring_U + 0.5f);
            qDebug() << x;
        }
        emit info(QString("I = %1 U = %2 measuring_U = %3 parrot_I %4")
                  .arg(i)
                  .arg(U)
                  .arg(measuring_U)
                  .arg(parrot_I));
        grapher->add_curve_data(curve_I_parrot, parrot_I);
        grapher->add_curve_data(curve_powersuply, U - measuring_U);
        //QMessageBox::information(this, "test", "found!");
    }
    powersupply->set_UI_parrot(0, 0);

    //---

    if(!powersupply->rc_off())
    {
        return powersupply->print_last_error();
    }

    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
void B590::find_devices(void)
{
    bool ok = false;

    emit info(tr("Поиск устройств"));

    emit debug("B590");
    powersupply->set_ignore_bad_cmd(ui->cb_ignore_bad_cmd->isChecked());
    ok = powersupply->find_device();
    if(!ok) emit error("Источник питания Б5-90 НЕ найден");

    if(ui->cb_active_V7862->isChecked())
    {
        QString port_name;
        ok = multimeter_V7862->find_device(&port_name);
        if(!ok) emit error("Мультиметр В7-86/2 НЕ найден");
    }

    if(ui->cb_active_V7642->isChecked())
    {
        ok = multimeter->find_device();
#ifdef USE_V764
        if(!ok) emit error("Мультиметр В7-64 НЕ найден");
#endif
#ifdef USE_V7642
        if(!ok) emit error("Мультиметр В7-64/2 НЕ найден");
#endif
    }

    emit info(tr("Поиск устройств закончен"));
}
//--------------------------------------------------------------------------------
void B590::wait_msec(int timeout_msec)
{
#ifdef FAKE
    Q_UNUSED(timeout_msec)
#else
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
            break;
    }
#endif
}
//--------------------------------------------------------------------------------
void B590::init_multimeter(void)
{
#ifdef USE_V7642
    multimeter->init_modbus();
#endif
}
//--------------------------------------------------------------------------------
void B590::measuring_V7642(void)
{
    int multimeter_value = 0;
    bool ok = multimeter->get_value(&multimeter_value);
    if(!ok)
    {
        multimeter->print_last_error();
        return;
    }
#if 0
    if(multimeter_value < 1e3)
    {
        emit info(QString(tr("В7-64/2: измеренное значение: %1")).arg(multimeter_value));
        grapher->add_curve_data(curve_B7642, multimeter_value * ui->sb_factor_V7642->value());
        return;
    }
    if(multimeter_value < 1e6)
    {
        multimeter_value /= 1e3;
        emit info(QString(tr("В7-64/2: измеренное значение: %1")).arg((float)multimeter_value / 1e3));
        grapher->add_curve_data(curve_B7642, ((float)multimeter_value / 1e3) * ui->sb_factor_V7642->value());
        return;
    }
    emit info(QString(tr("В7-64/2: измеренное значение: %1")).arg((float)multimeter_value / 1e6));
    grapher->add_curve_data(curve_B7642, ((float)multimeter_value / 1e6) * ui->sb_factor_V7642->value());
#else
    emit info(QString(tr("В7-64/2: измеренное значение: %1")).arg(multimeter_value));
    grapher->add_curve_data(curve_B7642, multimeter_value * ui->sb_factor_V7642->value());
#endif
}
//--------------------------------------------------------------------------------
void B590::measuring_V7862(void)
{
    double multimeter_value = 0;

    multimeter_V7862->set_retention(0);
    multimeter_V7862->set_deviation(0);
    multimeter_V7862->set_limit(0);
    multimeter_V7862->set_auto_limit(true);
    multimeter_V7862->set_timeout(ui->sb_timeout_V7862->value());
    multimeter_V7862->set_address(ui->sb_address_V7862->value());
    multimeter_V7862->set_channel(ui->sb_channel_V7862->value());
    multimeter_V7862->set_serno(ui->sb_serno_V7862->value());
    multimeter_V7862->set_year(ui->sb_year_V7862->value());

    bool ok = false;
    double value = 0;
    while(!ok)
    {
        multimeter_V7862->set_limit(LIMIT_AUTO);
        ok = multimeter_V7862->measuring_UDC(&value);
        if(!ok)
        {
            multimeter_V7862->print_last_error();
        }
    }
    emit info(QString(tr("В7-86/2: измеренное значение: %1")).arg(multimeter_value));
    grapher->add_curve_data(curve_B7862, (multimeter_value * 1000.0f) * ui->sb_factor_V7862->value());
}
//--------------------------------------------------------------------------------
int B590::min_to_max_b590_U(int min_U,
                            int max_U,
                            int step_U,
                            int current)
{
    bool ok = false;
    int voltage = 0;

#ifdef QT_DEBUG
    qDebug() << min_U << max_U << step_U << current;
#endif
    for(voltage = min_U; voltage < max_U; voltage+=step_U)
    {
        if(flag_stop_test_triangle) return E_B590_BREAK_MEASURING;

        ok = test_in_parrot();
        if(ok)
        {
            ok = powersupply->set_UI_parrot(voltage,
                                                 current);
            emit info(QString(tr("Установленное значение %1")).arg(voltage));
        }
        else
        {
            ok = powersupply->set_UI(voltage,
                                          current);
            emit info(QString(tr("Установленное значение %1")).arg((float)voltage / 1000.0f));
        }
        if(!ok)
        {
            return powersupply->print_last_error();
        }

#ifndef FAKE
        if(ui->sb_timeout_U->value() > 1) Sleeper::msleep(ui->sb_timeout_U->value());
#endif

        grapher->add_curve_data(curve_powersuply, grapher_index, voltage);
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }

        index_progressbar+=step_U;
        ui->progressbar_U->setValue(index_progressbar);
        grapher_index++;
    }

    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::min_to_max_b590_I(int min_I,
                            int max_I,
                            int step_I,
                            int voltage)
{
    bool ok = false;
    int current = 0;

#ifdef QT_DEBUG
    qDebug() << min_I << max_I << step_I << voltage;
#endif
    for(current = min_I; current < max_I; current+=step_I)
    {
        if(flag_stop_test_triangle) return E_B590_BREAK_MEASURING;

        ok = test_in_parrot();
        if(ok)
        {
            ok = powersupply->set_UI_parrot(voltage,
                                                 current);
            emit info(QString(tr("Установленное значение %1")).arg(current));
        }
        else
        {
            ok = powersupply->set_UI(voltage,
                                          current);
            emit info(QString(tr("Установленное значение %1")).arg((float)current / 1000.0f));
        }
        if(!ok)
        {
            return powersupply->print_last_error();
        }

#ifndef FAKE
        if(ui->sb_timeout_I->value() > 1) Sleeper::msleep(ui->sb_timeout_I->value());
#endif

        grapher->add_curve_data(curve_powersuply, grapher_index, current);
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }

        index_progressbar+=step_I;
        ui->progressbar_I->setValue(index_progressbar);
        grapher_index++;
    }

    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::max_to_min_b590_U(int min_U,
                            int max_U,
                            int step_U,
                            int current)
{
    bool ok = false;
    int voltage = 0;

#ifdef QT_DEBUG
    qDebug() << min_U << max_U << step_U << current;
#endif
    for(voltage = max_U; voltage >= min_U; voltage-=step_U)
    {
        if(flag_stop_test_triangle) return E_B590_BREAK_MEASURING;

        ok = test_in_parrot();
        if(ok)
        {
            ok = powersupply->set_UI_parrot(voltage,
                                                 current);
            emit info(QString(tr("Установленное значение %1")).arg(voltage));
        }
        else
        {
            ok = powersupply->set_UI(voltage,
                                          current);
            emit info(QString(tr("Установленное значение %1")).arg((float)voltage / 1000.0f));
        }
        if(!ok)
        {
            return powersupply->print_last_error();
        }

#ifndef FAKE
        if(ui->sb_timeout_U->value() > 1) Sleeper::msleep(ui->sb_timeout_U->value());
#endif

        grapher->add_curve_data(curve_powersuply, grapher_index, voltage);
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }

        index_progressbar+=step_U;
        ui->progressbar_U->setValue(index_progressbar);
        grapher_index++;
    }
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::max_to_min_b590_I(int min_I,
                            int max_I,
                            int step_I,
                            int voltage)
{
    bool ok = false;
    int current = 0;

#ifdef QT_DEBUG
    qDebug() << min_I << max_I << step_I << current;
#endif
    for(current = max_I; current >= min_I; current-=step_I)
    {
        if(flag_stop_test_triangle) return E_B590_BREAK_MEASURING;

        ok = test_in_parrot();
        if(ok)
        {
            ok = powersupply->set_UI_parrot(voltage,
                                                 current);
            emit info(QString(tr("Установленное значение %1")).arg(current));
        }
        else
        {
            ok = powersupply->set_UI(voltage,
                                          current);
            emit info(QString(tr("Установленное значение %1")).arg((float)current / 1000.0f));
        }
        if(!ok)
        {
            return powersupply->print_last_error();
        }

#ifndef FAKE
        if(ui->sb_timeout_I->value() > 1) Sleeper::msleep(ui->sb_timeout_I->value());
#endif

        grapher->add_curve_data(curve_powersuply, grapher_index, current);
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }

        index_progressbar+=step_I;
        ui->progressbar_I->setValue(index_progressbar);
        grapher_index++;
    }
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::min_to_max_b590_UI(int min_value, int max_value, int step)
{
    bool ok = false;
    int value = 0;

#ifdef QT_DEBUG
    qDebug() << min_value << max_value << step;
#endif
    for(value = min_value; value < max_value; value+=step)
    {
        if(flag_stop_test_triangle) return E_B590_BREAK_MEASURING;

        ok = test_in_parrot();
        if(ok)
        {
            ok = powersupply->set_UI_parrot(value,
                                                 value);
            emit info(QString(tr("Установленное значение %1")).arg(value));
        }
        else
        {
            ok = powersupply->set_UI(value,
                                          value);
            emit info(QString(tr("Установленное значение %1")).arg((float)value / 1000.0f));
        }
        if(!ok)
        {
            return powersupply->print_last_error();
        }

#ifndef FAKE
        if(ui->sb_timeout_UI->value() > 1) Sleeper::msleep(ui->sb_timeout_UI->value());
#endif

        grapher->add_curve_data(curve_powersuply, grapher_index, value);
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }

        index_progressbar+=step;
        ui->progressbar_UI->setValue(index_progressbar);
        grapher_index++;
    }

    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::max_to_min_b590_UI(int min_value, int max_value, int step)
{
    bool ok = false;
    int value = 0;

#ifdef QT_DEBUG
    qDebug() << min_value << max_value << step;
#endif
    for(value = max_value; value >= min_value; value-=step)
    {
        if(flag_stop_test_triangle) return E_B590_BREAK_MEASURING;

        ok = test_in_parrot();
        if(ok)
        {
            ok = powersupply->set_UI_parrot(value,
                                                 value);
            emit info(QString(tr("Установленное значение %1")).arg(value));
        }
        else
        {
            ok = powersupply->set_UI(value,
                                          value);
            emit info(QString(tr("Установленное значение %1")).arg((float)value / 1000.0f));
        }
        if(!ok)
        {
            return powersupply->print_last_error();
        }

#ifndef FAKE
        if(ui->sb_timeout_UI->value() > 1) Sleeper::msleep(ui->sb_timeout_UI->value());
#endif

        grapher->add_curve_data(curve_powersuply, grapher_index, value);
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }

        index_progressbar+=step;
        ui->progressbar_UI->setValue(index_progressbar);
        grapher_index++;
    }
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
void B590::test_triangle_U(void)
{
    test_triangle_b590_U();
}
//--------------------------------------------------------------------------------
void B590::test_triangle_I(void)
{
    test_triangle_b590_I();
}
//--------------------------------------------------------------------------------
void B590::test_triangle_UI(void)
{
    test_triangle_b590_UI();
}
//--------------------------------------------------------------------------------
void B590::test_stability(void)
{
    test_stability_b590();
}
//--------------------------------------------------------------------------------
int B590::test_stability_b590(void)
{
    bool ok = false;
    emit info(tr("Тест начат"));

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->set_retention(0);
        multimeter_V7862->set_deviation(0);
        multimeter_V7862->set_limit(0);
        multimeter_V7862->set_auto_limit(true);
        multimeter_V7862->set_timeout(ui->sb_timeout_V7862->value());
        multimeter_V7862->set_address(ui->sb_address_V7862->value());
        multimeter_V7862->set_channel(ui->sb_channel_V7862->value());
        multimeter_V7862->set_serno(ui->sb_serno_V7862->value());
        multimeter_V7862->set_year(ui->sb_year_V7862->value());
        bool ok = multimeter_V7862->remote_control_on();
        if(!ok)
        {
            multimeter_V7862->print_last_error();
            return multimeter_V7862->get_last_error();
        }
    }

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_delay_ms(1000);
    if(ui->cb_active_V7642->isChecked())
    {
        multimeter->set_delay_ms(1000);
    }

    powersupply->set_address(ui->sb_address->value());
    ok = powersupply->rc_on();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    QTime timer;
    timer.start();

    ok = stability_in_parrot();
    if(ok)
    {
        ok = powersupply->set_UI_parrot(ui->sb_stability_value_U->value(),
                                             ui->sb_stability_value_I->value());
    }
    else
    {
        ok = powersupply->set_UI(ui->sb_stability_value_U->value(),
                                      ui->sb_stability_value_I->value());
    }
    if(!ok)
    {
        return powersupply->print_last_error();
    }

#ifdef DISABLE_CORRIDOR
    ok = powersupply->corridor_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }
#endif

    flag_stop_test_stability = false;
    while(!flag_stop_test_stability)
    {
        if(ui->cb_active_V7862->isChecked())
        {
            measuring_V7862();
        }
        if(ui->cb_active_V7642->isChecked())
        {
            measuring_V7642();
        }
        measuring_B590();
    }

    ok = powersupply->rc_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->remote_control_off();
    }

    emit info(tr("Тест завершен"));
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
void B590::measuring_B590(void)
{
    uint32_t U_parrot = 0;
    uint32_t I_parrot = 0;
    uint32_t U_input_parrot = 0;
    powersupply->set_address(ui->sb_address->value());
    bool ok = powersupply->get_ADC(&U_parrot,
                                        &I_parrot,
                                        &U_input_parrot);
    if(ok)
    {
        emit info("add_curve_data");
        grapher->add_curve_data(curve_U_parrot,         U_parrot);
        grapher->add_curve_data(curve_I_parrot,         I_parrot);
        grapher->add_curve_data(curve_U_input_parrot,   U_input_parrot);
    }
}
//--------------------------------------------------------------------------------
void B590::block_interface(bool state)
{
    ui->sb_address->setDisabled(state);
    ui->sb_factor_V7642->setDisabled(state);
    ui->sb_adress_V7642->setDisabled(state);
    ui->btn_correct_V7642_value->setDisabled(state);

    ui->sb_stability_value_U->setDisabled(state);
    ui->sb_stability_value_I->setDisabled(state);

    ui->btn_start_test_stability->setDisabled(state);

    ui->btn_start_test_triangle_U->setDisabled(state);
    ui->btn_start_test_triangle_I->setDisabled(state);
    ui->btn_start_test_triangle_UI->setDisabled(state);

    ui->sb_min_value_U->setDisabled(state);
    ui->sb_max_value_U->setDisabled(state);

    ui->sb_min_value_I->setDisabled(state);
    ui->sb_max_value_I->setDisabled(state);

    ui->sb_min_value_UI->setDisabled(state);
    ui->sb_max_value_UI->setDisabled(state);

    ui->sb_step_U->setDisabled(state);
    ui->sb_step_I->setDisabled(state);

    ui->sb_step_UI->setDisabled(state);
    ui->sb_step_UI->setDisabled(state);

    ui->sb_value_U->setDisabled(state);
    ui->sb_value_I->setDisabled(state);

    ui->sb_timeout_U->setDisabled(state);
    ui->sb_timeout_I->setDisabled(state);

    ui->sb_timeout_UI->setDisabled(state);
    ui->sb_timeout_UI->setDisabled(state);

    ui->cb_in_parrot->setDisabled(state);
}
//--------------------------------------------------------------------------------
int B590::test_triangle_b590_U(void)
{
    bool ok = false;
    emit info(tr("Тест начат"));

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->set_retention(0);
        multimeter_V7862->set_deviation(0);
        multimeter_V7862->set_limit(3);
        multimeter_V7862->set_auto_limit(false);
        multimeter_V7862->set_timeout(ui->sb_timeout_V7862->value());
        multimeter_V7862->set_address(ui->sb_address_V7862->value());
        multimeter_V7862->set_channel(ui->sb_channel_V7862->value());
        multimeter_V7862->set_serno(ui->sb_serno_V7862->value());
        multimeter_V7862->set_year(ui->sb_year_V7862->value());
        bool ok = multimeter_V7862->remote_control_on();
        if(!ok)
        {
            return multimeter_V7862->print_last_error();
        }
    }

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_delay_ms(1000);
    if(ui->cb_active_V7642->isChecked())
    {
        multimeter->set_delay_ms(1000);
    }

    ok = powersupply->rc_on();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    int min = ui->sb_min_value_U->value();
    int max = ui->sb_max_value_U->value();
    int step = ui->sb_step_U->value();
    int current = ui->sb_value_I->value();
    ui->progressbar_U->setRange(min, min + (max - min) * 2);
    ui->progressbar_U->setValue(min);
    index_progressbar = min;

    block_interface(true);

    grapher_index = 0;
    flag_stop_test_triangle = false;
#ifdef DISABLE_CORRIDOR
    ok = powersupply->corridor_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }
#endif
    min_to_max_b590_U(min, max, step, current);
    max_to_min_b590_U(min, max, step, current);

    block_interface(false);

    ok = powersupply->rc_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->remote_control_off();
    }

    emit info(tr("Тест завершен"));
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::test_triangle_b590_I(void)
{
    bool ok = false;
    emit info(tr("Тест начат"));

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->set_retention(0);
        multimeter_V7862->set_deviation(0);
        multimeter_V7862->set_limit(3);
        multimeter_V7862->set_auto_limit(false);
        multimeter_V7862->set_timeout(ui->sb_timeout_V7862->value());
        multimeter_V7862->set_address(ui->sb_address_V7862->value());
        multimeter_V7862->set_channel(ui->sb_channel_V7862->value());
        multimeter_V7862->set_serno(ui->sb_serno_V7862->value());
        multimeter_V7862->set_year(ui->sb_year_V7862->value());
        bool ok = multimeter_V7862->remote_control_on();
        if(!ok)
        {
            multimeter_V7862->print_last_error();
            return multimeter_V7862->get_last_error();
        }
    }

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_delay_ms(1000);
    if(ui->cb_active_V7642->isChecked())
    {
        multimeter->set_delay_ms(1000);
    }

    ok = powersupply->rc_on();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    int min = ui->sb_min_value_I->value();
    int max = ui->sb_max_value_I->value();
    int step = ui->sb_step_I->value();
    int voltage = ui->sb_value_U->value();
    ui->progressbar_I->setRange(min, min + (max - min) * 2);
    ui->progressbar_I->setValue(min);
    index_progressbar = min;

    block_interface(true);

    grapher_index = 0;
    flag_stop_test_triangle = false;
#ifdef DISABLE_CORRIDOR
    ok = powersupply->corridor_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }
#endif
    min_to_max_b590_I(min, max, step, voltage);
    max_to_min_b590_I(min, max, step, voltage);

    block_interface(false);

    ok = powersupply->rc_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->remote_control_off();
    }

    emit info(tr("Тест завершен"));
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::test_triangle_b590_UI(void)
{
    bool ok = false;
    emit info(tr("Тест начат"));

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->set_retention(0);
        multimeter_V7862->set_deviation(0);
        multimeter_V7862->set_limit(3);
        multimeter_V7862->set_auto_limit(false);
        multimeter_V7862->set_timeout(ui->sb_timeout_V7862->value());
        multimeter_V7862->set_address(ui->sb_address_V7862->value());
        multimeter_V7862->set_channel(ui->sb_channel_V7862->value());
        multimeter_V7862->set_serno(ui->sb_serno_V7862->value());
        multimeter_V7862->set_year(ui->sb_year_V7862->value());
        bool ok = multimeter_V7862->remote_control_on();
        if(!ok)
        {
            multimeter_V7862->print_last_error();
            return multimeter_V7862->get_last_error();
        }
    }

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_delay_ms(1000);
    if(ui->cb_active_V7642->isChecked())
    {
        multimeter->set_delay_ms(1000);
    }

    ok = powersupply->rc_on();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    int min = ui->sb_min_value_UI->value();
    int max = ui->sb_max_value_UI->value();
    int step = ui->sb_step_UI->value();
    ui->progressbar_UI->setRange(min, min + (max - min) * 2);
    ui->progressbar_UI->setValue(min);
    index_progressbar = min;

    block_interface(true);

    grapher_index = 0;
    flag_stop_test_triangle = false;
#ifdef DISABLE_CORRIDOR
    ok = powersupply->corridor_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }
#endif
    min_to_max_b590_UI(min, max, step);
    max_to_min_b590_UI(min, max, step);

    block_interface(false);

    ok = powersupply->rc_off();
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    if(ui->cb_active_V7862->isChecked())
    {
        multimeter_V7862->remote_control_off();
    }

    emit info(tr("Тест завершен"));
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
void B590::stop_test_triangle(void)
{
    flag_stop_test_triangle = true;
}
//--------------------------------------------------------------------------------
void B590::stop_test_stability(void)
{
    flag_stop_test_stability = true;
}
//--------------------------------------------------------------------------------
bool B590::test_in_parrot(void)
{
    bool ok;
    int temp = ui->cb_in_parrot->itemData(ui->cb_in_parrot->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(ok)
    {
        if(temp == VALUE_IN_PARROT)
            return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool B590::stability_in_parrot(void)
{
    bool ok;
    int temp = ui->cb_in_parrot->itemData(ui->cb_in_parrot->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(ok)
    {
        if(temp == VALUE_IN_PARROT)
            return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void B590::correct_V7642_value(void)
{
    grapher->correct(curve_B7642, ui->sb_factor_V7642->value());
}
//--------------------------------------------------------------------------------
void B590::correct_V7862_value(void)
{
    grapher->correct(curve_B7862, ui->sb_factor_V7862->value());
}
//--------------------------------------------------------------------------------
void B590::active_V7642(bool state)
{
    ui->sb_adress_V7642->setEnabled(state);
    ui->sb_factor_V7642->setEnabled(state);
    ui->btn_correct_V7642_value->setEnabled(state);
}
//--------------------------------------------------------------------------------
void B590::active_V7862(bool state)
{
    ui->sb_serno_V7862->setEnabled(state);
    ui->sb_year_V7862->setEnabled(state);
    ui->sb_address_V7862->setEnabled(state);
    ui->sb_channel_V7862->setEnabled(state);
    ui->sb_timeout_V7862->setEnabled(state);
    ui->sb_factor_V7862->setEnabled(state);
    ui->btn_correct_V7862_value->setEnabled(state);
}
//--------------------------------------------------------------------------------
bool B590::is_blocked_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void B590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
