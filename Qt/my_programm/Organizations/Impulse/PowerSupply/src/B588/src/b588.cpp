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
#include <QStandardItemModel>
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QDebug>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_b588.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
#include "sleeper.h"
#include "b588.hpp"

#include "grapherbox.hpp"

#include "parrotbar.hpp"
#include "b588_powersupply.hpp"
#include "b588_toolbar.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void B588::init(void)
{
    ui->setupUi(this);

    createTestBar();
    //createParrotBar();
    createPowersupply();
    createWidgets();

    block_interface(true);
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
                                       QIcon(),
                                       "test",
                                       "test");
    
    connect(btn_test,  SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void B588::createParrotBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    ParrotBar *parrotBar = new ParrotBar(this);
    Q_CHECK_PTR(parrotBar);
    connect(parrotBar, SIGNAL(set_UI(int,int)), this, SLOT(set_UI_parrot(int,int)));

    mw->addToolBar(Qt::TopToolBarArea, parrotBar);
}
//--------------------------------------------------------------------------------
int B588::show_temp(void)
{
    bool ok = false;
    unsigned char temp = 0;

    powersupply->set_address(ui->sb_address->value());
    powersupply->set_state_silence(true);
    ok = powersupply->get_temperature(&temp);
    powersupply->set_state_silence(false);
    if(!ok) return powersupply->print_last_error();
    emit info(QString("temperature %1").arg(temp));

    return E_B588_NO_ERROR;
}
//--------------------------------------------------------------------------------
int B588::send_0_0(void)
{
    bool ok = false;

    ok = powersupply->set_UI(0, 0);
    if(!ok) return powersupply->print_last_error();

    emit info("OK");
    return E_B588_NO_ERROR;
}
//--------------------------------------------------------------------------------
void B588::createWidgets(void)
{
    ui->bt_search_power_supply->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->knob_U->setScaleMaxMinor(10);
    ui->knob_U->setScaleMaxMajor(10);
    ui->knob_U->setUpperBound(MAX_VOLTAGE_B588 / 1000);

    ui->knob_I->setScaleMaxMinor(10);
    ui->knob_I->setScaleMaxMajor(10);
    ui->knob_I->setUpperBound(MAX_CURRENT_B588 / 1000);

    ui->sb_address->setMinimum(MIN_ADDRESS_B588);
    ui->sb_address->setMaximum(MAX_ADDRESS_B588);

    ui->cb_U->clear();
    ui->cb_U->addItem("10000", Qt::UserRole + 10000);
    ui->cb_U->addItem("1000",  Qt::UserRole + 1000);
    ui->cb_U->addItem("880",   Qt::UserRole + 880);
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
    ui->cb_I->addItem("880",   Qt::UserRole + 880);
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

    //---
    model_profiles = new QStandardItemModel(0, 3, this);
    model_profiles->setHeaderData(0, Qt::Horizontal, tr("Напряжение"));
    model_profiles->setHeaderData(1, Qt::Horizontal, tr("Ток"));
    model_profiles->setHeaderData(2, Qt::Horizontal, tr("Время"));

    ui->tv_profiles->setModel(model_profiles);
    ui->tv_profiles->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tv_profiles->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tv_profiles->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    //---

    connect(ui->bt_search_power_supply, SIGNAL(clicked()), this, SLOT(search_power_supply()));
    connect(ui->knob_U, SIGNAL(valueChanged(double)), ui->lcd_U, SLOT(display(double)));
    connect(ui->knob_I, SIGNAL(valueChanged(double)), ui->lcd_I, SLOT(display(double)));
    connect(ui->btn_set_UI, SIGNAL(clicked()), this, SLOT(set_UI()));

    connect(ui->btn_inc_U, SIGNAL(clicked()), this, SLOT(inc_U()));
    connect(ui->btn_inc_I, SIGNAL(clicked()), this, SLOT(inc_I()));
    connect(ui->btn_dec_U, SIGNAL(clicked()), this, SLOT(dec_U()));
    connect(ui->btn_dec_I, SIGNAL(clicked()), this, SLOT(dec_I()));

    connect(ui->btn_get_UI, SIGNAL(clicked()), this, SLOT(get_UI()));

    connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(send_0_0()));

    //connect(ui->knob_U, SIGNAL(valueChanged(double)), this, SLOT(set_manual_U(double)));
    //connect(ui->knob_I, SIGNAL(valueChanged(double)), this, SLOT(set_manual_I(double)));
    connect(ui->sb_manual_U, SIGNAL(valueChanged(int)), this, SLOT(set_manual_U(int)));
    connect(ui->sb_manual_I, SIGNAL(valueChanged(int)), this, SLOT(set_manual_I(int)));

    connect(ui->btn_profile_1, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_2, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_3, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_4, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_5, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_6, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_7, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_8, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));
    connect(ui->btn_profile_9, SIGNAL(toggled(bool)), this, SLOT(load_profile(bool)));

    ui->btn_profile_1->setProperty("id", 1);
    ui->btn_profile_2->setProperty("id", 2);
    ui->btn_profile_3->setProperty("id", 3);
    ui->btn_profile_4->setProperty("id", 4);
    ui->btn_profile_5->setProperty("id", 5);
    ui->btn_profile_6->setProperty("id", 6);
    ui->btn_profile_7->setProperty("id", 7);
    ui->btn_profile_8->setProperty("id", 8);
    ui->btn_profile_9->setProperty("id", 9);

#ifdef USE_GRAPHER
    grapher = new GrapherBox(0, 1000, 0, MAX_B588_VALUE_U, "points", "time", "UI", this);
    curve_U = grapher->add_curve("U");
    curve_I = grapher->add_curve("I");

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);
    grapher->set_visible_btn_Clear(false);

    ui->grapher_layout->addWidget(grapher);
#endif

    connect(ui->btn_run_profile, SIGNAL(released()), this, SLOT(run_profile()));
    connect(ui->btn_add_point, SIGNAL(released()), this, SLOT(add_point()));
    connect(ui->btn_clear_profile, SIGNAL(released()), this, SLOT(clear_profile()));
}
//--------------------------------------------------------------------------------
void B588::set_manual_U(double value)
{
    ui->sb_manual_U->setValue(value * 1000.0f + 0.5f);
}
//--------------------------------------------------------------------------------
void B588::set_manual_I(double value)
{
    ui->sb_manual_I->setValue(value * 1000.0f + 0.5f);
}
//--------------------------------------------------------------------------------
void B588::set_manual_U(int value)
{
    ui->knob_U->setValue(value / 1000.0f);
}
//--------------------------------------------------------------------------------
void B588::set_manual_I(int value)
{
    ui->knob_I->setValue(value / 1000.0f);
}
//--------------------------------------------------------------------------------
void B588::show_U(double value)
{
    ui->lcd_U->display((int)value);
}
//--------------------------------------------------------------------------------
void B588::show_I(double value)
{
    ui->lcd_I->display((int)value);
}
//--------------------------------------------------------------------------------
void B588::createPowersupply(void)
{
    powersupply = new Powersupply_B588(this);

    connect(powersupply, SIGNAL(info(QString)),    topLevelWidget(), SIGNAL(info(QString)));
    connect(powersupply, SIGNAL(debug(QString)),   topLevelWidget(), SIGNAL(debug(QString)));
    connect(powersupply, SIGNAL(error(QString)),   topLevelWidget(), SIGNAL(error(QString)));
    connect(powersupply, SIGNAL(trace(QString)), topLevelWidget(), SIGNAL(trace(QString)));

    connect(ui->btn_rc_on,  SIGNAL(released()), powersupply, SLOT(rc_on()));
    connect(ui->btn_rc_off, SIGNAL(released()), powersupply, SLOT(rc_off()));

#ifdef ADD_TOOLBAR
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        B588_toolbar *psBar = new B588_toolbar(tr("powersupply B5-88"), this);
        psBar->setObjectName("ps_B588");
        psBar->adjustSize();
        psBar->setMinimumWidth(psBar->sizeHint().width());
        connect(psBar, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));

        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, psBar);
    }
#endif
}
//--------------------------------------------------------------------------------
void B588::set_UI(void)
{
    bool ok = powersupply->set_UI(ui->knob_U->value() * 1000.0f,
                                  ui->knob_I->value() * 1000.0f);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B588::inc_U(void)
{
    bool ok;
    int U = ui->cb_U->itemData(ui->cb_U->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_U->setValue(ui->knob_U->value()+(U / 1000.0f));
}
//--------------------------------------------------------------------------------
void B588::inc_I(void)
{
    bool ok;
    int I = ui->cb_I->itemData(ui->cb_I->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_I->setValue(ui->knob_I->value()+(I / 1000.0f));
}
//--------------------------------------------------------------------------------
void B588::dec_U(void)
{
    bool ok;
    int U = ui->cb_U->itemData(ui->cb_U->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_U->setValue(ui->knob_U->value()-(U / 1000.0f));
}
//--------------------------------------------------------------------------------
void B588::dec_I(void)
{
    bool ok;
    int I = ui->cb_I->itemData(ui->cb_I->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_I->setValue(ui->knob_I->value()-(I / 1000.0f));
}
//--------------------------------------------------------------------------------
void B588::block_interface(bool state)
{
    ui->btn_0->setDisabled(state);
    ui->btn_inc_U->setDisabled(state);
    ui->btn_inc_I->setDisabled(state);
    ui->btn_dec_U->setDisabled(state);
    ui->btn_dec_I->setDisabled(state);
    ui->btn_set_UI->setDisabled(state);
    ui->btn_get_UI->setDisabled(state);

    ui->knob_U->setDisabled(state);
    ui->knob_I->setDisabled(state);

    ui->cb_U->setDisabled(state);
    ui->cb_I->setDisabled(state);

    ui->lcd_U->setDisabled(state);
    ui->lcd_I->setDisabled(state);

    ui->sb_manual_U->setDisabled(state);
    ui->sb_manual_I->setDisabled(state);

    ui->btn_rc_on->setDisabled(state);
    ui->btn_rc_off->setDisabled(state);

    ui->btn_run_profile->setDisabled(state);
    ui->btn_add_point->setDisabled(state);
    ui->btn_clear_profile->setDisabled(state);
    ui->tv_profiles->setDisabled(state);

    ui->btn_profile_1->setDisabled(state);
    ui->btn_profile_2->setDisabled(state);
    ui->btn_profile_3->setDisabled(state);
    ui->btn_profile_4->setDisabled(state);
    ui->btn_profile_5->setDisabled(state);
    ui->btn_profile_6->setDisabled(state);
    ui->btn_profile_7->setDisabled(state);
    ui->btn_profile_8->setDisabled(state);
    ui->btn_profile_9->setDisabled(state);

    update();
}
//--------------------------------------------------------------------------------
bool B588::search_power_supply(void)
{
    bool ok = false;

    emit info(tr("Поиск устройства"));

    powersupply->set_ignore_bad_cmd(true);
    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-88 НЕ найден!");
        return false;
    }

    emit info(tr("Поиск устройств закончен"));
    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
void B588::get_UI(void)
{
    unsigned short  in_voltage = 0;
    unsigned short  in_current = 0;
    unsigned short  out_voltage = 0;
    unsigned short  out_current = 0;
    unsigned char   temp = 0;
    unsigned char   current_point = 0;
    unsigned char   active_profile = 0;

    powersupply->set_state_silence(true);
    powersupply->set_address(ui->sb_address->value());
    bool ok = powersupply->get_UI(&in_voltage,
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
}
//--------------------------------------------------------------------------------
void B588::test(void)
{
    emit info("Test");

    powersupply->run_dialog_0x15();
    powersupply->run_dialog_0x16();
    powersupply->run_dialog_0x54();
    powersupply->run_dialog_0x56();
    powersupply->run_dialog_0x5E();
    powersupply->run_dialog_0x5F();
    powersupply->run_dialog_0x6E();
    powersupply->run_dialog_0x6F();
}
//--------------------------------------------------------------------------------
#if 0
#define B588_CMD_0x54_TEXT tr("Б5-88 функция 0x54: Запрос данных по профилям.")
#define B588_CMD_0x56_TEXT tr("Б5-88 функция 0x56: Чтение профиля по точкам.")
#define B588_CMD_0x5E_TEXT tr("Б5-88 функция 0x5E: Запись данных точки в профиль для исполнения.")
#define B588_CMD_0x5F_TEXT tr("Б5-88 функция 0x5F: Запуск профиля на исполнение.")
#define B588_CMD_0x6E_TEXT tr("Б5-88 функция 0x6E: Запись количества повторов в профиль.")
#define B588_CMD_0x6F_TEXT tr("Б5-88 функция 0x6F: Очистка профиля")
#define B588_CMD_0x7B_TEXT tr("Б5-88 функция 0x7B: Запись количества повторов профиля")
#endif
//--------------------------------------------------------------------------------
int B588::get_current_profile(void)
{
    if(ui->btn_profile_1->isChecked()) return 1;
    if(ui->btn_profile_2->isChecked()) return 2;
    if(ui->btn_profile_3->isChecked()) return 3;
    if(ui->btn_profile_4->isChecked()) return 4;
    if(ui->btn_profile_5->isChecked()) return 5;
    if(ui->btn_profile_6->isChecked()) return 6;
    if(ui->btn_profile_7->isChecked()) return 7;
    if(ui->btn_profile_8->isChecked()) return 8;
    if(ui->btn_profile_9->isChecked()) return 9;
    return -1;
}
//--------------------------------------------------------------------------------
void B588::read_profile(int number_profile)
{
    if(number_profile < MIN_PROFILE_B588) return;
    if(number_profile > MAX_PROFILE_B588) return;

    emit info(QString("read_profile(%1)").arg(number_profile));

    unsigned char count_point = 0;
    unsigned char count_repeat_profile = 0;
    bool ok = powersupply->get_data_profile(number_profile, &count_point, &count_repeat_profile);
    if(!ok) return;

    for(unsigned char point=0; point<count_point; point++)
    {
        uint8_t  r_profile = 0;
        uint8_t  r_point = 0;
        uint16_t r_voltage = 0;
        int16_t  r_current = 0;
        uint16_t r_time = 0;
        ok = powersupply->get_point_profile(number_profile,
                                            point,
                                            &r_profile,
                                            &r_point,
                                            &r_voltage,
                                            &r_current,
                                            &r_time);
        if(ok)
        {
            model_profiles->insertRows(0, 3, QModelIndex());
            model_profiles->setData(model_profiles->index(0, 0, QModelIndex()), r_voltage);
            model_profiles->setData(model_profiles->index(0, 1, QModelIndex()), r_current);
            model_profiles->setData(model_profiles->index(0, 2, QModelIndex()), r_time);
        }
    }
}
//--------------------------------------------------------------------------------
void B588::load_profile(bool checked)
{
    if(checked == false) return;

    QToolButton *button = (QToolButton *)sender();
    if(!button) return;

    bool ok = false;
    int id = button->property("id").toInt(&ok);
    if(ok)
    {
        read_profile(id);
    }

    //emit info(QString("%1 %2").arg(button->objectName()).arg(button->isChecked()));
}
//--------------------------------------------------------------------------------
void B588::run_profile(void)
{
    powersupply->run_profile(get_current_profile());
    emit info("run_profile");
}
//--------------------------------------------------------------------------------
void B588::add_point(void)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    QSpinBox *sb_point = new QSpinBox();
    QSpinBox *sb_U = new QSpinBox();
    QSpinBox *sb_I = new QSpinBox();
    QSpinBox *sb_Time = new QSpinBox();

    sb_point->setMinimum(MIN_POINT_B588);
    sb_point->setMaximum(MAX_POINT_B588);

    sb_U->setMinimum(0);
    sb_U->setMaximum(0xFFFF);

    sb_I->setMinimum(0);
    sb_I->setMaximum(0xFFFF);

    sb_Time->setMinimum(1);
    sb_Time->setMaximum(9999);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("Point"),0, 0);
    grid->addWidget(sb_point,           0, 1);
    grid->addWidget(new QLabel("U"),    1, 0);
    grid->addWidget(sb_U,               1, 1);
    grid->addWidget(new QLabel("I"),    2, 0);
    grid->addWidget(sb_I,               2, 1);
    grid->addWidget(new QLabel("Time"), 3, 0);
    grid->addWidget(sb_Time,            3, 1);

    vbox->addWidget(new QLabel(tr("Добавление точки")));
    vbox->addLayout(grid);

    QDialog *dlg = new QDialog(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    buttonBox->setCenterButtons(true);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    vbox->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));

    dlg->setLayout(vbox);
    dlg->adjustSize();
    dlg->setFixedSize(dlg->sizeHint());
    int btn = dlg->exec();

    if(btn != QDialog::Accepted) return;

    //---
    bool ok = powersupply->add_point_profile(get_current_profile(),
                                             sb_point->value(),
                                             sb_U->value(),
                                             sb_I->value(),
                                             sb_Time->value());
    if(!ok) return;

    emit info("add_point");
}
//--------------------------------------------------------------------------------
void B588::clear_profile(void)
{
    powersupply->clear_profile(get_current_profile());
    emit info("clear_profile");
}
//--------------------------------------------------------------------------------
void B588::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
