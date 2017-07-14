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
#include <QStringListModel>
#include <QStandardItemModel>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QToolButton>
#include <QMessageBox>
#include <QToolBar>
#include <QDialog>
#include <QPixmap>
#include <QTime>
#include <QMenu>
//--------------------------------------------------------------------------------
#include <qtpropertymanager.h>
#include <qtpropertybrowser.h>
#include <qtvariantproperty.h>
#include <qttreepropertybrowser.h>
//--------------------------------------------------------------------------------
#include "myreport.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "b588_powersupply.hpp"
//--------------------------------------------------------------------------------
#ifdef V764
    #include "v764_multimeter.hpp"
#endif
#ifdef V7642
    #include "v764_2_multimeter.hpp"
#endif
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "b588.hpp"
#include "defines.hpp"
#include "parrotbar.hpp"
#include "b588_toolbar.hpp"
//--------------------------------------------------------------------------------
#ifndef USER_CALIBRATIONS
    #include "ui_b588.h"
#else
    #include "ui_b588_user.h"
#endif
//--------------------------------------------------------------------------------
B588::B588(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::B588)
{
    init();
}
//--------------------------------------------------------------------------------
B588::~B588()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void B588::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parentWidget(), SIGNAL(message(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void B588::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data);
#endif
}
//--------------------------------------------------------------------------------
void B588::init(void)
{
    ui->setupUi(this);
    connect_log();

#ifdef V764
    multimeter = new Multimeter_V764();
#endif
#ifdef V7642
    multimeter = new Multimeter_V764_2();
#endif

    powersupply = new Powersupply_B588();
    powersupply->set_delay_ms(TIME_READY_POWERSUPPLY_MS);

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        connect(multimeter, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(multimeter, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(multimeter, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(multimeter, SIGNAL(message(QString)), mw, SIGNAL(message(QString)));

        connect(powersupply, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(powersupply, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(powersupply, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(powersupply, SIGNAL(message(QString)), mw, SIGNAL(message(QString)));
    }

    block_interface(true);

#ifdef TEST
    createTestBar();
    createParrotBar();
    createPowerSupplyBar();
#endif
    createPropertyBox();

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->sb_address->setMinimum(MIN_ADDRESS);
    ui->sb_address->setMaximum(MAX_ADDRESS);

    ui->sb_shunt->setMinimum(0.0000001f);
    ui->sb_shunt->setSingleStep(0.0000001f);
    ui->sb_shunt->setDecimals(7);

    ui->pb_calibration_ADC_U->setValue(0);
    ui->pb_calibration_DAC_U->setValue(0);
    ui->pb_calibration_ADC_I->setValue(0);
    ui->pb_calibration_DAC_I->setValue(0);
    ui->pb_check_U->setValue(0);
    ui->pb_check_I->setValue(0);
    ui->pb_check_3_channel->setValue(0);

    ui->btn_find_devices->setDisabled(false);
    ui->label_address->setDisabled(false);
    ui->sb_address->setDisabled(false);
    ui->cb_ignore_error->setDisabled(false);

    ui->table_U->setMinimumWidth(512);
    ui->table_I->setMinimumWidth(512);

    flag_stop_calibration_ADC_U = false;
    flag_stop_calibration_DAC_U = false;
    flag_stop_calibration_ADC_I = false;
    flag_stop_calibration_DAC_I = false;

    flag_stop_check_U = false;
    flag_stop_check_I = false;

    //---
    //QSqlQueryModel model;
    //---
    model_U = new QStandardItemModel(0, 4, this);
    model_U->setHeaderData(0, Qt::Horizontal, tr("Text 1"));
    model_U->setHeaderData(1, Qt::Horizontal, tr("Text 2"));
    model_U->setHeaderData(2, Qt::Horizontal, tr("Text 3"));
    model_U->setHeaderData(3, Qt::Horizontal, tr("Text 4"));
    //---
    model_I = new QStandardItemModel(0, 4, this);
    model_I->setHeaderData(0, Qt::Horizontal, tr("Text 1"));
    model_I->setHeaderData(1, Qt::Horizontal, tr("Text 2"));
    model_I->setHeaderData(2, Qt::Horizontal, tr("Text 3"));
    model_I->setHeaderData(3, Qt::Horizontal, tr("Text 4"));
    //---
    ui->table_U->setModel(model_U);
    ui->table_I->setModel(model_I);
    //---
    model_U->insertRows(0, 2, QModelIndex());
    model_U->setData(model_U->index(0, 0, QModelIndex()), 100);
    model_U->setData(model_U->index(0, 1, QModelIndex()), 200);
    model_U->setData(model_U->index(0, 2, QModelIndex()), 300);
    model_U->setData(model_U->index(0, 3, QModelIndex()), 400);

    model_I->insertRows(0, 2, QModelIndex());
    model_I->setData(model_I->index(0, 0, QModelIndex()), 100);
    model_I->setData(model_I->index(0, 1, QModelIndex()), 200);
    model_I->setData(model_I->index(0, 2, QModelIndex()), 300);
    model_I->setData(model_I->index(0, 3, QModelIndex()), 400);
    //---

    connect(ui->btn_find_devices, SIGNAL(pressed()), this, SLOT(find_devices()));

    connect(ui->btn_calibration_ADC_U,  SIGNAL(pressed()), this, SLOT(calibration_ADC_U()));
    connect(ui->btn_calibration_DAC_U,  SIGNAL(pressed()), this, SLOT(calibration_DAC_U()));
    connect(ui->btn_check_U,            SIGNAL(pressed()), this, SLOT(check_control_point_U()));
    connect(ui->btn_check_3_channel,    SIGNAL(pressed()), this, SLOT(check_3_channel()));
    connect(ui->btn_calibration_ADC_I,  SIGNAL(pressed()), this, SLOT(calibration_ADC_I()));
    connect(ui->btn_calibration_DAC_I,  SIGNAL(pressed()), this, SLOT(calibration_DAC_I()));
    connect(ui->btn_check_I,            SIGNAL(pressed()), this, SLOT(check_control_point_I()));
    //connect(ui->btn_check_user_point,   SIGNAL(pressed()), this, SLOT(check_user_point()));
#ifndef USER_CALIBRATIONS
    connect(ui->btn_save_data,          SIGNAL(pressed()), this, SLOT(save_data()));
    connect(ui->btn_report,             SIGNAL(pressed()), this, SLOT(save_report()));
#endif

    connect(ui->cb_ignore_error, SIGNAL(toggled(bool)), powersupply, SLOT(set_ignore_bad_cmd(bool)));

    connect(ui->btn_stop_calibration_ADC_U, SIGNAL(pressed()), this, SLOT(stop_calibration_ADC_U()));
    connect(ui->btn_stop_calibration_DAC_U, SIGNAL(pressed()), this, SLOT(stop_calibration_DAC_U()));
    connect(ui->btn_stop_calibration_ADC_I, SIGNAL(pressed()), this, SLOT(stop_calibration_ADC_I()));
    connect(ui->btn_stop_calibration_DAC_I, SIGNAL(pressed()), this, SLOT(stop_calibration_DAC_I()));

    connect(ui->btn_stop_check_U,       SIGNAL(pressed()), this, SLOT(stop_check_U()));
    connect(ui->btn_stop_check_I,       SIGNAL(pressed()), this, SLOT(stop_check_I()));
}
//--------------------------------------------------------------------------------
void B588::find_devices(void)
{
    bool ok = false;

    ui->btn_find_devices->setEnabled(false);
    ui->sb_address->setEnabled(false);
    emit info(tr("Поиск устройств"));

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_ignore_bad_cmd(ui->cb_ignore_error->isChecked());

    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-88 НЕ найден");
        ui->btn_find_devices->setEnabled(true);
        ui->sb_address->setEnabled(true);
        return;
    }


    multimeter->set_state_silence(true);
    ok = multimeter->find_device();
    multimeter->set_state_silence(false);
    if(!ok)
    {
        emit error("Мультиметр В7-64 НЕ найден");
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
bool B588::process_calibration_ADC_U(void)
{
    if(Calibration_remote_control_on() == false)    return false;
    if(Calibration_ADC_U_begin() == false)          return false;
    if(Calibration_ADC_U_find_point_1() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    if(Calibration_ADC_U_find_point_2() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    if(Calibration_remote_control_off() == false)   return false;
    return true;
}
//--------------------------------------------------------------------------------
bool B588::process_calibration_DAC_U(void)
{
    if(Calibration_remote_control_on() == false)    return false;
    if(Calibration_DAC_U_begin() == false)          return false;
    if(Calibration_DAC_U_show_process() == false)   return false;
    if(Calibration_remote_control_off() == false)   return false;
    return true;
}
//--------------------------------------------------------------------------------
void B588::calibration_ADC_U(void)
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
    bool ok = process_calibration_ADC_U();
    block_interface(false);

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
}
//--------------------------------------------------------------------------------
void B588::calibration_DAC_U(void)
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
    bool ok = process_calibration_DAC_U();
    block_interface(false);

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
bool B588::wait_hot_temp(unsigned char begin_temp)
{
    unsigned char old_temp = 0;
    unsigned char temp = 0;
    emit info(QString(tr("Ожидаем температуру %1 C")).arg(begin_temp + DELTA_TEMP_CALIBRATION_CURRENT));
    while(temp < (begin_temp + 15))
    {
        bool ok = powersupply->get_temperature(&temp);
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
bool B588::wait_temp_30(unsigned char *current_temp)
{
    unsigned char old_temp = 100;
    unsigned char temp = 0;
    bool ok = powersupply->get_temperature(&temp);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    if(temp <= 30)
    {
        *current_temp = temp;
        return true;
    }

    powersupply->set_UI_parrot(0, 0);
    powersupply->set_vent_speed(1500);
    //powersupply->run_dialog_0x60();

    emit info(tr("Ожидаем температуру 30 C"));
    while(temp > 30)
    {
        bool ok = powersupply->get_temperature(&temp);
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
    }
    *current_temp = temp;
    return true;
}
//--------------------------------------------------------------------------------
bool B588::process_calibration_ADC_I(void)
{
    if(!Calibration_remote_control_on())    return false;

    unsigned char begin_temp = 0;
    if(!powersupply->get_temperature(&begin_temp))
    {
        Calibration_remote_control_off();
        return false;
    }

    if(begin_temp > 30)
    {
        emit error(QString("температура %1 больше 30").arg(begin_temp));
        powersupply->set_UI(0, 0);
        Calibration_remote_control_off();
        return false;
    }

    ui->label_calibration_ADC_I->setPixmap(QPixmap(PICTURE_FAIL));
    QTime timer;
    timer.start();

    if(Calibration_ADC_I_begin() == false)          return false;
    if(Calibration_ADC_I_find_point_1() == false)   return false;
    if(Calibration_press_enter() == false)          return false;
    if(Calibration_ADC_I_find_point_2() == false)   return false;
    if(Calibration_press_enter() == false)          return false;

#ifndef FAKE
    if(wait_hot_temp(begin_temp) == false)              return false;
    if(Calibration_press_enter() == false)          return false;
#endif

    if(Calibration_ADC_I_find_point_3() == false)       return false;
    if(Calibration_press_enter() == false)              return false;

    if(Calibration_remote_control_off() == false)       return false;
    return true;
}
//--------------------------------------------------------------------------------
bool B588::process_calibration_DAC_I(void)
{
    if(!Calibration_remote_control_on())    return false;

    if(Calibration_DAC_I_begin() == false)              return false;
    if(Calibration_DAC_I_show_process() == false)       return false;

    if(Calibration_remote_control_off() == false)       return false;
    return true;
}
//--------------------------------------------------------------------------------
void B588::calibration_ADC_I(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Калибровка тока"),
                                    tr("Подключите шунт, согласно документации. Не забудьте корректно указать его номинал в поле 'шунт'\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Калибровка тока ..."));

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    QTime timer;
    timer.start();

    block_interface(true);
    bool ok = process_calibration_ADC_I();
    block_interface(false);

    if(ok)
    {
        ui->label_calibration_ADC_I->setPixmap(QPixmap(PICTURE_OK));
        emit info(QString("Калибровка завершена успешно. Прошло %1 сек.").arg((int)(timer.elapsed() / 1000.0f + 0.5f)));
    }
}
//--------------------------------------------------------------------------------
void B588::calibration_DAC_I(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Калибровка тока"),
                                    tr("Подключите шунт, согласно документации. Не забудьте корректно указать его номинал в поле 'шунт'\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Калибровка тока ..."));

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    QTime timer;
    timer.start();

    block_interface(true);
    bool ok = process_calibration_DAC_I();
    block_interface(false);

    if(ok)
    {
        ui->label_calibration_DAC_I->setPixmap(QPixmap(PICTURE_OK));
        emit info(QString("Калибровка завершена успешно. Прошло %1 сек.").arg((int)(timer.elapsed() / 1000.0f + 0.5f)));
    }
}
//--------------------------------------------------------------------------------
void B588::check_control_point_U(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Проверка контрольных точек напряжения"),
                                    tr("Не забудьте подключить нагрузку 100-300 ом.\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Проверка контрольных точек напряжения ..."));

    powersupply->set_UI(0, 0);
    if(Calibration_DAC_U_check_points() == false)
    {
        ui->label_check_U->setPixmap(QPixmap(PICTURE_FAIL));
        emit error(tr("НЕ пройдено!"));
    }
    else
    {
        ui->label_check_U->setPixmap(QPixmap(PICTURE_OK));
        emit info(tr("OK"));
    }

    block_interface(false);
}
//--------------------------------------------------------------------------------
void B588::check_3_channel(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Проверка третьего канала."),
                                    tr("Отключите нагрузку.\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    emit info(tr("Проверка третьего канала."));

    block_interface(true);

    Point3 data;
    list.clear();

    data.number_point = 1;
    data.PWM = 1600;
    data.set_voltage_mV = 10000;
    data.get_voltage_mV = 15000;
    data.result_mV = 0;
    data.max_deviation_mV = 2500;
    list.append(data);

    data.number_point = 2;
    data.PWM = 1700;
    data.set_voltage_mV = 20000;
    data.get_voltage_mV = 34000;
    data.result_mV = 0;
    data.max_deviation_mV = 2500;
    list.append(data);

    data.number_point = 3;
    data.PWM = 1700;
    data.set_voltage_mV = 30001;
    data.get_voltage_mV = 45000;
    data.result_mV = 0;
    data.max_deviation_mV = 5000;
    list.append(data);

    data.number_point = 4;
    data.PWM = 1700;
    data.set_voltage_mV = 60000;
    data.get_voltage_mV = 70000;
    data.result_mV = 0;
    data.max_deviation_mV = 5000;
    list.append(data);

    powersupply->set_state_silence(true);
    emit info(tr("Проверка третьего канала начата"));

    bool check_is_true = true;
    bool ok = false;
    int cnt_err = 0;

    ui->pb_check_3_channel->setDisabled(false);
    ui->pb_check_3_channel->setMaximum(list.count());
    ui->pb_check_3_channel->setValue(0);
    powersupply->rc_on();

    int n = 1;
    foreach (Point3 data, list)
    {
        QCoreApplication::processEvents();

        ui->pb_check_3_channel->setValue(n++);

        cnt_err = 0;
        while(cnt_err < 11)
        {
            ok = powersupply->test_3_channel(data.PWM,
                                             data.set_voltage_mV,
                                             data.get_voltage_mV,
                                             &data.result_mV,
                                             data.max_deviation_mV);
            cnt_err++;
            if(ok) break;
        }
        if(cnt_err > 9)
        {
            emit error(QString("%1 точка НЕУДАЧНО "
                               "(кол-во попыток %2 "
                               "установленное значение %3 В "
                               "измеренное значение %4 В "
                               "дельта %5 В "
                               "допустимая дельта %6 В)")
                       .arg(data.number_point)
                       .arg(cnt_err)
                       .arg(data.set_voltage_mV / 1000.0f)
                       .arg(data.result_mV / 1000.0f)
                       .arg((data.result_mV - data.get_voltage_mV) / 1000.0f)
                       .arg(data.max_deviation_mV / 1000.0f));

            check_is_true = false;
        }
        else
        {
            emit info(QString("%1 точка "
                              "(кол-во попыток %2 "
                              "установленное значение %3 В "
                              "измеренное значение %4 В "
                              "дельта %5 В "
                              "допустимая дельта %6 В)")
                      .arg(data.number_point)
                      .arg(cnt_err)
                      .arg(data.set_voltage_mV / 1000.0f)
                      .arg(data.result_mV / 1000.0f)
                      .arg((data.result_mV - data.get_voltage_mV) / 1000.0f)
                      .arg(data.max_deviation_mV / 1000.0f));
        }
    }

    powersupply->set_state_silence(false);
    if(check_is_true)
    {
        ui->label_check_3_channel->setPixmap(QPixmap(PICTURE_OK));
        emit info(tr("Проверка третьего канала завершена УДАЧНО"));
    }
    else
    {
        ui->label_check_3_channel->setPixmap(QPixmap(PICTURE_FAIL));
        emit error(tr("Проверка третьего канала завершена НЕУДАЧНО"));
    }

    powersupply->set_UI(0, 0);
    powersupply->rc_off();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void B588::check_control_point_I(void)
{
    int btn = QMessageBox::question(this,
                                    tr("Проверка контрольных точек тока"),
                                    tr("Подключите шунт, согласно документации. Не забудьте корректно указать его номинал в поле 'шунт'\nПродолжить?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes) return;

    if(Calibration_DAC_I_check_points() == false)
    {
        ui->label_check_I->setPixmap(QPixmap(PICTURE_FAIL));
        emit error(tr("НЕ пройдено!"));
    }
    else
    {
        ui->label_check_I->setPixmap(QPixmap(PICTURE_OK));
        emit info(tr("OK"));
    }

    block_interface(false);
}
//--------------------------------------------------------------------------------
void B588::check_user_point(void)
{
    emit error(tr("Главный конструктор считает, что это не нужно! Когда-то было..."));
}
//--------------------------------------------------------------------------------
void B588::save_data(void)
{
    uint8_t  type = 0;
    uint16_t year = 0;
    uint8_t  month = 0;
    uint16_t serno = 0;
    bool ok = false;

    ok = powersupply->get_information(&type, &year, &month, &serno);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    QSpinBox *sb_year = new QSpinBox();
    sb_year->setMinimum(1980);
    sb_year->setMaximum(2020);
    sb_year->setValue(year);

    QSpinBox *sb_month = new QSpinBox();
    sb_month->setMinimum(1);
    sb_month->setMaximum(12);
    sb_month->setValue(month);

    QSpinBox *sb_serno = new QSpinBox();
    sb_serno->setMinimum(0);
    sb_serno->setMaximum(9999);
    sb_serno->setValue(serno);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("year"),  0, 0);
    grid->addWidget(sb_year,             0, 1);
    grid->addWidget(new QLabel("month"), 1, 0);
    grid->addWidget(sb_month,            1, 1);
    grid->addWidget(new QLabel("serno"), 2, 0);
    grid->addWidget(sb_serno,            2, 1);

    QDialog *dlg = new QDialog();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addWidget(buttonBox);

    dlg->setLayout(vbox);
    dlg->setFixedSize(dlg->sizeHint());

    int btn = dlg->exec();
    if(btn != QDialog::Accepted) return;

    emit info("Записываем дату и серийный номер");
    ok = powersupply->set_date_and_serno(sb_year->value(),
                                         sb_month->value(),
                                         sb_serno->value());
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
}
//--------------------------------------------------------------------------------
void B588::generate_fake_data(void)
{

}
//--------------------------------------------------------------------------------
void B588::save_report(void)
{
#ifndef USER_CALIBRATIONS

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

    report->clean_database_report();

    report->view(serno, year);
    report->close_database(database_name);
    report->deleteLater();

    emit info("OK");
#endif
}
//--------------------------------------------------------------------------------
void B588::block_interface(bool state)
{
    ui->btn_calibration_ADC_U->setDisabled(state);
    ui->btn_calibration_DAC_U->setDisabled(state);
    ui->btn_calibration_ADC_I->setDisabled(state);
    ui->btn_calibration_DAC_I->setDisabled(state);
    ui->btn_check_3_channel->setDisabled(state);
    ui->btn_check_I->setDisabled(state);
    ui->btn_check_U->setDisabled(state);
    ui->btn_find_devices->setDisabled(state);
#ifndef USER_CALIBRATIONS
    ui->btn_report->setDisabled(state);
    ui->btn_save_data->setDisabled(state);
#endif

    ui->btn_stop_calibration_ADC_U->setDisabled(state);
    ui->btn_stop_calibration_DAC_U->setDisabled(state);
    ui->btn_stop_calibration_ADC_I->setDisabled(state);
    ui->btn_stop_calibration_DAC_I->setDisabled(state);
    ui->btn_stop_check_I->setDisabled(state);
    ui->btn_stop_check_U->setDisabled(state);

    ui->cb_ignore_error->setDisabled(state);

    ui->label_address->setDisabled(state);
    ui->label_calibration_ADC_U->setDisabled(state);
    ui->label_calibration_DAC_U->setDisabled(state);
    ui->label_calibration_ADC_I->setDisabled(state);
    ui->label_calibration_DAC_I->setDisabled(state);
    ui->label_check_3_channel->setDisabled(state);
    ui->label_check_I->setDisabled(state);
    ui->label_check_U->setDisabled(state);
    ui->label_shunt->setDisabled(state);

    ui->pb_calibration_ADC_U->setDisabled(state);
    ui->pb_calibration_DAC_U->setDisabled(state);
    ui->pb_calibration_ADC_I->setDisabled(state);
    ui->pb_calibration_DAC_I->setDisabled(state);
    ui->pb_check_3_channel->setDisabled(state);
    ui->pb_check_I->setDisabled(state);
    ui->pb_check_U->setDisabled(state);

    ui->sb_address->setDisabled(state);
    ui->sb_shunt->setDisabled(state);
}
//--------------------------------------------------------------------------------
QToolButton *B588::add_button(QToolBar *tool_bar,
                              QToolButton *tool_button,
                              QIcon icon,
                              const QString &text,
                              const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void B588::createParrotBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(!mw) return;

    ParrotBar *parrotBar = new ParrotBar(this);
    connect(parrotBar, SIGNAL(set_UI(int,int)), this, SLOT(set_UI_parrot(int,int)));

    mw->addToolBar(Qt::TopToolBarArea, parrotBar);
}
//--------------------------------------------------------------------------------
void B588::set_UI_parrot(int U, int I)
{
    powersupply->set_address(ui->sb_address->value());
    //powersupply->set_state_silence(true);
    bool ok = powersupply->set_UI_parrot(U, I);
    //powersupply->set_state_silence(false);
    if(!ok) powersupply->print_last_error();
}
//--------------------------------------------------------------------------------
void B588::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(!mw) return;

    QToolBar *toolBar = new QToolBar(tr("testbar B5-88"));
    toolBar->setObjectName("toolbar_B588");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test B5-88",
                                       "test B5-88");
    QToolButton *btn_rc_on = add_button(toolBar,
                                        new QToolButton(this),
                                        QIcon(":/pictures/button_ok.png"),
                                        "ON",
                                        "ON");
    QToolButton *btn_rc_off = add_button(toolBar,
                                         new QToolButton(this),
                                         QIcon(":/pictures/button_cancel.png"),
                                         "OFF",
                                         "OFF");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_rc_on,  SIGNAL(pressed()), powersupply, SLOT(rc_on()));
    connect(btn_rc_off, SIGNAL(pressed()), powersupply, SLOT(rc_off()));
}
//--------------------------------------------------------------------------------
void B588::test(void)
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
bool B588::measuring(int value_uV, int *measuring_value_uV)
{
    int value = 0;
    bool ok = false;
    int find_is_true = 0;
    int cnt_measuring = 0;

    while(cnt_measuring < 10)
    {
        ok = multimeter->get_value(&value);
        if(ok)
        {
            int delta = value_uV - value;
            if(!delta)
            {
                find_is_true++;
                if(find_is_true >= 3)
                {
                    *measuring_value_uV = value;
                    return true;
                }
            }
            else
            {
                find_is_true = 0;
            }
        }
        cnt_measuring++;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool B588::find_U(uint16_t value_mV,
                  uint16_t max_delta_mV)
{
    int measuring_value_uV = 0;
    bool found_value = false;
    unsigned short parrot_value = value_mV;
    int cnt_repeat = 1;
    bool ok = false;

    int value_uV = value_mV * 1000.0f;
    emit info(QString("Поиск %1 В...").arg(value_mV / 1000.0f));
    if(value_mV > 0x7FFF) powersupply->set_UI(0x7FFF, DEFAULT_I);
    while(!found_value)
    {
        ok = powersupply->set_UI_parrot(parrot_value, DEFAULT_I);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }

#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

        if(measuring(value_mV * 1000.0f, &measuring_value_uV))
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

            int delta = (int)value_mV - (int)(measuring_value_uV / 1000.0f + 0.5f);
            emit debug(QString("need %1 measuring %2 delta %3")
                       .arg(value_uV)
                       .arg(measuring_value_uV)
                       .arg(delta));
            if(abs(delta) > max_delta_mV)
            {
                //parrot_value += (int)(delta / 2.0f + 0.5f);
                parrot_value += delta;
            }
            else
            {
                found_value = true;
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
        emit info(QString("Найдено за %1 попыток. Измеренное значение %2 мкВ")
                  .arg(cnt_repeat)
                  .arg(measuring_value_uV));
    }
    return found_value;
}
//--------------------------------------------------------------------------------
bool B588::find_I(uint16_t begin_parrot_I,
                  uint16_t value_mA,
                  uint16_t max_delta_uV)
{
    int value_uV = (int)(value_mA * 1000.0f * ui->sb_shunt->value() + 0.5f);
    emit info(QString("Поиск %1 mA. Макс. кол-во попыток %2. Для этого на шунте %3 ом должно быть напряжение %4 мкВ...")
              .arg(value_mA)
              .arg(MAX_REPEAT_FIND_I)
              .arg(ui->sb_shunt->value())
              .arg(value_uV));

    int measuring_value_uV = 0;
    unsigned short parrot_value = begin_parrot_I;
    bool found_value = false;
    int cnt_repeat = 1;
    bool ok = false;

    while(!found_value)
    {
        ok = powersupply->set_UI_parrot(DEFAULT_U, parrot_value);
        if(!ok) powersupply->print_last_error();

#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

        if(measuring(value_uV, &measuring_value_uV))
        {
            found_value = true;
        }
        else
        {
            ok = multimeter->get_value(&measuring_value_uV);
            if(!ok) multimeter->print_last_error();

            int delta = (int)(value_uV - measuring_value_uV + 0.5f);
            emit debug(QString("need %1 measuring %2 delta %3")
                       .arg(value_uV)
                       .arg(measuring_value_uV)
                       .arg(delta));
            if(abs(delta) > max_delta_uV)
            {
                parrot_value += (int)(delta / 20.0f + 0.5f);
            }
            else
            {
                found_value = true;
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
        emit info(QString("Найдено за %1 попыток. Измеренное значение %2 мкВ")
                  .arg(cnt_repeat)
                  .arg(measuring_value_uV));
    }
    return found_value;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_remote_control_on(void)
{
    bool ok = powersupply->rc_on();
    if(!ok)
    {
        emit error(tr("Не удалось включить удаленное управление!"));
        block_interface(false);
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_ADC_U_begin(void)
{
    int cnt_err = 0;
    bool ok = false;
    emit info(tr("Запуск калибровки АЦП по напряжению"));
    while(!ok)
    {
        ok = powersupply->Send_Command_To_Calib_ADC_U();
        if(!ok)
        {
            cnt_err++;
            if(cnt_err > 3)
            {
                cnt_err = 0;
                int btn = QMessageBox::question(this,
                                                tr("Ошибка!"),
                                                tr("Повторить последнюю операцию?"),
                                                QMessageBox::No | QMessageBox::Yes);
                if(btn != QMessageBox::Yes)
                {
                    return false;
                }
            }
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_ADC_U_find_point_1(void)
{
    int cnt_err = 0;
    bool ok = false;
    while(!ok)
    {
#ifndef USER_CALIBRATIONS
        emit info(QString(tr("Поиск точки %1 mV").arg(item_U_point_1->value().toInt())));
        bool ok = find_U(item_U_point_1->value().toInt(),
                         item_U_max_deviation_point_1->value().toInt());
#else
        emit info(QString(tr("Поиск точки %1 mV").arg(B588_U_POINT_1)));
        bool ok = find_U(B588_U_POINT_1,
                         B588_U_MAX_DEVIATION_POINT_1);
#endif
        if(!ok)
        {
            cnt_err++;
            if(cnt_err > 3)
            {
                cnt_err = 0;
                int btn = QMessageBox::question(this,
                                                tr("Ошибка!"),
                                                tr("Повторить последнюю операцию?"),
                                                QMessageBox::No | QMessageBox::Yes);
                if(btn != QMessageBox::Yes)
                {
                    return false;
                }
            }
        }
        else
        {
            emit info("find point_1");
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_ADC_U_find_point_2(void)
{
    int cnt_err = 0;
    bool ok = false;
#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mV").arg(item_U_point_2->value().toInt())));
#else
    emit info(QString(tr("Поиск точки %1 mV").arg(B588_U_POINT_2)));
#endif
    while(!ok)
    {
#ifndef USER_CALIBRATIONS
        ok = find_U(item_U_point_2->value().toInt(),
                    item_U_max_deviation_point_2->value().toInt());
#else
        ok = find_U(B588_U_POINT_2,
                    B588_U_MAX_DEVIATION_POINT_2);
#endif
        if(!ok)
        {
            cnt_err++;
            if(cnt_err > 3)
            {
                cnt_err = 0;
                int btn = QMessageBox::question(this,
                                                tr("Ошибка!"),
                                                tr("Повторить последнюю операцию?"),
                                                QMessageBox::No | QMessageBox::Yes);
                if(btn != QMessageBox::Yes)
                {
                    return false;
                }
            }
        }
        else
        {
            emit info("find point_1");
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_DAC_U_begin(void)
{
    int cnt_err = 0;
    bool ok = false;
    emit info(tr("Запуск калибровки ЦАП по напряжению"));
    while(!ok)
    {
        ok = powersupply->Send_Command_To_Calib_DAC_U();
        if(!ok)
        {
            cnt_err++;
            if(cnt_err > 3)
            {
                cnt_err = 0;
                int btn = QMessageBox::question(this,
                                                tr("Ошибка!"),
                                                tr("Повторить последнюю операцию?"),
                                                QMessageBox::No | QMessageBox::Yes);
                if(btn != QMessageBox::Yes)
                {
                    return false;
                }
            }
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_DAC_U_show_process(void)
{
    bool ok = false;
    char point = 0;

    ui->pb_calibration_DAC_U->setDisabled(false);
    ui->pb_calibration_DAC_U->setRange(0, 119);
    ui->pb_calibration_DAC_U->setValue(0);

    emit info(tr("Калибровка ЦАП напряжения"));
    while(point < 119)
    {
        //emit info(QString("point %1").arg((int)point));
        ok = powersupply->get_current_calibration_point(&point);
        if(!ok)
        {
            powersupply->print_last_error();
            powersupply->set_UI(0, 0);
            powersupply->rc_off();
            block_interface(false);
            return false;
        }
        ui->pb_calibration_DAC_U->setValue(point);
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_DAC_U_check_points(void)
{
    bool ok = false;
    bool check_is_true = true;

    block_interface(true);
    ui->pb_check_U->setDisabled(false);

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    powersupply->rc_on();

    ui->pb_check_U->setMaximum(MAX_VOLTAGE_B588);
    ui->pb_check_U->setValue(0);

    emit info(tr("Проверка контрольных точек напряжения"));
    for(int u=1; u<(MAX_VOLTAGE_B588 / 1000); u++)
    {
        QCoreApplication::processEvents();
        ui->pb_check_U->setValue(u);

        ok = powersupply->set_UI(u * 1000, DEFAULT_I);
        if(!ok)
        {
            powersupply->print_last_error();
            powersupply->set_UI(0, 0);
            powersupply->rc_off();
            block_interface(false);
            return false;
        }
#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

        int value_uV = 0;
        ok = multimeter->get_value(&value_uV);
        if(!ok)
        {
            powersupply->print_last_error();
            powersupply->set_UI(0, 0);
            powersupply->rc_off();
            block_interface(false);
            return false;
        }

        int delta_uV = (u * 1e6) - value_uV;
        int max_delta_uV = (u * 0.001f + 0.005f) * 1e6;

        //---
#if 0
        QList<QAction *> lists_U;
        lists_U.append(new QAction(tr("№"), this));
        lists_U.append(new QAction(tr("Контрольное значение"), this));

        QList<QAction *> lists_I;
        lists_I.append(new QAction(tr("№"), this));
        lists_I.append(new QAction(tr("Контрольное значение"), this));

        QHeaderView *header_U = new QHeaderView(Qt::Horizontal);
        header_U->addActions(lists_U);

        QHeaderView *header_I = new QHeaderView(Qt::Horizontal);
        header_I->addActions(lists_I);

        ui->table_U->setHorizontalHeader(header_U);
        ui->table_I->setHorizontalHeader(header_I);
#endif

        QStringListModel *model = new QStringListModel(this);
        model->setHeaderData(0, Qt::Horizontal, "1");
        model->setHeaderData(1, Qt::Horizontal, "2");
        model->setHeaderData(0, Qt::Horizontal, "3");
        model->setHeaderData(1, Qt::Horizontal, "4");
        model->setHeaderData(0, Qt::Horizontal, "5");
        model->setHeaderData(1, Qt::Horizontal, "6");
        model->setHeaderData(0, Qt::Horizontal, "7");
        model->setHeaderData(1, Qt::Horizontal, "8");
        model->setHeaderData(0, Qt::Horizontal, "9");

        QStringList lists;
        lists.append("1");
        lists.append("2");
        lists.append("3");
        lists.append("4");
        lists.append("5");
        lists.append("6");
        lists.append("7");
        lists.append("8");
        lists.append("9");

        model->setStringList(lists);

        ui->table_U->setModel(model);
        //---

        if(abs(delta_uV) < max_delta_uV)
        {
            emit info(QString("U %1 V delta_uV %2 max_delta_uV %3").arg(u).arg(delta_uV).arg(max_delta_uV));
        }
        else
        {
            emit error(QString("U %1 V delta_uV %2 max_delta_uV %3").arg(u).arg(delta_uV).arg(max_delta_uV));
            check_is_true = false;
        }
    }
    powersupply->set_UI(0, 0);
    powersupply->rc_off();
    return check_is_true;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_ADC_I_begin(void)
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
bool B588::Calibration_ADC_I_find_point_1(void)
{
    bool ok = false;
#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_1->value().toInt()));
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B588_I_POINT_1));
#endif
    while(!ok)
    {
#ifndef USER_CALIBRATIONS
        ok = find_I(item_I_begin_DAC_point_1->value().toInt(),
                    item_I_point_1->value().toInt(),
                    item_I_max_deviation_point_1->value().toInt());
#else
        ok = find_I(B588_I_BEGIN_DAC_POINT_1,
                    B588_I_POINT_1,
                    B588_I_MAX_DEVIATION_POINT_3);
#endif
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
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_ADC_I_find_point_2(void)
{
    bool ok = false;
#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_2->value().toInt()));
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B588_I_POINT_2));
#endif
    while(!ok)
    {
#ifndef USER_CALIBRATIONS
        ok = find_I(item_I_begin_DAC_point_2->value().toInt(),
                    item_I_point_2->value().toInt(),
                    item_I_max_deviation_point_2->value().toInt());
#else
        ok = find_I(B588_I_BEGIN_DAC_POINT_2,
                    B588_I_POINT_2,
                    B588_I_MAX_DEVIATION_POINT_2);
#endif
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
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_ADC_I_find_point_3(void)
{
    bool ok = false;
#ifndef USER_CALIBRATIONS
    emit info(QString(tr("Поиск точки %1 mA")).arg(item_I_point_3->value().toInt()));
#else
    emit info(QString(tr("Поиск точки %1 mA")).arg(B588_I_POINT_3));
#endif
    while(!ok)
    {
#ifndef USER_CALIBRATIONS
        ok = find_I(item_I_begin_DAC_point_3->value().toInt(),
                    item_I_point_3->value().toInt(),
                    item_I_max_deviation_point_3->value().toInt());
#else
        ok = find_I(B588_I_BEGIN_DAC_POINT_3,
                    B588_I_POINT_3,
                    B588_I_MAX_DEVIATION_POINT_3);
#endif
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
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_DAC_I_begin(void)
{
    bool ok = false;
    emit info(tr("Калибровка ЦАП тока"));
    while(!ok)
    {
        ok = powersupply->Send_Command_To_Calib_DAC_I();
        if(!ok)
        {
            int btn = QMessageBox::question(this,
                                            tr("Ошибка!"),
                                            tr("Повторить последнюю операцию?"),
                                            QMessageBox::No | QMessageBox::Yes);
            if(btn != QMessageBox::Yes)
            {
                powersupply->print_last_error();
                return false;
            }
        }
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_DAC_I_show_process(void)
{
    bool ok = false;
    char point = 0;

    ui->pb_calibration_DAC_I->setDisabled(false);
    ui->pb_calibration_DAC_I->setRange(0, 61);
    ui->pb_calibration_DAC_I->setValue(0);

    emit info(tr("Калибровка ЦАП тока"));
    while(point < 61)
    {
        ok = false;
        while(!ok)
        {
            ok = powersupply->get_current_calibration_point(&point);
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
        }
        ui->pb_calibration_DAC_I->setValue(point);
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_DAC_I_check_points(void)
{
    bool ok = false;
    bool check_is_true = true;

    block_interface(true);
    ui->pb_check_I->setDisabled(false);

    powersupply->set_state_silence(true);
    multimeter->set_state_silence(true);

    powersupply->rc_on();

    ui->pb_check_I->setMaximum(30);
    ui->pb_check_I->setValue(0);

    emit info(tr("Проверка контрольных точек тока"));
    for(int i=1; i<=30; i++)
    {
        QCoreApplication::processEvents();
        ui->pb_check_I->setValue(i);

        ok = powersupply->set_UI(DEFAULT_U, i * 1000);
        if(!ok)
        {
            powersupply->print_last_error();
            powersupply->set_UI(0, 0);
            powersupply->rc_off();
            block_interface(false);
            return false;
        }
#ifndef FAKE
        Sleeper::msleep(TIME_READY_POWERSUPPLY_MS);
#endif

        int value_uV = 0;
        ok = multimeter->get_value(&value_uV);
        if(!ok)
        {
            powersupply->print_last_error();
            powersupply->set_UI(0, 0);
            powersupply->rc_off();
            block_interface(false);
            return false;
        }

        double r = ui->sb_shunt->value();
        int delta_uV = (i * r * 1e6) - value_uV;
        int max_delta_uV = (i * r * 0.005f + 0.005f) * 1e6;
        if(abs(delta_uV) < max_delta_uV)
        {
            emit info(QString("I %1 A delta_uV %2 max_delta_uV %3").arg(i).arg(delta_uV).arg(max_delta_uV));
        }
        else
        {
            emit error(QString("I %1 A delta_uV %2 max_delta_uV %3").arg(i).arg(delta_uV).arg(max_delta_uV));
            check_is_true = false;
        }
    }
    powersupply->set_UI(0, 0);
    powersupply->rc_off();
    block_interface(false);
    return check_is_true;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_remote_control_off(void)
{
    bool ok = powersupply->rc_off();
    if(!ok)
    {
        emit error(tr("Не удалось выключить удаленное управление!"));
        block_interface(false);
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588::Calibration_press_enter(void)
{
    bool ok = powersupply->Send_Command_To_Next_Point();
    if(!ok)
    {
        powersupply->set_UI(0, 0);
        powersupply->rc_off();
        block_interface(false);
        return false;
    }

#ifndef FAKE
    Sleeper::sleep(15); //по документации "не менее 10 секунд". Подождем 15...
#endif
    return true;
}
//--------------------------------------------------------------------------------
void B588::createPowerSupplyBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(!mw) return;

    B588_toolbar *psBar = new B588_toolbar(tr("powersupply B5-88"), this);
    psBar->setObjectName("ps_B588");
    mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, psBar);

    connect(psBar, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
}
//--------------------------------------------------------------------------------
void B588::createPropertyBox(void)
{
#ifndef USER_CALIBRATIONS
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();

    QtProperty *top_item_U_point_1 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Напряжение (1 точка)"));
    QtProperty *top_item_U_point_2 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Напряжение (2 точка)"));

    QtProperty *top_item_I_point_1 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (1 точка)"));
    QtProperty *top_item_I_point_2 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (2 точка)"));
    QtProperty *top_item_I_point_3 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Ток (3 точка)"));

    //---
    item_U_point_1 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_U_max_deviation_point_1 = variantManager->addProperty(QVariant::Int, tr("Макс. откл., uV"));
    item_U_begin_DAC_point_1 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_U_point_2 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_U_max_deviation_point_2 = variantManager->addProperty(QVariant::Int, tr("Макс. откл., uV"));
    item_U_begin_DAC_point_2 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_1 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_I_max_deviation_point_1 = variantManager->addProperty(QVariant::Int, tr("Макс. откл., uV"));
    item_I_begin_DAC_point_1 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_2 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_I_max_deviation_point_2 = variantManager->addProperty(QVariant::Int, tr("Макс. откл., uV"));
    item_I_begin_DAC_point_2 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    item_I_point_3 = variantManager->addProperty(QVariant::Int, tr("Напряжение, mV"));
    item_I_max_deviation_point_3 = variantManager->addProperty(QVariant::Int, tr("Макс. откл., uV"));
    item_I_begin_DAC_point_3 = variantManager->addProperty(QVariant::Int, tr("begin DAC"));

    //---
    item_U_point_1->setValue(1000);
    item_U_max_deviation_point_1->setValue(3);
    item_U_begin_DAC_point_1->setValue(1000);

    item_U_point_2->setValue(59000);
    item_U_max_deviation_point_2->setValue(200);
    item_U_begin_DAC_point_2->setValue(59000);
    //---
    item_I_point_1->setValue(50);
    item_I_max_deviation_point_1->setValue(3);
    item_I_begin_DAC_point_1->setValue(1000);

    item_I_point_2->setValue(5000);
    item_I_max_deviation_point_2->setValue(200);
    item_I_begin_DAC_point_2->setValue(5000);

    item_I_point_3->setValue(25000);
    item_I_max_deviation_point_3->setValue(250);
    item_I_begin_DAC_point_3->setValue(25000);
    //---

    top_item_U_point_1->addSubProperty(item_U_point_1);
    top_item_U_point_1->addSubProperty(item_U_max_deviation_point_1);
    top_item_U_point_1->addSubProperty(item_U_begin_DAC_point_1);

    top_item_U_point_2->addSubProperty(item_U_point_2);
    top_item_U_point_2->addSubProperty(item_U_max_deviation_point_2);
    top_item_U_point_2->addSubProperty(item_U_begin_DAC_point_2);

    top_item_I_point_1->addSubProperty(item_I_point_1);
    top_item_I_point_1->addSubProperty(item_I_max_deviation_point_1);
    top_item_I_point_1->addSubProperty(item_I_begin_DAC_point_1);

    top_item_I_point_2->addSubProperty(item_I_point_2);
    top_item_I_point_2->addSubProperty(item_I_max_deviation_point_2);
    top_item_I_point_2->addSubProperty(item_I_begin_DAC_point_2);

    top_item_I_point_3->addSubProperty(item_I_point_3);
    top_item_I_point_3->addSubProperty(item_I_max_deviation_point_3);
    top_item_I_point_3->addSubProperty(item_I_begin_DAC_point_3);

    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser(this);
    variantEditor->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    variantEditor->setFactoryForManager(variantManager, variantFactory);

    QtBrowserItem *item_U_0 = variantEditor->addProperty(top_item_U_point_1);
    QtBrowserItem *item_U_1 = variantEditor->addProperty(top_item_U_point_2);

    QtBrowserItem *item_I_0 = variantEditor->addProperty(top_item_I_point_1);
    QtBrowserItem *item_I_1 = variantEditor->addProperty(top_item_I_point_2);
    QtBrowserItem *item_I_2 = variantEditor->addProperty(top_item_I_point_3);

    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    variantEditor->adjustSize();
    variantEditor->setMinimumSize(variantEditor->sizeHint());

    ui->property_layout->addWidget(variantEditor);

    variantEditor->setExpanded(item_U_0, false);
    variantEditor->setExpanded(item_U_1, false);

    variantEditor->setExpanded(item_I_0, false);
    variantEditor->setExpanded(item_I_1, false);
    variantEditor->setExpanded(item_I_2, false);
#endif
}
//--------------------------------------------------------------------------------
void B588::stop_calibration_ADC_U(void)
{
    flag_stop_calibration_ADC_U = true;
}
//--------------------------------------------------------------------------------
void B588::stop_calibration_DAC_U(void)
{
    flag_stop_calibration_DAC_U = true;
}
//--------------------------------------------------------------------------------
void B588::stop_calibration_ADC_I(void)
{
    flag_stop_calibration_ADC_I = true;
}
//--------------------------------------------------------------------------------
void B588::stop_calibration_DAC_I(void)
{
    flag_stop_calibration_DAC_I = true;
}
//--------------------------------------------------------------------------------
void B588::stop_check_U(void)
{
    flag_stop_check_U = true;
}
//--------------------------------------------------------------------------------
void B588::stop_check_I(void)
{
    flag_stop_check_I = true;
}
//--------------------------------------------------------------------------------
