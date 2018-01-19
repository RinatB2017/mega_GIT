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
#include <QVBoxLayout>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "doublehexeditor.hpp"
#include "defines.hpp"
#include "qhexedit.h"
//--------------------------------------------------------------------------------
DoubleHexEditor::DoubleHexEditor(const QString &caption1,
                                 const QString &caption2,
                                 QWidget *parent) :
    QFrame(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    hexedit1 = new QHexEdit();
    hexedit1->setMinimumWidth(640);
    hexedit1->setReadOnly(true);

    hexedit2 = new QHexEdit();
    hexedit2->setMinimumWidth(640);
    hexedit2->setReadOnly(true);

    vbox->addWidget(new QLabel(caption1));
    vbox->addWidget(hexedit1);
    vbox->addWidget(new QLabel(caption2));
    vbox->addWidget(hexedit2);

    setLayout(vbox);

    setWindowIcon(QIcon(ICON_PROGRAMM));

    setFrameShape(StyledPanel);
    setFrameShadow(Sunken);
}
//--------------------------------------------------------------------------------
void DoubleHexEditor::append(const QByteArray &array1,
                             const QByteArray &array2)
{
    hexedit1->setData(array1);
    hexedit2->setData(array2);
}
//--------------------------------------------------------------------------------
