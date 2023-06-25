/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "parrotbar.hpp"
//--------------------------------------------------------------------------------
ParrotBar::ParrotBar(QWidget *parent) : QToolBar(parent)
{
    parrot_U = new QSpinBox();
    parrot_U->setMinimum(0);
    parrot_U->setMaximum(0xFFFF);
    parrot_U->setObjectName("parrot_U");

    parrot_I = new QSpinBox();
    parrot_I->setMinimum(0);
    parrot_I->setMaximum(0xFFFF);
    parrot_I->setObjectName("parrot_I");

    btn = new QToolButton();
    btn->setText("set");

    addWidget(new QLabel("U (0-65535)"));
    addWidget(parrot_U);
    addWidget(new QLabel("I (0-65535)"));
    addWidget(parrot_I);
    addWidget(btn);

    connect(btn,        &QToolButton::clicked,      this, &ParrotBar::set);
    connect(parrot_U,   &QSpinBox::editingFinished, this, &ParrotBar::set);
    connect(parrot_I,   &QSpinBox::editingFinished, this, &ParrotBar::set);
}
//--------------------------------------------------------------------------------
void ParrotBar::set(void)
{
    emit set_UI(parrot_U->value(), parrot_I->value());
}
//--------------------------------------------------------------------------------

