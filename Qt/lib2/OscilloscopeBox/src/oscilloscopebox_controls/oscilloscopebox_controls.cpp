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
#include "oscilloscopebox_controls.hpp"
#include "ui_oscilloscopebox_controls.h"
//--------------------------------------------------------------------------------
Oscilloscopebox_controls::Oscilloscopebox_controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Oscilloscopebox_controls)
{
    init();;
}
//--------------------------------------------------------------------------------
Oscilloscopebox_controls::~Oscilloscopebox_controls()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Oscilloscopebox_controls::init(void)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
bool Oscilloscopebox_controls::add_control(QColor color, QString text)
{
    CURVE_BTNS btns;
    btns.btn_color = new QToolButton(this);
    btns.btn_select = new QPushButton(this);

    btns.btn_color->setStyleSheet(QString("background:rgb(%1,%2,%3);")
                                  .arg(color.red())
                                  .arg(color.green())
                                  .arg(color.blue()));
    btns.btn_color->setProperty("index", curves);

    btns.btn_select->setText(text);
    btns.btn_select->setCheckable(true);
    btns.btn_select->setProperty("index", curves);

    connect(btns.btn_color,     SIGNAL(clicked(bool)),  this,   SLOT(click_color()));
    connect(btns.btn_select,    SIGNAL(clicked(bool)),  this,   SLOT(click_select(bool)));

    //connect(btns.btn_select,    SIGNAL(clicked(bool)),  this,   SLOT(check_buttons(bool)));

    ui->grid->addWidget(btns.btn_color,  curves, 0);
    ui->grid->addWidget(btns.btn_select, curves, 1);
    curves++;

    l_curves.append(btns);
    return true;
}
//--------------------------------------------------------------------------------
void Oscilloscopebox_controls::check_buttons(bool state)
{
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    current_index = btn->property("index").toInt();
    foreach (CURVE_BTNS btns, l_curves)
    {
        if(btns.btn_select == btn)
        {
            btns.btn_select->setChecked(state);
        }
        else
        {
            btns.btn_select->setChecked(false);
        }
    }
}
//--------------------------------------------------------------------------------
int Oscilloscopebox_controls::get_active_index(void)
{
    return current_index;
}
//--------------------------------------------------------------------------------
void Oscilloscopebox_controls::click_color(void)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    int index = btn->property("index").toInt();

    emit s_color(index);
}
//--------------------------------------------------------------------------------
void Oscilloscopebox_controls::click_select(bool state)
{
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    current_index = btn->property("index").toInt();
    foreach (CURVE_BTNS btns, l_curves)
    {
        if(btns.btn_select == btn)
        {
            btns.btn_select->setChecked(state);
        }
        else
        {
            btns.btn_select->setChecked(false);
        }
    }

    emit s_select(state);
}
//--------------------------------------------------------------------------------
bool Oscilloscopebox_controls::set_curve_color(int index, QColor color)
{
    if(index < 0) return false;
    if(index >= curves) return false;

    l_curves[index].btn_color->setStyleSheet(QString("background:rgb(%1,%2,%3);")
                                             .arg(color.red())
                                             .arg(color.green())
                                             .arg(color.blue()));
    return true;
}
//--------------------------------------------------------------------------------
bool Oscilloscopebox_controls::set_curve_text(int index, QString text)
{
    if(index < 0) return false;
    if(index >= curves) return false;

    l_curves[index].btn_select->setText(text);
    return true;
}
//--------------------------------------------------------------------------------
