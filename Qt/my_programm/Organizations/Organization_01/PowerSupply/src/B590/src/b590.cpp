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
#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QDialog>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "ui_b590.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
#include "sleeper.h"
#include "b590.hpp"

#include "grapherbox.hpp"

#include "b590_powersupply.hpp"
#include "parrotbar.hpp"
#include "b590_toolbar.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
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
void B590::init(void)
{
    ui->setupUi(this);

#ifdef TEST
    createTestBar();
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
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test,  SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void B590::createParrotBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    ParrotBar *parrotBar = new ParrotBar(this);
    Q_CHECK_PTR(parrotBar);
    connect(parrotBar, SIGNAL(set_UI(int,int)), this, SLOT(set_UI_parrot(int,int)));

    mw->addToolBar(Qt::TopToolBarArea, parrotBar);
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
    if(!ok) return powersupply->print_last_error();

    emit info("OK");
    return E_B590_NO_ERROR;
}
//--------------------------------------------------------------------------------
void B590::createWidgets(void)
{
    ui->bt_search_power_supply->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

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

    //---
    model_profiles = new QStandardItemModel(0, 5, this); //TODO
    model_profiles->setHeaderData(0, Qt::Horizontal, tr("Напряжение"));
    model_profiles->setHeaderData(1, Qt::Horizontal, tr("Ток"));
    model_profiles->setHeaderData(2, Qt::Horizontal, tr("Часы"));
    model_profiles->setHeaderData(3, Qt::Horizontal, tr("Минуты"));
    model_profiles->setHeaderData(4, Qt::Horizontal, tr("Секунды"));

    ui->tv_profiles->setModel(model_profiles);
    ui->tv_profiles->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tv_profiles->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tv_profiles->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    //---

    connect(ui->bt_search_power_supply, SIGNAL(clicked()), this, SLOT(search_power_supply()));
    connect(ui->knob_U, SIGNAL(valueChanged(double)), this, SLOT(show_U(double)));
    connect(ui->knob_I, SIGNAL(valueChanged(double)), this, SLOT(show_I(double)));
    connect(ui->btn_set_UI, SIGNAL(clicked()), this, SLOT(set_UI()));

    connect(ui->btn_inc_U, SIGNAL(clicked()), this, SLOT(inc_U()));
    connect(ui->btn_inc_I, SIGNAL(clicked()), this, SLOT(inc_I()));
    connect(ui->btn_dec_U, SIGNAL(clicked()), this, SLOT(dec_U()));
    connect(ui->btn_dec_I, SIGNAL(clicked()), this, SLOT(dec_I()));

    connect(ui->btn_get_UI, SIGNAL(clicked()), this, SLOT(get_UI()));

    connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(send_0_0()));

    for(int n=MIN_PROFILE_B590; n<MAX_PROFILE_B590; n++)
    {
        ui->cb_number_profile->addItem(QString::number(n));
    }
    connect(ui->pb_load_profile, SIGNAL(released()), this, SLOT(load_profile()));
    connect(ui->pb_save_profile, SIGNAL(released()), this, SLOT(save_profile()));

    ui->tv_profiles->setSelectionBehavior(QAbstractItemView::SelectRows);

#ifdef USE_GRAPHER
    grapher = new GrapherBox(0, 1000, 0, MAX_B590_VALUE_U, "points", "time", "UI", this);
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
    connect(ui->btn_add_point,   SIGNAL(released()), this, SLOT(add_point()));
    connect(ui->btn_del_point,   SIGNAL(released()), this, SLOT(del_point()));
}
//--------------------------------------------------------------------------------
void B590::createPowersupply(void)
{
    powersupply = new Powersupply_B590(this);

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
        B590_toolbar *psBar = new B590_toolbar(tr("powersupply B5-90"), this);
        psBar->setObjectName("ps_B590");
        psBar->adjustSize();
        psBar->setMinimumWidth(psBar->sizeHint().width());
        connect(psBar, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));

        mw->addToolBarBreak();
        mw->addToolBar(Qt::TopToolBarArea, psBar);
    }
#endif
}
//--------------------------------------------------------------------------------
void B590::set_UI(void)
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
void B590::inc_U(void)
{
    bool ok;
    int U = ui->cb_U->itemData(ui->cb_U->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_U->setValue(ui->knob_U->value()+((float)U / 1000.0f));
}
//--------------------------------------------------------------------------------
void B590::inc_I(void)
{
    bool ok;
    int I = ui->cb_I->itemData(ui->cb_I->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_I->setValue(ui->knob_I->value()+((float)I / 1000.0f));
}
//--------------------------------------------------------------------------------
void B590::dec_U(void)
{
    bool ok;
    int U = ui->cb_U->itemData(ui->cb_U->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_U->setValue(ui->knob_U->value()-((float)U / 1000.0f));
}
//--------------------------------------------------------------------------------
void B590::dec_I(void)
{
    bool ok;
    int I = ui->cb_I->itemData(ui->cb_I->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    ui->knob_I->setValue(ui->knob_I->value()-((float)I / 1000.0f));
}
//--------------------------------------------------------------------------------
void B590::block_interface(bool state)
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

    ui->btn_rc_on->setDisabled(state);
    ui->btn_rc_off->setDisabled(state);

    ui->btn_run_profile->setDisabled(state);
    ui->btn_add_point->setDisabled(state);
    ui->btn_del_point->setDisabled(state);
    ui->tv_profiles->setDisabled(state);

    ui->cb_number_profile->setDisabled(state);
    ui->pb_load_profile->setDisabled(state);
    ui->pb_save_profile->setDisabled(state);

    update();
}
//--------------------------------------------------------------------------------
bool B590::search_power_supply(void)
{
    bool ok = false;

    emit info(tr("Поиск устройства"));

    powersupply->set_ignore_bad_cmd(true);
    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-90 НЕ найден!");
        return false;
    }

    emit info(tr("Поиск устройств закончен"));
    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
void B590::get_UI(void)
{
    int32_t  in_voltage = 0;
    uint16_t in_current = 0;
    int32_t  out_voltage = 0;
    uint16_t out_current = 0;
    int8_t   temp = 0;
    uint8_t  active_profile = 0;

    powersupply->set_state_silence(true);
    powersupply->set_address(ui->sb_address->value());
    bool ok = powersupply->get_UI(&in_voltage,
                                  &in_current,
                                  &out_voltage,
                                  &out_current,
                                  &temp,
                                  &active_profile);
    powersupply->set_state_silence(false);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info(QString("in_voltage %1").arg(in_voltage));
    emit info(QString("in_current %1").arg(in_current));
    emit info(QString("out_voltage %1").arg(out_voltage));
    emit info(QString("out_current %1").arg(out_current));
}
//--------------------------------------------------------------------------------
#if 0
#define B590_CMD_0x54_TEXT tr("Б5-90 функция 0x54: Запрос данных по профилям.")
#define B590_CMD_0x56_TEXT tr("Б5-90 функция 0x56: Чтение профиля по точкам.")
#define B590_CMD_0x5E_TEXT tr("Б5-90 функция 0x5E: Запись данных точки в профиль для исполнения.")
#define B590_CMD_0x5F_TEXT tr("Б5-90 функция 0x5F: Запуск профиля на исполнение.")
#define B590_CMD_0x6E_TEXT tr("Б5-90 функция 0x6E: Запись количества повторов в профиль.")
#define B590_CMD_0x6F_TEXT tr("Б5-90 функция 0x6F: Очистка профиля")
#endif
//--------------------------------------------------------------------------------
void B590::test(void)
{
    emit info("Test begin");
    bool ok = false;
    int number_profile = 1;

#if 1
    ok = powersupply->clear_profile(number_profile);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }

    for(int n=1000; n<10000; n+=1000)
    {
        ok = powersupply->add_point_profile(number_profile, n, 1000, 10);
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
        ok = powersupply->add_point_profile(number_profile, n, 1000, 0);
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
    }
#endif

#if 1
    uint8_t r_profile = 0;
    ok = powersupply->run_profile(number_profile, &r_profile);
    if(!ok)
    {
        powersupply->print_last_error();
        return;
    }
    emit info(QString("r_profile %1").arg(r_profile));
#endif

    emit info("Test end");
}
//--------------------------------------------------------------------------------
void B590::show_U(double value)
{
    ui->lcd_U->display(value);
}
//--------------------------------------------------------------------------------
void B590::show_I(double value)
{
    ui->lcd_I->display(value);
}
//--------------------------------------------------------------------------------
int B590::get_current_profile(void)
{
    return ui->cb_number_profile->currentIndex();
}
//--------------------------------------------------------------------------------
void B590::read_profile(int number_profile)
{
    if(number_profile < MIN_PROFILE_B590) return;
    if(number_profile > MAX_PROFILE_B590) return;

    emit info(QString("read_profile(%1)").arg(number_profile));

    unsigned char count_point = 0;
    unsigned char count_repeat_profile = 0;
    bool ok = powersupply->get_data_profile(number_profile, &count_point, &count_repeat_profile);
    if(!ok) return;

    emit info(QString("count_point %1").arg(count_point));
    emit info(QString("count_repeat_profile %1").arg(count_repeat_profile));

    model_profiles->clear();
    model_profiles->setColumnCount(5);
    model_profiles->setHeaderData(0, Qt::Horizontal, tr("Напряжение"));
    model_profiles->setHeaderData(1, Qt::Horizontal, tr("Ток"));
    model_profiles->setHeaderData(2, Qt::Horizontal, tr("Часы"));
    model_profiles->setHeaderData(3, Qt::Horizontal, tr("Минуты"));
    model_profiles->setHeaderData(4, Qt::Horizontal, tr("Секунды"));

    for(unsigned char point=0; point<count_point; point++)
    {
        uint8_t  r_profile = 0;
        uint8_t  r_point = 0;
        uint16_t r_voltage = 0;
        uint16_t r_current = 0;
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
            int count = model_profiles->rowCount();
            model_profiles->insertRows(count, 1);
            model_profiles->setData(model_profiles->index(count, 0), r_voltage);
            model_profiles->setData(model_profiles->index(count, 1), r_current);
            model_profiles->setData(model_profiles->index(count, 2), r_time);

            emit info(QString("r_profile %1").arg(r_profile));
            emit info(QString("r_point %1").arg(r_point));
            emit info(QString("r_voltage %1").arg(r_voltage));
            emit info(QString("r_current %1").arg(r_current));
            emit info(QString("r_time %1").arg(r_time));
        }
        else
        {
            powersupply->print_last_error();
        }
    }
}
//--------------------------------------------------------------------------------
void B590::load_profile(void)
{
    read_profile(ui->cb_number_profile->currentIndex());
}
//--------------------------------------------------------------------------------
void B590::save_profile(void)
{
    emit info(QString("save to %1 profile").arg(get_current_profile()));
    int size = model_profiles->rowCount();
    if(size <= 0)
    {
        emit error(tr("no data"));
        return;
    }

    bool ok = false;
    s_point point;
    for(int n=0; n<size; n++)
    {
        point.voltage = model_profiles->data(model_profiles->index(n, 0)).toInt();
        point.current = model_profiles->data(model_profiles->index(n, 1)).toInt();
        point.time    = model_profiles->data(model_profiles->index(n, 2)).toInt();
        emit info(QString("%1: U %2 I %3 T %4")
                  .arg(n)
                  .arg(point.voltage)
                  .arg(point.current)
                  .arg(point.time));
        ok = powersupply->add_point_profile(get_current_profile(),
                                            point.voltage,
                                            point.current,
                                            point.time);
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
    }
    emit info(QString("size %1").arg(size));
}
//--------------------------------------------------------------------------------
void B590::run_profile(void)
{
    emit info("run_profile");

    uint8_t r_profile = 0;
    bool ok = powersupply->run_profile(get_current_profile(), &r_profile);
    if(!ok)
    {
        powersupply->print_last_error();
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::add_point(void)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    QSpinBox *sb_U = new QSpinBox();
    QSpinBox *sb_I = new QSpinBox();
    QSpinBox *sb_Hour = new QSpinBox();
    QSpinBox *sb_Min  = new QSpinBox();
    QSpinBox *sb_Sec  = new QSpinBox();

    sb_U->setMinimum(MIN_VOLTAGE_B590);
    sb_U->setMaximum(MAX_VOLTAGE_B590);

    sb_I->setMinimum(MIN_CURRENT_B590);
    sb_I->setMaximum(MAX_CURRENT_B590);

    sb_Hour->setMinimum(0);
    sb_Hour->setMaximum(23);

    sb_Min->setMinimum(0);
    sb_Min->setMaximum(59);

    sb_Sec->setMinimum(0);
    sb_Sec->setMaximum(59);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("Напряжение, mV"),   1, 0);  grid->addWidget(sb_U,       1, 1);
    grid->addWidget(new QLabel("Ток, mA"),          2, 0);  grid->addWidget(sb_I,       2, 1);
    grid->addWidget(new QLabel("Часы"),             3, 0);  grid->addWidget(sb_Hour,    3, 1);
    grid->addWidget(new QLabel("Минуты"),           4, 0);  grid->addWidget(sb_Min,     4, 1);
    grid->addWidget(new QLabel("Секунды"),          5, 0);  grid->addWidget(sb_Sec,     5, 1);

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
    //int index = ui->tv_profiles->currentIndex().row();
    int index = ui->tv_profiles->model()->rowCount();
    if(index < 0) index = 0;
    //emit info(QString("index %1").arg(index));
    model_profiles->insertRows(index, 1);
    model_profiles->setData(model_profiles->index(index, 0), sb_U->value());
    model_profiles->setData(model_profiles->index(index, 1), sb_I->value());
    model_profiles->setData(model_profiles->index(index, 2), sb_Hour->value());
    model_profiles->setData(model_profiles->index(index, 3), sb_Min->value());
    model_profiles->setData(model_profiles->index(index, 4), sb_Sec->value());
    //---

    emit info("OK");
}
//--------------------------------------------------------------------------------
void B590::del_point(void)
{
    //---
    int index = ui->tv_profiles->currentIndex().row();
    if(index < 0) index = 0;
    //emit info(QString("index %1").arg(index));
    model_profiles->removeRows(index, 1);
    //---
}
//--------------------------------------------------------------------------------
void B590::clear_profile(void)
{
    powersupply->clear_profile(get_current_profile());
    emit info("clear_profile");
}
//--------------------------------------------------------------------------------
void B590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
