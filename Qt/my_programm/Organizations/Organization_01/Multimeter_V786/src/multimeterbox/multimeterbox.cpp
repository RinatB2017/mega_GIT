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
#include <QDialogButtonBox>
#include <QToolButton>
#include <QMessageBox>
#include <QPushButton>
#include <QLCDNumber>
#include <QCheckBox>
#include <QSpinBox>
#include <QToolBar>
#include <QThread>
#include <QSlider>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QTime>
#include <qmath.h>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "batteryprogressbar.hpp"
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
//--------------------------------------------------------------------------------
#include "multimeterbox.hpp"
//--------------------------------------------------------------------------------
#ifdef NO_GRAPHER
#   include "ui_multimeterbox_no_grapher.h"
#else
#   include "ui_multimeterbox.h"
#endif
//--------------------------------------------------------------------------------
#ifdef TESTBAR
#   include "handcontrol.hpp"
#endif
//--------------------------------------------------------------------------------
#include "qwt_symbol.h"
#include "mainwindow.hpp"
#include "grapherbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MultimeterBox::MultimeterBox(const QString &title,
                             QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Multimeter),
    title(title),
    flag_measuring(false)
{
    init();
}
//--------------------------------------------------------------------------------
MultimeterBox::~MultimeterBox()
{
    flag_measuring = false;
    delete ui;
}
//--------------------------------------------------------------------------------
#ifdef TESTBAR
void MultimeterBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
#endif
//--------------------------------------------------------------------------------
#ifdef TESTBAR
void MultimeterBox::test(void)
{
    HandControl *hc = new HandControl(0, multimeter, this);
    hc->set_serno(ui->sb_SerNo->value());
    hc->set_year(ui->sb_Year->value());
    hc->set_address(ui->sb_Address->value());
    hc->set_channel(ui->sb_Channel->value());
    hc->exec();
}
#endif
//--------------------------------------------------------------------------------
void MultimeterBox::init(void)
{
    ui->setupUi(this);

#ifdef TESTBAR
    createTestBar();
#endif

#ifdef FAKE
    createFakeBar();
#endif

#ifndef NO_GRAPHER
    init_grapherbox();
#endif

    multimeter = new Multimeter_V786_2(this);
    connect(multimeter, SIGNAL(info(QString)),    this, SIGNAL(info(QString)));
    connect(multimeter, SIGNAL(debug(QString)),   this, SIGNAL(debug(QString)));
    connect(multimeter, SIGNAL(error(QString)),   this, SIGNAL(error(QString)));
    connect(multimeter, SIGNAL(trace(QString)), this, SIGNAL(trace(QString)));

    ui->cb_measurement->addItem(tr("Измерение напряжения постоянного тока"), QVariant(Qt::UserRole + V764_2_CMD_0x06));
    ui->cb_measurement->addItem(tr("Измерение напряжения переменного тока"), QVariant(Qt::UserRole + V764_2_CMD_0x07));
    ui->cb_measurement->addItem(tr("Измерение силы постоянного тока"), QVariant(Qt::UserRole + V764_2_CMD_0x08));
    ui->cb_measurement->addItem(tr("Измерение силы переменного тока"), QVariant(Qt::UserRole + V764_2_CMD_0x09));
    ui->cb_measurement->addItem(tr("Измерение сопротивления"), QVariant(Qt::UserRole + V764_2_CMD_0x0A));
    ui->cb_measurement->addItem(tr("Измерение емкости"), QVariant(Qt::UserRole + V764_2_CMD_0x0B));
    ui->cb_measurement->addItem(tr("Измерение индуктивности"), QVariant(Qt::UserRole + V764_2_CMD_0x0C));
    ui->cb_measurement->addItem(tr("Измерение частоты"), QVariant(Qt::UserRole + V764_2_CMD_0x0E));
    ui->cb_measurement->addItem(tr("Измерение активной мощности"), QVariant(Qt::UserRole + V764_2_CMD_0x0F));
    ui->cb_measurement->addItem(tr("Измерение реактивной мощности"), QVariant(Qt::UserRole + V764_2_CMD_0x10));
    ui->cb_measurement->addItem(tr("Измерение полной мощности"), QVariant(Qt::UserRole + V764_2_CMD_0x11));
    ui->cb_measurement->addItem(tr("Измерение коэффициента мощности"), QVariant(Qt::UserRole + V764_2_CMD_0x12));

    ui->cb_retention->addItem(tr("Удержания нет"), QVariant(Qt::UserRole + Multimeter_V786_2::RETENTION_0));
    ui->cb_retention->addItem(tr("Удержание текущего значения"), QVariant(Qt::UserRole + Multimeter_V786_2::RETENTION_1));
    ui->cb_retention->addItem(tr("Удержание минимального значения"), QVariant(Qt::UserRole + Multimeter_V786_2::RETENTION_2));
    ui->cb_retention->addItem(tr("Удержание максимального значения"), QVariant(Qt::UserRole + Multimeter_V786_2::RETENTION_3));

    ui->cb_deviation->addItem(tr("Вычисление не производится"), QVariant(Qt::UserRole + Multimeter_V786_2::DEVIATION_0));
    ui->cb_deviation->addItem(tr("Вычисление абсолютного отклонения"), QVariant(Qt::UserRole + Multimeter_V786_2::DEVIATION_1));
    ui->cb_deviation->addItem(tr("Вычисление относительного отклонения в %"), QVariant(Qt::UserRole + Multimeter_V786_2::DEVIATION_2));

    ui->sb_Address->setMinimum(0);
    ui->sb_Address->setMaximum(255);

    ui->sb_SerNo->setMinimum(0);
    ui->sb_SerNo->setMaximum(9999);

    ui->sb_Year->setMinimum(2013);
    ui->sb_Year->setMaximum(2100);

    ui->sb_Channel->setMinimum(1);
    ui->sb_Channel->setMaximum(15);

    ui->sb_MaxError->setMinimum(1);
    ui->sb_MaxError->setMaximum(0xFF);

    connect(ui->btn_power, SIGNAL(clicked(bool)), this, SLOT(power(bool)));

    connect(ui->cb_UnlimError, SIGNAL(toggled(bool)), ui->sb_MaxError, SLOT(setDisabled(bool)));

    QLabel *battery_label = new QLabel(tr("Заряд батареи"));
    battery_progress_bar = new BatteryProgressBar(true, this);
    ui->battery_layout->addStretch(1);
    ui->battery_layout->addWidget(battery_label);
    ui->battery_layout->addWidget(battery_progress_bar);

    measurement_changed(0);

    connect(ui->cb_measurement, SIGNAL(currentIndexChanged(int)), this, SLOT(measurement_changed(int)));

    connect(ui->cb_auto, SIGNAL(toggled(bool)), ui->cb_limits, SLOT(setDisabled(bool)));
    ui->cb_auto->setChecked(true);

    block_interface(true);
    ui->btn_power->setEnabled(false);

    btn_find_multimeter = new QPushButton(tr("Найти USB приемо-передатчик мультиметра"));
    btn_find_multimeter->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    connect(btn_find_multimeter, SIGNAL(clicked()), this, SLOT(search_devices()));

    connect(multimeter, SIGNAL(get_measuring_value(QString)), ui->lcd_value_number, SLOT(display(QString)));
    connect(multimeter, SIGNAL(get_battery_value(unsigned short)), battery_progress_bar, SLOT(setVoltage(unsigned short)));

    connect(multimeter, SIGNAL(change_retention(QString)), ui->retention_label, SLOT(setText(QString)));
    connect(multimeter, SIGNAL(change_deviation(QString)), ui->deviation_label, SLOT(setText(QString)));
    connect(multimeter, SIGNAL(change_limit(QString)), ui->limit_label, SLOT(setText(QString)));

    connect(this, SIGNAL(start()), this, SLOT(start_measuring()));
    connect(this, SIGNAL(stop()), this, SLOT(stop_measuring()));

    ui->layout_serial->addWidget(btn_find_multimeter);

#ifdef ADD_MENU
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        QMenu *m = new QMenu(tr("Devices"));
        m->addMenu(multimeter->add_menu());

        mw->add_menu(2, m);
    }
#endif
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MultimeterBox::reset_show_values(void)
{
    ui->lcd_value_number->display(0);
    ui->retention_label->setText("---");
    ui->deviation_label->setText("---");
    ui->limit_label->setText("---");
}
//--------------------------------------------------------------------------------
int MultimeterBox::get_measurement(bool *ok)
{
    bool temp_ok = false;
    int cmd = ui->cb_measurement->itemData(ui->cb_measurement->currentIndex(), Qt::UserRole).toInt(&temp_ok) - Qt::UserRole;
    if(temp_ok)
    {
        *ok = true;
        return cmd;
    }
    *ok = false;
    return -1;
}
//--------------------------------------------------------------------------------
int MultimeterBox::get_retention(bool *ok)
{
    bool temp_ok = false;
    int cmd = ui->cb_retention->itemData(ui->cb_retention->currentIndex(), Qt::UserRole).toInt(&temp_ok) - Qt::UserRole;
    if(temp_ok)
    {
        *ok = true;
        return cmd;
    }
    *ok = false;
    return -1;
}
//--------------------------------------------------------------------------------
int MultimeterBox::get_deviation(bool *ok)
{
    bool temp_ok = false;
    int cmd = ui->cb_deviation->itemData(ui->cb_deviation->currentIndex(), Qt::UserRole).toInt(&temp_ok) - Qt::UserRole;
    if(temp_ok)
    {
        *ok = true;
        return cmd;
    }
    *ok = false;
    return -1;
}
//--------------------------------------------------------------------------------
int MultimeterBox::get_limit(bool *ok)
{
    *ok = true;
    int limit = ui->cb_limits->currentIndex();
    return limit;
}
//--------------------------------------------------------------------------------
void MultimeterBox::prepare_param(void)
{
    unsigned char limit = 0;
    bool ok = false;

    if(ui->cb_auto->isChecked()) limit = LIMIT_AUTO;
    else limit = get_limit(&ok);
    multimeter->set_limit(limit);
    multimeter->set_auto_limit(ui->cb_auto->isChecked());
    multimeter->set_deviation(get_deviation(&ok));
    multimeter->set_retention(get_retention(&ok));
}
//--------------------------------------------------------------------------------
void MultimeterBox::start_measuring(void)
{
    emit debug("MultimeterBox::start_measuring()");

    if(flag_measuring) return;

    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_Year->value());
    multimeter->set_address(ui->sb_Address->value());
    multimeter->set_channel(ui->sb_Channel->value());
    multimeter->set_timeout(MAX_TIMEOUT_MULTIMETER_CONTROL_UI);

    bool ok = multimeter->remote_control_on();
    if(!ok)
    {
        multimeter->print_last_error();
        measuring_finished();
        return;
    }

    reset_show_values();
    flag_measuring = true;
    int counter_battery = 0;
    int counter_error = 1;
    ok = false;
    while(flag_measuring)
    {
        if(counter_battery == 0)
        {
            multimeter->measuring_battery(&ok);
            if(!ok)
            {
                multimeter->print_last_error();
            }
        }
        if(counter_battery < 3) counter_battery++;
        else counter_battery = 0;

        bool ok = false;
        double value = 0;
        int cmd = get_measurement(&ok);
        switch(cmd)
        {
        case V764_2_CMD_0x06:
            prepare_param();
            ok = multimeter->measuring_UDC(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x07:
            prepare_param();
            ok = multimeter->measuring_UAC(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x08:
            prepare_param();
            ok = multimeter->measuring_IDC(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x09:
            prepare_param();
            ok = multimeter->measuring_IAC(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                counter_error++;
            }
            break;

        case V764_2_CMD_0x0A:
            prepare_param();
            ok = multimeter->measuring_R(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x0B:
            prepare_param();
            ok = multimeter->measuring_C(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x0C:
            prepare_param();
            ok = multimeter->measuring_L(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x0E:
            prepare_param();
            ok = multimeter->measuring_F(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x0F:
            prepare_param();
            ok = multimeter->measuring_P(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x10:
            prepare_param();
            ok = multimeter->measuring_Q(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                counter_error++;
            }
            break;

        case V764_2_CMD_0x11:
            prepare_param();
            ok = multimeter->measuring_S(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        case V764_2_CMD_0x12:
            prepare_param();
            ok = multimeter->measuring_PF(&value);
            if(ok)
            {
#ifndef NO_GRAPHER
                add_grapher_value(value);
#endif
            }
            else
            {
                multimeter->print_last_error();
                counter_error++;
            }
            break;

        default:
            break;
        }
        if(ui->cb_UnlimError->isChecked() == false)
        {
            if(counter_error > ui->sb_MaxError->value())
                flag_measuring = false;
        }
    }

    multimeter->remote_control_off();
    measuring_finished();
}
//--------------------------------------------------------------------------------
void MultimeterBox::stop_measuring(void)
{
    emit debug("MultimeterBox::stop_measuring()");
    flag_measuring = false;
}
//--------------------------------------------------------------------------------
void MultimeterBox::power(bool state)
{
    block_interface(state);
    if(state)
        emit start();
    else
        emit stop();
}
//--------------------------------------------------------------------------------
#ifdef FAKE
void MultimeterBox::createFakeBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *fakeBar = new QToolBar(tr("fakebar"));
    mw->addToolBar(Qt::TopToolBarArea, fakeBar);

    QToolButton *btn_fake = add_button(fakeBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "fake",
                                       "fake");

    connect(btn_fake, SIGNAL(pressed()), this, SLOT(fake()));
}
#endif
//--------------------------------------------------------------------------------
#ifdef FAKE
void MultimeterBox::fake(void)
{
    QSpinBox *serno = new QSpinBox();
    QSpinBox *year  = new QSpinBox();
    QSpinBox *address = new QSpinBox();
    QSpinBox *channel = new QSpinBox();

    serno->setMinimum(1);
    serno->setMaximum(9999);
    serno->setValue(multimeter->get_serno());

    year->setMinimum(2000);
    year->setMaximum(2020);
    year->setValue(multimeter->get_year());

    address->setMinimum(1);
    address->setMaximum(255);
    address->setValue(multimeter->get_address());

    channel->setMinimum(1);
    channel->setMaximum(15);
    channel->setValue(multimeter->get_channel());

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(new QLabel("serno"), 0, 0);
    grid->addWidget(serno, 0, 1);
    grid->addWidget(new QLabel("year"), 1, 0);
    grid->addWidget(year,  1, 1);
    grid->addWidget(new QLabel("address"), 2, 0);
    grid->addWidget(address, 2, 1);
    grid->addWidget(new QLabel("channel"), 3, 0);
    grid->addWidget(channel, 3, 1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(grid);
    vbox->addWidget(buttonBox);

    QDialog *dlg = new QDialog();
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));

    dlg->setLayout(vbox);
    dlg->setFixedSize(dlg->sizeHint());
    int btn = dlg->exec();
    if(btn != QDialog::Accepted) return;

#ifdef QT_DEBUG
    emit debug(QString("serno %1").arg(serno->value()));
    emit debug(QString("year %1").arg(year->value()));
    emit debug(QString("address %1").arg(address->value()));
    emit debug(QString("channel %1").arg(channel->value()));
#endif

    multimeter->set_fake_serno(serno->value());
    multimeter->set_fake_year(year->value());
    multimeter->set_fake_address(address->value());
    multimeter->set_fake_channel(channel->value());
}
#endif
//--------------------------------------------------------------------------------
void MultimeterBox::block_interface(bool state)
{
    ui->cb_auto->setDisabled(state);
    ui->cb_deviation->setDisabled(state);
    ui->cb_limits->setDisabled(state);
    ui->cb_measurement->setDisabled(state);
    ui->cb_retention->setDisabled(state);
    ui->cb_UnlimError->setDisabled(state);
    ui->sb_MaxError->setDisabled(state);

    ui->sb_SerNo->setDisabled(state);
    ui->sb_Year->setDisabled(state);
    ui->sb_Address->setDisabled(state);
    ui->sb_Channel->setDisabled(state);

    if(ui->cb_auto->isChecked()) ui->cb_limits->setDisabled(true);
    if(ui->cb_UnlimError->isChecked()) ui->sb_MaxError->setDisabled(true);
}
//--------------------------------------------------------------------------------
void MultimeterBox::search_devices(void)
{
    bool ok = false;

    emit info(tr("Поиск устройства"));

    QString port_name;
    ok = multimeter->find_device(&port_name);
    if(!ok) emit error("USB приемо-передатчик мультиметра В7-86/2 НЕ найден");

    if(ok)
    {
        block_interface(false);
        ui->btn_power->setEnabled(true);
    }
    emit info(tr("Поиск устройства закончен"));
}
//--------------------------------------------------------------------------------
#ifndef NO_GRAPHER
void MultimeterBox::init_grapherbox(void)
{
#if 0
    QColor color = Qt::red;
    QColor color_background = Qt::yellow;
    QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse,
                                      QBrush(color_background),
                                      QPen(color, 2),
                                      QSize(8, 8));
#endif

#ifdef USE_SCALE_POINT_DATETIME
    grapher = new GrapherBox(this);
    grapher->set_title("График");
    grapher->set_axis_scale_x(QDateTime::currentDateTime().toTime_t(),
                              QDateTime::currentDateTime().toTime_t()+100);
    grapher->set_axis_scale_y(-100, 100);

#elif USE_SCALE_POINT_TIME
    int x = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
    grapher = new GrapherBox(this);
    grapher->set_title("График");
    grapher->set_axis_scale_x(x, x+100);
    grapher->set_axis_scale_y(-100, 100);
#else
    grapher = new GrapherBox(tr("График"), tr(""), tr(""), this);
#endif

    grapher->setMinimumSize(640, 480);
    curve_UDC = grapher->add_curve(tr("UDC"));
    curve_UAC = grapher->add_curve(tr("UAC"));
    curve_IDC = grapher->add_curve(tr("IDC"));
    curve_IAC = grapher->add_curve(tr("IAC"));
    curve_R = grapher->add_curve(tr("R"));
    curve_C = grapher->add_curve(tr("C"));
    curve_L = grapher->add_curve(tr("L"));
    curve_F = grapher->add_curve(tr("F"));
    curve_P = grapher->add_curve(tr("P"));
    curve_Q = grapher->add_curve(tr("Q"));
    curve_S = grapher->add_curve(tr("S"));
    curve_PF = grapher->add_curve(tr("PF"));

    ui->layout_grapher->addWidget(grapher);

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);
}
#endif
//--------------------------------------------------------------------------------
void MultimeterBox::measurement_changed(int index)
{
    switch(index)
    {
    case 0: //измерение постоянного напряжения
        ui->cb_limits->clear();
        ui->cb_limits->addItem(tr("200 мВ"));
        ui->cb_limits->addItem(tr("2 В"));
        ui->cb_limits->addItem(tr("20 В"));
        ui->cb_limits->addItem(tr("200 В"));
        ui->cb_limits->addItem(tr("1000 В"));
        ui->cb_auto->setEnabled(true);
        ui->cb_limits->setEnabled(!ui->cb_auto->isChecked());
        break;

    case 1: //измерение переменного напряжения
        ui->cb_limits->clear();
        ui->cb_limits->addItem(tr("200 мВ"));
        ui->cb_limits->addItem(tr("2 В"));
        ui->cb_limits->addItem(tr("20 В"));
        ui->cb_limits->addItem(tr("200 В"));
        ui->cb_limits->addItem(tr("750 В"));
        ui->cb_auto->setEnabled(true);
        ui->cb_limits->setEnabled(!ui->cb_auto->isChecked());
        break;

    case 2: //измерение постоянного тока
        ui->cb_limits->clear();
        ui->cb_limits->addItem(tr("200 мкА"));
        ui->cb_limits->addItem(tr("2 мА"));
        ui->cb_limits->addItem(tr("20 мА"));
        ui->cb_limits->addItem(tr("200 мА"));
        ui->cb_limits->addItem(tr("2000 мА"));
        ui->cb_limits->addItem(tr("20 А"));
        ui->cb_auto->setEnabled(true);
        ui->cb_limits->setEnabled(!ui->cb_auto->isChecked());
        break;

    case 3: //измерение переменного тока
        ui->cb_limits->clear();
        ui->cb_limits->addItem(tr("2 мА"));
        ui->cb_limits->addItem(tr("20 мА"));
        ui->cb_limits->addItem(tr("200 мА"));
        ui->cb_limits->addItem(tr("2000 мА"));
        ui->cb_limits->addItem(tr("20 А"));
        ui->cb_auto->setEnabled(true);
        ui->cb_limits->setEnabled(!ui->cb_auto->isChecked());
        break;

    case 4: //измерение сопротивления
        ui->cb_limits->clear();
        ui->cb_limits->addItem(tr("200 Ом"));
        ui->cb_limits->addItem(tr("2 кОм"));
        ui->cb_limits->addItem(tr("20 кОм"));
        ui->cb_limits->addItem(tr("200 кОм"));
        ui->cb_limits->addItem(tr("2 МОм"));
        ui->cb_auto->setEnabled(true);
        ui->cb_limits->setEnabled(!ui->cb_auto->isChecked());
        break;

    case 5: //измерение емкости
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 6: //измерение индуктивности
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 7: //прозвонка
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 8: //измерение частоты
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 9: //измерение активной мощности
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 10: //измерение реактивной мощности
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 11: //измерение полной мощности
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    case 12: //измерение коэффициента мощности
        ui->cb_limits->clear();
        ui->cb_limits->setEnabled(false);
        ui->cb_auto->setEnabled(false);
        break;

    default:
        ui->cb_limits->clear();
        emit error(QString(tr("странный индекс %1")).arg(index));
        break;

    }
}
//--------------------------------------------------------------------------------
void MultimeterBox::msgError(const QString &message)
{
    QMessageBox::critical(this, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
void MultimeterBox::measuring_finished(void)
{
    emit info(tr("Измерения завершены"));
    block_interface(false);
    ui->btn_power->setChecked(false);
}
//--------------------------------------------------------------------------------
#ifndef NO_GRAPHER
void MultimeterBox::add_grapher_value(double value)
{
#ifdef USE_SCALE_POINT_DATETIME
    uint x = QDateTime::currentDateTime().toTime_t();
#elif USE_SCALE_POINT_TIME
    uint x = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
#else
    uint x = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
#endif

    bool ok = false;
    int cmd = get_measurement(&ok);
    if(!ok) return;
    switch(cmd)
    {
    case V764_2_CMD_0x06:
        grapher->add_curve_data(curve_UDC, x, value);
        break;
    case V764_2_CMD_0x07:
        grapher->add_curve_data(curve_UAC, x, value);
        break;
    case V764_2_CMD_0x08:
        grapher->add_curve_data(curve_IDC, x, value);
        break;
    case V764_2_CMD_0x09:
        grapher->add_curve_data(curve_IDC, x, value);
        break;
    case V764_2_CMD_0x0A:
        grapher->add_curve_data(curve_R, x, value);
        break;
    case V764_2_CMD_0x0B:
        grapher->add_curve_data(curve_C, x, value);
        break;
    case V764_2_CMD_0x0C:
        grapher->add_curve_data(curve_L, x, value);
        break;
    case V764_2_CMD_0x0E:
        grapher->add_curve_data(curve_F, x, value);
        break;
    case V764_2_CMD_0x0F:
        grapher->add_curve_data(curve_P, x, value);
        break;
    case V764_2_CMD_0x10:
        grapher->add_curve_data(curve_Q, x, value);
        break;
    case V764_2_CMD_0x11:
        grapher->add_curve_data(curve_S, x, value);
        break;
    case V764_2_CMD_0x12:
        grapher->add_curve_data(curve_PF, x, value);
        break;
    }
}
#endif
//--------------------------------------------------------------------------------
void MultimeterBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
