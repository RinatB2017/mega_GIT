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
#ifndef DEVICE_BASE_HPP
#define DEVICE_BASE_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QObject>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_bos.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
union UINT32_BYTES{
    quint32 value;
    struct {
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
    } bytes;
};
//--------------------------------------------------------------------------------
const int16_t impulse_pattern [PET_WAVE_POINT_MAX] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x131C, 0x1E77, 0x2481, 0x26F5,
    0x2710, 0x25AD, 0x2368, 0x20A8, 0x1DB5, 0x1ABE, 0x17E1, 0x152F,
    0x12B2, 0x106F, 0x0E64, 0x0C92, 0x0AF4, 0x0987, 0x0845, 0x072C,
    0x0636, 0x0560, 0x04A6, 0x0404, 0x0378, 0x02FE, 0x0295, 0x023A,
    0x01EB, 0x01A8, 0x016D, 0x013A, 0x010F, 0x00E9, 0x00C9, 0x00AD,
    0x0095, 0x0080, 0x006E, 0x005F, 0x0052, 0x0047, 0x003D, 0x0034,
    0x002D, 0x0027, 0x0021, 0x001D, 0x0019, 0x0015, 0x0012, 0x0010,
    0x000E, 0x000C, 0x000A, 0x0009, 0x0007, 0x0006, 0x0006, 0x0005
};
//--------------------------------------------------------------------------------
class Device_base : public QObject
{
    Q_OBJECT
public:
    explicit Device_base(QObject *parent = nullptr);
    virtual PET_RESULT check(QByteArray input_packet, QByteArray *output_packet) = 0;

    PET_RESULT check_packet(QByteArray input_packet, uint8_t cmd);

    PET_RESULT f_PET_CMD_TEST(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_RESET(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_SHUTDOWN(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_DISCOVERY(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_CONFIG_SAVE(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_CONFIG_DEFAULT(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_GET_DEVICE_INFO(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_WAVE(QByteArray input_packet, QByteArray *output_packet);

    //добавлено 28.01.2017
    PET_RESULT f_PET_CMD_RUN(QByteArray input_packet, QByteArray *output_packet);
    PET_RESULT f_PET_CMD_STOP(QByteArray input_packet, QByteArray *output_packet);

    void set_vid(PET_VID value);
    PET_VID get_vid(void);

    void set_pid(PET_PID value);
    PET_PID get_pid(void);

    void set_uid(uint8_t uid_0, uint8_t uid_1, uint8_t uid_2, uint8_t uid_3, uint8_t uid_4, uint8_t uid_5);
    QString get_str_uid(void);

    PET_PID     pid;
    PET_VID     vid;
    uint8_t     uid[6];

    quint16     cmd_port;
    quint16     event_port;

    uint8_t pet_address;

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void log(const QString data);

private:
    void init(void);

    pet_discovery_info      discovery_info;
    pet_device_info         device_info;

    int16_t test_pattern [PET_WAVE_POINT_MAX] = { 0 };
};
//--------------------------------------------------------------------------------
#endif // DEVICE_BASE_HPP
