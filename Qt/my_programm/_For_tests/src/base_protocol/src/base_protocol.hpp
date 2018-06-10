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
#ifndef BASE_PROTOCOL_HPP
#define BASE_PROTOCOL_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QObject>
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef struct HEADER
{
    uint8_t   addr;     // адрес
    uint8_t   cmd;      // команда
    uint16_t  len;      // длина данных
    uint8_t   data[];   // данные
} header_t;
//--------------------------------------------------------------------------------
#pragma pack(pop)
//--------------------------------------------------------------------------------
class Base_protocol : public QObject
{
    Q_OBJECT

public:
    Base_protocol();

    //typedef int (*func)(QByteArray, QByteArray *);
    enum ERRORS {
        E_NO_ERROR = 0,
        E_PACKET_EMPTY,
        E_BAD_ADDRESS,
        E_BAD_CMD,
        E_BAD_SIZE,
        E_BAD_CRC16
    };

    typedef struct CMD
    {
        int cmd;
        int (*func)(QByteArray, QByteArray *);
    } CMD_t;

    int check_packet(QByteArray question,
                     QByteArray *answer);
    void add_command(CMD cmd);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    QList<CMD> commands;

protected:
    uint8_t address = 0;

};
//--------------------------------------------------------------------------------
#endif // BASE_PROTOCOL_HPP
