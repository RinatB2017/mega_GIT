/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include "lockbutton.hpp"
//--------------------------------------------------------------------------------
LockButton::LockButton(QWidget *parent) :
    QWidget(parent)
{
    box = new QCheckBox(this);
    box->setToolTip("lock");

    btn = new QPushButton(this);
    btn->setText("Test");
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(box,    SIGNAL(toggled(bool)),  btn,    SLOT(setDisabled(bool)));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setSpacing(0);
    hbox->setMargin(0);
    hbox->addWidget(box);
    hbox->addWidget(btn);
    hbox->setStretchFactor(box, 0);
    hbox->setStretchFactor(btn, 1);

    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void LockButton::setText(QString text)
{
    btn->setText(text);
}
//--------------------------------------------------------------------------------
