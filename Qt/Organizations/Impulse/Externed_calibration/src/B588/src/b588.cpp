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
#include "ui_b588.h"
//--------------------------------------------------------------------------------
#include "b588.hpp"

#include "b588_powersupply.hpp"
#include "b588_toolbar.hpp"

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
B588::B588(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B588)
{
    init();
}
//--------------------------------------------------------------------------------
B588::~B588()
{
    qDebug() << "~B588()";

    if(multimeter) multimeter->deleteLater();

    if(powersupply) powersupply->deleteLater();

    if(grapher) grapher->deleteLater();
}
//--------------------------------------------------------------------------------
void B588::init(void)
{
    ui->setupUi(this);

    createTestBar();

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(MIN_VOLTAGE_B588, MAX_VOLTAGE_B588);

    curve_powersuply = grapher->add_curve(tr("Источник"));
    curve_B7642 = grapher->add_curve(tr("В7-64/2"));

    curve_U_parrot = grapher->add_curve(tr("U parrot"));
    curve_I_parrot = grapher->add_curve(tr("I parrot"));
    curve_U_input_parrot = grapher->add_curve(tr("Uin parrot"));

    grapher->setMinimumWidth(640);
    ui->grapher_layout->addWidget(grapher);

    powersupply = new Powersupply_B588();
    connect(powersupply, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),    this, SIGNAL(trace(QString)));

#ifdef ADD_TOOLBAR
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        mw->addToolBarBreak();
        B588_toolbar *psBar_b588 = new B588_toolbar(tr("powersupply B5-90"), this);
        psBar_b588->setObjectName("ps_B588");
        connect(psBar_b588, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
        mw->addToolBar(Qt::TopToolBarArea, psBar_b588);
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

    setWindowIcon(QIcon(ICON_PROGRAMM));
}
//--------------------------------------------------------------------------------
void B588::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(nw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *toolBar = new QToolBar("testbar");
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       tr("test_sinus"),
                                       tr("test_sinus"));

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void B588::find_devices(void)
{
    bool ok = false;

    emit info(tr("Поиск устройств"));

    emit debug("B588");
    powersupply->set_ignore_bad_cmd(ui->cb_ignore_bad_cmd->isChecked());
    ok = powersupply->find_device();
    if(!ok) emit error("Источник питания Б5-90 НЕ найден");

    ok = multimeter->find_device();
#ifdef USE_V764
    if(!ok) emit error("Мультиметр В7-64 НЕ найден");
#endif
#ifdef USE_V7642
    if(!ok) emit error("Мультиметр В7-64/2 НЕ найден");
#endif

    emit info(tr("Поиск устройств закончен"));
}
//--------------------------------------------------------------------------------
void B588::wait_msec(int timeout_msec)
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
void B588::init_multimeter(void)
{
#ifdef USE_V7642
    multimeter->init_modbus();
#endif
}
//--------------------------------------------------------------------------------
void B588::measuring_V7642(void)
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
void B588::measuring_B588(void)
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
void B588::block_interface(bool state)
{

}
//--------------------------------------------------------------------------------
bool B588::is_blocked_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
int B588::xxx(int x)
{
    return x+5;
}
//--------------------------------------------------------------------------------
