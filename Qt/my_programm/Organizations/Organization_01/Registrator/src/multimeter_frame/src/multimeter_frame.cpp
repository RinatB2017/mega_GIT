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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "multimeter_frame.hpp"
#include "multimeter_options.hpp"
//--------------------------------------------------------------------------------
Multimeter_frame::Multimeter_frame(int number, QWidget *parent) :
    QFrame(parent),
    number(number)
{
    init();
}
//--------------------------------------------------------------------------------
void Multimeter_frame::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  topLevelWidget(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), topLevelWidget(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), topLevelWidget(), SIGNAL(error(QString)));
}
//--------------------------------------------------------------------------------
void Multimeter_frame::init(void)
{
    connect_log();

    QString temp;
    temp.clear();
    temp.append("QPushButton#btn_power { ");
    temp.append("   image: url(:/power_knock/image/black/off.png); ");
    temp.append("   border-style: inset; ");
    temp.append("} ");
    temp.append("QPushButton#btn_power:checked { ");
    temp.append("   image: url(:/power_knock/image/green/on.png); ");
    temp.append("   border-style: inset; ");
    temp.append("} ");

    btn_power = new QPushButton(this);
    btn_power->setObjectName("btn_power");
    btn_power->setStyleSheet(temp);
    btn_power->setCheckable(true);
    btn_power->setFixedSize(33, 70);

    QPushButton *btn = new QPushButton();
    btn->setFixedSize(33, 33);
    btn->setText(QString("%1").arg(number));
    connect(btn, SIGNAL(pressed()), this, SLOT(set_options()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(btn);
    vbox->addWidget(btn_power);
    vbox->addStretch(1);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(vbox);
    hbox->addStretch(1);

    setLayout(hbox);

    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}
//--------------------------------------------------------------------------------
void Multimeter_frame::set_options(void)
{
    options = new Multimeter_options();
    options->exec();
}
//--------------------------------------------------------------------------------
