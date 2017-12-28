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
#include <QStandardItemModel>
#include <QDialogButtonBox>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QTextStream>
#include <QToolButton>
#include <QMessageBox>
#include <QSettings>
#include <QVariant>
#include <QToolBar>
#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QMenu>
//--------------------------------------------------------------------------------
#include <qtpropertymanager.h>
#include <qtpropertybrowser.h>
#include <qtvariantproperty.h>
#include <qttreepropertybrowser.h>
//--------------------------------------------------------------------------------
#include "database.hpp"
#include "openreports.h"
#include "myreport.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "b590_powersupply.hpp"
//--------------------------------------------------------------------------------
#ifdef V764
#include "v764_multimeter.hpp"
#endif
#ifdef V7642
#include "v764_2_multimeter.hpp"
#endif
#ifdef V780
#include "v780_multimeter.hpp"
#endif
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "b590.hpp"
#include "defines.hpp"
#include "b590_toolbar.hpp"
#include "parrotbar.hpp"
//--------------------------------------------------------------------------------
#ifndef USER_CALIBRATIONS
#include "ui_b590.h"
#else
#include "ui_b590_user.h"
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
    delete ui;
}
//--------------------------------------------------------------------------------
bool B590::get_value_setting(const QString &name,
                             QVariant *value,
                             QVariant *max_deviation,
                             QVariant *begin_DAC,
                             QVariant *end_DAC)
{
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    QVariant temp;
    bool ok = true;

    settings->beginGroup(name);

    temp = settings->value("value", -1);
    if(temp == -1)
        ok = false;
    else
        *value = temp;

    temp = settings->value("max_deviation", -1);
    if(temp == -1)
        ok = false;
    else
        *max_deviation = temp;

    temp = settings->value("begin_DAC", -1);
    if(temp == -1)
        ok = false;
    else
        *begin_DAC = temp;

    temp = settings->value("end_DAC", -1);
    if(temp == -1)
        ok = false;
    else
        *end_DAC = temp;

    settings->endGroup();
    settings->deleteLater();

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::create_setting(const QString  &name,
                          const QVariant &value,
                          const QVariant &max_deviation,
                          const QVariant &begin_DAC,
                          const QVariant &end_DAC)
{
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(name);
    settings->setValue("value", value);
    settings->setValue("max_deviation", max_deviation);
    settings->setValue("begin_DAC", begin_DAC);
    settings->setValue("end_DAC",   end_DAC);
    settings->endGroup();
    settings->deleteLater();

    return true;
}
//--------------------------------------------------------------------------------
#include <QDebug>
void B590::load_setting(void)
{
#ifndef USER_CALIBRATIONS
    QVariant value = 0;
    QVariant max_deviation = 0;
    QVariant begin_DAC = 0;
    QVariant end_DAC = 0;
    bool ok = false;

    //---
    ok = get_value_setting("U_0", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_U_point_0->setValue(value);
        item_U_max_deviation_point_0->setValue(max_deviation);
        item_U_begin_DAC_point_0->setValue(begin_DAC);
    }
    ok = get_value_setting("U_1", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_U_point_1->setValue(value);
        item_U_max_deviation_point_1->setValue(max_deviation);
        item_U_begin_DAC_point_1->setValue(begin_DAC);
    }
    ok = get_value_setting("U_2", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_U_point_2->setValue(value);
        item_U_max_deviation_point_2->setValue(max_deviation);
        item_U_begin_DAC_point_2->setValue(begin_DAC);
    }

    ok = get_value_setting("U_3", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_U_point_3->setValue(value);
        item_U_max_deviation_point_3->setValue(max_deviation);
        item_U_begin_DAC_point_3->setValue(begin_DAC);
    }

    //---
    ok = get_value_setting("I_0", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_0->setValue(value);
        item_I_max_deviation_point_0->setValue(max_deviation);
        item_I_begin_DAC_point_0->setValue(begin_DAC);
        item_I_end_DAC_point_0->setValue(end_DAC);
    }
    ok = get_value_setting("I_1", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_1->setValue(value);
        item_I_max_deviation_point_1->setValue(max_deviation);
        item_I_begin_DAC_point_1->setValue(begin_DAC);
    }
    ok = get_value_setting("I_2", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_2->setValue(value);
        item_I_max_deviation_point_2->setValue(max_deviation);
        item_I_begin_DAC_point_2->setValue(begin_DAC);
    }
    ok = get_value_setting("I_3", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_3->setValue(value);
        item_I_max_deviation_point_3->setValue(max_deviation);
        item_I_begin_DAC_point_3->setValue(begin_DAC);
    }
    ok = get_value_setting("I_4", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_4->setValue(value);
        item_I_max_deviation_point_4->setValue(max_deviation);
        item_I_begin_DAC_point_4->setValue(begin_DAC);
    }
    ok = get_value_setting("I_5", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_5->setValue(value);
        item_I_max_deviation_point_5->setValue(max_deviation);
        item_I_begin_DAC_point_5->setValue(begin_DAC);
    }
    ok = get_value_setting("I_6", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_6->setValue(value);
        item_I_max_deviation_point_6->setValue(max_deviation);
        item_I_begin_DAC_point_6->setValue(begin_DAC);
    }
    ok = get_value_setting("I_7", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_7->setValue(value);
        item_I_max_deviation_point_7->setValue(max_deviation);
        item_I_begin_DAC_point_7->setValue(begin_DAC);
    }
    ok = get_value_setting("I_8", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_8->setValue(value);
        item_I_max_deviation_point_8->setValue(max_deviation);
        item_I_begin_DAC_point_8->setValue(begin_DAC);
    }
    ok = get_value_setting("I_9", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_9->setValue(value);
        item_I_max_deviation_point_9->setValue(max_deviation);
        item_I_begin_DAC_point_9->setValue(begin_DAC);
    }
    ok = get_value_setting("I_10", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_10->setValue(value);
        item_I_max_deviation_point_10->setValue(max_deviation);
        item_I_begin_DAC_point_10->setValue(begin_DAC);
    }
    ok = get_value_setting("I_11", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_11->setValue(value);
        item_I_max_deviation_point_11->setValue(max_deviation);
        item_I_begin_DAC_point_11->setValue(begin_DAC);
    }
    ok = get_value_setting("I_12", &value, &max_deviation, &begin_DAC, &end_DAC);
    if(ok)
    {
        item_I_point_12->setValue(value);
        item_I_max_deviation_point_12->setValue(max_deviation);
        item_I_begin_DAC_point_12->setValue(begin_DAC);
    }
#endif
}
//--------------------------------------------------------------------------------
void B590::save_setting(void)
{
#ifndef USER_CALIBRATIONS
    bool ok = false;


    ok = create_setting("U_0",
                        item_U_point_0->value(),
                        item_U_max_deviation_point_0->value(),
                        item_U_begin_DAC_point_0->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save U_0");
        return;
    }

    ok = create_setting("U_1",
                        item_U_point_1->value(),
                        item_U_max_deviation_point_1->value(),
                        item_U_begin_DAC_point_1->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save U_1");
        return;
    }

    ok = create_setting("U_2",
                        item_U_point_2->value(),
                        item_U_max_deviation_point_2->value(),
                        item_U_begin_DAC_point_2->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save U_2");
        return;
    }

    ok = create_setting("U_3",
                        item_U_point_3->value(),
                        item_U_max_deviation_point_3->value(),
                        item_U_begin_DAC_point_3->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save U_3");
        return;
    }

    //---
    ok = create_setting("I_0",
                        item_I_point_0->value(),
                        item_I_max_deviation_point_0->value(),
                        item_I_begin_DAC_point_0->value(),
                        item_I_end_DAC_point_0->value());
    if(!ok)
    {
        emit error("error save I_0");
        return;
    }

    ok = create_setting("I_1",
                        item_I_point_1->value(),
                        item_I_max_deviation_point_1->value(),
                        item_I_begin_DAC_point_1->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_1");
        return;
    }

    ok = create_setting("I_2",
                        item_I_point_2->value(),
                        item_I_max_deviation_point_2->value(),
                        item_I_begin_DAC_point_2->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_2");
        return;
    }

    ok = create_setting("I_3",
                        item_I_point_3->value(),
                        item_I_max_deviation_point_3->value(),
                        item_I_begin_DAC_point_3->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_3");
        return;
    }

    ok = create_setting("I_4",
                        item_I_point_4->value(),
                        item_I_max_deviation_point_4->value(),
                        item_I_begin_DAC_point_4->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_4");
        return;
    }

    ok = create_setting("I_5",
                        item_I_point_5->value(),
                        item_I_max_deviation_point_5->value(),
                        item_I_begin_DAC_point_5->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_5");
        return;
    }

    ok = create_setting("I_6",
                        item_I_point_6->value(),
                        item_I_max_deviation_point_6->value(),
                        item_I_begin_DAC_point_6->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_6");
        return;
    }

    ok = create_setting("I_7",
                        item_I_point_7->value(),
                        item_I_max_deviation_point_7->value(),
                        item_I_begin_DAC_point_7->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_7");
        return;
    }

    ok = create_setting("I_8",
                        item_I_point_8->value(),
                        item_I_max_deviation_point_8->value(),
                        item_I_begin_DAC_point_8->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_8");
        return;
    }

    ok = create_setting("I_9",
                        item_I_point_9->value(),
                        item_I_max_deviation_point_9->value(),
                        item_I_begin_DAC_point_9->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_9");
        return;
    }

    ok = create_setting("I_10",
                        item_I_point_10->value(),
                        item_I_max_deviation_point_10->value(),
                        item_I_begin_DAC_point_10->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_10");
        return;
    }

    ok = create_setting("I_11",
                        item_I_point_11->value(),
                        item_I_max_deviation_point_11->value(),
                        item_I_begin_DAC_point_11->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_11");
        return;
    }

    ok = create_setting("I_12",
                        item_I_point_12->value(),
                        item_I_max_deviation_point_12->value(),
                        item_I_begin_DAC_point_12->value(),
                        0xFFFF);
    if(!ok)
    {
        emit error("error save I_12");
        return;
    }

    emit info("OK");
#endif
}
//--------------------------------------------------------------------------------
void B590::init(void)
{
    ui->setupUi(this);

#ifdef V764
    multimeter = new Multimeter_V764();
    ui->btn_find_devices->setText(tr("Поиск Б5-90 и В7-64 "));
#endif
#ifdef V7642
    multimeter = new Multimeter_V764_2();
    ui->btn_find_devices->setText(tr("Поиск Б5-90 и В7-64/2 "));
#endif
#ifdef V780
    multimeter = new Multimeter_V780();
    ui->btn_find_devices->setText(tr("Поиск Б5-90 и В7-80 "));
#endif

    powersupply = new Powersupply_B590();
    powersupply->set_delay_ms(TIME_READY_POWERSUPPLY_MS);

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        connect(multimeter, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(multimeter, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(multimeter, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(multimeter, SIGNAL(trace(QString)), mw, SIGNAL(trace(QString)));

        connect(powersupply, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(powersupply, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(powersupply, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(powersupply, SIGNAL(trace(QString)), mw, SIGNAL(trace(QString)));
    }

#ifdef TEST
    createParrotBar();
    createTestBar();
    createPowerSupplyBar();
#endif

    createInfoBar();

#ifndef USER_CALIBRATIONS
    ui->property_layout->setMargin(0);
    ui->property_layout->setSpacing(0);
    btn_set_max_U = new QPushButton("set MAX U");
    btn_set_max_I = new QPushButton("set MAX I");
    btn_clear_UI = new QPushButton("clear UI");
    connect(btn_set_max_U,  SIGNAL(released()), this, SLOT(set_max_U()));
    connect(btn_set_max_I,  SIGNAL(released()), this, SLOT(set_max_I()));
    connect(btn_clear_UI,   SIGNAL(released()), this, SLOT(set_clear_UI()));
    ui->property_layout->addWidget(btn_set_max_U);
    ui->property_layout->addWidget(btn_set_max_I);
    ui->property_layout->addWidget(btn_clear_UI);

    createPropertyBox();
    //---
    QPushButton *btn_save = new QPushButton("save setting");
    connect(btn_save, SIGNAL(released()), this, SLOT(save_setting()));
    ui->property_layout->addWidget(btn_save);
    //---
    load_setting();
    //---
#endif

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

#ifndef USER_CALIBRATIONS
    ui->sb_year->setMinimum(1980);
    ui->sb_year->setMaximum(2020);

    ui->sb_month->setMinimum(1);
    ui->sb_month->setMaximum(12);

    ui->sb_serno->setMinimum(0);
    ui->sb_serno->setMaximum(9999);
#endif

    ui->sb_address->setMinimum(MIN_ADDRESS_B590);
    ui->sb_address->setMaximum(MAX_ADDRESS_B590);

    ui->sb_shunt->setMinimum(0.0000001f);
    ui->sb_shunt->setSingleStep(0.0000001f);
    ui->sb_shunt->setDecimals(7);

    ui->pb_calibration_ADC_U->setValue(0);
    ui->pb_calibration_DAC_U->setValue(0);
    ui->pb_calibration_ADC_I->setValue(0);
    ui->pb_calibration_DAC_I->setValue(0);
    ui->pb_check_U->setValue(0);
    ui->pb_check_I->setValue(0);

    ui->table_U->setMinimumWidth(640);
    ui->table_I->setMinimumWidth(640);

    flag_stop_calibration_ADC_U = false;
    flag_stop_calibration_DAC_U = false;
    flag_stop_calibration_ADC_I = false;
    flag_stop_calibration_DAC_I = false;
    flag_stop_check_U = false;
    flag_stop_check_I = false;

    //---
    model_U = new QStandardItemModel(0, 5, this);
    model_U->setHeaderData(0, Qt::Horizontal, tr("Напряжение, В"));
    model_U->setHeaderData(1, Qt::Horizontal, tr("Макс. откл., мВ"));
    model_U->setHeaderData(2, Qt::Horizontal, tr("Отклонение, мВ"));
    model_U->setHeaderData(3, Qt::Horizontal, tr("%. откл."));
    model_U->setHeaderData(4, Qt::Horizontal, tr("Результат теста"));
    //---
    model_I = new QStandardItemModel(0, 5, this);
    model_I->setHeaderData(0, Qt::Horizontal, tr("Ток, А"));
    model_I->setHeaderData(1, Qt::Horizontal, tr("Макс. откл., мВ"));
    model_I->setHeaderData(2, Qt::Horizontal, tr("Отклонение, мВ"));
    model_I->setHeaderData(3, Qt::Horizontal, tr("%. откл."));
    model_I->setHeaderData(4, Qt::Horizontal, tr("Результат теста"));
    //---
    ui->table_U->setModel(model_U);
    ui->table_I->setModel(model_I);
    //---
    ui->table_U->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->table_U->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table_U->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->table_U->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->table_U->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    //---
    ui->table_I->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->table_I->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table_I->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->table_I->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->table_I->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    //---

    ui->dsb_pz_U->setDecimals(1);
    ui->dsb_pz_U->setMinimum(0.1f);
    ui->dsb_pz_U->setMaximum(1.0f);
    ui->dsb_pz_U->setSingleStep(0.1f);

    ui->dsb_pz_I->setDecimals(1);
    ui->dsb_pz_I->setMinimum(0.1f);
    ui->dsb_pz_I->setMaximum(1.0f);
    ui->dsb_pz_I->setSingleStep(0.1f);

    connect(ui->btn_find_devices, SIGNAL(pressed()), this, SLOT(find_devices()));

    connect(ui->btn_calibration_ADC_U,  SIGNAL(pressed()), this, SLOT(calibration_ADC_U()));
    connect(ui->btn_calibration_DAC_U,  SIGNAL(pressed()), this, SLOT(calibration_DAC_U()));
    connect(ui->btn_calibration_ADC_I,  SIGNAL(pressed()), this, SLOT(calibration_ADC_I()));
    connect(ui->btn_calibration_DAC_I,  SIGNAL(pressed()), this, SLOT(calibration_DAC_I()));
    connect(ui->btn_check_U,            SIGNAL(pressed()), this, SLOT(check_control_point_U()));
    connect(ui->btn_check_I,            SIGNAL(pressed()), this, SLOT(check_control_point_I()));

#ifndef USER_CALIBRATIONS
    connect(ui->btn_save_data,          SIGNAL(pressed()), this, SLOT(save_data()));
    connect(ui->btn_check,              SIGNAL(pressed()), this, SLOT(check()));
    connect(ui->btn_report,             SIGNAL(pressed()), this, SLOT(save_report()));
#endif

    connect(ui->btn_stop_calibration_ADC_U, SIGNAL(pressed()), this, SLOT(stop_calibration_ADC_U()));
    connect(ui->btn_stop_calibration_DAC_U, SIGNAL(pressed()), this, SLOT(stop_calibration_DAC_U()));
    connect(ui->btn_stop_calibration_ADC_I, SIGNAL(pressed()), this, SLOT(stop_calibration_ADC_I()));
    connect(ui->btn_stop_calibration_DAC_I, SIGNAL(pressed()), this, SLOT(stop_calibration_DAC_I()));
    connect(ui->btn_stop_check_U,           SIGNAL(pressed()), this, SLOT(stop_check_U()));
    connect(ui->btn_stop_check_I,           SIGNAL(pressed()), this, SLOT(stop_check_I()));

    connect(ui->btn_save_table_U, SIGNAL(pressed()), this, SLOT(save_table_U()));
    connect(ui->btn_save_table_I, SIGNAL(pressed()), this, SLOT(save_table_I()));

    block_interface(true);
}
//--------------------------------------------------------------------------------
void B590::find_devices(void)
{
    bool ok = false;

    ui->btn_find_devices->setEnabled(false);
    ui->sb_address->setEnabled(false);
    emit info(tr("Поиск устройств"));

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_ignore_bad_cmd(true);

    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-90 НЕ найден");
        ui->btn_find_devices->setEnabled(true);
        ui->sb_address->setEnabled(true);
        return;
    }

#ifndef USER_CALIBRATIONS
    //---
    uint8_t  type = 0;
    uint16_t year = 0;
    uint8_t  month = 0;
    uint16_t serno = 0;
    ok = powersupply->get_information(&type, &year, &month, &serno);
    if(ok)
    {
        ui->sb_year->setValue(year);
        ui->sb_month->setValue(month);
        ui->sb_serno->setValue(serno);
    }
    //---
#endif

    multimeter->set_state_silence(true);
    ok = multimeter->find_device();
    multimeter->set_state_silence(false);
    if(!ok)
    {
        emit error(QString("Мультиметр %1 НЕ найден").arg(MULTIMETER_NAME));
        ui->btn_find_devices->setEnabled(true);
        ui->sb_address->setEnabled(true);
        return;
    }

    emit info(tr("Поиск устройств закончен"));
    block_interface(false);
    ui->btn_find_devices->setEnabled(true);
    ui->sb_address->setEnabled(true);
}
//--------------------------------------------------------------------------------
bool B590::process_calibration_ADC_U(void)
{
    flag_stop_calibration_ADC_U = false;

    ui->pb_calibration_ADC_U->setRange(0, 3);
    ui->pb_calibration_ADC_U->setValue(0);

    if(wait_temp_30() == false)                     return false;

    if(Calibration_ADC_U_begin() == false)          return false;

    if(Calibration_ADC_U_find_point_0() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_U->setValue(0);

    if(Calibration_ADC_U_find_point_1() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_U->setValue(1);

    if(Calibration_ADC_U_find_point_2() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_U->setValue(2);

    if(Calibration_ADC_U_find_point_3() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_U->setValue(3);

    bool ok = powersupply->set_UI_parrot(0, 0);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590::process_calibration_DAC_U(void)
{
    flag_stop_calibration_DAC_U = false;

    if(Calibration_remote_control_on() == false)    return false;
    if(Calibration_DAC_U_begin() == false)          return false;
    if(Calibration_DAC_U_show_process() == false)   return false;

    bool ok = powersupply->set_UI_parrot(0, 0);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }

    if(Calibration_remote_control_off() == false)   return false;
    return true;
}
//--------------------------------------------------------------------------------
void B590::calibration_ADC_U(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Калибровка напряжения"),
                                    tr("Не забудьте подключить нагрузку 100-300 ом, согласно документации.\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Калибровка напряжения ..."));

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    ui->label_calibration_ADC_U->setPixmap(QPixmap(PICTURE_FAIL));

    QTime timer;
    timer.start();

    block_interface(true);
    ui->btn_stop_calibration_ADC_U->setEnabled(true);
    ui->pb_calibration_ADC_U->setEnabled(true);

    if(Calibration_remote_control_on() == false)
    {
        powersupply->print_last_error();
        return;
    }

    bool ok = process_calibration_ADC_U();
    block_interface(false);

    powersupply->set_UI_parrot(0, 0);

    if(ok)
    {
        ui->label_calibration_ADC_U->setPixmap(QPixmap(PICTURE_OK));
        emit info(QString("Калибровка завершена успешно. Прошло %1 сек.").arg((int)(timer.elapsed() / 1000.0f + 0.5f)));
    }
    else
    {
        ui->label_calibration_ADC_U->setPixmap(QPixmap(PICTURE_FAIL));
        emit error("Калибровка завершена с ошибкой");
    }

    if(Calibration_remote_control_off() == false)
    {
        powersupply->print_last_error();
        return;
    }
}
//--------------------------------------------------------------------------------
void B590::calibration_DAC_U(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Калибровка напряжения"),
                                    tr("Не забудьте подключить нагрузку 100-300 ом, согласно документации.\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Калибровка напряжения ..."));

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    ui->label_calibration_DAC_U->setPixmap(QPixmap(PICTURE_FAIL));

    QTime timer;
    timer.start();

    block_interface(true);
    ui->btn_stop_calibration_DAC_U->setEnabled(true);
    ui->pb_calibration_DAC_U->setEnabled(true);
    bool ok = process_calibration_DAC_U();
    block_interface(false);

    powersupply->set_UI_parrot(0, 0);

    if(ok)
    {
        ui->label_calibration_DAC_U->setPixmap(QPixmap(PICTURE_OK));
        emit info(QString("Калибровка завершена успешно. Прошло %1 сек.").arg((int)(timer.elapsed() / 1000.0f + 0.5f)));
    }
    else
    {
        ui->label_calibration_DAC_U->setPixmap(QPixmap(PICTURE_FAIL));
        emit error("Калибровка завершена с ошибкой");
    }
}
//--------------------------------------------------------------------------------
bool B590::wait_hot_temp(unsigned char begin_temp)
{
    unsigned char old_temp = 0;
    unsigned char temp = 0;
    bool flag_zummer = false;
    emit info(QString(tr("Ожидаем температуру %1 C")).arg(begin_temp + DELTA_TEMP_CALIBRATION_CURRENT));
    while(temp < (begin_temp + 15))
    {
        bool ok = powersupply->get_temperature(&temp, &flag_zummer);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }
        if(temp > old_temp)
        {
            old_temp = temp;
            emit info(QString(tr("Текущая температура %1 C")).arg(temp));
        }
    }
    emit info(tr("Температура достигнута"));
    return true;
}
//--------------------------------------------------------------------------------
bool B590::wait_temp_30(void)
{
    unsigned char old_temp = 100;
    unsigned char temp = 0;
    bool flag_zummer = false;

    bool ok = powersupply->get_temperature(&temp, &flag_zummer);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    if(temp <= 30)
    {
        return true;
    }

    powersupply->set_UI_parrot(0, 0);
    powersupply->set_vent_speed(1590);
    //powersupply->run_dialog_0x60();

    flag_stop_calibration_ADC_I = false;
    flag_stop_calibration_DAC_I = false;
    emit info(tr("Ожидаем температуру 30 C"));
    while(temp > 30)
    {
        if(flag_stop_calibration_ADC_I || flag_stop_calibration_DAC_I)
        {
            emit error(tr("Проверка прервана пользователем"));
            return false;
        }

        bool ok = powersupply->get_temperature(&temp, &flag_zummer);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }
        if(temp < old_temp)
        {
            old_temp = temp;
            emit info(QString(tr("Текущая температура %1 C")).arg(temp));
        }
        if(flag_zummer)
        {
            emit info(tr("Температура достигнута"));
            return true;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
bool B590::process_calibration_ADC_I(void)
{
    flag_stop_calibration_ADC_I = false;

    ui->pb_calibration_ADC_I->setRange(0, 12);
    ui->pb_calibration_ADC_I->setValue(0);

    if(wait_temp_30() == false)                     return false;

    if(Calibration_ADC_I_begin() == false)          return false;

    if(Calibration_ADC_I_find_zero_point() == false) return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(0);

    if(Calibration_ADC_I_find_point_1() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(1);

    if(Calibration_ADC_I_find_point_2() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(2);

    if(Calibration_ADC_I_find_point_3() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(3);

    if(Calibration_ADC_I_find_point_4() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(4);

    if(Calibration_ADC_I_find_point_5() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(5);

    if(Calibration_ADC_I_find_point_6() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(6);

    if(Calibration_ADC_I_find_point_7() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(7);

    if(Calibration_ADC_I_find_point_8() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(8);

    if(Calibration_ADC_I_find_point_9() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(9);

    if(Calibration_ADC_I_find_point_10() == false)  return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(10);

    //---
    if(Calibration_ADC_I_find_point_11() == false)  return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(11);

#ifndef FAKE
    bool flag_zummer = false;
    unsigned char old_temp = 0;
    unsigned char temp = 0;
    bool ok = false;
    emit info(tr("Ожидаем нужную температуру..."));
    while(!flag_zummer)
    {
        ok = powersupply->get_temperature(&temp, &flag_zummer);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }
        if(temp > old_temp)
        {
            old_temp = temp;
            emit info(QString(tr("Текущая температура %1 C")).arg(temp));
        }
    }
#endif
    if(Calibration_press_enter() == false)          return false;

    if(Calibration_ADC_I_find_point_11_HOT() == false)  return false;
    if(Calibration_press_enter() == false)          return false;

    if(Calibration_ADC_I_find_point_12() == false)  return false;
    if(Calibration_press_enter() == false)          return false;
    ui->pb_calibration_ADC_I->setValue(12);

    return true;
}
//--------------------------------------------------------------------------------
bool B590::process_calibration_DAC_I(void)
{
    flag_stop_calibration_DAC_I = false;

    if(Calibration_remote_control_on() == false)    return false;

    if(wait_temp_30() == false)                     return false;

    if(Calibration_DAC_I_begin() == false)          return false;
    if(Calibration_DAC_I_show_process() == false)   return false;

    if(Calibration_remote_control_off() == false)   return false;
    return true;
}
//--------------------------------------------------------------------------------
void B590::calibration_ADC_I(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Калибровка тока"),
                                    tr("Подключите шунт, согласно документации. Не забудьте корректно указать его номинал в поле 'шунт'\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Калибровка тока ..."));

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    ui->label_calibration_ADC_I->setPixmap(QPixmap(PICTURE_FAIL));

    QTime timer;
    timer.start();

    block_interface(true);
    ui->btn_stop_calibration_ADC_I->setEnabled(true);
    ui->pb_calibration_ADC_I->setEnabled(true);

    if(Calibration_remote_control_on() == false)
    {
        powersupply->print_last_error();
        return;
    }

    bool ok = process_calibration_ADC_I();
    block_interface(false);
    powersupply->set_UI_parrot(0, 0);

    if(ok)
    {
        ui->label_calibration_ADC_I->setPixmap(QPixmap(PICTURE_OK));
        emit info(QString("Калибровка завершена успешно. Прошло %1 сек.").arg((int)(timer.elapsed() / 1000.0f + 0.5f)));
    }
    else
    {
        ui->label_calibration_ADC_I->setPixmap(QPixmap(PICTURE_FAIL));
        emit error("Калибровка завершена с ошибкой");
    }

    if(Calibration_remote_control_off() == false)
    {
        powersupply->print_last_error();
    }
}
//--------------------------------------------------------------------------------
void B590::calibration_DAC_I(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Калибровка тока"),
                                    tr("Подключите шунт, согласно документации. Не забудьте корректно указать его номинал в поле 'шунт'\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Калибровка тока ..."));

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    ui->label_calibration_DAC_I->setPixmap(QPixmap(PICTURE_FAIL));

    QTime timer;
    timer.start();

    block_interface(true);
    ui->btn_stop_calibration_DAC_I->setEnabled(true);
    ui->pb_calibration_DAC_I->setEnabled(true);
    bool ok = process_calibration_DAC_I();
    block_interface(false);
    powersupply->set_UI_parrot(0, 0);

    if(ok)
    {
        ui->label_calibration_DAC_I->setPixmap(QPixmap(PICTURE_OK));
        emit info(QString("Калибровка завершена успешно. Прошло %1 сек.").arg((int)(timer.elapsed() / 1000.0f + 0.5f)));
    }
    else
    {
        ui->label_calibration_DAC_I->setPixmap(QPixmap(PICTURE_FAIL));
        emit error("Калибровка завершена с ошибкой");
    }
}
//--------------------------------------------------------------------------------
void B590::check_control_point_U(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Проверка контрольных точек напряжения"),
                                    tr("Не забудьте подключить нагрузку 100-300 ом.\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    ui->label_check_U->setPixmap(QPixmap(PICTURE_FAIL));

    block_interface(true);
    ui->btn_stop_check_U->setEnabled(true);
    bool ok = Calibration_DAC_U_check_points();
    if(ok)
    {
        ui->label_check_U->setPixmap(QPixmap(PICTURE_OK));
        emit info(tr("OK"));
    }
    else
    {
        ui->label_check_U->setPixmap(QPixmap(PICTURE_FAIL));
        emit error("Проверка завершена с ошибкой");
    }

    ok = powersupply->set_UI_parrot(0, 0);
    if(!ok)
    {
        powersupply->print_last_error();
    }

    block_interface(false);
}
//--------------------------------------------------------------------------------
void B590::check_control_point_I(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Проверка контрольных точек тока"),
                                    tr("Подключите шунт, согласно документации. Не забудьте корректно указать его номинал в поле 'шунт'\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Проверка контрольных точек тока ..."));

    ui->label_check_I->setPixmap(QPixmap(PICTURE_FAIL));

    block_interface(true);
    ui->btn_stop_check_I->setEnabled(true);
    bool ok = Calibration_DAC_I_check_points();
    if(ok)
    {
        ui->label_check_I->setPixmap(QPixmap(PICTURE_OK));
        emit info(tr("OK"));
    }
    else
    {
        ui->label_check_I->setPixmap(QPixmap(PICTURE_FAIL));
        emit error("Проверка завершена с ошибкой");
    }

    ok = powersupply->set_UI_parrot(0, 0);
    if(!ok)
    {
        powersupply->print_last_error();
    }
    block_interface(false);
}
//--------------------------------------------------------------------------------
void B590::check_user_point(void)
{
    emit error(tr("Пока не сделано!"));
}
//--------------------------------------------------------------------------------
void B590::save_data(void)
{
#ifndef USER_CALIBRATIONS
    bool ok = false;
    emit info("Записываем дату и серийный номер");
    ok = powersupply->set_date_and_serno(ui->sb_year->value(),
                                         ui->sb_month->value(),
                                         ui->sb_serno->value());
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    emit info("Сохраняем заводскую калибровку");
    ok = powersupply->copy_user_calibration_point_to_factory();
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    emit info("OK");
#endif
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void B590::generate_fake_data(void)
{
    //TODO
    int serno = ui->sb_serno->value();
    int year  = ui->sb_year->value();

    MyReport *report = new MyReport(this);
    QString database_name = QString(REPORT_NAME)
            .arg(serno)
            .arg(year);
    bool ok = report->open_database("QSQLITE", database_name);
    if(!ok) return;

    double result_delta = 0;
    report->clean_database_report();

    //---
    result_delta = 999.0f;
    ok = report->update_data("data_report_UDC", CONTROL_1V_TEXT,    0, QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_1V_TEXT));

    result_delta = 14999.0f;
    ok = report->update_data("data_report_UDC", CONTROL_15V_TEXT,   0, QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_15V_TEXT));

    result_delta = 29999.0f;
    ok = report->update_data("data_report_UDC", CONTROL_30V_TEXT,   0, QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_30V_TEXT));

    result_delta = 59999.0f;
    ok = report->update_data("data_report_UDC", CONTROL_60V_TEXT,   0, QString(tr("%1 мВ")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_60V_TEXT));

    //---
    result_delta = 99.0f;
    ok = report->update_data("data_report_IDC", CONTROL_01A_TEXT,   0, QString(tr("%1 мA")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_01A_TEXT));

    result_delta = 4999.0f;
    ok = report->update_data("data_report_IDC", CONTROL_5A_TEXT,    0, QString(tr("%1 мA")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_5A_TEXT));

    result_delta = 9999.0f;
    ok = report->update_data("data_report_IDC", CONTROL_10A_TEXT,   0, QString(tr("%1 мA")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_10A_TEXT));

    result_delta = 29999.0f;
    ok = report->update_data("data_report_IDC", CONTROL_30A_TEXT,   0, QString(tr("%1 мA")).arg(result_delta, 0, 'f', 2));
    if(!ok) emit error(QString("generate_fake_data: %1").arg(CONTROL_30A_TEXT));

    report->close_database(database_name);
}
#endif
//--------------------------------------------------------------------------------
void B590::check(void)
{
    emit error("Без электронной нагрузки делать не имеет смысла!");
}
//--------------------------------------------------------------------------------
void B590::save_report(void)
{
#ifndef USER_CALIBRATIONS
#if 1
    emit error("Без электронной нагрузки делать не имеет смысла!");
    return;
#endif

    if(model_U->rowCount() == 0)
    {
        emit error("No data U");
        return;
    }
    if(model_I->rowCount() == 0)
    {
        emit error("No data I");
        return;
    }

#ifdef FAKE
    generate_fake_data();
#endif

    int serno = ui->sb_serno->value();
    int year  = ui->sb_year->value();

    MyReport *report = new MyReport(this);
    QString database_name = QString(REPORT_NAME)
            .arg(serno)
            .arg(year);
    bool ok = report->open_database("QSQLITE", database_name);
    if(!ok) return;

    //report->clean_database_report();

    report->view(serno, year);
    report->close_database(database_name);
    report->deleteLater();

    emit info("OK");
#endif
}
//--------------------------------------------------------------------------------
void B590::block_interface(bool state)
{
    ui->btn_calibration_ADC_U->setDisabled(state);
    ui->btn_calibration_DAC_U->setDisabled(state);
    ui->btn_calibration_ADC_I->setDisabled(state);
    ui->btn_calibration_DAC_I->setDisabled(state);
    ui->btn_check_U->setDisabled(state);
    ui->btn_check_I->setDisabled(state);

    ui->btn_stop_calibration_ADC_U->setDisabled(state);
    ui->btn_stop_calibration_DAC_U->setDisabled(state);
    ui->btn_stop_calibration_ADC_I->setDisabled(state);
    ui->btn_stop_calibration_DAC_I->setDisabled(state);
    ui->btn_stop_check_U->setDisabled(state);
    ui->btn_stop_check_I->setDisabled(state);

    ui->label_calibration_ADC_U->setDisabled(state);
    ui->label_calibration_DAC_U->setDisabled(state);
    ui->label_calibration_ADC_I->setDisabled(state);
    ui->label_calibration_DAC_I->setDisabled(state);
    ui->label_check_U->setDisabled(state);
    ui->label_check_I->setDisabled(state);

    ui->pb_calibration_ADC_U->setDisabled(state);
    ui->pb_calibration_DAC_U->setDisabled(state);
    ui->pb_calibration_ADC_I->setDisabled(state);
    ui->pb_calibration_DAC_I->setDisabled(state);
    ui->pb_check_U->setDisabled(state);
    ui->pb_check_I->setDisabled(state);

    ui->sb_shunt->setDisabled(state);

    ui->btn_save_table_U->setDisabled(state);
    ui->btn_save_table_I->setDisabled(state);

    ui->dsb_pz_U->setDisabled(state);
    ui->dsb_pz_I->setDisabled(state);

#ifndef USER_CALIBRATIONS
    ui->btn_save_data->setDisabled(state);
    ui->btn_report->setDisabled(state);
    ui->btn_check->setDisabled(state);

    ui->sb_month->setDisabled(state);
    ui->sb_year->setDisabled(state);
    ui->sb_serno->setDisabled(state);

    //---
    item_U_point_0->setEnabled(!state);
    item_U_max_deviation_point_0->setEnabled(!state);
    item_U_begin_DAC_point_0->setEnabled(!state);

    item_U_point_1->setEnabled(!state);
    item_U_max_deviation_point_1->setEnabled(!state);
    item_U_begin_DAC_point_1->setEnabled(!state);

    item_U_point_2->setEnabled(!state);
    item_U_max_deviation_point_2->setEnabled(!state);
    item_U_begin_DAC_point_2->setEnabled(!state);

    item_U_point_3->setEnabled(!state);
    item_U_max_deviation_point_3->setEnabled(!state);
    item_U_begin_DAC_point_3->setEnabled(!state);

    //---
    item_I_point_0->setEnabled(!state);
    item_I_max_deviation_point_0->setEnabled(!state);
    item_I_begin_DAC_point_0->setEnabled(!state);
    item_I_end_DAC_point_0->setEnabled(!state);

    item_I_point_1->setEnabled(!state);
    item_I_max_deviation_point_1->setEnabled(!state);
    item_I_begin_DAC_point_1->setEnabled(!state);

    item_I_point_2->setEnabled(!state);
    item_I_max_deviation_point_2->setEnabled(!state);
    item_I_begin_DAC_point_2->setEnabled(!state);

    item_I_point_3->setEnabled(!state);
    item_I_max_deviation_point_3->setEnabled(!state);
    item_I_begin_DAC_point_3->setEnabled(!state);

    item_I_point_4->setEnabled(!state);
    item_I_max_deviation_point_4->setEnabled(!state);
    item_I_begin_DAC_point_4->setEnabled(!state);

    item_I_point_5->setEnabled(!state);
    item_I_max_deviation_point_5->setEnabled(!state);
    item_I_begin_DAC_point_5->setEnabled(!state);

    item_I_point_6->setEnabled(!state);
    item_I_max_deviation_point_6->setEnabled(!state);
    item_I_begin_DAC_point_6->setEnabled(!state);

    item_I_point_7->setEnabled(!state);
    item_I_max_deviation_point_7->setEnabled(!state);
    item_I_begin_DAC_point_7->setEnabled(!state);

    item_I_point_8->setEnabled(!state);
    item_I_max_deviation_point_8->setEnabled(!state);
    item_I_begin_DAC_point_8->setEnabled(!state);

    item_I_point_9->setEnabled(!state);
    item_I_max_deviation_point_9->setEnabled(!state);
    item_I_begin_DAC_point_9->setEnabled(!state);

    item_I_point_10->setEnabled(!state);
    item_I_max_deviation_point_10->setEnabled(!state);
    item_I_begin_DAC_point_10->setEnabled(!state);

    item_I_point_11->setEnabled(!state);
    item_I_max_deviation_point_11->setEnabled(!state);
    item_I_begin_DAC_point_11->setEnabled(!state);

    item_I_point_12->setEnabled(!state);
    item_I_max_deviation_point_12->setEnabled(!state);
    item_I_begin_DAC_point_12->setEnabled(!state);

    btn_set_max_U->setDisabled(state);
    btn_set_max_I->setDisabled(state);
    btn_clear_UI->setDisabled(state);
#endif
}
//--------------------------------------------------------------------------------
void B590::createParrotBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    ParrotBar *parrotBar = new ParrotBar(this);
    connect(parrotBar, SIGNAL(set_UI(int,int)), this, SLOT(set_UI_parrot(int,int)));

    mw->addToolBar(Qt::TopToolBarArea, parrotBar);
}
//--------------------------------------------------------------------------------
void B590::set_UI_parrot(int U, int I)
{
    powersupply->set_address(ui->sb_address->value());
    //powersupply->set_state_silence(true);
    bool ok = powersupply->set_UI_parrot(U, I);
    //powersupply->set_state_silence(false);
    if(!ok) powersupply->print_last_error();
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

    QToolBar *testbar = new QToolBar("testbar B5-90");
    testbar->setObjectName("toolbar_B590");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test B5-90",
                                       "test B5-90");
    QToolButton *btn_rc_on = add_button(testbar,
                                        new QToolButton(this),
                                        QIcon(":/pictures/button_ok.png"),
                                        "ON",
                                        "ON");
    QToolButton *btn_rc_off = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(":/pictures/button_cancel.png"),
                                         "OFF",
                                         "OFF");

    //---
    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");

    cb_test->addItem(CMD_FIND_U_POINT_0_TEXT,  QVariant(Qt::UserRole + CMD_FIND_U_POINT_0));
    cb_test->addItem(CMD_FIND_U_POINT_1_TEXT,  QVariant(Qt::UserRole + CMD_FIND_U_POINT_1));
    cb_test->addItem(CMD_FIND_U_POINT_2_TEXT,  QVariant(Qt::UserRole + CMD_FIND_U_POINT_2));
    cb_test->addItem(CMD_FIND_U_POINT_3_TEXT,  QVariant(Qt::UserRole + CMD_FIND_U_POINT_3));

    cb_test->addItem(CMD_FIND_I_POINT_0_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_0));
    cb_test->addItem(CMD_FIND_I_POINT_1_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_1));
    cb_test->addItem(CMD_FIND_I_POINT_2_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_2));
    cb_test->addItem(CMD_FIND_I_POINT_3_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_3));
    cb_test->addItem(CMD_FIND_I_POINT_4_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_4));
    cb_test->addItem(CMD_FIND_I_POINT_5_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_5));
    cb_test->addItem(CMD_FIND_I_POINT_6_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_6));
    cb_test->addItem(CMD_FIND_I_POINT_7_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_7));
    cb_test->addItem(CMD_FIND_I_POINT_8_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_8));
    cb_test->addItem(CMD_FIND_I_POINT_9_TEXT,  QVariant(Qt::UserRole + CMD_FIND_I_POINT_9));
    cb_test->addItem(CMD_FIND_I_POINT_10_TEXT, QVariant(Qt::UserRole + CMD_FIND_I_POINT_10));
    cb_test->addItem(CMD_FIND_I_POINT_11_TEXT, QVariant(Qt::UserRole + CMD_FIND_I_POINT_11));
    cb_test->addItem(CMD_FIND_I_POINT_12_TEXT, QVariant(Qt::UserRole + CMD_FIND_I_POINT_12));

    cb_test->addItem(CMD_TEST_TEXT, QVariant(Qt::UserRole + CMD_TEST));

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
    //---

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_rc_on,  SIGNAL(pressed()), powersupply, SLOT(rc_on()));
    connect(btn_rc_off, SIGNAL(pressed()), powersupply, SLOT(rc_off()));
}
//--------------------------------------------------------------------------------
void B590::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    switch (cmd)
    {
    case CMD_FIND_U_POINT_0:    Calibration_ADC_U_find_point_0();   break;
    case CMD_FIND_U_POINT_1:    Calibration_ADC_U_find_point_1();   break;
    case CMD_FIND_U_POINT_2:    Calibration_ADC_U_find_point_2();   break;
    case CMD_FIND_U_POINT_3:    Calibration_ADC_U_find_point_3();   break;

#if 1
    case CMD_FIND_I_POINT_0:    Calibration_ADC_I_find_zero_point(); break;
#else
    case CMD_FIND_I_POINT_0:    Calibration_ADC_I_find_point_0();   break;
#endif
    case CMD_FIND_I_POINT_1:    Calibration_ADC_I_find_point_1();    break;
    case CMD_FIND_I_POINT_2:    Calibration_ADC_I_find_point_2();    break;
    case CMD_FIND_I_POINT_3:    Calibration_ADC_I_find_point_3();    break;
    case CMD_FIND_I_POINT_4:    Calibration_ADC_I_find_point_4();    break;
    case CMD_FIND_I_POINT_5:    Calibration_ADC_I_find_point_5();    break;
    case CMD_FIND_I_POINT_6:    Calibration_ADC_I_find_point_6();    break;
    case CMD_FIND_I_POINT_7:    Calibration_ADC_I_find_point_7();    break;
    case CMD_FIND_I_POINT_8:    Calibration_ADC_I_find_point_8();    break;
    case CMD_FIND_I_POINT_9:    Calibration_ADC_I_find_point_9();    break;
    case CMD_FIND_I_POINT_10:   Calibration_ADC_I_find_point_10();   break;
    case CMD_FIND_I_POINT_11:   Calibration_ADC_I_find_point_11();   break;
    case CMD_FIND_I_POINT_12:   Calibration_ADC_I_find_point_12();   break;

    case CMD_TEST:              test();     break;
    default: break;
    }
}
//--------------------------------------------------------------------------------
void B590::createInfoBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *testbar = new QToolBar("testbar B5-90");
    testbar->setObjectName("toolbar_B590");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_info = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "info B5-90",
                                       "info B5-90");

    connect(btn_info, SIGNAL(clicked()), this, SLOT(show_tables()));
}
//--------------------------------------------------------------------------------
void B590::show_tables(void)
{
    bool ok  = false;
    QTextEdit *te = new QTextEdit();
    te->setMinimumSize(640, 480);

    ok = powersupply->get_info(te);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    QRect available_geom = QDesktopWidget().availableGeometry();
    int app_left = available_geom.width() / 2 - te->width() / 2;
    int app_top = available_geom.height() / 2 - te->height() / 2;

    te->move(app_left, app_top);
    te->show();

    emit info("OK!");
}
//--------------------------------------------------------------------------------
void B590::test(void)
{
    double I_mA = 50;
    double R = ui->sb_shunt->value();
    emit info(QString("I_mA %1").arg(I_mA));
    emit info(QString("R %1").arg(R));
    emit info(QString("U_uV %1").arg(convert_uA_to_uV(I_mA * 1000.0f, R)));
    return;

#ifndef USER_CALIBRATIONS
    int serno = ui->sb_serno->value();
    int year  = ui->sb_year->value();

    MyReport *report = new MyReport(this);
    QString database_name = QString(REPORT_NAME)
            .arg(serno)
            .arg(year);
    bool ok = report->open_database("QSQLITE", database_name);
    if(!ok)
    {
        emit error(QString("Open database is failed! Error: %1").arg(report->get_lastError()));
        return;
    }

    report->clean_database_report();

    report->view(serno, year);
    report->close_database(database_name);
    report->deleteLater();

    emit info("OK");
#endif
}
//--------------------------------------------------------------------------------
void B590::clear(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Стирание FLASH"),
                                    tr("Вы уверены, что хотите стереть FLASH?"),
                                    QMessageBox::No | QMessageBox::Yes);
    if(btn != QMessageBox::Yes) return;

    QByteArray ba;
    ba.clear();

    powersupply->set_state_silence(true);
    QByteArray data;
    data.clear();
    for(int n=0; n<100; n++)
    {
        data.append((char)0);
    }
    for(uint16_t address = 0; address < 8192; address+=100)
    {

        int len = 8192 - address;
        if(len > 100) len=100;
        bool ok = powersupply->fram_write(address, len, data);
        if(!ok)
        {
            powersupply->print_last_error();
            powersupply->set_state_silence(false);
            return;
        }
    }
    powersupply->set_state_silence(false);
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::new_calibration_U(void)
{
    QTime t;

    t.start();
    for(int U=1; U<MAX_CALIBRATION_POINTS_B590_I; U++)
    {
        double max_delta_mV = (U * 0.001f + 0.005f) * 1e6 * ui->dsb_pz_U->value();

        DATA_U data;
        data.value_mV = U * 1000;
        data.begin_DAC_U = U * 1000;
        data.end_DAC_U = 0xFFFF;
        data.max_delta_mV = max_delta_mV;
        bool ok = find_U(&data);
        if(!ok)
        {
            emit error(tr("Не найдено!"));
            powersupply->set_UI(0, 0);
            powersupply->rc_off();
            return;
        }
        emit info(QString("Найдено за %1 попыток. Значение %2 mV Допуск %3 mV DAC %4")
                  .arg(data.counter)
                  .arg(data.result_value_mV)
                  .arg(data.max_delta_mV)
                  .arg(data.result_dac_value));
        emit info(QString("Запись точки %1 значение DAC %2").arg(U).arg(data.result_dac_value));
        ok = powersupply->set_calibration_point_U(U, data.result_dac_value);
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
    }
    emit info(QString("time elapsed %1 sec").arg(t.elapsed() / 1000.0f));
}
//--------------------------------------------------------------------------------
void B590::find_1V(void)
{
    int U = 1;

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    //---
    double max_delta_mV = (U * 0.001f + 0.005f) * 1e6 * ui->dsb_pz_U->value();

    powersupply->rc_on();
    emit info(QString(tr("Поиск %1 V")).arg(U));

    DATA_U data;
    data.value_mV = U * 1000;
    data.begin_DAC_U = 900;
    data.end_DAC_U   = 0xFFFF;
    data.max_delta_mV = max_delta_mV;
    bool ok = find_U(&data);
    if(!ok)
    {
        emit error(tr("Не найдено!"));
    }
    else
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mV").arg(data.result_value_mV));
        emit info(QString("Допуск %1 mV").arg(data.max_delta_mV));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    powersupply->rc_off();
    //---

    powersupply->set_UI_parrot(0, 0);

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void B590::find_59V(void)
{
    int U = 59;

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    //---
    double max_delta_mV = (U * 0.001f + 0.005f) * 1e6 * ui->dsb_pz_U->value();

    powersupply->rc_on();
    emit info(QString(tr("Поиск %1 V")).arg(U));

    DATA_U data;
    data.value_mV = U * 1000;
    data.begin_DAC_U = U * 1000;
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = max_delta_mV;
    bool ok = find_U(&data);
    if(!ok)
    {
        emit error(tr("Не найдено!"));
    }
    else
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mV").arg(data.result_value_mV));
        emit info(QString("Допуск %1 mV").arg(data.max_delta_mV));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    powersupply->rc_off();
    //---

    powersupply->set_UI_parrot(0, 0);

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void B590::find_50mA(void)
{
    float    I = 0.05; //значение в A

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    //---
    powersupply->rc_on();
    emit info(QString(tr("Поиск %1 A")).arg(I));
    DATA_I data;
#ifndef USER_CALIBRATIONS
    data.value_mA = item_I_point_1->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_1->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_1->value().toInt();
    bool ok = find_I(&data);
#else
    data.value_mA = B590_I_POINT_1;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_1;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_1;
    bool ok = find_I(&data);
#endif
    if(!ok)
    {
        emit error(tr("Не найдено!"));
    }
    else
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
        emit info(QString("Допуск %1 mA").arg(data.max_delta_mA));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    powersupply->rc_off();
    //---

    powersupply->set_UI_parrot(0, 0);

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void B590::find_5A(void)
{
    float I = 5; //значение в A

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    //---
    powersupply->rc_on();
    emit info(QString(tr("Поиск %1 A")).arg(I));
    DATA_I data;
#ifndef USER_CALIBRATIONS
    data.value_mA = item_I_point_2->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_2->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_2->value().toInt();
    bool ok = find_I(&data);
#else
    data.value_mA = B590_I_POINT_2;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_2;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_2;
    bool ok = find_I(&data);
#endif
    if(!ok)
    {
        emit error(tr("Не найдено!"));
    }
    else
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
        emit info(QString("Допуск %1 mA").arg(data.max_delta_mA));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    powersupply->rc_off();
    //---

    powersupply->set_UI_parrot(0, 0);

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void B590::find_25A(void)
{
    float I = 25; //значение в A

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    //---
    powersupply->rc_on();
    emit info(QString(tr("Поиск %1 A")).arg(I));
#ifndef USER_CALIBRATIONS
    DATA_I data;
    data.value_mA = item_I_point_3->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_3->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_3->value().toInt();
    bool ok = find_I(&data);
#else
    DATA_I data;
    data.value_mA = B590_I_POINT_3;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_3;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_3;
    bool ok = find_I(&data);
#endif
    if(!ok)
    {
        emit error(tr("Не найдено!"));
    }
    else
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
        emit info(QString("Допуск %1 mA").arg(data.max_delta_mA));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    powersupply->rc_off();
    //---

    powersupply->set_UI_parrot(0, 0);

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void B590::find_48A(void)
{
    float I = 48; //значение в A

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    //---
    powersupply->rc_on();
    emit info(QString(tr("Поиск %1 A")).arg(I));
#ifndef USER_CALIBRATIONS
    DATA_I data;
    data.value_mA = item_I_point_4->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_4->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_4->value().toInt();
    bool ok = find_I(&data);
#else
    DATA_I data;
    data.value_mA = B590_I_POINT_4;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_4;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_4;
    bool ok = find_I(&data);
#endif
    if(!ok)
    {
        emit error(tr("Не найдено!"));
    }
    else
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
        emit info(QString("Допуск %1 mA").arg(data.max_delta_mA));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    powersupply->rc_off();
    //---

    powersupply->set_UI_parrot(0, 0);

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
bool B590::measuring_U(int value_mV,
                       double max_delta_mV,
                       int *measuring_value_mV)
{
    int value_uV = 0;
    bool ok = false;

    ok = multimeter->get_value(&value_uV);
    if(!ok)
    {
        multimeter->print_last_error();
        return false;
    }

    double delta_mV = value_mV - (value_uV / 1000.0f);
#if 0
    emit trace(QString("measuring_U: value_mV %1").arg(value_mV));
    emit trace(QString("measuring_U: delta_mV %1").arg(delta_mV));
    emit trace(QString("measuring_U: max_delta_mV %1").arg(max_delta_mV));
#endif
    if(delta_mV <= max_delta_mV)
    {
        *measuring_value_mV = value_mV;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool B590::measuring_I(int value_mA,
                       double R_om,
                       double max_delta_mA,
                       int *measuring_value_mA)
{
    int value_uV = 0;
    bool ok = false;

    ok = multimeter->get_value(&value_uV);
    if(!ok)
    {
        multimeter->print_last_error();
        return false;
    }

    double delta_mA = value_mA - convert_uV_to_uA(value_uV, R_om) / 1000.0f;
#if 0
    emit trace(QString("measuring_I: value_mA %1").arg(value_mA));
    emit trace(QString("measuring_I: delta_mA %1").arg(delta_mA));
    emit trace(QString("measuring_I: max_delta_mA %1").arg(max_delta_mA));
#endif
    if(delta_mA <= max_delta_mA)
    {
        *measuring_value_mA = value_mA;
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool B590::find_U(DATA_U *data)
{
    int measuring_value_uV = 0;
    int measuring_value_mV = 0;

    bool found_value = false;
    uint16_t parrot_value = data->begin_DAC_U;
    int cnt_repeat = 1;
    bool ok = false;

    flag_stop_calibration_ADC_U = false;
    flag_stop_calibration_DAC_U = false;
    while(!found_value)
    {
        QCoreApplication::processEvents();

        if(flag_stop_calibration_ADC_U || flag_stop_calibration_DAC_U)
        {
            emit error(tr("Проверка прервана пользователем"));
            return false;
        }

#if 0
        emit trace(QString("find_U: parrot_value %1").arg(parrot_value));
#endif
        ok = powersupply->set_UI_parrot(parrot_value, DEFAULT_I);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }

#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

        if(measuring_U(data->value_mV,
                       data->max_delta_mV,
                       &measuring_value_mV))
        {
            found_value = true;
        }
        else
        {
            ok = multimeter->get_value(&measuring_value_uV);
            if(!ok)
            {
                multimeter->print_last_error();
                return false;
            }

            int delta_mV = (int)data->value_mV - (int)(measuring_value_uV / 1000.0f + 0.5f);
            measuring_value_mV = measuring_value_uV / 1000.0f;
#if 0
            emit trace(QString("find_U: value_mV %1").arg(data->value_mV));
            emit trace(QString("find_U: measuring_value_uV %1").arg(measuring_value_uV));
            emit trace(QString("find_U: measuring_value_mV %1").arg(measuring_value_mV));
            emit trace(QString("find_U: delta_mV %1").arg(data->value_mV - measuring_value_mV));
            emit trace(QString("find_U: max_delta_mV %1").arg(data->max_delta_mV));
#endif
            if((delta_mV * 1000.0f) > data->max_delta_mV)
            {
                parrot_value += delta_mV;
            }
        }

        if(!found_value)
        {
            cnt_repeat++;
            if(cnt_repeat > MAX_REPEAT_FIND_U)
            {
                break;
            }
        }
    }
    if(found_value)
    {
        (*data).counter = cnt_repeat;
        (*data).result_dac_value = parrot_value;
        (*data).result_value_mV = measuring_value_mV;
    }
    return found_value;
}
//--------------------------------------------------------------------------------
double B590::convert_uA_to_uV(double value_uA, double R_om)
{
    return value_uA * R_om;
}
//--------------------------------------------------------------------------------
double B590::convert_uV_to_uA(double value_uV, double R_om)
{
    return value_uV / R_om;
}
//--------------------------------------------------------------------------------
bool B590::find_I(DATA_I *data)
{
    double R = ui->sb_shunt->value();

    int measuring_value_uV = 0;
    int measuring_value_mA = 0;
    uint16_t begin_parrot_value = data->begin_DAC_I;
    uint16_t end_parrot_value   = data->end_DAC_I;
    bool found_value = false;
    int cnt_repeat = 1;
    bool ok = false;

    flag_stop_calibration_ADC_I = false;
    flag_stop_calibration_DAC_I = false;
    while(!found_value)
    {
        QCoreApplication::processEvents();

        if(flag_stop_calibration_ADC_I || flag_stop_calibration_DAC_I)
        {
            emit error(tr("Проверка прервана пользователем"));
            return false;
        }

#if 0
        emit trace(QString("find_I: begin_parrot_value %1").arg(begin_parrot_value));
#endif
        ok = powersupply->set_UI_parrot(DEFAULT_U, begin_parrot_value);
        if(!ok) powersupply->print_last_error();

#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

        if(measuring_I(data->value_mA,
                       R,
                       data->max_delta_mA,
                       &measuring_value_mA))
        {
            found_value = true;
        }
        else
        {
            ok = multimeter->get_value(&measuring_value_uV);
            if(!ok)
            {
                multimeter->print_last_error();
            }
            else
            {
                int measuring_value_uA = convert_uV_to_uA(measuring_value_uV, R);
                double delta_mA = data->value_mA - (measuring_value_uA / 1000.0f);
#if 0
                emit trace(QString("find_I: value_mA %1").arg(data->value_mA));
                emit trace(QString("find_I: delta_mA %1").arg(delta_mA));
                emit trace(QString("find_I: measuring_value_uV %1").arg(measuring_value_uV));
                emit trace(QString("find_I: measuring_value_uA %1").arg(measuring_value_uA));
#endif
                if(delta_mA > data->max_delta_mA)
                {
                    begin_parrot_value += delta_mA;
#if 0
                    emit trace(QString("find_I:correct: begin_parrot_value %1").arg(begin_parrot_value));
#endif
                    if(begin_parrot_value > end_parrot_value)
                    {
                        return false;
                    }
                }
            }
        }

        if(!found_value)
        {
            cnt_repeat++;
            if(cnt_repeat > MAX_REPEAT_FIND_I)
            {
                break;
            }
        }
    }

    if(found_value)
    {
        (*data).counter = cnt_repeat;
        (*data).result_dac_value = begin_parrot_value;
        (*data).result_value_mA = measuring_value_mA;
    }
    return found_value;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_remote_control_on(void)
{
    bool ok = powersupply->rc_on();
    if(!ok)
    {
        emit error(tr("Не удалось включить удаленное управление!"));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_U_begin(void)
{
    emit info(tr("Запуск калибровки АЦП по напряжению"));
    bool ok = powersupply->Send_Command_To_Calib_ADC_U();
    if(!ok)
    {
        powersupply->print_last_error();
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_U_find_point_0(void)
{
    bool ok = false;

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mV").arg(item_U_point_0->value().toInt())));

    DATA_U data;
    data.value_mV = item_U_point_0->value().toInt();
    data.begin_DAC_U = item_U_begin_DAC_point_0->value().toInt();
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = item_U_max_deviation_point_0->value().toInt();
    ok = find_U(&data);
#else
    emit info(QString(tr("Поиск точки %1 mV").arg(B590_U_POINT_0)));

    DATA_U data;
    data.value_mV = B590_U_POINT_0;
    data.begin_DAC_U = B590_U_BEGIN_DAC_POINT_0;
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = B590_U_MAX_DEVIATION_POINT_0;
    ok = find_U(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mV").arg(data.value_mV));
        emit info(QString("Допуск %1 mV").arg(data.max_delta_mV));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    else
    {
        emit error(QString("Точка %1 mV НЕ найдена").arg(data.value_mV));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_U_find_point_1(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mV").arg(item_U_point_1->value().toInt())));

    DATA_U data;
    data.value_mV = item_U_point_1->value().toInt();
    data.begin_DAC_U = item_U_begin_DAC_point_1->value().toInt();
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = item_U_max_deviation_point_1->value().toInt();
    ok = find_U(&data);
#else
    emit info(QString(tr("Поиск точки %1 mV").arg(B590_U_POINT_1)));

    DATA_U data;
    data.value_mV = B590_U_POINT_1;
    data.begin_DAC_U = B590_U_BEGIN_DAC_POINT_1;
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = B590_U_MAX_DEVIATION_POINT_1;
    ok = find_U(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mV").arg(data.value_mV));
        emit info(QString("Допуск %1 mV").arg(data.max_delta_mV));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    else
    {
        emit error(QString("Точка %1 mV НЕ найдена").arg(data.value_mV));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_U_find_point_2(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mV").arg(item_U_point_2->value().toInt())));

    DATA_U data;
    data.value_mV = item_U_point_2->value().toInt();
    data.begin_DAC_U = item_U_begin_DAC_point_2->value().toInt();
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = item_U_max_deviation_point_2->value().toInt();
    ok = find_U(&data);
#else
    emit info(QString(tr("Поиск точки %1 mV").arg(B590_U_POINT_2)));

    DATA_U data;
    data.value_mV = B590_U_POINT_2;
    data.begin_DAC_U = B590_U_BEGIN_DAC_POINT_2;
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = B590_U_MAX_DEVIATION_POINT_2;
    ok = find_U(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mV").arg(data.value_mV));
        emit info(QString("Допуск %1 mV").arg(data.max_delta_mV));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    else
    {
        emit error(QString("Точка %1 mV НЕ найдена").arg(data.value_mV));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_U_find_point_3(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mV").arg(item_U_point_3->value().toInt())));

    DATA_U data;
    data.value_mV = item_U_point_3->value().toInt();
    data.begin_DAC_U = item_U_begin_DAC_point_3->value().toInt();
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = item_U_max_deviation_point_3->value().toInt();
    ok = find_U(&data);
#else
    emit info(QString(tr("Поиск точки %1 mV").arg(B590_U_POINT_3)));

    DATA_U data;
    data.value_mV = B590_U_POINT_3;
    data.begin_DAC_U = B590_U_BEGIN_DAC_POINT_3;
    data.end_DAC_U = 0xFFFF;
    data.max_delta_mV = B590_U_MAX_DEVIATION_POINT_3;
    ok = find_U(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mV").arg(data.value_mV));
        emit info(QString("Допуск %1 mV").arg(data.max_delta_mV));
        emit info(QString("DAC %1").arg(data.result_dac_value));
    }
    else
    {
        emit error(QString("Точка %1 mV НЕ найдена").arg(data.value_mV));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E09_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_DAC_U_begin(void)
{
    emit info(tr("Запуск калибровки ЦАП по напряжению"));
    bool ok = powersupply->Send_Command_To_Calib_DAC_U();
    if(!ok)
    {
        powersupply->print_last_error();
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_DAC_U_show_process(void)
{
    bool ok = false;
    uint8_t point = 0;
    uint8_t state = 0;

    ui->pb_calibration_DAC_U->setDisabled(false);
    ui->pb_calibration_DAC_U->setRange(0, COUNT_B590_CALIB_POINTS_DAC_U - 1);
    ui->pb_calibration_DAC_U->setValue(0);

    emit info(tr("Калибровка ЦАП напряжения"));
    flag_stop_calibration_DAC_U = false;
    while(point < COUNT_B590_CALIB_POINTS_DAC_U)
    {
        QCoreApplication::processEvents();
        //emit info(QString("point %1").arg((int)point));

        if(flag_stop_calibration_DAC_U)
        {
            emit error(tr("Калибровка прервана пользователем"));
            return false;
        }

        ok = false;
        while(!ok)
        {
            if(flag_stop_calibration_DAC_U)
            {
                emit error(tr("Калибровка прервана пользователем"));
                return false;
            }
            ok = powersupply->get_calibration_state(&state);
#if 0
            if(!ok)
            {
                powersupply->print_last_error();
            }
#endif
#if 0
            if(state != TASK_CALIBRATION_DAC_U)
            {
                emit error(QString("TASK %1").arg(state));
                emit error(QString("point %1").arg(point));
                return false;
            }
#endif
        }

        ok = false;
        while(!ok)
        {
            if(flag_stop_calibration_DAC_U)
            {
                emit error(tr("Калибровка прервана пользователем"));
                return false;
            }
            ok = powersupply->get_current_calibration_point(&point, &state);
#if 0
            if(!ok)
            {
                int btn = QMessageBox::question(this,
                                                tr("Ошибка!"),
                                                tr("Повторить последнюю операцию?"),
                                                QMessageBox::No | QMessageBox::Yes);
                if(btn != QMessageBox::Yes)
                {
                    return false;
                }
            }
#endif
        }
        if(point == (COUNT_B590_CALIB_POINTS_DAC_U - 1))
        {
            // находимся на последней точке
            if(state == NO_TASK)
            {
                emit info("OK");
                return ok;
            }
        }
#if 0
        else
        {
            if(state != TASK_CALIBRATION_DAC_U)
            {
                emit trace(QString("point %1 state %2").arg((int)point).arg((int)state));
                emit error(tr("Калибровка прервана"));
                return false;
            }
        }
#endif
        ui->pb_calibration_DAC_U->setValue(point);
    }
    ui->pb_calibration_DAC_U->setValue(COUNT_B590_CALIB_POINTS_DAC_U - 1);
    emit info("OK");
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::check_point_U(int value_mV,
                         int *result_uV)
{
    bool ok = false;
    ok = powersupply->set_UI(value_mV, DEFAULT_I);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
#ifndef FAKE
    Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

    ok = measuring(result_uV);
    if(!ok)
    {
        multimeter->print_last_error();
        return ok;
    }

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::check_point_I(int value_mA,
                         int *result_uV)
{
    bool ok = false;
    ok = powersupply->set_UI(DEFAULT_U, value_mA);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
#ifndef FAKE
    Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

    ok = measuring(result_uV);
    if(!ok)
    {
        multimeter->print_last_error();
        return ok;
    }

    return ok;
}

//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_begin(void)
{
    emit info(tr("Запуск калибровки АЦП по току"));
    bool ok = powersupply->Send_Command_To_Calib_ADC_I();
    if(!ok)
    {
        powersupply->print_last_error();
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_zero_point(void)
{
    bool ok = false;
    int old_measuring_value_uV = 1000;
    int measuring_value_uV = 0;

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

#ifndef USER_CALIBRATIONS
    emit info(tr("Поиск нулевой точки тока"));
    for(int n=item_I_begin_DAC_point_0->value().toInt(); n<item_I_end_DAC_point_0->value().toInt(); n++)
#else
    for(int n=B590_I_BEGIN_DAC_POINT_0; n<B590_I_END_DAC_POINT_0; n++)
#endif
    {
        ok = powersupply->set_UI_parrot(DEFAULT_U, n);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }
#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif
        ok = multimeter->get_value(&measuring_value_uV);
        if(!ok)
        {
            multimeter->print_last_error();
            return false;
        }
        if(measuring_value_uV > (old_measuring_value_uV + 5))
        {
            emit info(QString("FOUND: n %1 value %2")
                      .arg(n)
                      .arg(measuring_value_uV));
            return true;
        }
        old_measuring_value_uV = measuring_value_uV;
        emit info(QString("n %1 value %2")
                  .arg(n)
                  .arg(measuring_value_uV));
    }

    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    powersupply->set_state_silence(false);
    multimeter->set_state_silence(false);
    return true;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_1(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_1->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_1->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_1->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_1->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_1));

    DATA_I data;
    data.value_mA = B590_I_POINT_1;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_1;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_1;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_2(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_2->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_2->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_2->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_2->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_2));

    DATA_I data;
    data.value_mA = B590_I_POINT_2;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_2;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_2;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_3(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_3->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_3->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_3->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_3->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_3));

    DATA_I data;
    data.value_mA = B590_I_POINT_3;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_3;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_3;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_4(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_4->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_4->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_4->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_4->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_4));

    DATA_I data;
    data.value_mA = B590_I_POINT_4;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_4;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_4;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_5(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_5->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_5->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_5->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_5->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_5));

    DATA_I data;
    data.value_mA = B590_I_POINT_5;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_5;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_5;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_6(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_6->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_6->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_6->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_6->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_6));

    DATA_I data;
    data.value_mA = B590_I_POINT_6;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_6;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_6;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_7(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_7->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_7->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_7->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_7->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_7));

    DATA_I data;
    data.value_mA = B590_I_POINT_7;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_7;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_7;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_8(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_8->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_8->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_8->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_8->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_8));

    DATA_I data;
    data.value_mA = B590_I_POINT_8;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_8;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_8;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_9(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_9->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_9->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_9->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_9->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_9));

    DATA_I data;
    data.value_mA = B590_I_POINT_9;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_9;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_9;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_10(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_10->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_10->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_10->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_10->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_10));

    DATA_I data;
    data.value_mA = B590_I_POINT_10;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_10;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_10;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_11(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_11->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_11->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_11->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_11->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_11));

    DATA_I data;
    data.value_mA = B590_I_POINT_11;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_11;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_11;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_11_HOT(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA (горячая)")).arg(item_I_point_11->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_11->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_11->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_11->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_11));

    DATA_I data;
    data.value_mA = B590_I_POINT_11;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_11;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_11;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_ADC_I_find_point_12(void)
{
    bool ok = false;
    //---
    uint32_t bits = 0;
    uint16_t code = 0;
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_12->value().toInt()));

    DATA_I data;
    data.value_mA = item_I_point_12->value().toInt();
    data.begin_DAC_I = item_I_begin_DAC_point_12->value().toInt();
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = item_I_max_deviation_point_12->value().toInt();
    ok = find_I(&data);
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B590_I_POINT_12));

    DATA_I data;
    data.value_mA = B590_I_POINT_12;
    data.begin_DAC_I = B590_I_BEGIN_DAC_POINT_12;
    data.end_DAC_I = 0xFFFF;
    data.max_delta_mA = B590_I_MAX_DEVIATION_POINT_12;
    ok = find_I(&data);
#endif
    if(ok)
    {
        emit info(QString("Найдено за %1 попыток").arg(data.counter));
        emit info(QString("Значение %1 mA").arg(data.result_value_mA));
    }
    else
    {
        emit error(QString("Точка %1 mA НЕ найдена").arg(data.value_mA));
        powersupply->set_UI_parrot(0, 0);
        return ok;
    }

    //---
    ok = powersupply->get_error(&bits, &code);
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
    if(bits & ERROR_E10_B590)
    {
        powersupply->print_adv_error(bits, code);
        return false;
    }
    //---

    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_DAC_I_begin(void)
{
    emit info(tr("Запуск калибровки ЦАП по току"));
    bool ok = powersupply->Send_Command_To_Calib_DAC_I();
    if(!ok)
    {
        powersupply->print_last_error();
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_DAC_I_show_process(void)
{
    bool ok = false;
    uint8_t point = 0;
    uint8_t state = 0;

    ui->pb_calibration_DAC_I->setDisabled(false);
    ui->pb_calibration_DAC_I->setRange(0, COUNT_B590_CALIB_POINTS_DAC_I - 1);
    ui->pb_calibration_DAC_I->setValue(0);

    emit info(tr("Калибровка ЦАП тока"));
    flag_stop_calibration_DAC_I = false;
    while(point < COUNT_B590_CALIB_POINTS_DAC_I)
    {
        QCoreApplication::processEvents();

        if(flag_stop_calibration_DAC_I)
        {
            emit error(tr("Калибровка прервана пользователем"));
            return false;
        }

        ok = false;
        while(!ok)
        {
            if(flag_stop_calibration_DAC_I)
            {
                emit error(tr("Калибровка прервана пользователем"));
                return false;
            }
            ok = powersupply->get_calibration_state(&state);
#if 0
            if(!ok)
            {
                powersupply->print_last_error();
            }
#endif
#if 0
            if(state != TASK_CALIBRATION_DAC_I)
            {
                emit error(QString("TASK %1").arg(state));
                emit error(QString("point %1").arg(point));
            }
#endif
        }

        ok = false;
        while(!ok)
        {
            if(flag_stop_calibration_DAC_I)
            {
                emit error(tr("Калибровка прервана пользователем"));
                return false;
            }
            ok = powersupply->get_current_calibration_point(&point, &state);
#if 0
            if(!ok)
            {
                int btn = QMessageBox::question(this,
                                                tr("Ошибка!"),
                                                tr("Повторить последнюю операцию?"),
                                                QMessageBox::No | QMessageBox::Yes);
                if(btn != QMessageBox::Yes)
                {
                    return false;
                }
            }
#endif
        }
        if(point == (COUNT_B590_CALIB_POINTS_DAC_I - 1))
        {
            // находимся на последней точке
            if(state == NO_TASK)
            {
                emit info("OK");
                return ok;
            }
        }
#if 0
        else
        {
            if(state != TASK_CALIBRATION_DAC_I)
            {
                emit trace(QString("point %1 state %2").arg((int)point).arg((int)state));
                emit error(tr("Калибровка прервана"));
                return false;
            }
        }
#endif
        ui->pb_calibration_DAC_I->setValue(point);
    }
    ui->pb_calibration_DAC_I->setValue(COUNT_B590_CALIB_POINTS_DAC_I - 1);
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool B590::measuring_V764(int *value_uV)
{
#ifdef V764
    for(int n=0; n<3; n++)
    {
        bool ok = multimeter->get_value(value_uV);
        if(!ok)
        {
            multimeter->print_last_error();
            return false;
        }
    }
#else
    Q_UNUSED(value_uV)
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool B590::measuring_V7642(int *value_uV)
{
#ifdef V7642
    bool ok = multimeter->get_value(value_uV);
    if(!ok)
    {
        multimeter->print_last_error();
        return false;
    }
#else
    Q_UNUSED(value_uV)
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool B590::measuring_V780(int *value_uV)
{
#ifdef V780
    int temp = { 0 };
    *value_uV = 0;
    for(int n=0; n<3; n++)
    {
        bool ok = multimeter->get_value(&temp);
        if(!ok)
        {
            multimeter->print_last_error();
            return false;
        }
        *value_uV += temp;
    }
    *value_uV /= 3;
#else
    Q_UNUSED(value_uV)
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool B590::measuring(int *value_uV)
{
    bool ok = false;
#ifdef V764
    ok = measuring_V764(value_uV);
#endif

#ifdef V7642
    ok = measuring_V7642(value_uV);
#endif

#ifdef V780
    ok = measuring_V780(value_uV);
#endif
    return ok;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool B590::Calibration_DAC_U_check_points(void)
{
    bool ok = false;
    bool check_is_true = true;

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    powersupply->rc_on();

    ui->pb_check_U->setDisabled(false);
    ui->pb_check_U->setMaximum(MAX_VOLTAGE_B590 / 1000);
    ui->pb_check_U->setValue(0);

    flag_stop_check_U = false;
    emit info(tr("Проверка контрольных точек напряжения"));
    for(int n=0; n<(MAX_VOLTAGE_B590 / 1000); n++)
    {
        model_U->removeRows(0, 1, QModelIndex());
    }
    check_is_true = true;
    int index = 1;

#if 1
    int result_uV = 0;
    int u_mV  = 200;
    float u_V = (float)u_mV / 1000.0f;
    ok = check_point_U(u_mV, &result_uV);
    if(!ok)
    {
        return false;
    }

    int delta_uV = result_uV - (u_V * 1e6);
    int max_delta_uV = (u_V * 0.001f + 0.005f) * 1e6 * ui->dsb_pz_U->value();
    //double percent_delta = 100.0f - (result_uV * 100.0f / (u_V * 1000000.0f));
    double percent_delta = (double)delta_uV / ((double)u_V * 1000000.0f) * 100.0f;    //TODO
    //if(percent_delta < 0) percent_delta = -percent_delta;

    model_U->insertRow(index-1);
    model_U->setData(model_U->index(index-1, 0, QModelIndex()), u_V);
    model_U->setData(model_U->index(index-1, 1, QModelIndex()), max_delta_uV / 1000.0f);
    model_U->setData(model_U->index(index-1, 2, QModelIndex()), delta_uV / 1000.0f);
    model_U->setData(model_U->index(index-1, 3, QModelIndex()), percent_delta);
    model_U->setData(model_U->index(index-1, 4, QModelIndex()), (abs(delta_uV) < max_delta_uV) ? "ПРОЙДЕНО" : "ОШИБКА");
    index++;

    if(abs(delta_uV) > max_delta_uV)
    {
        check_is_true = false;
    }
#endif

    for(int u_V=1; u_V<=(MAX_VOLTAGE_B590 / 1000); u_V++)
    {
        if(flag_stop_check_U)
        {
            emit error(tr("Проверка прервана пользователем"));
            return false;
        }
        QCoreApplication::processEvents();
        ui->pb_check_U->setValue(u_V);

        int result_uV = 0;
        ok = check_point_U(u_V * 1000, &result_uV);
        if(!ok)
        {
            return false;
        }

        int delta_uV = result_uV - (u_V * 1e6);
        int max_delta_uV = (u_V * 0.001f + 0.005f) * 1e6 * ui->dsb_pz_U->value();
        //double percent_delta = 100.0f - result_uV * 100.0f / (u_V * 1000000.0f);
        double percent_delta = (double)delta_uV / ((double)u_V * 1000000.0f) * 100.0f;    //TODO
        //if(percent_delta < 0) percent_delta = -percent_delta;

        model_U->insertRow(index-1);
        model_U->setData(model_U->index(index-1, 0, QModelIndex()), u_V);
        model_U->setData(model_U->index(index-1, 1, QModelIndex()), max_delta_uV / 1000.0f);
        model_U->setData(model_U->index(index-1, 2, QModelIndex()), delta_uV / 1000.0f);
        model_U->setData(model_U->index(index-1, 3, QModelIndex()), percent_delta);
        model_U->setData(model_U->index(index-1, 4, QModelIndex()), (abs(delta_uV) < max_delta_uV) ? "ПРОЙДЕНО" : "ОШИБКА");
        index++;

        if(abs(delta_uV) > max_delta_uV)
        {
            check_is_true = false;
        }
    }
    powersupply->set_UI(0, 0);
    powersupply->rc_off();

    if(Calibration_remote_control_off() == false)   return false;
    return check_is_true;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_DAC_I_check_points(void)
{
    bool ok = false;
    bool check_is_true = true;

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    powersupply->rc_on();

    ui->pb_check_I->setDisabled(false);
    ui->pb_check_I->setMaximum(MAX_CURRENT_B590 / 1000);
    ui->pb_check_I->setValue(0);

    flag_stop_check_I = false;
    emit info(tr("Проверка контрольных точек тока"));
    for(int n=0; n<(MAX_CURRENT_B590 / 1000); n++)
    {
        model_I->removeRows(0, 1, QModelIndex());
    }
    int index = 1;

#if 1
    int   i_mA = 10;
    float i_A  = (float)i_mA / 1000.0f;
    int result_uV = 0;
    ok = check_point_I(i_mA, &result_uV);
    if(!ok)
    {
        return false;
    }

    double r = ui->sb_shunt->value();
    int delta_uV = (i_A * r * 1e6) - result_uV;
    int max_delta_uV = (i_A * 0.005f + 0.005f) * r * 1e6 * ui->dsb_pz_I->value();
    //double percent_delta = 100.0f - (result_uV * 100.0f / (i_A * r * 1e6));
    double percent_delta = (double)delta_uV / (i_A * r * 1e6) * 100.0f; //TODO
    //if(percent_delta < 0) percent_delta = -percent_delta;

    model_I->insertRow(index-1);
    model_I->setData(model_I->index(index-1, 0, QModelIndex()), i_A);
    model_I->setData(model_I->index(index-1, 1, QModelIndex()), max_delta_uV / 1000.0f);
    model_I->setData(model_I->index(index-1, 2, QModelIndex()), delta_uV / 1000.0f);
    model_I->setData(model_I->index(index-1, 3, QModelIndex()), percent_delta);
    model_I->setData(model_I->index(index-1, 4, QModelIndex()), (abs(delta_uV) < max_delta_uV) ? "ПРОЙДЕНО" : "ОШИБКА");
    index++;

    if(abs(delta_uV) > max_delta_uV)
    {
        check_is_true = false;
    }
#endif

    check_is_true = true;
    for(int i_A=1; i_A<=(MAX_CURRENT_B590 / 1000); i_A++)
    {
        if(flag_stop_check_I)
        {
            emit error(tr("Проверка прервана пользователем"));
            return false;
        }
        QCoreApplication::processEvents();
        ui->pb_check_I->setValue(i_A);

        int result_uV = 0;
        ok = check_point_I(i_A * 1000, &result_uV);
        if(!ok)
        {
            return false;
        }

        double r = ui->sb_shunt->value();
        int delta_uV = result_uV - (i_A * r * 1e6);
        int max_delta_uV = (i_A * 0.005f + 0.005f) * r * 1e6 * ui->dsb_pz_I->value();
        //double percent_delta = 100.0f - (result_uV * 100.0f / (i_A * r * 1e6));
        double percent_delta = (double)delta_uV / (i_A * r * 1e6) * 100.0f; //TODO
        //if(percent_delta < 0) percent_delta = -percent_delta;

        model_I->insertRow(index-1);
        model_I->setData(model_I->index(index-1, 0, QModelIndex()), i_A);
        model_I->setData(model_I->index(index-1, 1, QModelIndex()), max_delta_uV / 1000.0f);
        model_I->setData(model_I->index(index-1, 2, QModelIndex()), delta_uV / 1000.0f);
        model_I->setData(model_I->index(index-1, 3, QModelIndex()), percent_delta);
        model_I->setData(model_I->index(index-1, 4, QModelIndex()), (abs(delta_uV) < max_delta_uV) ? "ПРОЙДЕНО" : "ОШИБКА");
        index++;

        if(abs(delta_uV) > max_delta_uV)
        {
            check_is_true = false;
        }
    }

    powersupply->set_UI(0, 0);
    if(Calibration_remote_control_off() == false)   return false;
    return check_is_true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool B590::Calibration_remote_control_off(void)
{
    bool ok = powersupply->rc_off();
    if(!ok)
    {
        emit error(tr("Не удалось выключить удаленное кправление!"));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590::Calibration_press_enter(void)
{
    emit info(tr("Переход к следующей точке"));
    bool ok = powersupply->Send_Command_To_Next_Point();
    if(!ok)
    {
        powersupply->print_last_error();
        return ok;
    }
#ifndef FAKE
    Sleeper::sleep(10);
#endif
    return ok;
}
//--------------------------------------------------------------------------------
void B590::createPowerSupplyBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    B590_toolbar *psBar = new B590_toolbar(tr("powersupply B5-90"), this);
    psBar->setObjectName("ps_B590");
    mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, psBar);

    connect(psBar, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
}
//--------------------------------------------------------------------------------
void B590::createPropertyBox(void)
{
#ifndef USER_CALIBRATIONS
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();

    QtProperty *top_item_U_point_0 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Напряжение (0 точка)"));
    QtProperty *top_item_U_point_1 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Напряжение (1 точка)"));
    QtProperty *top_item_U_point_2 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Напряжение (2 точка)"));
    QtProperty *top_item_U_point_3 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Напряжение (3 точка)"));

    QtProperty *top_item_I_point_0 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (0 точка)"));
    QtProperty *top_item_I_point_1 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (1 точка)"));
    QtProperty *top_item_I_point_2 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (2 точка)"));
    QtProperty *top_item_I_point_3 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (3 точка)"));
    QtProperty *top_item_I_point_4 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (4 точка)"));
    QtProperty *top_item_I_point_5 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (5 точка)"));
    QtProperty *top_item_I_point_6 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (6 точка)"));
    QtProperty *top_item_I_point_7 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (7 точка)"));
    QtProperty *top_item_I_point_8 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (8 точка)"));
    QtProperty *top_item_I_point_9 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (9 точка)"));
    QtProperty *top_item_I_point_10 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (10 точка)"));
    QtProperty *top_item_I_point_11 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (11 точка)"));
    QtProperty *top_item_I_point_12 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (12 точка)"));

    //---
    item_U_point_0 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_U_max_deviation_point_0 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mV"));
    item_U_begin_DAC_point_0 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_U_point_1 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_U_max_deviation_point_1 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mV"));
    item_U_begin_DAC_point_1 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_U_point_2 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_U_max_deviation_point_2 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mV"));
    item_U_begin_DAC_point_2 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_U_point_3 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_U_max_deviation_point_3 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mV"));
    item_U_begin_DAC_point_3 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    //---
    item_I_point_0 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_0 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_0 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));
    item_I_end_DAC_point_0 = variantManager->addProperty(QVariant::Int, tr("end DAC"));

    item_I_point_1 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_1 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_1 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_2 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_2 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_2 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_3 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_3 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_3 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_4 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_4 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_4 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_5 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_5 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_5 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_6 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_6 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_6 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_7 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_7 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_7 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_8 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_8 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_8 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_9 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_9 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_9 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_10 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_10 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_10 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_11 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_11 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_11 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_12 = variantManager->addProperty(QVariant::Int, tr("Ток, mA"));
    item_I_max_deviation_point_12 = variantManager->addProperty(QVariant::Double, tr("Макс. откл., mA"));
    item_I_begin_DAC_point_12 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    //---
    item_U_point_0->setValue(POINT_U_0);
    item_U_max_deviation_point_0->setValue(B590_U_MAX_DEVIATION_POINT_0 / 1000.0f);
    item_U_begin_DAC_point_0->setValue(POINT_U_0);

    item_U_point_1->setValue(POINT_U_1);
    item_U_max_deviation_point_1->setValue(B590_U_MAX_DEVIATION_POINT_1 / 1000.0f);
    item_U_begin_DAC_point_1->setValue(POINT_U_1);

    item_U_point_2->setValue(POINT_U_2);
    item_U_max_deviation_point_2->setValue(B590_U_MAX_DEVIATION_POINT_2 / 1000.0f);
    item_U_begin_DAC_point_2->setValue(POINT_U_2);

    item_U_point_3->setValue(POINT_U_3);
    item_U_max_deviation_point_3->setValue(B590_U_MAX_DEVIATION_POINT_3 / 1000.0f);
    item_U_begin_DAC_point_3->setValue(POINT_U_3);

    //---
    item_I_point_0->setValue(POINT_I_0);
    item_I_max_deviation_point_0->setValue(B590_I_MAX_DEVIATION_POINT_0 / 1000.0f);
    item_I_begin_DAC_point_0->setValue(100);
    item_I_end_DAC_point_0->setValue(200);

    item_I_point_1->setValue(POINT_I_1);
    item_I_max_deviation_point_1->setValue(B590_I_MAX_DEVIATION_POINT_1 / 1000.0f);
    item_I_begin_DAC_point_1->setValue(POINT_I_1);

    item_I_point_2->setValue(POINT_I_2);
    item_I_max_deviation_point_2->setValue(B590_I_MAX_DEVIATION_POINT_2 / 1000.0f);
    item_I_begin_DAC_point_2->setValue(POINT_I_2);

    item_I_point_3->setValue(POINT_I_3);
    item_I_max_deviation_point_3->setValue(B590_I_MAX_DEVIATION_POINT_3 / 1000.0f);
    item_I_begin_DAC_point_3->setValue(POINT_I_3);

    item_I_point_4->setValue(POINT_I_4);
    item_I_max_deviation_point_4->setValue(B590_I_MAX_DEVIATION_POINT_4 / 1000.0f);
    item_I_begin_DAC_point_4->setValue(POINT_I_4);

    item_I_point_5->setValue(POINT_I_5);
    item_I_max_deviation_point_5->setValue(B590_I_MAX_DEVIATION_POINT_5 / 1000.0f);
    item_I_begin_DAC_point_5->setValue(POINT_I_5);

    item_I_point_6->setValue(POINT_I_6);
    item_I_max_deviation_point_6->setValue(B590_I_MAX_DEVIATION_POINT_6 / 1000.0f);
    item_I_begin_DAC_point_6->setValue(POINT_I_6);

    item_I_point_7->setValue(POINT_I_7);
    item_I_max_deviation_point_7->setValue(B590_I_MAX_DEVIATION_POINT_7 / 1000.0f);
    item_I_begin_DAC_point_7->setValue(POINT_I_7);

    item_I_point_8->setValue(POINT_I_8);
    item_I_max_deviation_point_8->setValue(B590_I_MAX_DEVIATION_POINT_8 / 1000.0f);
    item_I_begin_DAC_point_8->setValue(POINT_I_8);

    item_I_point_9->setValue(POINT_I_9);
    item_I_max_deviation_point_9->setValue(B590_I_MAX_DEVIATION_POINT_9 / 1000.0f);
    item_I_begin_DAC_point_9->setValue(POINT_I_9);

    item_I_point_10->setValue(POINT_I_10);
    item_I_max_deviation_point_10->setValue(B590_I_MAX_DEVIATION_POINT_10 / 1000.0f);
    item_I_begin_DAC_point_10->setValue(POINT_I_10);

    item_I_point_11->setValue(POINT_I_11);
    item_I_max_deviation_point_11->setValue(B590_I_MAX_DEVIATION_POINT_11 / 1000.0f);
    item_I_begin_DAC_point_11->setValue(POINT_I_11);

    item_I_point_12->setValue(POINT_I_12);
    item_I_max_deviation_point_12->setValue(B590_I_MAX_DEVIATION_POINT_12 / 1000.0f);
    item_I_begin_DAC_point_12->setValue(POINT_I_12);

    //---
    top_item_U_point_0->addSubProperty(item_U_point_0);
    top_item_U_point_0->addSubProperty(item_U_max_deviation_point_0);
    top_item_U_point_0->addSubProperty(item_U_begin_DAC_point_0);

    top_item_U_point_1->addSubProperty(item_U_point_1);
    top_item_U_point_1->addSubProperty(item_U_max_deviation_point_1);
    top_item_U_point_1->addSubProperty(item_U_begin_DAC_point_1);

    top_item_U_point_2->addSubProperty(item_U_point_2);
    top_item_U_point_2->addSubProperty(item_U_max_deviation_point_2);
    top_item_U_point_2->addSubProperty(item_U_begin_DAC_point_2);

    top_item_U_point_3->addSubProperty(item_U_point_3);
    top_item_U_point_3->addSubProperty(item_U_max_deviation_point_3);
    top_item_U_point_3->addSubProperty(item_U_begin_DAC_point_3);

    //---
    top_item_I_point_0->addSubProperty(item_I_point_0);
    top_item_I_point_0->addSubProperty(item_I_max_deviation_point_0);
    top_item_I_point_0->addSubProperty(item_I_begin_DAC_point_0);
    top_item_I_point_0->addSubProperty(item_I_end_DAC_point_0);

    top_item_I_point_1->addSubProperty(item_I_point_1);
    top_item_I_point_1->addSubProperty(item_I_max_deviation_point_1);
    top_item_I_point_1->addSubProperty(item_I_begin_DAC_point_1);

    top_item_I_point_2->addSubProperty(item_I_point_2);
    top_item_I_point_2->addSubProperty(item_I_max_deviation_point_2);
    top_item_I_point_2->addSubProperty(item_I_begin_DAC_point_2);

    top_item_I_point_3->addSubProperty(item_I_point_3);
    top_item_I_point_3->addSubProperty(item_I_max_deviation_point_3);
    top_item_I_point_3->addSubProperty(item_I_begin_DAC_point_3);

    top_item_I_point_4->addSubProperty(item_I_point_4);
    top_item_I_point_4->addSubProperty(item_I_max_deviation_point_4);
    top_item_I_point_4->addSubProperty(item_I_begin_DAC_point_4);

    top_item_I_point_5->addSubProperty(item_I_point_5);
    top_item_I_point_5->addSubProperty(item_I_max_deviation_point_5);
    top_item_I_point_5->addSubProperty(item_I_begin_DAC_point_5);

    top_item_I_point_6->addSubProperty(item_I_point_6);
    top_item_I_point_6->addSubProperty(item_I_max_deviation_point_6);
    top_item_I_point_6->addSubProperty(item_I_begin_DAC_point_6);

    top_item_I_point_7->addSubProperty(item_I_point_7);
    top_item_I_point_7->addSubProperty(item_I_max_deviation_point_7);
    top_item_I_point_7->addSubProperty(item_I_begin_DAC_point_7);

    top_item_I_point_8->addSubProperty(item_I_point_8);
    top_item_I_point_8->addSubProperty(item_I_max_deviation_point_8);
    top_item_I_point_8->addSubProperty(item_I_begin_DAC_point_8);

    top_item_I_point_9->addSubProperty(item_I_point_9);
    top_item_I_point_9->addSubProperty(item_I_max_deviation_point_9);
    top_item_I_point_9->addSubProperty(item_I_begin_DAC_point_9);

    top_item_I_point_10->addSubProperty(item_I_point_10);
    top_item_I_point_10->addSubProperty(item_I_max_deviation_point_10);
    top_item_I_point_10->addSubProperty(item_I_begin_DAC_point_10);

    top_item_I_point_11->addSubProperty(item_I_point_11);
    top_item_I_point_11->addSubProperty(item_I_max_deviation_point_11);
    top_item_I_point_11->addSubProperty(item_I_begin_DAC_point_11);

    top_item_I_point_12->addSubProperty(item_I_point_12);
    top_item_I_point_12->addSubProperty(item_I_max_deviation_point_12);
    top_item_I_point_12->addSubProperty(item_I_begin_DAC_point_12);

    //---
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser(this);
    variantEditor->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    variantEditor->setFactoryForManager(variantManager, variantFactory);

    QtBrowserItem *item_U_0 = variantEditor->addProperty(top_item_U_point_0);
    QtBrowserItem *item_U_1 = variantEditor->addProperty(top_item_U_point_1);
    QtBrowserItem *item_U_2 = variantEditor->addProperty(top_item_U_point_2);
    QtBrowserItem *item_U_3 = variantEditor->addProperty(top_item_U_point_3);

    QtBrowserItem *item_I_0 = variantEditor->addProperty(top_item_I_point_0);
    QtBrowserItem *item_I_1 = variantEditor->addProperty(top_item_I_point_1);
    QtBrowserItem *item_I_2 = variantEditor->addProperty(top_item_I_point_2);
    QtBrowserItem *item_I_3 = variantEditor->addProperty(top_item_I_point_3);
    QtBrowserItem *item_I_4 = variantEditor->addProperty(top_item_I_point_4);
    QtBrowserItem *item_I_5 = variantEditor->addProperty(top_item_I_point_5);
    QtBrowserItem *item_I_6 = variantEditor->addProperty(top_item_I_point_6);
    QtBrowserItem *item_I_7 = variantEditor->addProperty(top_item_I_point_7);
    QtBrowserItem *item_I_8 = variantEditor->addProperty(top_item_I_point_8);
    QtBrowserItem *item_I_9 = variantEditor->addProperty(top_item_I_point_9);
    QtBrowserItem *item_I_10 = variantEditor->addProperty(top_item_I_point_10);
    QtBrowserItem *item_I_11 = variantEditor->addProperty(top_item_I_point_11);
    QtBrowserItem *item_I_12 = variantEditor->addProperty(top_item_I_point_12);

    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    variantEditor->adjustSize();
    variantEditor->setMinimumSize(variantEditor->sizeHint());

    ui->property_layout->addWidget(variantEditor);

    variantEditor->setExpanded(item_U_0, false);
    variantEditor->setExpanded(item_U_1, false);
    variantEditor->setExpanded(item_U_2, false);
    variantEditor->setExpanded(item_U_3, false);

    variantEditor->setExpanded(item_I_0, false);
    variantEditor->setExpanded(item_I_1, false);
    variantEditor->setExpanded(item_I_2, false);
    variantEditor->setExpanded(item_I_3, false);
    variantEditor->setExpanded(item_I_4, false);
    variantEditor->setExpanded(item_I_5, false);
    variantEditor->setExpanded(item_I_6, false);
    variantEditor->setExpanded(item_I_7, false);
    variantEditor->setExpanded(item_I_8, false);
    variantEditor->setExpanded(item_I_9, false);
    variantEditor->setExpanded(item_I_10, false);
    variantEditor->setExpanded(item_I_11, false);
    variantEditor->setExpanded(item_I_12, false);
#endif
}
//--------------------------------------------------------------------------------
void B590::stop_calibration_ADC_U(void)
{
    flag_stop_calibration_ADC_U = true;
}
//--------------------------------------------------------------------------------
void B590::stop_calibration_DAC_U(void)
{
    flag_stop_calibration_DAC_U = true;
}
//--------------------------------------------------------------------------------
void B590::stop_calibration_ADC_I(void)
{
    flag_stop_calibration_ADC_I = true;
}
//--------------------------------------------------------------------------------
void B590::stop_calibration_DAC_I(void)
{
    flag_stop_calibration_DAC_I = true;
}
//--------------------------------------------------------------------------------
void B590::stop_check_U(void)
{
    flag_stop_check_U = true;
}
//--------------------------------------------------------------------------------
void B590::stop_check_I(void)
{
    flag_stop_check_I = true;
}
//--------------------------------------------------------------------------------
void B590::set_max_U(void)
{
    emit info(tr("Установка максимально возможного напряжения"));
    bool ok = powersupply->set_UI_parrot(0xFFFF, DEFAULT_I);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::set_max_I(void)
{
    emit info(tr("Установка максимально возможного тока"));
    bool ok = powersupply->set_UI_parrot(DEFAULT_U, 0xFFFF);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::set_clear_UI(void)
{
    emit info(tr("Установка минимально возможного напряжения и тока"));
    bool ok = powersupply->set_UI_parrot(0, 0);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::save_table_U(void)
{
#ifdef FAKE
    for(int n=0; n<(MAX_VOLTAGE_B590 / 1000); n++)
    {
        model_U->removeRows(0, 1, QModelIndex());
    }

    double delta_uV = 1.0f;
    double max_delta_uV = 10.0f;
    double percent_delta = 0.001f;
    for(int u=1; u<=(MAX_VOLTAGE_B590 / 1000); u++)
    {
        model_U->insertRow(u-1);
        model_U->setData(model_U->index(u-1, 0, QModelIndex()), u);
        model_U->setData(model_U->index(u-1, 1, QModelIndex()), max_delta_uV / 1000.0f);
        model_U->setData(model_U->index(u-1, 2, QModelIndex()), delta_uV / 1000.0f);
        model_U->setData(model_U->index(u-1, 3, QModelIndex()), percent_delta);
        model_U->setData(model_U->index(u-1, 4, QModelIndex()), (abs(delta_uV) < max_delta_uV) ? "ПРОЙДЕНО" : "ОШИБКА");
    }
#endif

    if(model_U->rowCount() < 2)
    {
        emit error("no data");
        return;
    }

    QStringList sl;
    QString str;
    sl.clear();
    str.clear();

    str.append(QString("Производственный запас %1\n").arg(ui->dsb_pz_U->value(), 0, 'f', 1));

    str.append(QString("%1").arg(model_U->headerData(0, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_U->headerData(1, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_U->headerData(2, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_U->headerData(3, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_U->headerData(4, Qt::Horizontal).toString()));
    str.append("\n");
    sl.append(str);

    for(int row=0; row<model_U->rowCount(); row++)
    {
        str.clear();
        for(int column=0; column<model_U->columnCount(); column++)
        {
            QModelIndex index = model_U->index(row, column);
            QString text = "";
            if(column < (model_U->columnCount() - 1))
                text = QString("%1").arg(index.data(Qt::DisplayRole).toFloat(), 0, 'f', 3);
            else
                text = index.data(Qt::DisplayRole).toString();
            str.append(text);
            if(column < (model_U->columnCount()-1)) str.append("|");
        }
        str.append("\n");
        sl.append(str);
    }

    QFileDialog *dlg;

    dlg = new QFileDialog();
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("csv files (*.csv)"));
    dlg->setDefaultSuffix(tr("csv"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("report_U");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QFile file(files.at(0));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            emit error("file not created!");
            return;
        }

        QTextStream out(&file);
        foreach (QString text, sl)
        {
            out << text;
        }
    }
    delete dlg;
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::save_table_I(void)
{
#ifdef FAKE
    for(int n=0; n<(MAX_CURRENT_B590 / 1000); n++)
    {
        model_I->removeRows(0, 1, QModelIndex());
    }

    double delta_uV = 1.0f;
    double max_delta_uV = 10.0f;
    double percent_delta = 0.001f;
    for(int i=1; i<(MAX_CURRENT_B590 / 1000); i++)
    {
        model_I->insertRow(i-1);
        model_I->setData(model_I->index(i-1, 0, QModelIndex()), i);
        model_I->setData(model_I->index(i-1, 1, QModelIndex()), max_delta_uV / 1000.0f);
        model_I->setData(model_I->index(i-1, 2, QModelIndex()), delta_uV / 1000.0f);
        model_I->setData(model_I->index(i-1, 3, QModelIndex()), percent_delta);
        model_I->setData(model_I->index(i-1, 4, QModelIndex()), (abs(delta_uV) < max_delta_uV) ? "ПРОЙДЕНО" : "ОШИБКА");
    }
#endif

    if(model_I->rowCount() < 2)
    {
        emit error("no data");
        return;
    }

    QStringList sl;
    QString str;
    sl.clear();
    str.clear();

    str.append(QString("Производственный запас %1\n").arg(ui->dsb_pz_I->value(), 0, 'f', 1));
    str.append(QString("Шунт %1\n").arg(ui->sb_shunt->value()));

    str.append(QString("%1").arg(model_I->headerData(0, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_I->headerData(1, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_I->headerData(2, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_I->headerData(3, Qt::Horizontal).toString()));
    str.append("|");
    str.append(QString("%1").arg(model_I->headerData(4, Qt::Horizontal).toString()));
    str.append("\n");
    sl.append(str);

    for(int row=0; row<model_I->rowCount(); row++)
    {
        str.clear();
        for(int column=0; column<model_I->columnCount(); column++)
        {
            QModelIndex index = model_I->index(row, column);
            QString text = "";
            if(column < (model_I->columnCount() - 1))
                text = QString("%1").arg(index.data(Qt::DisplayRole).toFloat(), 0, 'f', 3);
            else
                text = index.data(Qt::DisplayRole).toString();
            str.append(text);
            if(column < (model_I->columnCount()-1)) str.append("|");
        }
        str.append("\n");
        sl.append(str);
    }

    QFileDialog *dlg;

    dlg = new QFileDialog();
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter(tr("csv files (*.csv)"));
    dlg->setDefaultSuffix(tr("csv"));
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("report_I");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QFile file(files.at(0));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            emit error("file not created!");
            return;
        }

        QTextStream out(&file);
        foreach (QString text, sl)
        {
            out << text;
        }
    }
    delete dlg;
    emit info("OK");
}
//--------------------------------------------------------------------------------
