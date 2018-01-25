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
#ifndef FT2232_HPP
#define FT2232_HPP
//--------------------------------------------------------------------------------
#include <QWidget>

#include <ftd2xx.h>
//--------------------------------------------------------------------------------
class FT2232 : public QWidget
{
    Q_OBJECT
public:
    explicit FT2232(QWidget *parent = 0);

    bool Open(void);
    FT_STATUS Close(void);

    bool Init(void);
    bool SetStart(void);
    bool SetStop(void);
    bool SendByteAndCheckACK(unsigned char data);
    bool SendByteAndIgnoreACK(unsigned char data);
    bool ReadByteAndCheckACK(unsigned char *data);
    void SetBaudRate(ULONG BaudRate);

    bool test(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void log(const QString &data);

private:
    FT_HANDLE ftHandle;
    ULONG BaudRate;

    unsigned char OutputBuffer[1024];
    DWORD dwNumBytesToSend;
    DWORD dwNumBytesSent;

    unsigned char InputBuffer[1024];
    DWORD dwNumBytesToRead;
    DWORD dwNumBytesRead;

    void connect_log(void);
    void Print_FT_STATUS(FT_STATUS status);
    void send_byte(unsigned char data);
    bool send_byte_and_check_ACK(unsigned char data);

    bool write_OutputBuffer(void);
    FT_STATUS clear_buffer(void);

    bool check_ACK(void);

};
//--------------------------------------------------------------------------------
#endif // FT2232_HPP
