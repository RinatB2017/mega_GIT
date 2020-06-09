/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "simplewidget.hpp"
#include "ui_simplewidget.h"
//--------------------------------------------------------------------------------
SimpleWidget::SimpleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleWidget)
{
    init();
}
//--------------------------------------------------------------------------------
SimpleWidget::~SimpleWidget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
int SimpleWidget::getValue() const
{
    return value;
}

void SimpleWidget::setValue(int value)
{
    this->value = value;
    ui->log_widget->infoLog(QString("value = %1").arg(this->value));
}
//--------------------------------------------------------------------------------
void SimpleWidget::init(void)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window
         | Qt::WindowMinimizeButtonHint
         | Qt::WindowMaximizeButtonHint
         | Qt::CustomizeWindowHint);

    QList<QToolButton *> l_buttons;

    l_buttons.append(ui->btn_0);
    l_buttons.append(ui->btn_1);
    l_buttons.append(ui->btn_2);
    l_buttons.append(ui->btn_3);
    l_buttons.append(ui->btn_4);
    l_buttons.append(ui->btn_5);
    l_buttons.append(ui->btn_6);
    l_buttons.append(ui->btn_7);
    l_buttons.append(ui->btn_8);
    l_buttons.append(ui->btn_9);

    int cnt = 0;
    foreach (QToolButton *btn, l_buttons)
    {
        // btn->setIcon(QIcon(QString(":/white/%1.png").arg(cnt)));
        btn->setProperty("id", cnt++);
        connect(btn,  &QPushButton::clicked,  this,   &SimpleWidget::click);
    }

    connect(ui->btn_close,  &QPushButton::clicked,          this,   &QWidget::close);
    connect(this,           &SimpleWidget::x_value_changed, this,   &SimpleWidget::value_changed);
}
//--------------------------------------------------------------------------------
void SimpleWidget::click(void)
{
    QToolButton *button = reinterpret_cast<QToolButton *>(sender());
    if(button)
    {
        ui->log_widget->infoLog(button->property("id").toString());
    }
}
//--------------------------------------------------------------------------------
void SimpleWidget::value_changed(void)
{
    ui->log_widget->errorLog(QString("x_value %1").arg(x_value));
}
//--------------------------------------------------------------------------------
