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
#include <QDoubleSpinBox>
#include <QToolButton>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "v786_2_multimeter.hpp"
#include "v786_2_packets.hpp"
#include "v786_2_union.hpp"
//--------------------------------------------------------------------------------
#include "serialbox5.hpp"
//--------------------------------------------------------------------------------
#include "H4_X_calibrator.hpp"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "grapherbox.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
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

    createTestBar();

    ui->sb_channel->setMinimum(1);
    ui->sb_channel->setMaximum(15);

    ui->cb_TypeMultimeter->clear();
    ui->cb_TypeMultimeter->addItem(tr("В7-86"));
    ui->cb_TypeMultimeter->addItem(tr("В7-86/1"));
    ui->cb_TypeMultimeter->addItem(tr("В7-86/2"));
    ui->cb_TypeMultimeter->setCurrentIndex(2);

    ui->cb_TypeCalibrator->clear();
    ui->cb_TypeCalibrator->addItem(tr("H4-6"));
    ui->cb_TypeCalibrator->addItem(tr("H4-7"));

    ui->sb_time_calibration->setMinimum(100);
    ui->sb_time_calibration->setMaximum(65535);
    ui->sb_time_calibration->setValue(3000);

    ui->sb_time_radiomodule->setMinimum(100);
    ui->sb_time_radiomodule->setMaximum(65535);
    ui->sb_time_radiomodule->setValue(15000);

    ui->cb_Limit->clear();
    ui->cb_Limit->addItem(tr("200 мВ"));
    ui->cb_Limit->addItem(tr("2 В"));
    ui->cb_Limit->addItem(tr("20 В"));
    ui->cb_Limit->addItem(tr("200 В"));
    ui->cb_Limit->addItem(tr("1000 В"));

    grapher = new GrapherBox(this);
    grapher->set_title("АЧХ");
    grapher->set_title_axis_X("время");
    grapher->set_title_axis_Y("напряжение");
    grapher->set_axis_scale_x(0, 2000);
    grapher->set_axis_scale_y(-10, 10);

    grapher->add_curve(tr("Данные"));
    grapher->add_curve(tr("Идеал"));

    serialPort_calibrator = new SerialBox5(this, "Калибратор (RS-232)", "Калибратор (RS-232)");

    calibrator = new Calibrator(this);
    multimeter = new Multimeter_V786_2(this);

    ui->layout_grapher->addWidget(grapher);

    btn_find_multimeter = new QPushButton(tr("Найти мультиметр"));
    connect(btn_find_multimeter, SIGNAL(clicked()), this, SLOT(search_devices()));

    ui->layout_serial->addWidget(btn_find_multimeter);
    ui->layout_serial->addWidget(serialPort_calibrator);
    ui->layout_serial->addStretch();

    connect(ui->btn_Start, SIGNAL(clicked()), this, SLOT(run()));
    connect(ui->sb_TestValue, SIGNAL(editingFinished()), this, SLOT(change_limit()));
}
//--------------------------------------------------------------------------------
void MainBox::search_devices(void)
{
    bool ok = false;

    emit info(tr("Поиск устройства"));

    QString port_name;
    ok = multimeter->find_device(&port_name);
    if(!ok) emit error("Мультиметр В7-86/2 НЕ найден");

    emit info(tr("Поиск устройства закончен"));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
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
void MainBox::run(void)
{
    answers.clear();
    prepare_step_1_0(ui->cb_TypeCalibrator->currentIndex());

    if(answers.count() == 0)
    {
        msgError(tr("Не выбрано ни одного шага калибровки!"));
        return;
    }

    if(serialPort_calibrator->isOpen() == false)
    {
        msgError(tr("Порт калибратора не открыт!"));
        return;
    }

    if(serial_radiomodule.isOpen() == false)
    {
        msgError(tr("Порт мультиметра В7-86 не открыт!"));
        return;
    }

    if(ui->cb_TypeMultimeter->currentIndex() != 2)
    {
        msgError(tr("В данном мультиметре НЕТ радиомодуля!"));
        return;
    }

    emit info(tr("Запрос статуса прибора"));
    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_SerYear->value());
    multimeter->set_address(ui->sb_address->value());
    multimeter->set_channel(ui->sb_channel->value());
    multimeter->set_timeout(ui->sb_time_calibration->value());

    bool ok = multimeter->get_state();
    if(!ok)
    {
        multimeter->print_last_error();
        return;
    }

    emit info(tr("Включение удаленного управления"));

    ok = multimeter->remote_control_on();
    if(!ok)
    {
        multimeter->print_last_error();
        return;
    }

    QByteArray temp;
    for(int n=0; n<answers.count(); n++)
    {
        if(calibrator->check_calibrator())
        {
            temp.clear();
            temp.append(QString("%1").arg(answers.at(n).calibrator_question));

            if(answers.at(n).calibrator_step_by_step)
            {
                calibrator->step_to_step(answers.at(n).calibrator_question,
                                         500,
                                         ui->sb_time_calibration->value(),
                                         false);
            }
            else
            {
                calibrator->set_timeout(ui->sb_time_calibration->value());
                calibrator->send(temp);
            }
            emit debug(answers.at(n).calibrator_question);
            ok = multimeter->find_cmd(answers.at(n).cmd);
            if(!ok) break;

#if 0
            bool ok = false;
            float value = multimeter->get_value(&ok);
            if(ok)
            {
                //qDebug() << "answers.at(0).calibrator_freq" << answers.at(n).calibrator_freq;
                //???
                grapher->append(0, answers.at(n).calibrator_freq, value);
                float test_value = ui->sb_TestValue->value();
                if(test_value >= 0.1f)
                    grapher->append(1, answers.at(n).calibrator_freq, test_value);
                else
                    grapher->append(1, answers.at(n).calibrator_freq, test_value * 1000.0f);
            }
#endif
        }
    }

    calibrator->set_timeout(ui->sb_time_calibration->value());
    calibrator->reset_U();

    emit info(tr("Отключение удаленного управления"));
    multimeter->set_serno(ui->sb_SerNo->value());
    multimeter->set_year(ui->sb_SerYear->value());
    multimeter->set_address(ui->sb_address->value());
    multimeter->set_channel(ui->sb_channel->value());
    multimeter->set_timeout(ui->sb_time_calibration->value());

    multimeter->remote_control_off();

    msgInfo(tr("Измерения закончены!"));
}
//--------------------------------------------------------------------------------
QString MainBox::prepare_string(const QString &prefix,
                                float value,
                                float limit,
                                int step)
{
    QString temp;
    float freq = 0.0f;

    if(step == 0)
    {
        value = limit / 10.0f;
        freq = 20.0f;
    }
    if(step == 1)
    {
        value = limit;
        freq = 20.0f;
    }
    if(step == 2)
    {
        freq = 20.0f;
    }
    if(step == 3)
    {
        freq = 50.0f;
    }
    if(step >= 4 && step <=13)
    {
        freq = 100.0f + (step - 4) * 100.f;
    }
    if(step >= 14 && step <=31)
    {
        freq = 1500.0f + (step - 14) * 500.f;
    }
    if(step >= 32 && step <=49)
    {
        freq = 15000.0f + (step - 32) * 5000.f;
    }

    temp.append(QString("%1%2F%3\r\n")
                .arg(prefix)
                .arg(value)
                .arg(freq / 1000.0f));
    return temp;
}
//--------------------------------------------------------------------------------
void MainBox::prepare_step_1_0(int calibrator_type)
{
    QString before_string;
    before_string.clear();

    questions_data a_data;

    switch(calibrator_type)
    {
    case H4_6:
        for(int n=0; n<=49; n++)
        {
            a_data.calibrator_question=prepare_string("UA", 0.199f, 0.20f, n);
            a_data.cmd = V764_2_CMD_0x07;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_6;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 0;
            a_data.step_c = n;

            answers.append(a_data);
        }
        break;

    case H4_7:
        for(int n=0; n<=49; n++)
        {
            a_data.calibrator_question=prepare_string("UA", 0.199f, 0.20f, n);
            a_data.cmd = V764_2_CMD_0x07;
            a_data.cable_in = CABLE_IN;
            a_data.calibrator_type = H4_7;
            a_data.calibrator_step_by_step = false;
            a_data.need_amplifier_U = false;
            a_data.need_amplifier_I_DC = false;
            a_data.need_amplifier_I_AC = false;
            a_data.calibrator_question_before = before_string;
            a_data.step_a = 1;
            a_data.step_b = 0;
            a_data.step_c = n;

            answers.append(a_data);
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::msgInfo(const QString &message)
{
    QMessageBox::information(this, tr("Информация"), message);
}
//--------------------------------------------------------------------------------
void MainBox::msgError(const QString &message)
{
    QMessageBox::critical(this, tr("Ошибка"), message);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
    for(int n=0; n<1000; n++)
    {
        grapher->add_curve_data(0, ui->sb_TestValue->value() + 0.2f);
        grapher->add_curve_data(1, ui->sb_TestValue->value());
    }
}
//--------------------------------------------------------------------------------
void MainBox::change_limit(void)
{
    float value = ui->sb_TestValue->value();

    if(value <= 0.2f)
    {
        ui->cb_Limit->setCurrentIndex(0);
        return;
    }
    if(value <= 2.0f)
    {
        ui->cb_Limit->setCurrentIndex(1);
        return;
    }
    if(value <= 20.0f)
    {
        ui->cb_Limit->setCurrentIndex(2);
        return;
    }
    if(value <= 200.0f)
    {
        ui->cb_Limit->setCurrentIndex(3);
        return;
    }
    if(value <= 1000.0f)
    {
        ui->cb_Limit->setCurrentIndex(4);
        return;
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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
