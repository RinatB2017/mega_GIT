/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <stdint.h>
#include <QObject>
#include <QFile>
#include <qmath.h>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "B590_fram.hpp"
//--------------------------------------------------------------------------------
B590_Fram::B590_Fram(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void B590_Fram::init(void)
{
    for(int n=0; n<MAX_ADDRESS_FRAM_B590; n++) fram_data[n]=0;
    //bool ok = false;
    //ok = read_fram_from_file(":/fram/fram.dat");
    read_fram_from_default_data();
#ifdef QT_DEBUG
    //qDebug() << "read_fram_from_default_data" << ok;
#endif
}
//--------------------------------------------------------------------------------
bool B590_Fram::read_fram_from_file(const QString &filename)
{
    emit info(QString("read_fram_from_file(%1)").arg(filename));
    if(filename.isEmpty())
    {
        emit error("file is empty!");
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit error(QString("file %1 NOT open!").arg(filename));
        return false;
    }

    QByteArray ba = file.readAll();
    file.close();

    if(ba.size() != MAX_ADDRESS_FRAM_B590)
    {
        emit error(QString("%1 size == %2")
                   .arg(filename)
                   .arg(ba.size()));
        return false;
    }

    for(int address=0; address<MAX_ADDRESS_FRAM_B590; address++)
    {
        set_uint8_t(address, ba.at(address));
    }
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::write_user_U(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        bool ok = B590_Fram::set_user_calibration_point_U(n, n*1000);
        if(!ok) return false;
    }
    return B590_Fram::calc_and_save_CRC16_user_calibration_points_U();
}
//--------------------------------------------------------------------------------
bool B590_Fram::write_user_I(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        bool ok = B590_Fram::set_user_calibration_point_I(n, n*500);
        if(!ok) return false;
    }
    return B590_Fram::calc_and_save_CRC16_user_calibration_points_I();
}
//--------------------------------------------------------------------------------
bool B590_Fram::write_factory_U(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        bool ok = B590_Fram::set_factory_calibration_point_U(n, n*1000);
        if(!ok) return false;
    }
    return B590_Fram::calc_and_save_CRC16_factory_calibration_points_U();
}
//--------------------------------------------------------------------------------
bool B590_Fram::write_factory_I(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        bool ok = B590_Fram::set_factory_calibration_point_I(n, n*500);
        if(!ok) return false;
    }
    return B590_Fram::calc_and_save_CRC16_factory_calibration_points_I();
}
//--------------------------------------------------------------------------------
bool B590_Fram::read_fram_from_default_data(void)
{
    bool ok = false;

    ok = write_user_U();
    if(!ok) return false;
    ok = write_user_I();
    if(!ok) return false;
    ok = write_factory_U();
    if(!ok) return false;
    ok = write_factory_I();
    if(!ok) return false;

    ok = B590_Fram::set_moto_time(1);
    if(!ok) return false;

    ok = B590_Fram::set_Bright(MAX_BRIGHT_B590);
    if(!ok) return false;
    ok = B590_Fram::set_Sound(MAX_VOLUME_SOUND_B590);
    if(!ok) return false;
    ok = B590_Fram::set_Count_Last_U_I(1);
    if(!ok) return false;
    ok = B590_Fram::set_Speed_UART(2);
    if(!ok) return false;
    ok = B590_Fram::set_Adr_ModBus(1);
    if(!ok) return false;

    ok = B590_Fram::set_Year_Factory(2015);
    if(!ok) return false;
    ok = B590_Fram::set_Month_Factory(1);
    if(!ok) return false;
    ok = B590_Fram::set_Serial_Factory(1234);
    if(!ok) return false;

#if 0
    s_point point;
    point.voltage = 0;
    point.current = 0;
    point.time = 0;
    for(int profile_number=0; profile_number<MAX_PROFILE_B590; profile_number++)
    {
        for(int point_number=0; point_number<MAX_POINT_B590; point_number++)
        {
            ok = B590_Fram::add_profile_point(profile_number, point);
            if(!ok) return false;
        }
    }
#endif

    s_zero_b590_U value_U;
    s_zero_b590_I value_I;

    double zero_adc = qPow((double)2, (int)23);
    //double full_adc = qPow((double)2, (int)24);
    double step = qPow((double)2, (int)23) / (double)0xFFFF;

    //---
    value_U.Value_ADC_U[0] = (int32_t)(zero_adc + 0.5f);
    value_U.Value_ADC_U[1] = (int32_t)(zero_adc + POINT_U_1 * step + 0.5f);
    value_U.Value_ADC_U[2] = (int32_t)(zero_adc + POINT_U_2 * step + 0.5f);
    value_U.Value_ADC_U[3] = (int32_t)(zero_adc + POINT_U_3 * step + 0.5f);

    value_U.Value_DAC_U[0] = POINT_U_0;
    value_U.Value_DAC_U[1] = POINT_U_1;
    value_U.Value_DAC_U[2] = POINT_U_2;
    value_U.Value_DAC_U[3] = POINT_U_3;

    value_U.temp_cal_U[0] = 0;
    value_U.temp_cal_U[1] = 0;
    value_U.temp_cal_U[2] = 0;
    value_U.temp_cal_U[3] = 0;

    value_U.parrot_in_point = 128;
    //---

    //---
    value_I.Value_ADC_I[0] = (int32_t)(zero_adc + 0.5f);
    value_I.Value_ADC_I[1] = (int32_t)(zero_adc + POINT_I_1 * step + 0.5f);
    value_I.Value_ADC_I[2] = (int32_t)(zero_adc + POINT_I_2 * step + 0.5f);
    value_I.Value_ADC_I[3] = (int32_t)(zero_adc + POINT_I_3 * step + 0.5f);
    value_I.Value_ADC_I[4] = (int32_t)(zero_adc + POINT_I_4 * step + 0.5f);
    value_I.Value_ADC_I[5] = (int32_t)(zero_adc + POINT_I_5 * step + 0.5f);
    value_I.Value_ADC_I[6] = (int32_t)(zero_adc + POINT_I_6 * step + 0.5f);
    value_I.Value_ADC_I[7] = (int32_t)(zero_adc + POINT_I_7 * step + 0.5f);
    value_I.Value_ADC_I[8] = (int32_t)(zero_adc + POINT_I_8 * step + 0.5f);
    value_I.Value_ADC_I[9] = (int32_t)(zero_adc + POINT_I_9 * step + 0.5f);
    value_I.Value_ADC_I[10] = (int32_t)(zero_adc + POINT_I_10 * step + 0.5f);
    value_I.Value_ADC_I[11] = (int32_t)(zero_adc + POINT_I_11 * step + 0.5f);
    value_I.Value_ADC_I[12] = (int32_t)(zero_adc + POINT_I_12 * step + 0.5f);

    value_I.Value_DAC_I[0]  = POINT_I_0;
    value_I.Value_DAC_I[1]  = POINT_I_1;
    value_I.Value_DAC_I[2]  = POINT_I_2;
    value_I.Value_DAC_I[3]  = POINT_I_3;
    value_I.Value_DAC_I[4]  = POINT_I_4;
    value_I.Value_DAC_I[5]  = POINT_I_5;
    value_I.Value_DAC_I[6]  = POINT_I_6;
    value_I.Value_DAC_I[7]  = POINT_I_7;
    value_I.Value_DAC_I[8]  = POINT_I_8;
    value_I.Value_DAC_I[9]  = POINT_I_9;
    value_I.Value_DAC_I[10] = POINT_I_10;
    value_I.Value_DAC_I[11] = POINT_I_11;
    value_I.Value_DAC_I[12] = POINT_I_12;

    value_I.temp_cal_I[0]  = 0;
    value_I.temp_cal_I[1]  = 0;
    value_I.temp_cal_I[2]  = 0;
    value_I.temp_cal_I[3]  = 0;
    value_I.temp_cal_I[4]  = 0;
    value_I.temp_cal_I[5]  = 0;
    value_I.temp_cal_I[6]  = 0;
    value_I.temp_cal_I[7]  = 0;
    value_I.temp_cal_I[8]  = 0;
    value_I.temp_cal_I[9]  = 0;
    value_I.temp_cal_I[10] = 0;
    value_I.temp_cal_I[11] = 0;
    value_I.temp_cal_I[12] = 0;
    //---

    ok = B590_Fram::set_Value_K_Zero_U(value_U);
    if(!ok) return false;
    ok = B590_Fram::set_Value_K_Zero_I(value_I);
    if(!ok) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_user_calibration_point_U(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B590_U) return false;

    uint16_t address = offsetof(FRAM_B590, user_points_U) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_user_calibration_point_I(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B590_I) return false;

    uint16_t address = offsetof(FRAM_B590, user_points_I) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_factory_calibration_point_U(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B590_U) return false;

    uint16_t address = offsetof(FRAM_B590, factory_points_U) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_factory_calibration_point_I(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B590_I) return false;

    uint16_t address = offsetof(FRAM_B590, factory_points_I) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_user_calibration_point_U(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, user_points_U) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_user_calibration_point_I(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, user_points_I) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_factory_calibration_point_U(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, factory_points_U) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_factory_calibration_point_I(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, factory_points_I) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::calc_and_save_CRC16_user_calibration_points_U(void)
{
    bool ok = false;
    TEMP_U temp_U;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        ok = get_user_calibration_point_U(n, &temp_U.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_U.buf_08, MAX_CALIBRATION_POINTS_B590_U*2);

    uint16_t address = offsetof(FRAM_B590, user_points_crc16_U);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B590_Fram::calc_and_save_CRC16_user_calibration_points_I(void)
{
    bool ok = false;
    TEMP_I temp_I;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        ok = get_user_calibration_point_I(n, &temp_I.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_I.buf_08, MAX_CALIBRATION_POINTS_B590_I*2);

    uint16_t address = offsetof(FRAM_B590, user_points_crc16_I);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B590_Fram::calc_and_save_CRC16_factory_calibration_points_U(void)
{
    bool ok = false;
    TEMP_U temp_U;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        ok = get_factory_calibration_point_U(n, &temp_U.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_U.buf_08, MAX_CALIBRATION_POINTS_B590_U*2);

    uint16_t address = offsetof(FRAM_B590, factory_points_crc16_U);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B590_Fram::calc_and_save_CRC16_factory_calibration_points_I(void)
{
    bool ok = false;
    TEMP_I temp_I;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        ok = get_factory_calibration_point_I(n, &temp_I.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_I.buf_08, MAX_CALIBRATION_POINTS_B590_I*2);

    uint16_t address = offsetof(FRAM_B590, factory_points_crc16_I);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_CRC16_user_calibration_points_U(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, user_points_crc16_U);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_CRC16_user_calibration_points_I(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, user_points_crc16_I);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_CRC16_factory_calibration_points_U(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, factory_points_crc16_U);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_CRC16_factory_calibration_points_I(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, factory_points_crc16_I);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_moto_time(uint32_t value)
{
    uint16_t address = offsetof(FRAM_B590, moto_time);
    return set_uint32_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_moto_time(uint32_t *value)
{
    uint16_t address = offsetof(FRAM_B590, moto_time);
    uint32_t data = 0;
    bool ok = get_uint32_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Bright(int8_t value)
{
    uint16_t address = offsetof(FRAM_B590, Bright);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Sound(int8_t value)
{
    uint16_t address = offsetof(FRAM_B590, Sound);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Count_Last_U_I(uint8_t value)
{
    uint16_t address = offsetof(FRAM_B590, count_Last_U_I);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Speed_UART(uint8_t value)
{
    uint16_t address = offsetof(FRAM_B590, Speed_UART);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Adr_ModBus(uint8_t value)
{
    uint16_t address = offsetof(FRAM_B590, Adr_ModBus);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Bright(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Bright);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Sound(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Sound);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Count_Last_U_I(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, count_Last_U_I);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Speed_UART(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Speed_UART);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Adr_ModBus(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Adr_ModBus);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Year_Factory(uint16_t value)
{
    uint16_t address = offsetof(FRAM_B590, Year_Factory);
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Month_Factory(uint8_t value)
{
    uint16_t address = offsetof(FRAM_B590, Month_Factory);
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Serial_Factory(uint16_t value)
{
    uint16_t address = offsetof(FRAM_B590, Serial_Factory);
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Year_Factory(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Year_Factory);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Month_Factory(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Month_Factory);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Serial_Factory(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_B590, Serial_Factory);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::add_profile_point(uint16_t index_profile,
                                  s_point point)
{
    uint16_t address = offsetof(FRAM_B590, profiles) + (index_profile * sizeof(s_profile));
    union {
        s_profile profile;
        uint8_t buf[sizeof(profile)];
    } PROFILE;

    bool ok = false;
    for(unsigned int n=0; n<sizeof(s_profile); n++)
    {
        uint8_t data = 0;
        ok = get_uint8_t(address + n, &data);
        if(!ok)
        {
            return false;
        }
        PROFILE.buf[n] = data;
    }

    if(PROFILE.profile.points_in_profile >= MAX_POINT_B590)
    {
        PROFILE.profile.points_in_profile = 0;
    }
    PROFILE.profile.point[PROFILE.profile.points_in_profile].voltage = point.voltage;
    PROFILE.profile.point[PROFILE.profile.points_in_profile].current = point.current;
    PROFILE.profile.point[PROFILE.profile.points_in_profile].time    = point.time;
    PROFILE.profile.points_in_profile++;

    for(unsigned int n=0; n<sizeof(s_profile); n++)
    {
        ok = set_uint8_t(address + n, PROFILE.buf[n]);
        if(!ok)
        {
            return false;
        }
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_profile_point(uint16_t index_profile,
                                  uint16_t index_point,
                                  s_point *point)
{
    uint16_t address = offsetof(FRAM_B590, profiles) + (index_profile * sizeof(s_profile));
    union {
        s_profile profile;
        uint8_t buf[sizeof(profile)];
    } PROFILE;

    bool ok = false;
    for(unsigned int n=0; n<sizeof(s_profile); n++)
    {
        uint8_t data = 0;
        ok = get_uint8_t(address + n, &data);
        if(!ok)
        {
            return false;
        }
        PROFILE.buf[n] = data;
    }

    *point = PROFILE.profile.point[index_point];
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_profile(uint16_t index_profile,
                            uint8_t *count_point,
                            uint8_t *count_repeat_profile)
{
    uint16_t address = offsetof(FRAM_B590, profiles) + (index_profile * sizeof(s_profile));
    union {
        s_profile profile;
        uint8_t buf[sizeof(profile)];
    } PROFILE;
    bool ok = false;
    for(unsigned int n=0; n<sizeof(s_profile); n++)
    {
        uint8_t data = 0;
        ok = get_uint8_t(address + n, &data);
        if(!ok)
        {
            return false;
        }
        PROFILE.buf[n] = data;
    }

    *count_point = PROFILE.profile.points_in_profile;
    *count_repeat_profile = PROFILE.profile.repeat_count;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Value_K_Zero_U(s_zero_b590_U value)
{
    uint16_t address = offsetof(FRAM_B590, user_Value_K_Zero_U);
    bool ok = false;

    union
    {
        s_zero_b590_U value;
        uint8_t buf[sizeof(value)];
    } TEMP;

    TEMP.value = value;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = set_uint8_t(address + n, TEMP.buf[n]);
        if(!ok) return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_Value_K_Zero_I(s_zero_b590_I value)
{
    uint16_t address = offsetof(FRAM_B590, user_Value_K_Zero_I);
    bool ok = false;

    union
    {
        s_zero_b590_I temp;
        uint8_t buf[sizeof(temp)];
    } TEMP;

    TEMP.temp = value;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = set_uint8_t(address + n, TEMP.buf[n]);
        if(!ok) return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Value_K_Zero_U(s_zero_b590_U *local_calib_U)
{
    uint16_t address = offsetof(FRAM_B590, user_Value_K_Zero_U);
    bool ok = false;

    union
    {
        s_zero_b590_U temp;
        uint8_t buf[sizeof(temp)];
    } TEMP;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = get_uint8_t(address + n, &TEMP.buf[n]);
        if(!ok) return false;
    }

    *local_calib_U = TEMP.temp;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_Value_K_Zero_I(s_zero_b590_I *local_calib_I)
{
    uint16_t address = offsetof(FRAM_B590, user_Value_K_Zero_I);
    bool ok = false;

    union
    {
        s_zero_b590_I temp;
        uint8_t buf[sizeof(temp)];
    } TEMP;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = get_uint8_t(address + n, &TEMP.buf[n]);
        if(!ok) return false;
    }

    *local_calib_I = TEMP.temp;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_U_I(uint16_t index_last_value,
                        uint16_t *U,
                        uint16_t *I)
{
    //TODO
    Q_UNUSED(index_last_value)
    Q_UNUSED(U)
    Q_UNUSED(I)
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::check_CRC16_user_calibration_points_U(void)
{
    bool ok = false;
    TEMP_U temp_U;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        //__watchdog_reset();
        ok = get_user_calibration_point_U(n*2, &temp_U.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t calc_crc16 = get_CRC16(temp_U.buf_08, MAX_CALIBRATION_POINTS_B590_U*2);
    uint16_t data_crc16 = 0;

    ok = get_CRC16_user_calibration_points_U(&data_crc16);
    if(!ok) return false;

    return (calc_crc16 == data_crc16);
}
//--------------------------------------------------------------------------------
bool B590_Fram::check_CRC16_user_calibration_points_I(void)
{
    bool ok = false;
    TEMP_I temp_I;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        //__watchdog_reset();
        ok = get_user_calibration_point_I(n*2, &temp_I.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t calc_crc16 = get_CRC16(temp_I.buf_08, MAX_CALIBRATION_POINTS_B590_I*2);
    uint16_t data_crc16 = 0;

    ok = get_CRC16_user_calibration_points_I(&data_crc16);
    if(!ok) return false;

    return (calc_crc16 == data_crc16);
}
//--------------------------------------------------------------------------------
bool B590_Fram::copy_factory_to_user_calibration(void)
{
    uint16_t data = 0;
    bool ok = false;
    uint16_t n = 0;
    uint16_t user_points_U = offsetof(FRAM_B590, user_points_U);
    uint16_t user_points_I = offsetof(FRAM_B590, user_points_I);
    uint16_t factory_points_U = offsetof(FRAM_B590, factory_points_U);
    uint16_t factory_points_I = offsetof(FRAM_B590, factory_points_I);

    for(n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        ok = get_uint16_t(factory_points_U + (n * 2), &data);
        if(ok)
        {
            set_uint16_t(user_points_U + (n * 2), data);
        }
    }

    for(n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        ok = get_uint16_t(factory_points_I + (n * 2), &data);
        if(ok)
        {
            set_uint16_t(user_points_I + (n * 2), data);
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::copy_user_to_factory_calibration(void)
{
    uint16_t data = 0;
    bool ok = false;
    uint16_t n = 0;
    uint16_t user_points_U = offsetof(FRAM_B590, user_points_U);
    uint16_t user_points_I = offsetof(FRAM_B590, user_points_I);
    uint16_t factory_points_U = offsetof(FRAM_B590, factory_points_U);
    uint16_t factory_points_I = offsetof(FRAM_B590, factory_points_I);

    for(n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        ok = get_uint16_t(user_points_U + (n * 2), &data);
        if(ok)
        {
            set_uint16_t(factory_points_U + (n * 2), data);
        }
    }

    for(n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        ok = get_uint16_t(user_points_I + (n * 2), &data);
        if(ok)
        {
            set_uint16_t(factory_points_I + (n * 2), data);
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void B590_Fram::clear_all(void)
{
    for(int n=0;n<MAX_ADDRESS_FRAM_B590;n++)
    {
        set_uint8_t(n, 0);
    }
}
//--------------------------------------------------------------------------------
// ТОЛЬКО ДЛЯ ОТЛАДКИ
bool B590_Fram::write_data_8(uint16_t address,
                             uint8_t  data)
{
    return set_uint8_t(address, data);
}
//--------------------------------------------------------------------------------
bool B590_Fram::write_data_16(uint16_t address,
                              uint16_t  data)
{
    return set_uint16_t(address, data);
}
//--------------------------------------------------------------------------------
bool B590_Fram::write_data_32(uint16_t address,
                              uint32_t  data)
{
    return set_uint32_t(address, data);
}
//--------------------------------------------------------------------------------
bool B590_Fram::read_data_8(uint16_t address,
                            uint8_t  *data)
{
    uint8_t temp = 0;
    bool ok = get_uint8_t(address, &temp);
    if(ok)
    {
        *data = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::read_data_16(uint16_t address,
                             uint16_t  *data)
{
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *data = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B590_Fram::read_data_32(uint16_t address,
                             uint32_t  *data)
{
    uint32_t temp = 0;
    bool ok = get_uint32_t(address, &temp);
    if(ok)
    {
        *data = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool B590_Fram::set_uint8_t(uint16_t address,
                            uint8_t data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    fram_data[address] = data;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_uint16_t(uint16_t address,
                             uint16_t data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    UINT_16 temp;

    temp.value = data;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address+n] = temp.buf[n];
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_uint32_t(uint16_t address,
                             uint32_t data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    UINT_32 temp;

    temp.value = data;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address+n] = temp.buf[n];
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_uint8_t(uint16_t address,
                            uint8_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    *data = fram_data[address];
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_uint16_t(uint16_t address,
                             uint16_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    UINT_16 temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *data = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_int32_t(uint16_t address,
                            int32_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    UINT_32 temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *data = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_uint32_t(uint16_t address,
                             uint32_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B590) return false;

    INT_32 temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *data = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_factor_k(double factor_k_DAC,
                             double factor_k_ADC)
{
    uint16_t address_DAC = offsetof(FRAM_B590, factor_k_DAC);
    uint16_t address_ADC = offsetof(FRAM_B590, factor_k_ADC);
    U_DOUBLE temp;

    temp.value = factor_k_DAC;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address_DAC+n] = temp.buf[n];
    }
    temp.value = factor_k_ADC;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address_ADC+n] = temp.buf[n];
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_factor_k(double *factor_k_DAC,
                             double *factor_k_ADC)
{
    uint16_t address_DAC = offsetof(FRAM_B590, factor_k_DAC);
    uint16_t address_ADC = offsetof(FRAM_B590, factor_k_ADC);
    U_DOUBLE temp;

    temp.value = 0;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address_DAC+n];
    }
    *factor_k_DAC = temp.value;

    temp.value = 0;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address_ADC+n];
    }
    *factor_k_ADC = temp.value;

    return true;
}
//--------------------------------------------------------------------------------
bool B590_Fram::set_cal_temp(int8_t value)
{
    uint16_t address = offsetof(FRAM_B590, cal_temp);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B590_Fram::get_cal_temp(int8_t *value)
{
    uint16_t address = offsetof(FRAM_B590, cal_temp);
    uint8_t temp = 0;
    bool ok = get_uint8_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
uint16_t B590_Fram::get_CRC16(uint8_t* pcBlock,
                              uint16_t len)
{
    uint16_t crc = 0xFFFF;

    for (int pos=0; pos<len; pos++)
    {
        crc ^= (uint16_t)pcBlock[pos];

        for (int i=8; i!=0; i--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
                crc >>= 1;
        }
    }
    return crc;
}
//--------------------------------------------------------------------------------
