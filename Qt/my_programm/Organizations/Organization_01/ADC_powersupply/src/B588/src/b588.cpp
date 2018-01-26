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
#include <QtMath>
#include <QTimer>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_b588.h"
//--------------------------------------------------------------------------------
#include "b588_powersupply.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "b588.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
B588::B588(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B588),
    is_blocked(false)
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

    powersupply = new Powersupply_B588(this);
    connect(powersupply, SIGNAL(info(QString)),    topLevelWidget(), SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),   topLevelWidget(), SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),   topLevelWidget(), SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)),   topLevelWidget(), SIGNAL(trace(QString)));

    grapher = new GrapherBox(this);
    curve_U_adc  = grapher->add_curve("U(adc)");
    curve_I_adc  = grapher->add_curve("I(adc)");
    curve_U3_adc = grapher->add_curve("U3(adc)");
    ui->grapher_layout->addWidget(grapher);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(measuring()));

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
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void B588::test(void)
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
void B588::rc_on(void)
{
    powersupply->set_address(ui->sb_address->value());
    powersupply->rc_on();
}
//--------------------------------------------------------------------------------
void B588::rc_off(void)
{
    powersupply->set_address(ui->sb_address->value());
    powersupply->rc_off();
}
//--------------------------------------------------------------------------------
void B588::find_devices(void)
{
    emit info(tr("Поиск устройств начат"));
    bool ok = false;
    powersupply->set_ignore_bad_cmd(true);
    ok = powersupply->find_device();
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info(tr("Поиск устройств завершен!"));
}
//--------------------------------------------------------------------------------
void B588::auto_measuring(bool state)
{
    if(state)
        timer->start(5000);
    else
        timer->stop();
}
//--------------------------------------------------------------------------------
int B588::measuring(void)
{
    uint32_t U_parrot = 0;
    uint32_t I_parrot = 0;
    uint32_t U_input = 0;
    bool ok = false;

    powersupply->set_address(ui->sb_address->value());
    ok = powersupply->corridor_off();
    if(!ok) return powersupply->print_last_error();
    ok = powersupply->get_adv_ADC(&U_parrot,
                                  &I_parrot,
                                  &U_input);
    if(!ok) return powersupply->print_last_error();

    grapher->add_curve_data(curve_U_adc, U_parrot);
    grapher->add_curve_data(curve_I_adc, I_parrot);

    U_parrot = qAbs(U_parrot - qPow(2, 23));
    I_parrot = qAbs(I_parrot - qPow(2, 23));

    //U_parrot = qAbs(qPow(2, 23) - U_parrot);
    //I_parrot = qAbs(qPow(2, 23) - I_parrot);

    double U_real = (double)U_parrot * ui->sb_ref->value() / (double)qPow(2, 23);
    double I_real = (double)I_parrot * ui->sb_ref->value() / (double)qPow(2, 23);
    emit info(QString("Up %1 Ip %2 U %3 I %4")
              .arg(U_parrot)
              .arg(I_parrot)
              .arg(qAbs(U_real))
              .arg(qAbs(I_real)));
    //emit info(QString("I через шунт %1 A").arg(I_real / ui->sb_shunt->value()));

    return 0;
}
//--------------------------------------------------------------------------------
void B588::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
