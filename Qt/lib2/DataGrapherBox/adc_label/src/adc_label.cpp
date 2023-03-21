/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "adc_label.hpp"
#include "ui_adc_label.h"
//--------------------------------------------------------------------------------
ADC_label::ADC_label(QString lbl_name, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ADC_label)
{
    if(lbl_name.isEmpty())  lbl_name = "ADC";
    label = lbl_name;

    init();
}
//--------------------------------------------------------------------------------
ADC_label::~ADC_label()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void ADC_label::init(void)
{
    ui->setupUi(this);

    ui->lbl_name->setText(label);
    ui->lcd_value->setFixedSize(300, 48);
    //ui->lcd_value->setMinimumHeight(48);
    ui->lcd_value->setDigitCount(14);

    ui->lcd_value->setProperty(NO_SAVE, true);
}
//--------------------------------------------------------------------------------
void ADC_label::set_label_text(QString text)
{
    ui->lbl_name->setText(text);
}
//--------------------------------------------------------------------------------
void ADC_label::display(double num)
{
    ui->lcd_value->display(num);
}
//--------------------------------------------------------------------------------
void ADC_label::display(int num)
{
    ui->lcd_value->display(num);
}
//--------------------------------------------------------------------------------
void ADC_label::display(const QString &s)
{
    ui->lcd_value->display(s);
}
//--------------------------------------------------------------------------------
void ADC_label::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool ADC_label::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void ADC_label::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void ADC_label::save_setting(void)
{

}
//--------------------------------------------------------------------------------
