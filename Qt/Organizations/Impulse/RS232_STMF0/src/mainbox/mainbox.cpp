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
#include <QProgressDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStyle>
#include <QTime>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "controlbox.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "sleeper.h"

#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#if 0
unsigned int value_data = 0x12345678;
unsigned char d = (value_data) 	     & 0xFF;
unsigned char c = (value_data >> 8)  & 0xFF;
unsigned char b = (value_data >> 16) & 0xFF;
unsigned char a = (value_data >> 24) & 0xFF;

emit info(QString("%1%2%3%4")
          .arg(a, 0, 16)
          .arg(b, 0, 16)
          .arg(c, 0, 16)
          .arg(d, 0, 16));
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    controlBox(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    controlBox = new ControlBox(this);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(controlBox);

    setLayout(vbox);

    createTestBar();
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
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                       "test",
                                       "test");
    QToolButton *btn_reset = add_button(toolBar,
                                        new QToolButton(this),
                                        qApp->style()->standardIcon(QStyle::SP_DialogResetButton),
                                        "reset",
                                        "reset");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_reset, SIGNAL(clicked()), controlBox, SLOT(reset()));
}
//--------------------------------------------------------------------------------
double MainBox::convert_adc_voltage(unsigned int value_adc)
{
    double VREF = 2.5f;
    // return (double) VREF * ((double) value_adc / (double) 8388608.0f - (double) 1.0f) * 1000000.0f;
    return (double) VREF * ((double) value_adc / (double) 8388608.0f - (double) 1.0f);
}
//--------------------------------------------------------------------------------
unsigned int MainBox::convert_voltage_adc(double voltage)
{
    // Code = 2(N – 1) × [(AIN × gain/VREF) + 1]

    double VREF = 2.5f;
    if(voltage > VREF)
    {
        emit error("voltage > VREF");
        return -1;
    }

    return qPow(2, 23) * ((voltage * 1/VREF) + 1);
}
//--------------------------------------------------------------------------------
void MainBox::calc_data(void)
{
    /*
    1 11744051
    2 15099494
    0.5 10066329
    0.4 9730785
    0.25 9227468
    0.125 8808038
    0.0625 8598323
    0.05 8556380
    0.03125 8493465
    0.025 8472494
    0.015625 8441036
    0.0125 8430551
    0.00625 8409579
    0.003125 8399093
    */

    //unsigned int value_adc = 8388608;
    unsigned int value_adc = 4246112;
    double voltage = convert_adc_voltage(value_adc);
    emit debug(QString("value_adc %1 voltage %2")
               .arg(value_adc)
               .arg(voltage));
    voltage = -1.23456f;
    value_adc = convert_voltage_adc(voltage);
    emit debug(QString("voltage %1 value_adc %2")
               .arg(voltage)
               .arg(value_adc));
    emit info(" ");

    double value_05  = 1.0f;
    double value_1   = 2.0f;
    double value_4   = 2.0f / 4.0f;
    double value_5   = 2.0f / 5.0f;
    double value_8   = 2.0f / 8.0f;
    double value_16  = 2.0f / 16.0f;
    double value_32  = 2.0f / 32.0f;
    double value_40  = 2.0f / 40.0f;
    double value_64  = 2.0f / 64.0f;
    double value_80  = 2.0f / 80.0f;
    double value_128 = 2.0f / 128.0f;
    double value_160 = 2.0f / 160.0f;
    double value_320 = 2.0f / 320.0f;
    double value_640 = 2.0f / 640.0f;

    unsigned int value_adc_05  = convert_voltage_adc(value_05);
    unsigned int value_adc_1   = convert_voltage_adc(value_1);
    unsigned int value_adc_4   = convert_voltage_adc(value_4);
    unsigned int value_adc_5   = convert_voltage_adc(value_5);
    unsigned int value_adc_8   = convert_voltage_adc(value_8);
    unsigned int value_adc_16  = convert_voltage_adc(value_16);
    unsigned int value_adc_32  = convert_voltage_adc(value_32);
    unsigned int value_adc_40  = convert_voltage_adc(value_40);
    unsigned int value_adc_64  = convert_voltage_adc(value_64);
    unsigned int value_adc_80  = convert_voltage_adc(value_80);
    unsigned int value_adc_128 = convert_voltage_adc(value_128);
    unsigned int value_adc_160 = convert_voltage_adc(value_160);
    unsigned int value_adc_320 = convert_voltage_adc(value_320);
    unsigned int value_adc_640 = convert_voltage_adc(value_640);

    emit info(QString("#define VALUE_MAX_05 %1").arg(value_05));
    emit info(QString("#define VALUE_MAX_1 %1").arg(value_1));
    emit info(QString("#define VALUE_MAX_4 %1").arg(value_4));
    emit info(QString("#define VALUE_MAX_5 %1").arg(value_5));
    emit info(QString("#define VALUE_MAX_8 %1").arg(value_8));
    emit info(QString("#define VALUE_MAX_16 %1").arg(value_16));
    emit info(QString("#define VALUE_MAX_32 %1").arg(value_32));
    emit info(QString("#define VALUE_MAX_40 %1").arg(value_40));
    emit info(QString("#define VALUE_MAX_64 %1").arg(value_64));
    emit info(QString("#define VALUE_MAX_80 %1").arg(value_80));
    emit info(QString("#define VALUE_MAX_128 %1").arg(value_128));
    emit info(QString("#define VALUE_MAX_160 %1").arg(value_160));
    emit info(QString("#define VALUE_MAX_320 %1").arg(value_320));
    emit info(QString("#define VALUE_MAX_640 %1").arg(value_640));
    emit info(" ");

    emit info(QString("#define CALIBRATION_MAX_05 %1").arg(value_adc_05));
    emit info(QString("#define CALIBRATION_MAX_1 %1").arg(value_adc_1));
    emit info(QString("#define CALIBRATION_MAX_4 %1").arg(value_adc_4));
    emit info(QString("#define CALIBRATION_MAX_5 %1").arg(value_adc_5));
    emit info(QString("#define CALIBRATION_MAX_8 %1").arg(value_adc_8));
    emit info(QString("#define CALIBRATION_MAX_16 %1").arg(value_adc_16));
    emit info(QString("#define CALIBRATION_MAX_32 %1").arg(value_adc_32));
    emit info(QString("#define CALIBRATION_MAX_40 %1").arg(value_adc_40));
    emit info(QString("#define CALIBRATION_MAX_64 %1").arg(value_adc_64));
    emit info(QString("#define CALIBRATION_MAX_80 %1").arg(value_adc_80));
    emit info(QString("#define CALIBRATION_MAX_128 %1").arg(value_adc_128));
    emit info(QString("#define CALIBRATION_MAX_160 %1").arg(value_adc_160));
    emit info(QString("#define CALIBRATION_MAX_320 %1").arg(value_adc_320));
    emit info(QString("#define CALIBRATION_MAX_640 %1").arg(value_adc_640));
}
//--------------------------------------------------------------------------------
void MainBox::draw_progress(void)
{
    QProgressDialog *progress = new QProgressDialog(this);
    progress->setLabelText(tr("калибровка"));
    progress->setAutoClose(true);
    progress->setMaximum(99);
    for(int n=0; n<100; n++)
    {
        QCoreApplication::processEvents();
        progress->setLabelText(QString(tr("шаг %1")).arg(n));
        progress->setValue(n);
        if(progress->wasCanceled())
        {
            progress->close();
            break;
        }
        Sleeper::msleep(100);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QMessageBox::information(this, tr("info"), tr("test"));

    // emit info("start test");

    // calc_data();
    // draw_progress();

    // float value = 1.0f / 3.0f * 1000000.0f;
    // emit info(QString("%1").arg(value, 0, 'f', 4));

    // emit info("end test");
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
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
