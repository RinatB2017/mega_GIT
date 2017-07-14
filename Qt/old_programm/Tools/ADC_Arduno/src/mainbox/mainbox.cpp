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
#include <QCoreApplication>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "ui_mainbox.h"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init()
{
    //y[n] = double(308.0)*qSin(double(n)*double(M_PI)/double(180.0));;
    ui->setupUi(this);

    grapher = new GrapherBox(parentWidget());
    grapher->set_title("ADC");
    grapher->set_axis_scale_x(0, 1e6);
    grapher->set_axis_scale_y(0, 1024);

    ui->hbox->addWidget(grapher);

    updateText();

    setMinimumWidth( 1024 );
}
//--------------------------------------------------------------------------------
void MainBox::raw_data(QByteArray data)
{
    int len = data.length();
    if(len < 0) return;

    for(int n=0; n<len; n++)
    {
        switch(data.at(n))
        {
        case ':':
            dirty_data.clear();
            break;

        case '\n':
            analize_packet();
            dirty_data.clear();
            break;

        default:
            dirty_data.append(data.at(n));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::analize_packet(void)
{
    clean_data.clear();
    clean_data = QByteArray::fromHex(dirty_data);

    //???
}
//--------------------------------------------------------------------------------
unsigned char MainBox::convert_ansi_to_dec(char data)
{
    unsigned char temp = 0;
    switch(data)
    {
    case '0': temp = 0x00; break;
    case '1': temp = 0x01; break;
    case '2': temp = 0x02; break;
    case '3': temp = 0x03; break;
    case '4': temp = 0x04; break;
    case '5': temp = 0x05; break;
    case '6': temp = 0x06; break;
    case '7': temp = 0x07; break;
    case '8': temp = 0x08; break;
    case '9': temp = 0x09; break;
    case 'A': temp = 0x0A; break;
    case 'B': temp = 0x0B; break;
    case 'C': temp = 0x0C; break;
    case 'D': temp = 0x0D; break;
    case 'E': temp = 0x0E; break;
    case 'F': temp = 0x0F; break;
    default: break;
    }

    return temp;
}
//--------------------------------------------------------------------------------
QString MainBox::convert_dec_to_ansi(unsigned char data)
{
    uint8_t hi = (data >> 4) & 0x0F;
    uint8_t lo = (data & 0x0F);

    QString hi_str;
    switch(hi)
    {
    case 0x00: hi_str = "0"; break;
    case 0x01: hi_str = "1"; break;
    case 0x02: hi_str = "2"; break;
    case 0x03: hi_str = "3"; break;
    case 0x04: hi_str = "4"; break;
    case 0x05: hi_str = "5"; break;
    case 0x06: hi_str = "6"; break;
    case 0x07: hi_str = "7"; break;
    case 0x08: hi_str = "8"; break;
    case 0x09: hi_str = "9"; break;
    case 0x0A: hi_str = "A"; break;
    case 0x0B: hi_str = "B"; break;
    case 0x0C: hi_str = "C"; break;
    case 0x0D: hi_str = "D"; break;
    case 0x0E: hi_str = "E"; break;
    case 0x0F: hi_str = "F"; break;
    default: break;
    }

    QString lo_str;
    switch(lo)
    {
    case 0x00: lo_str = "0"; break;
    case 0x01: lo_str = "1"; break;
    case 0x02: lo_str = "2"; break;
    case 0x03: lo_str = "3"; break;
    case 0x04: lo_str = "4"; break;
    case 0x05: lo_str = "5"; break;
    case 0x06: lo_str = "6"; break;
    case 0x07: lo_str = "7"; break;
    case 0x08: lo_str = "8"; break;
    case 0x09: lo_str = "9"; break;
    case 0x0A: lo_str = "A"; break;
    case 0x0B: lo_str = "B"; break;
    case 0x0C: lo_str = "C"; break;
    case 0x0D: lo_str = "D"; break;
    case 0x0E: lo_str = "E"; break;
    case 0x0F: lo_str = "F"; break;
    default: break;
    }

    return QString("%1%2").arg(hi_str).arg(lo_str);
}
//--------------------------------------------------------------------------------
void MainBox::updateText()
{

}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        updateText();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
