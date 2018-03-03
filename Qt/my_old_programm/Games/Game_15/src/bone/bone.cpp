/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "game_15_mainbox.hpp"
#include "defines.hpp"
#include "bone.hpp"
//--------------------------------------------------------------------------------
Bone::Bone(QWidget *parent) :
    QFrame(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Bone::init(void)
{
    label = new QLabel(this);
    label->setFont(QFont("Liberation Sans", 24));
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QVBoxLayout *layout = new QVBoxLayout(parentWidget());
    layout->addWidget(label);

    setFrameStyle(QFrame::Box | QFrame::Raised);
    setMinimumSize(BONE_SIZE, BONE_SIZE);
    setLayout(layout);
}
//--------------------------------------------------------------------------------
void Bone::setNumber(int n)
{
    label->setText(QString("%1").arg(n));
}
//--------------------------------------------------------------------------------
