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

    if(powersupply) powersupply->deleteLater();

    if(grapher) grapher->deleteLater();
}
//--------------------------------------------------------------------------------
void B590::init(void)
{
    ui->setupUi(this);

    ui->sb_find->setMinimum(0);
    ui->sb_find->setMaximum(0xFFFF);

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(MIN_VOLTAGE_B590, MAX_VOLTAGE_B590);

    curve_B7642_U = grapher->add_curve(tr("В7-64/2 (U)"));
    curve_B7642_I = grapher->add_curve(tr("В7-64/2 (I)"));

    curve_U_parrot = grapher->add_curve(tr("U parrot"));
    curve_I_parrot = grapher->add_curve(tr("I parrot"));

    grapher->setMinimumWidth(640);
    ui->grapher_layout->addWidget(grapher);

    powersupply = new Powersupply_B590();
    connect(powersupply, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),  this, SIGNAL(trace(QString)));

    connect(ui->btn_on,  SIGNAL(released()), this, SLOT(on()));
    connect(ui->btn_off, SIGNAL(released()), this, SLOT(off()));

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

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    connect(ui->btn_find_devices, SIGNAL(clicked()), this, SLOT(find_devices()));

    block_interface(true);
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
void B590::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void B590::on(void)
{
    emit info("on");
}
//--------------------------------------------------------------------------------
void B590::off(void)
{
    emit info("off");
}
//--------------------------------------------------------------------------------
void B590::find_devices(void)
{
    bool ok = false;

    emit info(tr("Поиск устройств"));

    emit debug("B590");
    powersupply->set_ignore_bad_cmd(ui->cb_ignore_bad_cmd->isChecked());
    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok) emit error("Источник питания Б5-90 НЕ найден");

    multimeter->set_state_silence(true);
    ok = multimeter->find_device();
    multimeter->set_state_silence(false);
#ifdef USE_V764
    if(!ok) emit error("Мультиметр В7-64 НЕ найден");
#endif
#ifdef USE_V7642
    if(!ok) emit error("Мультиметр В7-64/2 НЕ найден");
#endif

    emit info(tr("Поиск устройств закончен"));
    block_interface(false);
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
#endif
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
    }
}
//--------------------------------------------------------------------------------
void B590::block_interface(bool state)
{
    ui->sb_find->setDisabled(state);
    ui->btn_on->setDisabled(state);
    ui->btn_off->setDisabled(state);
}
//--------------------------------------------------------------------------------
bool B590::is_blocked_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
int B590::xxx(int x)
{
    return x+5;
}
//--------------------------------------------------------------------------------
void B590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
