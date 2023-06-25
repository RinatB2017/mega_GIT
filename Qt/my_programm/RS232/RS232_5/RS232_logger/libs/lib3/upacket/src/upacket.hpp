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
#ifndef UPACKET_HPP
#define UPACKET_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
enum UP_ERRORS
{
    NO_ERRORS = 0,
    ERR_EMPTY_PACKET,
    ERR_BAD_PACKET,
    ERR_BAD_ADDR,
    ERR_BAD_CMD,
    ERR_BAD_LEN,
    ERR_BAD_CRC
};
//--------------------------------------------------------------------------------
class UPacket : public QObject
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public:
    UPacket(void);

    int get_address(void);

    bool check_packet(QByteArray packet, int *cmd, QByteArray *data);
    int get_err(void);
    QString get_err_str(void);

public slots:
    void set_address(int address);

private:
    int err = 0;
    int addr = 0;
};
//--------------------------------------------------------------------------------
#endif
