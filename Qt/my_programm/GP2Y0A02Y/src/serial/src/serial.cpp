/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "serial.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "ui_serial.h"
//--------------------------------------------------------------------------------
Serial::Serial(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Serial)
{
    init();
}
//--------------------------------------------------------------------------------
Serial::~Serial()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Serial::init(void)
{
    ui->setupUi(this);

    connect(ui->serial, SIGNAL(output(QByteArray)), this,   SLOT(get_data(QByteArray)));

    emit info("Serial started");
}
//--------------------------------------------------------------------------------
QString Serial::convert_data_to_ascii(uint8_t data)
{
    uint8_t hi = (data >> 4) & 0x0F;
    uint8_t lo = (data & 0x0F);

    QString hi_str;
    switch(hi)
    {
    case 0x00: hi_str="0"; break;
    case 0x01: hi_str="1"; break;
    case 0x02: hi_str="2"; break;
    case 0x03: hi_str="3"; break;
    case 0x04: hi_str="4"; break;
    case 0x05: hi_str="5"; break;
    case 0x06: hi_str="6"; break;
    case 0x07: hi_str="7"; break;
    case 0x08: hi_str="8"; break;
    case 0x09: hi_str="9"; break;
    case 0x0A: hi_str="A"; break;
    case 0x0B: hi_str="B"; break;
    case 0x0C: hi_str="C"; break;
    case 0x0D: hi_str="D"; break;
    case 0x0E: hi_str="E"; break;
    case 0x0F: hi_str="F"; break;
    default: break;
    }

    QString lo_str;
    switch(lo)
    {
    case 0x00: lo_str="0"; break;
    case 0x01: lo_str="1"; break;
    case 0x02: lo_str="2"; break;
    case 0x03: lo_str="3"; break;
    case 0x04: lo_str="4"; break;
    case 0x05: lo_str="5"; break;
    case 0x06: lo_str="6"; break;
    case 0x07: lo_str="7"; break;
    case 0x08: lo_str="8"; break;
    case 0x09: lo_str="9"; break;
    case 0x0A: lo_str="A"; break;
    case 0x0B: lo_str="B"; break;
    case 0x0C: lo_str="C"; break;
    case 0x0D: lo_str="D"; break;
    case 0x0E: lo_str="E"; break;
    case 0x0F: lo_str="F"; break;
    default: break;
    }

    return (hi_str+lo_str);
}
//--------------------------------------------------------------------------------
uint8_t Serial::convert_ascii_to_value(char hi, char lo)
{
    uint8_t b_hi = 0;
    uint8_t b_lo = 0;

    //---
    switch(hi)
    {
    case '0':  b_hi = 0x0;  break;
    case '1':  b_hi = 0x1;  break;
    case '2':  b_hi = 0x2;  break;
    case '3':  b_hi = 0x3;  break;
    case '4':  b_hi = 0x4;  break;
    case '5':  b_hi = 0x5;  break;
    case '6':  b_hi = 0x6;  break;
    case '7':  b_hi = 0x7;  break;
    case '8':  b_hi = 0x8;  break;
    case '9':  b_hi = 0x9;  break;
    case 'A':  b_hi = 0xA;  break;
    case 'B':  b_hi = 0xB;  break;
    case 'C':  b_hi = 0xC;  break;
    case 'D':  b_hi = 0xD;  break;
    case 'E':  b_hi = 0xE;  break;
    case 'F':  b_hi = 0xF;  break;
    default:
        break;
    }
    //---
    switch(lo)
    {
    case '0':  b_lo = 0x0;  break;
    case '1':  b_lo = 0x1;  break;
    case '2':  b_lo = 0x2;  break;
    case '3':  b_lo = 0x3;  break;
    case '4':  b_lo = 0x4;  break;
    case '5':  b_lo = 0x5;  break;
    case '6':  b_lo = 0x6;  break;
    case '7':  b_lo = 0x7;  break;
    case '8':  b_lo = 0x8;  break;
    case '9':  b_lo = 0x9;  break;
    case 'A':  b_lo = 0xA;  break;
    case 'B':  b_lo = 0xB;  break;
    case 'C':  b_lo = 0xC;  break;
    case 'D':  b_lo = 0xD;  break;
    case 'E':  b_lo = 0xE;  break;
    case 'F':  b_lo = 0xF;  break;
    default:
        break;
    }
    //---
    uint8_t r_byte = (b_hi << 4) | b_lo;
    return r_byte;
}
//--------------------------------------------------------------------------------
void Serial::get_data(QByteArray data)
{
    for(int n=0; n<data.length(); n++)
    {
        analize(data.at(n));
    }
}
//--------------------------------------------------------------------------------
void Serial::analize(char data)
{
    QByteArray ba;
    switch(data)
    {
    case ':':
        data_rs232.clear();
        break;

    case '\r':
    case '\n':
        ba.clear();
        ba = QByteArray::fromHex(data_rs232);

        work(ba);

        data_rs232.clear();
        break;

    default:
        data_rs232.append(data);
        break;
    }
}
//--------------------------------------------------------------------------------
bool Serial::work(QByteArray clean_data)
{
    //emit debug(QString("%1").arg(ba.toHex().data()));

    if(clean_data.isEmpty())
    {
        return false;
    }
    if(clean_data.size() % 2)
    {
        emit error("Serial::work ba.size() % 2");
        return false;
    }

    int max_n = clean_data.size();
    int index = 0;
    for(int n=0 ;n<max_n - 1; n+=2)
    {
        uint8_t hi = clean_data.at(n);
        uint8_t lo = clean_data.at(n+1);
        uint16_t value = (hi << 8) | lo;

        //emit info("Serial: set_value");
        emit set_value(index, value);
        index++;
    }

    return true;
}
//--------------------------------------------------------------------------------
void Serial::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Serial::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Serial::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Serial::save_setting(void)
{

}
//--------------------------------------------------------------------------------
