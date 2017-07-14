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

#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
class MCP4922 : public QWidget
{
    Q_OBJECT
public:
    explicit MCP4922(QWidget *parent = 0);
    virtual ~MCP4922();

    BOOL init(void);
    BOOL set_voltage(float value);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);

private:
    FT_STATUS ftStatus;     // Status defined in D2XX to indicate operation result
    BYTE OutputBuffer[512]; // Buffer to hold MPSSE commands and data to be sent to FT2232H
    BYTE InputBuffer[512];  // Buffer to hold Data bytes to be read from FT2232H
    DWORD dwNumBytesToSend; // Index of output buffer
    DWORD dwClockDivisor;   // Value of clock divisor, SCL Frequency = 60/((1+29)*2) (MHz) = 1Mhz
    DWORD dwNumBytesSent, dwNumBytesRead, dwNumInputBuffer;

    FT_HANDLE ftdiHandle;

    void connect_log(void);

    BOOL open(int deviceNumber);
    BOOL close(void);

    void SPI_CSEnable(void);
    void SPI_CSDisable(void);

    BOOL SPI_Initial(FT_HANDLE ftHandle);

    void print_error(const QString &function, FT_STATUS status);
};
//--------------------------------------------------------------------------------
#endif // MCP4922_HPP
