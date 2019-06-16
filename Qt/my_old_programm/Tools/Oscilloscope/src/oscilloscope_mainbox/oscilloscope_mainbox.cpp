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
#include "ui_oscilloscope_mainbox.h"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "oscilloscope_mainbox.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
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
void MainBox::init()
{
    ui->setupUi(this);

    grapher = new GrapherBox(this);
    grapher->set_title("осциллограф");
    grapher->set_title_axis_X("время");
    grapher->set_title_axis_Y("напряжение");
    grapher->set_axis_scale_x(0, SIZE);
    grapher->set_axis_scale_y(-MAX_VOLTAGE, MAX_VOLTAGE);

    curve_voltage = grapher->add_curve(tr("напряжение"));
    curve_interference = grapher->add_curve(tr("помеха"));
    curve_envelope = grapher->add_curve(tr("огибающая"));

    ui->frame->setFixedSize(ui->frame->sizeHint());

    ui->grapher_layout->addWidget(grapher);

    ui->Voltage->setScaleMaxMajor(10);
    ui->Voltage->setScaleMaxMinor(10);

    ui->Voltage_Noice->setScaleMaxMajor(10);
    ui->Voltage_Noice->setScaleMaxMinor(10);

    ui->Voltage->setUpperBound(MAX_VOLTAGE);
    ui->Voltage->setValue(NORMAL_VOLTAGE);
    ui->Voltage_Value->display(NORMAL_VOLTAGE);

    ui->Voltage_Noice->setUpperBound(MAX_NOISE);
    ui->Voltage_Noice->setValue(NORMAL_NOISE);
    ui->Voltage_Noice_Value->display(NORMAL_NOISE);

    updateText();

    calc(ui->Voltage->value());

    connect(ui->Voltage, SIGNAL(valueChanged(double)), ui->Voltage_Value, SLOT(display(double)));
    connect(ui->Voltage, SIGNAL(sliderReleased()), this, SLOT(updateGraphics()));
    //connect(ui->Voltage, SIGNAL(valueChanged(double)), this, SLOT(calc(double)));

    connect(ui->Voltage_Noice, SIGNAL(valueChanged(double)), ui->Voltage_Noice_Value, SLOT(display(double)));
    connect(ui->Voltage_Noice, SIGNAL(sliderReleased()), this, SLOT(updateGraphics()));

    drawGraphics();
}
//--------------------------------------------------------------------------------
void MainBox::calc(double voltage)
{
    int n;
    int local_max = 0;

    for(n=0; n<SIZE; n++)
    {
        y[n] = voltage*qSin(double(n)*double(M_PI)/double(180.0));
        y2[n]=0;
        y3[n]=0;
    }
    //qDebug() << "voltage = " << voltage;
    n=0;
    int voltage_noice = ui->Voltage_Noice->value();
    while(n<SIZE)
    {
        if(y[n] > 0)
        {
            y[n]+=voltage_noice;
        }
        else
        {
            y[n]-=voltage_noice;
        }
        n+=7;
    }

    double temp = 0;
    double temp2 = 0;
    for(n=0; n<SIZE; n++)
    {
        if(!(n%18))
        {
            if(y[n]>0) temp2+=y[n];
        }
        if((n%18) < 3)
        {
            if(y[n]>0) y2[n] = 380.0f;
            else  y2[n] = -380.0f;

            if(y[n] > temp) temp = y[n];
        }
        else
        {
            y2[n] = 0;
        }
    }
    //qDebug() << temp/qSqrt(2.0f);

    for(n=0; n<SIZE; n++)
    {
        if((n%18) < 3)
        {
            if(y[n]>0) y2[n] = 380.0f;
            else  y2[n] = -380.0f;

            if(y[n] > 0)
            {
                if(y[n] > local_max) local_max = y[n];
            }
            else
            {
                if(y[n] < local_max) local_max = y[n];
            }

            if(y[n] > temp) temp = y[n];
        }
        else
        {
            y3[n-2] = local_max;
            local_max = 0;
        }
    }

    max_U = 0;
    for(n=0; n<SIZE; n++)
    {
        current_U = y3[n]/qSqrt(2.0f);
        if(current_U > max_U) max_U = current_U;
    }

    grapher->set_silense(true);
    grapher->clear();
    for(int n=0; n<SIZE; n++)
    {
        grapher->add_curve_data(curve_voltage, y[n]);
        grapher->add_curve_data(curve_interference, y2[n]);
        grapher->add_curve_data(curve_envelope, y3[n]);
    }
    grapher->set_silense(false);
}
//--------------------------------------------------------------------------------
void MainBox::drawGraphics()
{
    ui->Voltage_Operable->display(max_U);
}
//--------------------------------------------------------------------------------
void MainBox::updateGraphics()
{
    calc(ui->Voltage->value());
    drawGraphics();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
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
