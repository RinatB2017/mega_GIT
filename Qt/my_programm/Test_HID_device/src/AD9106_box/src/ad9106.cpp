/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QObject>
//--------------------------------------------------------------------------------
#include "ad9106.hpp"
//--------------------------------------------------------------------------------
AD9106::AD9106(QObject *parent) : QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void AD9106::init(void)
{
    reg r = { "xxx", 5, 4 };
    AD9106_registers.append(r);

    AD9106_registers.clear();
    r = { "SPICONFIG", 0x0, 0x4002 };       AD9106_registers.append(r);
    r = { "POWERCONFIG", 0x1, 0x0 };        AD9106_registers.append(r);
    r = { "CLOCKCONFIG", 0x2, 0x0 };        AD9106_registers.append(r);
    r = { "REFADJ", 0x3, 0x0 };             AD9106_registers.append(r);
    r = { "DAC4AGAIN", 0x4, 0x0 };          AD9106_registers.append(r);
    r = { "DAC3AGAIN", 0x5, 0x0 };          AD9106_registers.append(r);
    r = { "DAC2AGAIN", 0x6, 0x0 };          AD9106_registers.append(r);
    r = { "DAC1AGAIN", 0x7, 0x0 };          AD9106_registers.append(r);
    r = { "DACxRANGE", 0x8, 0x0 };          AD9106_registers.append(r);
    r = { "DAC4RSET", 0x9, 0xA };           AD9106_registers.append(r);
    r = { "DAC3RSET", 0xA, 0xA };           AD9106_registers.append(r);
    r = { "DAC2RSET", 0xB, 0xA };           AD9106_registers.append(r);
    r = { "DAC1RSET", 0xC, 0xA };           AD9106_registers.append(r);
    r = { "CALCONFIG", 0xD, 0x0 };          AD9106_registers.append(r);
    r = { "COMPOFFSET", 0xE, 0x0 };         AD9106_registers.append(r);
    r = { "RAMUPDATE", 0x1D, 0x0 };         AD9106_registers.append(r);
    r = { "PAT_STATUS", 0x1E, 0x0 };        AD9106_registers.append(r);
    r = { "PAT_TYPE", 0x1F, 0x0 };          AD9106_registers.append(r);
    r = { "PATTERN_DLY", 0x20, 0xE };       AD9106_registers.append(r);
    r = { "DAC4DOF", 0x22, 0x0 };           AD9106_registers.append(r);
    r = { "DAC3DOF", 0x23, 0x0 };           AD9106_registers.append(r);
    r = { "DAC2DOF", 0x24, 0x0 };           AD9106_registers.append(r);
    r = { "DAC1DOF", 0x25, 0x0 };           AD9106_registers.append(r);
    r = { "WAV4_3CONFIG", 0x26, 0x0 };      AD9106_registers.append(r);
    r = { "WAV2_1CONFIG", 0x27, 0x0 };      AD9106_registers.append(r);
    r = { "PAT_TIMEBASE", 0x28, 0x111 };    AD9106_registers.append(r);
    r = { "PAT_PERIOD", 0x29, 0x8000 };     AD9106_registers.append(r);
    r = { "DAC4_3PATx", 0x2A, 0x101 };      AD9106_registers.append(r);
    r = { "DAC2_1PATx", 0x2B, 0x101 };      AD9106_registers.append(r);
    r = { "DOUT_START_DLY", 0x2C, 0x3 };    AD9106_registers.append(r);
    r = { "DOUT_CONFIG", 0x2D, 0x0 };       AD9106_registers.append(r);
    r = { "DAC4_CST", 0x2E, 0x0 };          AD9106_registers.append(r);
    r = { "DAC3_CST", 0x2F, 0x0 };          AD9106_registers.append(r);
    r = { "DAC2_CST", 0x30, 0x0 };          AD9106_registers.append(r);
    r = { "DAC1_CST", 0x31, 0x0 };          AD9106_registers.append(r);
    r = { "DAC4_DGAIN", 0x32, 0x0 };        AD9106_registers.append(r);
    r = { "DAC3_DGAIN", 0x33, 0x0 };        AD9106_registers.append(r);
    r = { "DAC2_DGAIN", 0x34, 0x0 };        AD9106_registers.append(r);
    r = { "DAC1_DGAIN", 0x35, 0x0 };        AD9106_registers.append(r);
    r = { "SAW4_3CONFIG", 0x36, 0x0 };      AD9106_registers.append(r);
    r = { "SAW2_1CONFIG", 0x37, 0x0 };      AD9106_registers.append(r);
    r = { "DDS_TW32", 0x3E, 0x0 };          AD9106_registers.append(r);
    r = { "DDS_TW1", 0x3F, 0x0 };           AD9106_registers.append(r);
    r = { "DDS4_PW", 0x40, 0x0 };           AD9106_registers.append(r);
    r = { "DDS3_PW", 0x41, 0x0 };           AD9106_registers.append(r);
    r = { "DDS2_PW", 0x42, 0x0 };           AD9106_registers.append(r);
    r = { "DDS1_PW", 0x43, 0x0 };           AD9106_registers.append(r);
    r = { "TRIG_TW_SEL", 0x44, 0x0 };       AD9106_registers.append(r);
    r = { "DDSx_CONFIG", 0x45, 0x0 };       AD9106_registers.append(r);
    r = { "TWRAM_CONFIG", 0x47, 0x0 };      AD9106_registers.append(r);
    r = { "START_DLY4", 0x50, 0x0 };        AD9106_registers.append(r);
    r = { "START_ADDR4", 0x51, 0x0 };       AD9106_registers.append(r);
    r = { "STOP_ADDR4", 0x52, 0x0 };        AD9106_registers.append(r);
    r = { "DDS_CYC4", 0x53, 0x1 };          AD9106_registers.append(r);
    r = { "START_DLY3", 0x54, 0x0 };        AD9106_registers.append(r);
    r = { "START_ADDR3", 0x55, 0x0 };       AD9106_registers.append(r);
    r = { "STOP_ADDR3", 0x56, 0x0 };        AD9106_registers.append(r);
    r = { "DDS_CYC3", 0x57, 0x1 };          AD9106_registers.append(r);
    r = { "START_DLY2", 0x58, 0x0 };        AD9106_registers.append(r);
    r = { "START_ADDR2", 0x59, 0x0 };       AD9106_registers.append(r);
    r = { "STOP_ADDR2", 0x5A, 0x0 };        AD9106_registers.append(r);
    r = { "DDS_CYC2", 0x5B, 0x1 };          AD9106_registers.append(r);
    r = { "START_DLY1", 0x5C, 0x0 };        AD9106_registers.append(r);
    r = { "START_ADDR1", 0x5D, 0x0 };       AD9106_registers.append(r);
    r = { "STOP_ADDR1", 0x5E, 0x0 };        AD9106_registers.append(r);
    r = { "DDS_CYC1", 0x5F, 0x1 };          AD9106_registers.append(r);
    r = { "CFG_ERROR", 0x60, 0x1 };         AD9106_registers.append(r);
}
//--------------------------------------------------------------------------------
bool AD9106::get_sl_name_registers(QStringList *sl)
{
    (*sl).clear();

    foreach (reg r, AD9106_registers) {
        (*sl).append(r.name);
    }
    return true;
}
//--------------------------------------------------------------------------------
uint16_t AD9106::get_SRAM_StartAddress(void)
{
    return SRAM_StartAddress;
}
//--------------------------------------------------------------------------------
bool AD9106::get_reg(QString name, reg_t *reg)
{
    if(name.isEmpty())
    {
        return false;
    }
    foreach (reg_t r, AD9106_registers) {
        if(r.name == name)
        {
            (*reg).name = r.name;
            (*reg).address = r.address;
            (*reg).default_value = r.default_value;
            return true;
        }
    }

    return false;
}
//--------------------------------------------------------------------------------
int AD9106::get_cnt_registers(void)
{
    return AD9106_registers.length();
}
//--------------------------------------------------------------------------------
