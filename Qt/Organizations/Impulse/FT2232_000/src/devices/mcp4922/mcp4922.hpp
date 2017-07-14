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
#ifndef MCP4922_HPP
#define MCP4922_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class MCP4922 : public QWidget
{
    Q_OBJECT
public:
    explicit MCP4922(I2C_Freq freq, QWidget *parent = 0);
    virtual ~MCP4922();

    BOOL init(void);
    FT_STATUS set_voltage(float value);

    BOOL open(int deviceNumber);
    BOOL close(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);

private:
    FT2232H *ft2232h;
    DWORD dwClockDivisor;

    BOOL SPI_Initial(DWORD dwClockDivisor);

    void SPI_CSEnable(void);
    void SPI_CSDisable(void);

    void connect_log(void);
};
//--------------------------------------------------------------------------------
#endif // MCP4922_HPP
