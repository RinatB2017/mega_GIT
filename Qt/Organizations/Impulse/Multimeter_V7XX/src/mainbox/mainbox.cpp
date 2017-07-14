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
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
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
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    flag_measuring(false),
    multimeter_state(UDC),
    multimeter_limit(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete multimeter;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    //createTestBar();
#ifdef MEASURING_CURRENT_IN_SHUNT
    createMeasureCurrentBar();
#endif

    ui->btn_1->setProperty("code",  BUTTON_1);
    ui->btn_2->setProperty("code",  BUTTON_2);
    ui->btn_3->setProperty("code",  BUTTON_3);
    ui->btn_4->setProperty("code",  BUTTON_4);
    ui->btn_5->setProperty("code",  BUTTON_5);
    ui->btn_6->setProperty("code",  BUTTON_6);
    ui->btn_7->setProperty("code",  BUTTON_7);
    ui->btn_8->setProperty("code",  BUTTON_8);
    ui->btn_9->setProperty("code",  BUTTON_9);
    ui->btn_10->setProperty("code", BUTTON_10);
    ui->btn_11->setProperty("code", BUTTON_11);
    ui->btn_12->setProperty("code", BUTTON_12);
    ui->btn_13->setProperty("code", BUTTON_13);
    ui->btn_14->setProperty("code", BUTTON_14);
    ui->btn_15->setProperty("code", BUTTON_15);
    ui->btn_16->setProperty("code", BUTTON_16);

    connect(ui->btn_1,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_2,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_3,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_4,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_5,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_6,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_7,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_8,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_9,  SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_10, SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_11, SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_12, SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_13, SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_14, SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_15, SIGNAL(released()), this, SLOT(send_cmd()));
    connect(ui->btn_16, SIGNAL(released()), this, SLOT(send_cmd()));

    grapher = new GrapherBox(this);
    curve_UDC = grapher->add_curve("UDC");
    curve_UAC = grapher->add_curve("UAC");
    curve_IDC = grapher->add_curve("IDC");
    curve_IAC = grapher->add_curve("IAC");
    curve_R   = grapher->add_curve("R");
#ifdef MEASURING_CURRENT_IN_SHUNT
    curve_I   = grapher->add_curve("I");
#endif
    ui->grapher_layout->addWidget(grapher);

    ui->sb_address->setMinimum(MIN_ADDRESS);
    ui->sb_address->setMaximum(MAX_ADDRESS);
    ui->sb_address->setObjectName("sb_address");

    ui->sb_timeout->setMinimum(1000);
    ui->sb_timeout->setMaximum(10000);

    ui->btn_fibd_device->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

#ifdef V764
    multimeter = new Multimeter_V764();
#endif
#ifdef V7642
    multimeter = new Multimeter_V764_2();
#endif
#ifdef V780
    multimeter = new Multimeter_V780();
#endif
    connect(multimeter, SIGNAL(current_limit(int)), this, SLOT(get_limit(int)));
    connect(multimeter, SIGNAL(measurement_mode(MODE)), this, SLOT(get_measurement_mode(MODE)));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        connect(multimeter, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(multimeter, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(multimeter, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(multimeter, SIGNAL(message(QString)), mw, SIGNAL(message(QString)));
    }

    connect(ui->btn_power, SIGNAL(clicked(bool)), this, SLOT(power(bool)));
    connect(ui->btn_fibd_device, SIGNAL(clicked()), this, SLOT(find_device()));

    connect(this, SIGNAL(start()), this, SLOT(start_measuring()));
    connect(this, SIGNAL(stop()),  this, SLOT(stop_measuring()));
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
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
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
#ifdef MEASURING_CURRENT_IN_SHUNT
void MainBox::createMeasureCurrentBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    QToolBar *measuringBar = new QToolBar(tr("measuringbar"));
    mw->addToolBar(Qt::TopToolBarArea, measuringBar);

    db_voltage_value = new QDoubleSpinBox();
    db_voltage_value->setDecimals(7);
    db_voltage_value->setMinimum(0.0000001f);
    db_voltage_value->setMaximum(1000);
    db_voltage_value->setObjectName("db_voltage_value");

    db_shunt_value = new QDoubleSpinBox();
    db_shunt_value->setDecimals(7);
    db_shunt_value->setMinimum(0.0000001f);
    db_shunt_value->setMaximum(1000);
    db_shunt_value->setObjectName("db_shunt_value");

    db_current_value = new QDoubleSpinBox();
    db_current_value->setDecimals(3);
    db_current_value->setMinimum(0.0000001f);
    db_current_value->setMaximum(1000000);
    db_current_value->setObjectName("db_current_value");
    db_current_value->setReadOnly(true);

    db_shunt_value->setValue(0.0100050);    //???

    measuringBar->addWidget(db_voltage_value);
    measuringBar->addWidget(new QLabel("V / "));
    measuringBar->addWidget(db_shunt_value);
    measuringBar->addWidget(new QLabel("O = "));
    measuringBar->addWidget(db_current_value);
    measuringBar->addWidget(new QLabel("mA"));

    connect(db_voltage_value,   SIGNAL(valueChanged(double)), this, SLOT(calc()));
    connect(db_shunt_value,     SIGNAL(valueChanged(double)), this, SLOT(calc()));
}
#endif
//--------------------------------------------------------------------------------
#ifdef MEASURING_CURRENT_IN_SHUNT
void MainBox::calc(void)
{
    double voltage  = db_voltage_value->value();
    double shunt    = db_shunt_value->value();
    double result   = 0;

    if(shunt == 0) return;
    result = voltage / shunt * 1000.0f;

    db_current_value->setValue(result);
    grapher->add_curve_data(curve_I, result);
}
#endif
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test begin");

    emit info("Test end!");
}
//--------------------------------------------------------------------------------
void MainBox::power(bool state)
{
    if(state)
    {
        emit start();
    }
    else
    {
        emit stop();
    }
}
//--------------------------------------------------------------------------------
void MainBox::find_device(void)
{
    emit debug("find_device");
    multimeter->set_state_silence(true);
    bool ok = multimeter->find_device();
    multimeter->set_state_silence(false);
    if(!ok)
    {
        emit error("divice not found!");
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::get_limit(int limit)
{
    multimeter_limit = limit;
}
//--------------------------------------------------------------------------------
void MainBox::get_measurement_mode(MODE mode)
{
    multimeter_state = mode;
}
//--------------------------------------------------------------------------------
void MainBox::display_measuring_value(int value_uV)
{
    double value;
    QString text;

    //emit trace(QString("display_measuring_value(%1)").arg(value_uV));
#ifdef MEASURING_CURRENT_IN_SHUNT
    db_voltage_value->setValue(value_uV / 1000000.0f); //???
#endif

    switch(multimeter_state)
    {
    case UDC:
        if(qAbs(value_uV) < 1e3)
        {
            value = value_uV;
            text = "uV";
            ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
            break;
        }
        if(qAbs(value_uV) < 1e6)
        {
            value = (double)value_uV / (double)1e3;
            text = "mV";
            ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
            break;
        }
        value = (double)value_uV / (double)1e6;
        text = "V";
        ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
        break;

    case UAC:
        if(qAbs(value_uV) < 1e3)
        {
            value = value_uV;
            text = "uV";
            ui->multimeter_display->setText(QString("~ %1 %2").arg(value).arg(text));
            break;
        }
        if(qAbs(value_uV) < 1e6)
        {
            value = (double)value_uV / (double)1e3;
            text = "mV";
            ui->multimeter_display->setText(QString("~ %1 %2").arg(value).arg(text));
            break;
        }
        value = (double)value_uV / (double)1e6;
        text = "V";
        ui->multimeter_display->setText(QString("~ %1 %2").arg(value).arg(text));
        break;

    case IDC:
        if(qAbs(value_uV) < 1e3)
        {
            value = value_uV;
            text = "uA";
            ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
            break;
        }
        if(qAbs(value_uV) < 1e6)
        {
            value = (double)value_uV / (double)1e3;
            text = "mA";
            ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
            break;
        }
        value = (double)value_uV / (double)1e6;
        text = "A";
        ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
        break;

    case IAC:
        if(qAbs(value_uV) < 1e3)
        {
            value = value_uV;
            text = "uA";
            ui->multimeter_display->setText(QString("~ %1 %2").arg(value).arg(text));
            break;
        }
        if(qAbs(value_uV) < 1e6)
        {
            value = (double)value_uV / (double)1e3;
            text = "mA";
            ui->multimeter_display->setText(QString("~ %1 %2").arg(value).arg(text));
            break;
        }
        value = (double)value_uV / (double)1e6;
        text = "A";
        ui->multimeter_display->setText(QString("~ %1 %2").arg(value).arg(text));
        break;

    case R:
        emit debug(QString("value %1").arg(value_uV));
        if(qAbs(value_uV) < 1e6)
        {
            value = (double)value_uV / (double)1e3;
            text = "O";
            ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
            break;
        }
        if(qAbs(value_uV) < 1e9)
        {
            value = (double)value_uV / (double)1e3;
            text = "kO";
            ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
            break;
        }
        value = (double)value_uV / (double)1e6;
        text = "mO";
        ui->multimeter_display->setText(QString("%1 %2").arg(value).arg(text));
        break;

    case HZ:
        ui->multimeter_display->setText(QString("%1 Hz").arg(value_uV));
        break;

    case MHZ:
        ui->multimeter_display->setText(QString("%1 MHz").arg(value_uV));
        break;

    default:
        ui->multimeter_display->setText(QString("state %1").arg(multimeter_state));
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::start_measuring(void)
{
    emit debug("MultimeterBox::start_measuring()");

    if(flag_measuring) return;

    flag_measuring = true;
    int err = -1;
    multimeter->set_state_silence(true);
    while(flag_measuring)
    {
        QCoreApplication::processEvents();
        emit debug("measuring");
        int value_uV = 0;
        bool ok = multimeter->get_value(&value_uV);
        if(!ok)
        {
            err = multimeter->print_last_error();
#ifdef V764
            if(err == E_V764_OVERLOAD)
            {
                ui->multimeter_display->setText("overload");
            }
#endif
#ifdef V7642
            if(err == E_V764_2_OVERLOAD)
            {
                ui->multimeter_display->setText("overload");
            }
#endif
#ifdef V780
            if(err == E_V780_OVERLOAD)
            {
                ui->multimeter_display->setText("overload");
            }
#endif
            //stop_measuring();
        }
        else
        {
            display_measuring_value(value_uV);
            switch(multimeter_state)
            {
            case UDC: grapher->add_curve_data(curve_UDC, value_uV); break;
            case UAC: grapher->add_curve_data(curve_UAC, value_uV); break;
            case IDC: grapher->add_curve_data(curve_IDC, value_uV); break;
            case IAC: grapher->add_curve_data(curve_IAC, value_uV); break;
            case R:   grapher->add_curve_data(curve_R,   value_uV); break;
            }
        }

        Sleeper::msleep(ui->sb_timeout->value());
    }
    multimeter->set_state_silence(false);
}
//--------------------------------------------------------------------------------
void MainBox::stop_measuring(void)
{
    emit debug("MultimeterBox::stop_measuring()");
    flag_measuring = false;
}
//--------------------------------------------------------------------------------
void MainBox::send_cmd(void)
{
    QPushButton *btn = (QPushButton *)sender();
    QChar code = btn->property("code").toChar();

    //emit info(QString("0x%1").arg(code, 0, 16));
    multimeter->send_cmd(code.toLatin1());
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
