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
#include <QHBoxLayout>
#include <QToolButton>
//--------------------------------------------------------------------------------
#include "frame_8bit.hpp"
//--------------------------------------------------------------------------------
union U_BYTE
{
    unsigned char value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
    } bites;
};
//--------------------------------------------------------------------------------
Frame_8bit::Frame_8bit(QWidget *parent) :
    QFrame(parent)
{
    btn7 = new QToolButton(this);
    btn6 = new QToolButton(this);
    btn5 = new QToolButton(this);
    btn4 = new QToolButton(this);
    btn3 = new QToolButton(this);
    btn2 = new QToolButton(this);
    btn1 = new QToolButton(this);
    btn0 = new QToolButton(this);

    btn7->setText("7");
    btn6->setText("6");
    btn5->setText("5");
    btn4->setText("4");
    btn3->setText("3");
    btn2->setText("2");
    btn1->setText("1");
    btn0->setText("0");

    btn7->setCheckable(true);
    btn6->setCheckable(true);
    btn5->setCheckable(true);
    btn4->setCheckable(true);
    btn3->setCheckable(true);
    btn2->setCheckable(true);
    btn1->setCheckable(true);
    btn0->setCheckable(true);

    QHBoxLayout *hbox = new QHBoxLayout;
    //hbox->setMargin(0);
    hbox->setSpacing(0);

    hbox->addWidget(btn7);
    hbox->addWidget(btn6);
    hbox->addWidget(btn5);
    hbox->addWidget(btn4);
    hbox->addWidget(btn3);
    hbox->addWidget(btn2);
    hbox->addWidget(btn1);
    hbox->addWidget(btn0);

    setLayout(hbox);

    setFrameStyle(QFrame::Box | QFrame::Raised);
}
//--------------------------------------------------------------------------------
void Frame_8bit::set_value(unsigned char value)
{
    U_BYTE byte;
    byte.value = value;
    btn0->setChecked(byte.bites.bit0);
    btn1->setChecked(byte.bites.bit1);
    btn2->setChecked(byte.bites.bit2);
    btn3->setChecked(byte.bites.bit3);
    btn4->setChecked(byte.bites.bit4);
    btn5->setChecked(byte.bites.bit5);
    btn6->setChecked(byte.bites.bit6);
    btn7->setChecked(byte.bites.bit7);
}
//--------------------------------------------------------------------------------
unsigned char Frame_8bit::get_value(void)
{
    U_BYTE byte;

    byte.bites.bit0 = btn0->isChecked();
    byte.bites.bit1 = btn1->isChecked();
    byte.bites.bit2 = btn2->isChecked();
    byte.bites.bit3 = btn3->isChecked();
    byte.bites.bit4 = btn4->isChecked();
    byte.bites.bit5 = btn5->isChecked();
    byte.bites.bit6 = btn6->isChecked();
    byte.bites.bit7 = btn7->isChecked();

    return byte.value;
}
//--------------------------------------------------------------------------------
