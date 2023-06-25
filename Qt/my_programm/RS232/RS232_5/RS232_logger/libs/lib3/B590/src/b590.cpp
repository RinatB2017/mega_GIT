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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_b590.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
#include "b590.hpp"

#include "grapherbox.hpp"

#include "b590_powersupply.hpp"
//#include "b590_toolbar.hpp"

#include "parrotbar.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
enum {
    IN_PARROT = 0x1000,
    IN_NORMAL = 0x2000
};
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
void B590::init(void)
{
    ui->setupUi(this);

    createGrapherBox();
    createTimer();

#ifdef TEST
    createTestBar();
#endif

#ifdef PARROT_BAR
    createParrotBar();
#endif

    createPowersupply();
    createWidgets();

    block_interface(true);
}
//--------------------------------------------------------------------------------
void B590::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    Q_ASSERT(testbar);

    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    //sb_begin = new HexSpinBox();
    sb_begin = new QSpinBox();
    sb_begin->setMinimum(0);
    sb_begin->setMaximum(0xFFFF);

    //sb_end = new HexSpinBox();
    sb_end = new QSpinBox();
    sb_end->setMinimum(0);
    sb_end->setMaximum(0xFFFF);

    sb_begin->setValue(0);
    sb_end->setValue(0xFFFF);

    testbar->addWidget(sb_begin);
    testbar->addWidget(sb_end);

    QToolButton *btn_test_U = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(),
                                         "test U",
                                         "test U");
    QToolButton *btn_test_I = add_button(testbar,
                                         new QToolButton(this),
                                         QIcon(),
                                         "test I",
                                         "test I");
    testbar->addSeparator();

    QToolButton *btn_read = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon(),
                                       "read points",
                                       "read points");
    QToolButton *btn_test_temp = add_button(testbar,
                                            new QToolButton(this),
                                            QIcon(),
                                            "temperature",
                                            "temperature");

    connect(btn_test_U,  SIGNAL(clicked()), this, SLOT(test_U()));
    connect(btn_test_I,  SIGNAL(clicked()), this, SLOT(test_I()));
    connect(btn_read,  SIGNAL(clicked()), this, SLOT(read()));
    connect(btn_test_temp, SIGNAL(clicked()), this, SLOT(show_temp()));
}
//--------------------------------------------------------------------------------
void B590::createParrotBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    ParrotBar *parrotBar = new ParrotBar(this);
    Q_ASSERT(parrotBar);
    parrotBar->setObjectName("parrotBar");

    connect(parrotBar, SIGNAL(set_UI(int,int)), this, SLOT(set_UI_parrot(int,int)));

    mw->addToolBar(Qt::TopToolBarArea, parrotBar);
}
//--------------------------------------------------------------------------------
int B590::set_UI_parrot(int U, int I)
{
    bool ok = false;
    powersupply->set_address(ui->sb_address->value());
    //powersupply->set_state_silence(true);
    ok = powersupply->set_UI_parrot(static_cast<uint16_t>(U),
                                    static_cast<uint16_t>(I));
    //powersupply->set_state_silence(false);
    if(!ok)
    {
        return powersupply->print_last_error();
    }
    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::show_temp(void)
{
    bool ok = false;
    unsigned char temp = 0;
    bool flag_zummer = false;

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_state_silence(true);
    ok = powersupply->get_temperature(&temp, &flag_zummer);
    powersupply->set_state_silence(false);
    if(!ok) return powersupply->print_last_error();
    emit info(QString("temperature %1").arg(temp));
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::send_0_0(void)
{
    bool ok = false;

    ok = powersupply->set_UI(0, 0);
    if(!ok)
    {
        return powersupply->print_last_error();
    }

    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::set_vent_speed(void)
{
    bool ok = false;

    ok = powersupply->set_vent_speed(static_cast<uint16_t>(ui->sl_vent_speed->value()));
    if(!ok) return powersupply->print_last_error();

    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::set_vent_speed_0(void)
{
    bool ok = false;

    ok = powersupply->set_vent_speed(0);
    if(!ok) return powersupply->print_last_error();

    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::set_vent_speed_max(void)
{
    bool ok = false;

    ok = powersupply->set_vent_speed(1600);
    if(!ok) return powersupply->print_last_error();

    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::set_vent_speed_auto(void)
{
    bool ok = false;

    ok = powersupply->set_vent_speed(1700);
    if(!ok) return powersupply->print_last_error();

    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B590::test_U(void)
{
    emit info("Test_U begin...");

    if(sb_begin->value() >= sb_end->value())
    {
        emit error("Incorrect data");
        return -1;
    }

    bool ok = false;

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_state_silence(true);
    ok = powersupply->rc_on();
    if(!ok)
    {
        powersupply->print_last_error();
        return -1;
    }

    //---
    int curve_V1 = grapher->add_curve("U: V1");
    int curve_new_setting_U = grapher->add_curve("U: new_setting_U");
    emit info("draw graphes");
    emit info(QString("MAGIC_B590_U = %1").arg(MAGIC_B590_U));
    unsigned long new_setting_U = 0;
    signed long V1 = 0;
    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", sb_begin->value(), sb_end->value(), this);
    pd->setWindowModality(Qt::WindowModal);
    for(new_setting_U = static_cast<unsigned long>(sb_begin->value()); new_setting_U < static_cast<unsigned long>(sb_end->value()); new_setting_U++)
    {
        pd->setValue(static_cast<int>(new_setting_U));
        if(pd->wasCanceled())
        {
            break;
        }

        QCoreApplication::processEvents();
        V1 = powersupply->get_Correct_value_U(static_cast<unsigned int>(new_setting_U));
        grapher->add_curve_data(curve_V1, V1);
        grapher->add_curve_data(curve_new_setting_U, new_setting_U);
    }
    pd->close();
    //---

    powersupply->rc_off();
    powersupply->set_state_silence(false);

    emit info("Test end!");
    return 0;
}
//--------------------------------------------------------------------------------
int B590::test_I(void)
{
    emit info("Test begin...");

    if(sb_begin->value() >= sb_end->value())
    {
        emit error("Incorrect data");
        return -1;
    }

    bool ok = false;

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_state_silence(true);
    ok = powersupply->rc_on();
    if(!ok)
    {
        powersupply->print_last_error();
        return -1;
    }

    //---
    int curve_V1 = grapher->add_curve("I: V1");
    int curve_new_setting_I = grapher->add_curve("I: new_setting_I");
    emit info("draw graphes");
    emit info(QString("MAGIC_B590_I = %1").arg(MAGIC_B590_I));
    unsigned long new_setting_I = 0;
    signed long V1 = 0;
    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", sb_begin->value(), sb_end->value(), this);
    pd->setWindowModality(Qt::WindowModal);
    for(new_setting_I = static_cast<unsigned long>(sb_begin->value()); new_setting_I < static_cast<unsigned long>(sb_end->value()); new_setting_I++)
    {
        pd->setValue(static_cast<int>(new_setting_I));
        if(pd->wasCanceled())
        {
            break;
        }

        QCoreApplication::processEvents();
        V1 = powersupply->get_Correct_value_I(new_setting_I);
        grapher->add_curve_data(curve_V1, V1);
        grapher->add_curve_data(curve_new_setting_I, new_setting_I);
    }
    pd->close();

    powersupply->rc_off();
    powersupply->set_state_silence(false);

    emit info("Test end!");
    return 0;
}
//--------------------------------------------------------------------------------
void B590::read_calibration_point_B590(void)
{
    bool ok = false;

    unsigned short value = 0;
    unsigned short old_value = 0;

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_state_silence(true);
    powersupply->rc_on();
    old_value = 0;
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        ok = powersupply->get_calibration_point_U(static_cast<uint8_t>(n), &value);
        if(ok)
        {
            emit info(QString("dac_U[%1]=%2;")
                      .arg(n)
                      .arg(value));
            grapher->add_curve_data(static_cast<uint16_t>(curve_U), value);
            //???
            if(n)
            {
                int delta = abs(value - old_value);
                old_value = value;
                grapher->add_curve_data(static_cast<uint16_t>(curve_W), delta);
            }
        }
        else
        {
            powersupply->print_last_error();
        }
    }

    emit info("---");
    old_value = 0;
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        ok = powersupply->get_calibration_point_I(static_cast<uint8_t>(n),
                                                  &value);
        if(ok)
        {
            emit info(QString("dac_I[%1]=%2;")
                      .arg(n)
                      .arg(value));
            grapher->add_curve_data(static_cast<uint16_t>(curve_I),
                                    value);
            //???
            if(n)
            {
                int delta = abs(value - old_value);
                old_value = value;
                grapher->add_curve_data(static_cast<uint16_t>(curve_T),
                                        delta);
            }
        }
        else
        {
            powersupply->print_last_error();
        }
    }
    powersupply->rc_off();
    powersupply->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void B590::read(void)
{
    read_calibration_point_B590();
}
//--------------------------------------------------------------------------------
void B590::createWidgets(void)
{
    ui->btn_search_power_supply->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    ui->cb_state_powersupply->clear();
    ui->cb_state_powersupply->addItem(tr("parrot"), QVariant(Qt::UserRole + IN_PARROT));
    ui->cb_state_powersupply->addItem(tr("normal"), QVariant(Qt::UserRole + IN_NORMAL));
    connect(ui->cb_state_powersupply, SIGNAL(currentIndexChanged(int)), this, SLOT(change_state_powersupply(int)));
    ui->cb_state_powersupply->setCurrentIndex(1);

    ui->sb_interval->setRange(1000, 0xFFFF);
    ui->sb_test_U->setRange(0, 0xFFFF);
    ui->sb_test_I->setRange(0, 0xFFFF);
    ui->dsb_begin_temp->setRange(20, 75);
    ui->dsb_hot_temp->setRange(20, 75);

    ui->knob_U->setScaleMaxMinor(10);
    ui->knob_U->setScaleMaxMajor(10);

    ui->knob_U->setLowerBound(MIN_VOLTAGE_B590 / 1000);
    ui->knob_U->setUpperBound(MAX_VOLTAGE_B590 / 1000);

    ui->knob_I->setScaleMaxMinor(10);
    ui->knob_I->setScaleMaxMajor(10);

    ui->knob_I->setLowerBound(MIN_CURRENT_B590 / 1000);
    ui->knob_I->setUpperBound(MAX_CURRENT_B590 / 1000);

    ui->sb_address->setMinimum(MIN_ADDRESS_B590);
    ui->sb_address->setMaximum(MAX_ADDRESS_B590);

    ui->sb_max_current->setMinimum(MIN_CURRENT_B590);
    ui->sb_max_current->setMaximum(MAX_CURRENT_B590);

    ui->sb_first_U->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_first_U->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_first_I->setMinimum(MIN_CURRENT_B590);
    ui->sb_first_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_second_U->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_second_U->setMaximum(MAX_VOLTAGE_B590);

    ui->sb_second_I->setMinimum(MIN_CURRENT_B590);
    ui->sb_second_I->setMaximum(MAX_CURRENT_B590);

    ui->sb_delay_msec->setMinimum(1);
    ui->sb_delay_msec->setMaximum(0xFFFF);

    ui->cb_U->clear();
    ui->cb_U->addItem("10000", Qt::UserRole + 10000);
    ui->cb_U->addItem("1000",  Qt::UserRole + 1000);
    ui->cb_U->addItem("900",   Qt::UserRole + 900);
    ui->cb_U->addItem("800",   Qt::UserRole + 800);
    ui->cb_U->addItem("700",   Qt::UserRole + 700);
    ui->cb_U->addItem("600",   Qt::UserRole + 600);
    ui->cb_U->addItem("500",   Qt::UserRole + 500);
    ui->cb_U->addItem("400",   Qt::UserRole + 400);
    ui->cb_U->addItem("300",   Qt::UserRole + 300);
    ui->cb_U->addItem("200",   Qt::UserRole + 200);
    ui->cb_U->addItem("100",   Qt::UserRole + 100);
    ui->cb_U->addItem("10",    Qt::UserRole + 10);
    ui->cb_U->addItem("1",     Qt::UserRole + 1);

    ui->cb_I->clear();
    ui->cb_I->addItem("10000", Qt::UserRole + 10000);
    ui->cb_I->addItem("1000",  Qt::UserRole + 1000);
    ui->cb_I->addItem("900",   Qt::UserRole + 900);
    ui->cb_I->addItem("800",   Qt::UserRole + 800);
    ui->cb_I->addItem("700",   Qt::UserRole + 700);
    ui->cb_I->addItem("600",   Qt::UserRole + 600);
    ui->cb_I->addItem("500",   Qt::UserRole + 500);
    ui->cb_I->addItem("400",   Qt::UserRole + 400);
    ui->cb_I->addItem("300",   Qt::UserRole + 300);
    ui->cb_I->addItem("200",   Qt::UserRole + 200);
    ui->cb_I->addItem("100",   Qt::UserRole + 100);
    ui->cb_I->addItem("10",    Qt::UserRole + 10);
    ui->cb_I->addItem("1",     Qt::UserRole + 1);

    ui->sl_vent_speed->setMinimum(0);
    ui->sl_vent_speed->setMaximum(1600);

    connect(ui->btn_vent_speed,      SIGNAL(clicked()), this, SLOT(set_vent_speed()));
    connect(ui->btn_vent_speed_0,    SIGNAL(clicked()), this, SLOT(set_vent_speed_0()));
    connect(ui->btn_vent_speed_MAX,  SIGNAL(clicked()), this, SLOT(set_vent_speed_max()));
    connect(ui->btn_vent_speed_AUTO, SIGNAL(clicked()), this, SLOT(set_vent_speed_auto()));

    connect(ui->btn_search_power_supply, SIGNAL(clicked()), this, SLOT(search_power_supply()));
    connect(ui->knob_U, SIGNAL(valueChanged(double)), ui->lcd_U, SLOT(display(double)));
    connect(ui->knob_I, SIGNAL(valueChanged(double)), ui->lcd_I, SLOT(display(double)));
    connect(ui->btn_set_UI, SIGNAL(clicked()), this, SLOT(set_UI()));

    connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(timer_start()));
    connect(ui->btn_stop,  SIGNAL(clicked()), this, SLOT(timer_stop()));

    connect(ui->btn_rc_on,  SIGNAL(clicked()), this, SLOT(rc_on()));
    connect(ui->btn_rc_off, SIGNAL(clicked()), this, SLOT(rc_off()));

    connect(ui->btn_inc_U, SIGNAL(clicked()), this, SLOT(inc_U()));
    connect(ui->btn_inc_I, SIGNAL(clicked()), this, SLOT(inc_I()));
    connect(ui->btn_dec_U, SIGNAL(clicked()), this, SLOT(dec_U()));
    connect(ui->btn_dec_I, SIGNAL(clicked()), this, SLOT(dec_I()));

    connect(ui->btn_find_start, SIGNAL(clicked()), this, SLOT(find_max_power_start()));
    connect(ui->btn_find_stop,  SIGNAL(clicked()), this, SLOT(find_max_power_stop()));
    connect(ui->btn_test_start, SIGNAL(clicked()), this, SLOT(test_start()));
    connect(ui->btn_test_stop,  SIGNAL(clicked()), this, SLOT(test_stop()));

    connect(ui->btn_get_UI, SIGNAL(clicked()), this, SLOT(get_UI()));

    connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(send_0_0()));

    connect(ui->btn_start_testing,  &QPushButton::clicked,
            this,                   &B590::start_testing);
    connect(ui->btn_stop_testing,   &QPushButton::clicked,
            this,                   &B590::stop_testing);

    ui->btn_close_connect->setDisabled(true);

#ifndef QT_DEBUG
    ui->frame_finder_max_power->setVisible(false);
    ui->frame_testing->setVisible(false);
    ui->frame_vent->setVisible(false);
    ui->gb_testing->setVisible(false);
    ui->label_RC->setVisible(false);
    ui->btn_rc_on->setVisible(false);
    ui->btn_rc_off->setVisible(false);

    ui->cb_state_powersupply->setVisible(false);
#endif
}
//--------------------------------------------------------------------------------
void B590::createTimer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(get_UI()));
}
//--------------------------------------------------------------------------------
void B590::createPowersupply(void)
{
    powersupply = new Powersupply_B590(this);

    connect(ui->btn_close_connect, &QPushButton::clicked, this, &B590::close_power_supply);
    connect(ui->sb_address, SIGNAL(valueChanged(int)),  powersupply,   SLOT(set_address(int)));

#ifdef ADD_TOOLBAR
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        mw->addToolBarBreak();
        B590_toolbar *psBar_b590 = new B590_toolbar(tr("powersupply B5-90"), this);
        psBar_b590->setObjectName("ps_B590");
        connect(psBar_b590, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
        mw->addToolBar(Qt::TopToolBarArea, psBar_b590);
    }
#endif
}
//--------------------------------------------------------------------------------
void B590::createGrapherBox(void)
{
    QwtSymbol *symbol_U = new QwtSymbol(QwtSymbol::Ellipse,
                                        QBrush(Qt::darkRed),
                                        QPen(Qt::red, 2),
                                        QSize(8, 8));
    QwtSymbol *symbol_I = new QwtSymbol(QwtSymbol::Ellipse,
                                        QBrush(Qt::darkGreen),
                                        QPen(Qt::green, 2),
                                        QSize(8, 8));
    QwtSymbol *symbol_R = new QwtSymbol(QwtSymbol::Ellipse,
                                        QBrush(Qt::darkBlue),
                                        QPen(Qt::blue, 2),
                                        QSize(8, 8));
    QwtSymbol *symbol_W = new QwtSymbol(QwtSymbol::Ellipse,
                                        QBrush(Qt::darkCyan),
                                        QPen(Qt::cyan, 2),
                                        QSize(8, 8));
    QwtSymbol *symbol_T = new QwtSymbol(QwtSymbol::Ellipse,
                                        QBrush(Qt::darkMagenta),
                                        QPen(Qt::magenta, 2),
                                        QSize(8, 8));

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(0, 1000);
    grapher->set_axis_scale_y(0, 0xFFFF);

    curve_U = grapher->add_curve("U", symbol_U);
    curve_I = grapher->add_curve("I", symbol_I);
    curve_R = grapher->add_curve("R", symbol_R);
    curve_W = grapher->add_curve("W", symbol_W);
    curve_T = grapher->add_curve("T", symbol_T);

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Autoscroll(false);
    grapher->set_visible_btn_Statistic(false);

    ui->grapher_layout->addWidget(grapher);
}
//--------------------------------------------------------------------------------
void B590::timer_start(void)
{
    timer->start(ui->sb_interval->value());
}
//--------------------------------------------------------------------------------
void B590::timer_stop(void)
{
    timer->stop();
}
//--------------------------------------------------------------------------------
void B590::change_state_powersupply(int)
{
    int state_powersupply = ui->cb_state_powersupply->itemData(ui->cb_state_powersupply->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    switch(state_powersupply)
    {
    case IN_NORMAL:
        ui->knob_U->setLowerBound(MIN_VOLTAGE_B590 / 1000);
        ui->knob_U->setUpperBound(MAX_VOLTAGE_B590 / 1000);

        ui->knob_I->setLowerBound(MIN_CURRENT_B590 / 1000);
        ui->knob_I->setUpperBound(MAX_CURRENT_B590 / 1000);

        ui->sb_max_current->setMaximum(MAX_CURRENT_B590);

        ui->sb_first_U->setMinimum(MIN_VOLTAGE_B590);
        ui->sb_first_U->setMaximum(MAX_VOLTAGE_B590);

        ui->sb_first_I->setMinimum(MIN_CURRENT_B590);
        ui->sb_first_I->setMaximum(MAX_CURRENT_B590);

        ui->sb_second_U->setMinimum(MIN_VOLTAGE_B590);
        ui->sb_second_U->setMaximum(MAX_VOLTAGE_B590);

        ui->sb_second_U->setMinimum(MIN_CURRENT_B590);
        ui->sb_second_I->setMaximum(MAX_CURRENT_B590);
        break;

    case IN_PARROT:
        ui->knob_U->setLowerBound(0);
        ui->knob_U->setUpperBound(MAX_VOLTAGE_B590);

        ui->knob_I->setLowerBound(0);
        ui->knob_I->setUpperBound(MAX_CURRENT_B590);

        ui->sb_max_current->setMaximum(0xFFFF);

        ui->sb_first_U->setMinimum(0);
        ui->sb_first_U->setMaximum(MAX_VOLTAGE_B590);

        ui->sb_first_I->setMinimum(0);
        ui->sb_first_I->setMaximum(MAX_CURRENT_B590);

        ui->sb_second_U->setMinimum(0);
        ui->sb_second_U->setMaximum(MAX_VOLTAGE_B590);

        ui->sb_second_I->setMinimum(0);
        ui->sb_second_I->setMaximum(MAX_CURRENT_B590);
        break;
    }
}
//--------------------------------------------------------------------------------
void B590::set_UI(void)
{
    int state_powersupply = ui->cb_state_powersupply->itemData(ui->cb_state_powersupply->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    bool ok = false;

    switch(state_powersupply)
    {
    case IN_PARROT:
        ok = powersupply->set_UI_parrot(static_cast<uint16_t>(ui->knob_U->value()),
                                        static_cast<uint16_t>(ui->knob_I->value()));
        break;
    case IN_NORMAL:
        ok = powersupply->set_UI(static_cast<int32_t>(ui->knob_U->value() * 1000.0),
                                 static_cast<uint16_t>(ui->knob_I->value() * 1000.0));
        break;
    }
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::inc_U(void)
{
    bool ok;
    int state_powersupply = ui->cb_state_powersupply->itemData(ui->cb_state_powersupply->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    int U = ui->cb_U->itemData(ui->cb_U->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(ok)
    {
        switch(state_powersupply)
        {
        case IN_PARROT:
            ui->knob_U->setValue(ui->knob_U->value()+U);
            break;
        case IN_NORMAL:
            ui->knob_U->setValue(ui->knob_U->value()+(U / 1000.0));
            break;
        default:
            emit error(QString("unknown state_powersupply %1").arg(state_powersupply));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void B590::inc_I(void)
{
    bool ok;
    int state_powersupply = ui->cb_state_powersupply->itemData(ui->cb_state_powersupply->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    int I = ui->cb_I->itemData(ui->cb_I->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(ok)
    {
        switch(state_powersupply)
        {
        case IN_PARROT:
            ui->knob_I->setValue(ui->knob_I->value()+I);
            break;
        case IN_NORMAL:
            ui->knob_I->setValue(ui->knob_I->value()+(I / 1000.0));
            break;
        default:
            emit error(QString("unknown state_powersupply %1").arg(state_powersupply));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void B590::dec_U(void)
{
    bool ok;
    int state_powersupply = ui->cb_state_powersupply->itemData(ui->cb_state_powersupply->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    int U = ui->cb_U->itemData(ui->cb_U->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(ok)
    {
        switch(state_powersupply)
        {
        case IN_PARROT:
            ui->knob_U->setValue(ui->knob_U->value()-U);
            break;
        case IN_NORMAL:
            ui->knob_U->setValue(ui->knob_U->value()-(U / 1000.0));
            break;
        default:
            emit error(QString("unknown state_powersupply %1").arg(state_powersupply));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void B590::dec_I(void)
{
    bool ok;
    int state_powersupply = ui->cb_state_powersupply->itemData(ui->cb_state_powersupply->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    int I = ui->cb_I->itemData(ui->cb_I->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(ok)
    {
        switch(state_powersupply)
        {
        case IN_PARROT:
            ui->knob_I->setValue(ui->knob_I->value()-I);
            break;
        case IN_NORMAL:
            ui->knob_I->setValue(ui->knob_I->value()-(I / 1000.0));
            break;
        default:
            emit error(QString("unknown state_powersupply %1").arg(state_powersupply));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void B590::find_max_power(void)
{
    int32_t value = MIN_VOLTAGE_B590;
    unsigned short increment = (MAX_VOLTAGE_B590 / 2);

    QCoreApplication::processEvents();

    int cnt = 0;
    flag_find_stop = false;
    while(!flag_find_stop)
    {
        QCoreApplication::processEvents();

        //---
        cnt++;
        if(cnt > 1e6)
        {
            flag_find_stop = true;
        }
        //---

        powersupply->set_address(ui->sb_address->value());
        powersupply->set_UI(value,
                            static_cast<uint16_t>(ui->sb_max_current->value()));
        switch(powersupply->get_last_error())
        {
        case E_B590_HI_VOLTAGE:
        case E_B590_MAX_POWER:
            value -= increment;
            increment = increment / 2;
            if(increment < 1) flag_find_stop = true;
            break;

        case E_B590_NO_ERROR:
            emit info(QString("U %1 mV").arg(value));
            value += increment;
            break;

        default:
            powersupply->print_last_error();
            flag_find_stop = true;
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void B590::extreme(void)
{
    bool ok = false;
    if(!rc_on()) return;

    flag_test_stop = false;
    powersupply->set_address(ui->sb_address->value());
    while(!flag_test_stop)
    {
        QCoreApplication::processEvents();
        ok = powersupply->set_UI(ui->sb_first_U->value(),
                                 static_cast<uint16_t>(ui->sb_first_I->value()));
        if(!ok)
        {
            powersupply->print_last_error();
            flag_test_stop = true;
        }
        if(ui->sb_delay_msec->value()>1)
            Sleeper::msleep(static_cast<unsigned long>(ui->sb_delay_msec->value()));
        get_UI();
        ok = powersupply->set_UI(ui->sb_second_U->value(),
                                 static_cast<uint16_t>(ui->sb_second_I->value()));
        if(!ok)
        {
            powersupply->print_last_error();
            flag_test_stop = true;
        }
        if(ui->sb_delay_msec->value()>1)
            Sleeper::msleep(static_cast<unsigned long>(ui->sb_delay_msec->value()));
        get_UI();
    }

    if(!rc_off()) return;
}
//--------------------------------------------------------------------------------
void B590::find_max_power_start(void)
{
    emit info("Find_max_power begin");
    find_max_power();
    emit info("Find_max_power end");
}
//--------------------------------------------------------------------------------
void B590::find_max_power_stop(void)
{
    flag_find_stop = true;
}
//--------------------------------------------------------------------------------
void B590::test_start(void)
{
    emit info("Test begin");
    extreme();
    emit info("Test end");
}
//--------------------------------------------------------------------------------
void B590::test_stop(void)
{
    flag_test_stop = true;
}
//--------------------------------------------------------------------------------
void B590::block_interface(bool state)
{
    ui->btn_0->setDisabled(state);
    ui->btn_rc_on->setDisabled(state);
    ui->btn_rc_off->setDisabled(state);
    ui->btn_inc_U->setDisabled(state);
    ui->btn_inc_I->setDisabled(state);
    ui->btn_dec_U->setDisabled(state);
    ui->btn_dec_I->setDisabled(state);
    ui->btn_find_start->setDisabled(state);
    ui->btn_find_stop->setDisabled(state);
    ui->btn_set_UI->setDisabled(state);
    ui->btn_get_UI->setDisabled(state);
    ui->btn_start->setDisabled(state);
    ui->btn_stop->setDisabled(state);
    ui->btn_test_start->setDisabled(state);
    ui->btn_test_stop->setDisabled(state);

    ui->btn_vent_speed->setDisabled(state);
    ui->btn_vent_speed_0->setDisabled(state);
    ui->btn_vent_speed_MAX->setDisabled(state);
    ui->btn_vent_speed_AUTO->setDisabled(state);

    ui->sb_max_current->setDisabled(state);
    ui->sb_delay_msec->setDisabled(state);
    ui->sb_first_U->setDisabled(state);
    ui->sb_first_I->setDisabled(state);
    ui->sb_second_U->setDisabled(state);
    ui->sb_second_I->setDisabled(state);
    ui->sb_interval->setDisabled(state);

    ui->knob_U->setDisabled(state);
    ui->knob_I->setDisabled(state);

    ui->cb_state_powersupply->setDisabled(state);
    ui->cb_U->setDisabled(state);
    ui->cb_I->setDisabled(state);

    ui->sl_vent_speed->setDisabled(state);

    ui->lcd_U->setDisabled(state);
    ui->lcd_I->setDisabled(state);
    ui->lcd_U_in->setDisabled(state);
    ui->lcd_I_in->setDisabled(state);
    ui->lcd_R_in->setDisabled(state);
    ui->lcd_W_in->setDisabled(state);
    ui->lcd_temp->setDisabled(state);

    ui->dsb_begin_temp->setDisabled(state);
    ui->dsb_hot_temp->setDisabled(state);
    ui->sb_test_U->setDisabled(state);
    ui->sb_test_I->setDisabled(state);
    ui->btn_start_testing->setDisabled(state);
    ui->btn_stop_testing->setDisabled(state);

    update();
}
//--------------------------------------------------------------------------------
bool B590::rc_on(void)
{
    bool ok = false;

    powersupply->set_address(ui->sb_address->value());
    ok = powersupply->rc_on();
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool B590::rc_off(void)
{
    bool ok = false;

    powersupply->set_address(ui->sb_address->value());
    ok = powersupply->rc_off();
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool B590::search_power_supply(void)
{
    emit trace("search_power_supply");
    bool ok = false;

    emit info(tr("Поиск устройств"));

    powersupply->set_state_silence(true);
    lock_this_button();
    ok = powersupply->find_device();
    unlock_this_button();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-90 НЕ найден!");
        return false;
    }

    emit info("Поиск устройств закончен");
    block_interface(false);
    ui->btn_search_power_supply->setDisabled(true);
    ui->btn_close_connect->setEnabled(true);
    return true;
}
//--------------------------------------------------------------------------------
void B590::close_power_supply(void)
{
    powersupply->close_connect();

    ui->btn_search_power_supply->setEnabled(true);
    ui->btn_close_connect->setDisabled(true);
}
//--------------------------------------------------------------------------------
void B590::get_UI(void)
{
    int32_t  in_voltage = 0;
    uint16_t in_current = 0;
    int32_t  out_voltage = 0;
    uint16_t out_current = 0;
    int8_t   temp = 0;
    uint8_t  current_point = 0;
    uint8_t  active_profile = 0;

    if(powersupply->port_is_open() == false)
    {
        emit error("Измерения остановлены");
        timer->stop();
        return;
    }

    bool ok = false;
    powersupply->set_state_silence(true);
    powersupply->set_address(ui->sb_address->value());
    ok = powersupply->get_UI(&in_voltage,
                             &in_current,
                             &out_voltage,
                             &out_current,
                             &temp,
                             &current_point,
                             &active_profile);
    powersupply->set_state_silence(false);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    double u = static_cast<double>(out_voltage) / 1000.0;
    double i = static_cast<double>(out_current / 1000.0);
    if(i < 0.000001) i = 0.000001;
    double r = u / i;
    double w = u * i;

    if(u > ui->knob_U->value()) ui->lcd_U_in->setStyleSheet("color: red");
    else ui->lcd_U_in->setStyleSheet("");
    if(i > ui->knob_I->value()) ui->lcd_I_in->setStyleSheet("color: red");
    else ui->lcd_I_in->setStyleSheet("");

    ui->lcd_U_in->display(u);
    ui->lcd_I_in->display(i);
    ui->lcd_R_in->display(r);
    ui->lcd_W_in->display(w);
    ui->lcd_temp->display(temp);

    //emit info(QString("R %1 W %2").arg(r).arg(w));
    grapher->add_curve_data(curve_U, out_voltage);
    grapher->add_curve_data(curve_I, out_current);
    grapher->add_curve_data(curve_R, r);
    grapher->add_curve_data(curve_W, w);
    grapher->add_curve_data(curve_T, temp);
}
//--------------------------------------------------------------------------------
bool B590::check_current_temp(void)
{
    uint8_t current_temp = 0;
    uint8_t begin_temp = ui->dsb_begin_temp->value();
    bool flag_zummer;
    bool ok;

    emit info("Проверяем температуру");
    ok = powersupply->get_temperature(&current_temp, &flag_zummer);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }

    if(current_temp < begin_temp)
    {
        emit error("Неправильно заданная температура");
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590::wait_needed_temp(void)
{
    uint8_t current_temp = 0;
    uint8_t begin_temp = ui->dsb_begin_temp->value();
    bool flag_zummer;
    bool ok;

    emit info("Ждём заданную температуру (греемся 50A)");
    ok = powersupply->set_UI(DEFAULT_U, 50000);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }

    ok = powersupply->get_temperature(&current_temp, &flag_zummer);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    while(current_temp < begin_temp)
    {
        ok = powersupply->get_temperature(&current_temp, &flag_zummer);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
bool B590::set_testing_ui(void)
{
    bool ok;
    emit info("Выыставляем нужное напряжение и ток");
    ok = powersupply->set_UI(ui->sb_test_U->value(),
                             ui->sb_test_I->value());
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool B590::measuring(void)
{
    messagebox_critical("Внимание", "Обрати внимание на показание мультиметра");
    return true;
}
//--------------------------------------------------------------------------------
bool B590::wait_hot_temp(void)
{
    uint8_t current_temp = 0;
    uint8_t hot_temp = ui->dsb_begin_temp->value();
    bool flag_zummer;
    bool ok;

    while(current_temp < hot_temp)
    {
        ok = powersupply->get_temperature(&current_temp, &flag_zummer);
        if(!ok)
        {
            powersupply->print_last_error();
            return false;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void B590::testing(void)
{
    bool ok;

    emit info("Тестирование начато");

    ok = check_current_temp();  if(!ok) return;
    ok = wait_needed_temp();    if(!ok) return;
    ok = set_testing_ui();      if(!ok) return;
    ok = measuring();           if(!ok) return;
    ok = wait_hot_temp();       if(!ok) return;
    ok = set_testing_ui();      if(!ok) return;
    ok = measuring();           if(!ok) return;
    ok = wait_needed_temp();    if(!ok) return;
    ok = measuring();           if(!ok) return;

    emit info("Тестирование завершено");
}
//--------------------------------------------------------------------------------
void B590::start_testing(void)
{
    testing();
}
//--------------------------------------------------------------------------------
void B590::stop_testing(void)
{
    f_testing_running = false;
}
//--------------------------------------------------------------------------------
void B590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool B590::programm_is_exit(void)
{
    if(powersupply->port_is_open())
    {
        messagebox_critical("Ошибка", "Сначала закройте порт");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void B590::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void B590::save_setting(void)
{

}
//--------------------------------------------------------------------------------
