/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "oscilloscope_controls.hpp"
#include "ui_oscilloscope_controls.h"
//--------------------------------------------------------------------------------
Oscilloscope_controls::Oscilloscope_controls(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Oscilloscope_controls)
{
    init();
}
//--------------------------------------------------------------------------------
Oscilloscope_controls::~Oscilloscope_controls()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::init(void)
{
    ui->setupUi(this);

    ui->tb_CH1->setPalette(QPalette(Qt::red));
    ui->tb_CH2->setPalette(QPalette(Qt::green));
    ui->tb_CH3->setPalette(QPalette(Qt::blue));
    ui->tb_CH4->setPalette(QPalette(Qt::magenta));

//    connect(ui->tb_CH1,     SIGNAL(clicked(bool)),  this,   SLOT(click_color_CH1()));
//    connect(ui->tb_CH2,     SIGNAL(clicked(bool)),  this,   SLOT(click_color_CH2()));
//    connect(ui->tb_CH3,     SIGNAL(clicked(bool)),  this,   SLOT(click_color_CH3()));
//    connect(ui->tb_CH4,     SIGNAL(clicked(bool)),  this,   SLOT(click_color_CH4()));

    ui->tb_CH1->setProperty("curve_ID", 1);
    ui->tb_CH2->setProperty("curve_ID", 2);
    ui->tb_CH3->setProperty("curve_ID", 3);
    ui->tb_CH4->setProperty("curve_ID", 4);

    connect(ui->tb_CH1,     SIGNAL(clicked(bool)),  this,   SLOT(set_color()));
    connect(ui->tb_CH2,     SIGNAL(clicked(bool)),  this,   SLOT(set_color()));
    connect(ui->tb_CH3,     SIGNAL(clicked(bool)),  this,   SLOT(set_color()));
    connect(ui->tb_CH4,     SIGNAL(clicked(bool)),  this,   SLOT(set_color()));

    connect(ui->btn_CH1,    SIGNAL(clicked(bool)),  this,   SLOT(click_CH1()));
    connect(ui->btn_CH2,    SIGNAL(clicked(bool)),  this,   SLOT(click_CH2()));
    connect(ui->btn_CH3,    SIGNAL(clicked(bool)),  this,   SLOT(click_CH3()));
    connect(ui->btn_CH4,    SIGNAL(clicked(bool)),  this,   SLOT(click_CH4()));

    connect(ui->btn_RUN,    SIGNAL(clicked(bool)),  this,   SIGNAL(click_RUN()));

    connect(ui->knob_position,  SIGNAL(valueChanged(double)),   this,   SIGNAL(knob_position_changed(double)));
    connect(ui->knob_multiply,  SIGNAL(valueChanged(double)),   this,   SIGNAL(knob_multiply_changed(double)));
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_position(double value)
{
    ui->knob_position->setValue(value);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_multiply(double value)
{
    ui->knob_multiply->setValue(value);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_max_position(double max_value)
{
    if(max_value <= 0)
    {
        return;
    }
    ui->knob_position->setUpperBound(max_value);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_max_multiply(double max_value)
{
    if(max_value <= 0)
    {
        return;
    }
    ui->knob_multiply->setUpperBound(max_value);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_background_channel(int channel, bool state)
{
    switch(channel)
    {
    case 0:
        if(state)
            ui->btn_CH1->setStyleSheet("background:red;");
        else
            ui->btn_CH1->setStyleSheet("");
        break;

    case 1:
        if(state)
            ui->btn_CH2->setStyleSheet("background:green;");
        else
            ui->btn_CH2->setStyleSheet("");
        break;

    case 2:
        if(state)
            ui->btn_CH3->setStyleSheet("background:blue;");
        else
            ui->btn_CH3->setStyleSheet("");
        break;

    case 3:
        if(state)
            ui->btn_CH4->setStyleSheet("background:magenta;");
        else
            ui->btn_CH4->setStyleSheet("");
        break;
    }
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_state_RUN(bool state)
{
    ui->btn_RUN->setText(state ? "STOP" : "RUN");
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::set_color(void)
{
    QToolButton *button = dynamic_cast<QToolButton *>(sender());
    if(button == nullptr)
    {
        return;
    }
    emit debug(QString("ID %1").arg(button->property("curve_ID").toInt()));
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_color_CH1(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_color_CH2(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_color_CH3(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_color_CH4(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_CH1(void)
{
    emit click_channel(0);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_CH2(void)
{
    emit click_channel(1);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_CH3(void)
{
    emit click_channel(2);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::click_CH4(void)
{
    emit click_channel(3);
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Oscilloscope_controls::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Oscilloscope_controls::save_setting(void)
{

}
//--------------------------------------------------------------------------------
