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
#ifndef CLIENT_HPP
#define CLIENT_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define MAX_EVENT_IN_PACKET 89
//--------------------------------------------------------------------------------
typedef struct
{
    uint8_t  Source;    /* Источник события */
    uint8_t  Flags;     /* Информационное поле (флаги) события */
    uint16_t Reserve;   /* Резерв для дальнейшего использования */
    uint32_t TS;        /* Метка времени */
    uint16_t XP;        /* Безразмерное значение координатного сигнала X+ */
    uint16_t XN;        /* Безразмерное значение координатного сигнала X- */
    uint16_t YP;        /* Безразмерное значение координатного сигнала Y+ */
    uint16_t YN;        /* Безразмерное значение координатного сигнала Y- */
} PETEvent;

union PACKET
{
    struct
    {
        uint32_t num_packet;
        PETEvent event[MAX_EVENT_IN_PACKET];
        uint16_t crc16;
    } body;
    uint8_t buf[sizeof(body)];
};
//--------------------------------------------------------------------------------
class QUdpSocket;
//--------------------------------------------------------------------------------
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

public slots:
    void process(void);

private:
    QUdpSocket *udpSocket;

    void send_data(int size_packet, int max_num_packet, int *err_cnt);
    void send_raw_data(int max_num_packet, int *err_cnt);
};
//--------------------------------------------------------------------------------
#endif
