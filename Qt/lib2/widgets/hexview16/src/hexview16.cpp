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
#include "hexview16.hpp"
//--------------------------------------------------------------------------------
HexView16::HexView16(QWidget *parent) :
    QWidget(parent)
{
    init();;
}
//--------------------------------------------------------------------------------
HexView16::~HexView16()
{
    if(tv)      tv->deleteLater();
    if(model)   model->deleteLater();
}
//--------------------------------------------------------------------------------
void HexView16::init(void)
{
    QStringList h_sl;
    for(int n=0; n<0x10; n++)
    {
        h_sl.append(QString("%1").arg(n, 4, 16, QChar('0')).toUpper());
    }
    QStringList v_sl;
    for(int n=0; n<=0xFFFF; n+=0x10)
    {
        v_sl.append(QString("%1").arg(n, 4, 16, QChar('0')).toUpper());
    }

    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(h_sl);
    model->setVerticalHeaderLabels(v_sl);

    int index = 0;
    for(int row=0; row<MAX_ROW; row++)
    {
        for (int col=0; col<MAX_COL; col++)
        {
            items[index] = new QStandardItem("0000");
            model->setItem(row, col, items[index]);
            index++;
        }
    }

    tv = new QTableView(this);
    tv->setModel(model);

    //костыль, но работает :)
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QApplication::processEvents();

    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    //---

    tv->setEditTriggers(QTableView::NoEditTriggers);

    QFont font("Courier", 10);
    tv->setFont(font);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(tv);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void HexView16::set(uint16_t address, uint16_t value)
{
    if(address >= 0xFFFF)   return;

    buf[address] = value;
    items[address]->setText(QString("%1").arg(value, 4, 16, QChar('0')).toUpper());
}
//--------------------------------------------------------------------------------
uint16_t HexView16::get(uint16_t address)
{
    if(address >= 0xFFFF)   return 0;

    return buf[address];
}
//--------------------------------------------------------------------------------
