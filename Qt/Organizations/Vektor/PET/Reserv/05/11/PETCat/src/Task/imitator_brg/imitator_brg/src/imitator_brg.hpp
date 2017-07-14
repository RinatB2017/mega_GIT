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
#ifndef IMITATOR_BRG_HPP
#define IMITATOR_BRG_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <hidapi/hidapi.h>
#endif
#ifdef Q_OS_WIN
#   include "hidapi_win/hidapi.h"
#endif
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
class Imitator_BRG : public QObject
{
    Q_OBJECT

public:
    explicit Imitator_BRG(QObject *parent = 0);
    ~Imitator_BRG();

    enum ADC_GetVoltage
    {
        AVCC    = 0,
        AVSS    = 1,
        BIAS    = 2,
        VCCIN   = 3
    };

    typedef struct PARAM {
        uint64_t DDS_TW;

        uint16_t DAC_DGAIN_XP;
        uint16_t DAC_DGAIN_XN;
        uint16_t DAC_DGAIN_YP;
        uint16_t DAC_DGAIN_YN;

        uint16_t DAC_DOF_XP;
        uint16_t DAC_DOF_XN;
        uint16_t DAC_DOF_YP;
        uint16_t DAC_DOF_YN;

        uint16_t DDS_PW_XP;
        uint16_t DDS_PW_XN;
        uint16_t DDS_PW_YP;
        uint16_t DDS_PW_YN;
    } PARAM_t;

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

public slots:
    bool dev_open(bool is_silence = false);
    void dev_close(void);

    bool is_open(void);

    bool write_registers(void);

    void convert_xml(void);

    bool refresh(void);

    bool set_param(PARAM param);

    bool ApplySettings(void);
    bool StopGeneration(void);
    void ManualReset(void);

    void start(void);
    void stop(void);

    bool ReadADC(uint8_t channel, uint16_t *data);
    bool ReadTemperature(float *temperature);
    bool ReadVoltage(int channel, double *voltage);

private:
    QStringList sl_registers;

#pragma pack(push, 1)
    typedef struct question
    {
        uint8_t     zero;
        uint8_t     cmd;
        uint8_t     num;
        uint16_t    addr;
    } question_t;

    typedef struct answer
    {
        uint8_t     zero;
        uint8_t     cmd;
        uint8_t     num;
        uint16_t    addr;
        uint16_t    data;
    } answer_t;

    enum DAC_SetVoltage
    {
        REFIO = 0,
        TEMPRETURE = 1,
        NU0 = 2,
        NU1 = 3
    };
#pragma pack(pop)

#ifdef Q_OS_LINUX
    uint8_t output_buf[128];
#endif
#ifdef Q_OS_WIN
    uint8_t output_buf[256];
#endif

    int VID = IMITATOR_BRG_DEFAULT_VID;
    int PID = IMITATOR_BRG_DEFAULT_PID;

    void init(void);
    void init_sl_registers(void);

    //---
    hid_device *dev = 0;
    bool AD9106_read(QString name_reg, uint16_t *data);
    bool AD9106_write(QString name_reg, uint16_t data);

};
//--------------------------------------------------------------------------------
#endif // IMITATOR_BRG_HPP
