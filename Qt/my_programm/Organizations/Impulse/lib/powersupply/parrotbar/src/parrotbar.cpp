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
#include <QToolButton>
#include <QSpinBox>
#include <QToolBar>
#include <QLabel>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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

    addWidget(new QLabel(tr("U (0-65535)")));
    addWidget(parrot_U);
    addWidget(new QLabel(tr("I (0-65535)")));
    addWidget(parrot_I);
    addWidget(btn);

    connect(btn,        SIGNAL(pressed()),          this, SLOT(set()));
    connect(parrot_U,   SIGNAL(editingFinished()),  this, SLOT(set()));
    connect(parrot_I,   SIGNAL(editingFinished()),  this, SLOT(set()));
}
//--------------------------------------------------------------------------------
void ParrotBar::set(void)
{
    emit set_UI(parrot_U->value(), parrot_I->value());
}
//--------------------------------------------------------------------------------

