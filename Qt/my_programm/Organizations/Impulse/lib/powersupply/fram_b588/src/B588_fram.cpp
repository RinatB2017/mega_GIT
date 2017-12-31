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
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "B588_fram.hpp"
//--------------------------------------------------------------------------------
B588_Fram::B588_Fram(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void B588_Fram::init(void)
{
    for(int n=0; n<MAX_ADDRESS_FRAM_B588; n++) fram_data[n]=0;
    bool ok = false;
    //ok = read_fram_from_file(":/fram/fram.dat");
    ok = read_fram_from_default_data();
    qDebug() << "read_fram_from_default_data" << ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::read_fram_from_file(const QString &filename)
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

    if(ba.size() != MAX_ADDRESS_FRAM_B588)
    {
        emit error(QString("%1 size == %2")
                   .arg(filename)
                   .arg(ba.size()));
        return false;
    }

    for(int address=0; address<MAX_ADDRESS_FRAM_B588; address++)
    {
        set_uint8_t(address, ba.at(address));
    }
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::write_user_U(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        bool ok = B588_Fram::set_user_calibration_point_U(n, n*1000);
        if(!ok) return false;
    }
    return B588_Fram::calc_and_save_CRC16_user_calibration_points_U();
}
//--------------------------------------------------------------------------------
bool B588_Fram::write_user_I(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        bool ok = B588_Fram::set_user_calibration_point_I(n, n*500);
        if(!ok) return false;
    }
    return B588_Fram::calc_and_save_CRC16_user_calibration_points_I();
}
//--------------------------------------------------------------------------------
bool B588_Fram::write_factory_U(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        bool ok = B588_Fram::set_factory_calibration_point_U(n, n*1000);
        if(!ok) return false;
    }
    return B588_Fram::calc_and_save_CRC16_factory_calibration_points_U();
}
//--------------------------------------------------------------------------------
bool B588_Fram::write_factory_I(void)
{
    for(int n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        bool ok = B588_Fram::set_factory_calibration_point_I(n, n*500);
        if(!ok) return false;
    }
    return B588_Fram::calc_and_save_CRC16_factory_calibration_points_I();
}
//--------------------------------------------------------------------------------
bool B588_Fram::read_fram_from_default_data(void)
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

    ok = B588_Fram::set_moto_time(1);
    if(!ok) return false;

    ok = B588_Fram::set_Bright(MAX_BRIGHT_B588);
    if(!ok) return false;
    ok = B588_Fram::set_Sound(MAX_VOLUME_SOUND_B588);
    if(!ok) return false;
    ok = B588_Fram::set_Adr_GPIB(1);
    if(!ok) return false;
    ok = B588_Fram::set_Count_Last_U_I(1);
    if(!ok) return false;
    ok = B588_Fram::set_Save_Counts_U_I(1);
    if(!ok) return false;
    ok = B588_Fram::set_Speed_UART(2);
    if(!ok) return false;
    ok = B588_Fram::set_Adr_ModBus(1);
    if(!ok) return false;

    ok = B588_Fram::set_Year_Factory(2015);
    if(!ok) return false;
    ok = B588_Fram::set_Month_Factory(1);
    if(!ok) return false;
    ok = B588_Fram::set_Serial_Factory(1234);
    if(!ok) return false;

    s_point point;
    point.voltage = 0;
    point.current = 0;
    point.time = 0;
    for(int profile_number=0; profile_number<MAX_PROFILE_B588; profile_number++)
    {
        for(int point_number=0; point_number<MAX_POINT_B588; point_number++)
        {
            ok = B588_Fram::set_profile_point(profile_number, point_number, point);
            if(!ok) return false;
        }
    }

    ok = B588_Fram::set_Value_K_Zero_U(0, 0, 0, 0);
    if(!ok) return false;
    ok = B588_Fram::set_Value_K_Zero_I(0, 0, 0, 0);
    if(!ok) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_user_calibration_point_U(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B588_U) return false;

    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_U) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_user_calibration_point_I(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B588_I) return false;

    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_I) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_factory_calibration_point_U(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B588_U) return false;

    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_U) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_factory_calibration_point_I(uint16_t index_point, uint16_t value)
{
    if(index_point > MAX_CALIBRATION_POINTS_B588_I) return false;

    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_I) + (index_point * sizeof(uint16_t));
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_user_calibration_point_U(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_U) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_user_calibration_point_I(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_I) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_factory_calibration_point_U(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_U) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_factory_calibration_point_I(uint16_t index_point, uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_I) + (index_point * sizeof(uint16_t));
    uint16_t temp = 0;
    bool ok = get_uint16_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::calc_and_save_CRC16_user_calibration_points_U(void)
{
    bool ok = false;
    TEMP_U temp_U;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        ok = get_user_calibration_point_U(n, &temp_U.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_U.buf_08, MAX_CALIBRATION_POINTS_B588_U*2);

    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_crc16_U);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B588_Fram::calc_and_save_CRC16_user_calibration_points_I(void)
{
    bool ok = false;
    TEMP_I temp_I;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        ok = get_user_calibration_point_I(n, &temp_I.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_I.buf_08, MAX_CALIBRATION_POINTS_B588_I*2);

    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_crc16_I);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B588_Fram::calc_and_save_CRC16_factory_calibration_points_U(void)
{
    bool ok = false;
    TEMP_U temp_U;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        ok = get_factory_calibration_point_U(n, &temp_U.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_U.buf_08, MAX_CALIBRATION_POINTS_B588_U*2);

    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_crc16_U);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B588_Fram::calc_and_save_CRC16_factory_calibration_points_I(void)
{
    bool ok = false;
    TEMP_I temp_I;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        ok = get_factory_calibration_point_I(n, &temp_I.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t crc16 = get_CRC16(temp_I.buf_08, MAX_CALIBRATION_POINTS_B588_I*2);

    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_crc16_I);
    return set_uint16_t(address, crc16);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_CRC16_user_calibration_points_U(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_crc16_U);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_CRC16_user_calibration_points_I(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, user_points_crc16_I);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_CRC16_factory_calibration_points_U(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_crc16_U);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_CRC16_factory_calibration_points_I(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, factory_points_crc16_I);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_moto_time(uint32_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, moto_time);
    return set_uint32_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_moto_time(uint32_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, moto_time);
    uint32_t data = 0;
    bool ok = get_uint32_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Bright(int8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Bright);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Sound(int8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Sound);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Adr_GPIB(int8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Adr_GPIB);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Count_Last_U_I(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Count_Last_U_I);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Save_Counts_U_I(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Save_Counts_U_I);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Speed_UART(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Speed_UART);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Adr_ModBus(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Adr_ModBus);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Bright(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Bright);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Sound(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Sound);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Adr_GPIB(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Adr_GPIB);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Count_Last_U_I(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Count_Last_U_I);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Save_Counts_U_I(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Save_Counts_U_I);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Speed_UART(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Speed_UART);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Adr_ModBus(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Adr_ModBus);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Year_Factory(uint16_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Year_Factory);
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Month_Factory(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Month_Factory);
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Serial_Factory(uint16_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Serial_Factory);
    return set_uint16_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Year_Factory(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Year_Factory);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Month_Factory(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Month_Factory);
    uint8_t data = 0;
    bool ok = get_uint8_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Serial_Factory(uint16_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Serial_Factory);
    uint16_t data = 0;
    bool ok = get_uint16_t(address, &data);
    if(ok)
    {
        *value = data;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_profile_point(uint16_t index_profile,
                                  uint16_t index_point,
                                  s_point point)
{
    //TODO
    Q_UNUSED(index_profile)

    uint16_t address = offsetof(FRAM_ADDR_B588, profiles) + (index_point * sizeof(s_point));
    bool ok = false;

    ok = set_uint16_t(address,   point.voltage);
    if(!ok) return false;

    ok = set_uint16_t(address+2, point.current);
    if(!ok) return false;

    ok = set_uint16_t(address+4, point.time);
    if(!ok) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_profile_point(uint16_t index_profile,
                                  uint16_t index_point,
                                  s_point *point)
{
    //TODO
    Q_UNUSED(index_profile)
    Q_UNUSED(point)

    uint16_t address = offsetof(FRAM_ADDR_B588, profiles) + (index_point * sizeof(s_point));
    s_point temp_point;
    bool ok = false;

    ok = get_uint16_t(address,   &temp_point.voltage);
    if(!ok) return false;

    ok = get_uint16_t(address+2, &temp_point.current);
    if(!ok) return false;

    ok = get_uint16_t(address+4, &temp_point.time);
    if(!ok) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Value_K_Zero_U(int32_t  new_Value_ADC_Zero,
                                   int32_t  new_Value_ADC_Delta,
                                   uint16_t new_Value_DAC_MIN,
                                   uint16_t new_Value_DAC_MAX)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Value_K_Zero_U);
    bool ok = false;

    union
    {
        s_zero_b588_U value;
        uint8_t buf[sizeof(value)];
    } TEMP;

    TEMP.value.Value_ADC_Zero  = new_Value_ADC_Zero;
    TEMP.value.Value_ADC_Delta = new_Value_ADC_Delta;
    TEMP.value.Value_DAC_MIN   = new_Value_DAC_MIN;
    TEMP.value.Value_DAC_MAX   = new_Value_DAC_MAX;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = set_uint8_t(address + n, TEMP.buf[n]);
        if(!ok) return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_Value_K_Zero_I(int32_t  new_Value_ADC_Zero,
                                   int32_t  new_Value_ADC_Delta,
                                   uint16_t new_Value_DAC_MIN,
                                   uint16_t new_Value_DAC_MAX)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Value_K_Zero_I);
    bool ok = false;

    union
    {
        s_zero_b588_I value;
        uint8_t buf[sizeof(value)];
    } TEMP;

    TEMP.value.Value_ADC_Zero  = new_Value_ADC_Zero;
    TEMP.value.Value_ADC_Delta = new_Value_ADC_Delta;
    TEMP.value.Value_DAC_MIN   = new_Value_DAC_MIN;
    TEMP.value.Value_DAC_MAX   = new_Value_DAC_MAX;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = set_uint8_t(address + n, TEMP.buf[n]);
        if(!ok) return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Value_K_Zero_U(s_zero_b588_U *local_calib_U)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Value_K_Zero_U);
    bool ok = false;

    union
    {
        s_zero_b588_U value;
        uint8_t buf[sizeof(value)];
    } TEMP;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = get_uint8_t(address + n, &TEMP.buf[n]);
        if(!ok) return false;
    }

    *local_calib_U = TEMP.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_Value_K_Zero_I(s_zero_b588_I *local_calib_I)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, Value_K_Zero_I);
    bool ok = false;

    union
    {
        s_zero_b588_I value;
        uint8_t buf[sizeof(value)];
    } TEMP;

    for(uint16_t n=0; n<sizeof(TEMP); n++)
    {
        ok = get_uint8_t(address + n, &TEMP.buf[n]);
        if(!ok) return false;
    }

    *local_calib_I = TEMP.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_U_I(uint16_t index_last_value,
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
bool B588_Fram::check_CRC16_user_calibration_points_U(void)
{
    bool ok = false;
    TEMP_U temp_U;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        //__watchdog_reset();
        ok = get_user_calibration_point_U(n*2, &temp_U.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t calc_crc16 = get_CRC16(temp_U.buf_08, MAX_CALIBRATION_POINTS_B588_U*2);
    uint16_t data_crc16 = 0;

    ok = get_CRC16_user_calibration_points_U(&data_crc16);
    if(!ok) return false;

    return (calc_crc16 == data_crc16);
}
//--------------------------------------------------------------------------------
bool B588_Fram::check_CRC16_user_calibration_points_I(void)
{
    bool ok = false;
    TEMP_I temp_I;

    for(uint16_t n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
    {
        //__watchdog_reset();
        ok = get_user_calibration_point_I(n*2, &temp_I.buf_16[n]);
        if(!ok) return false;
    }
    uint16_t calc_crc16 = get_CRC16(temp_I.buf_08, MAX_CALIBRATION_POINTS_B588_I*2);
    uint16_t data_crc16 = 0;

    ok = get_CRC16_user_calibration_points_I(&data_crc16);
    if(!ok) return false;

    return (calc_crc16 == data_crc16);
}
//--------------------------------------------------------------------------------
bool B588_Fram::copy_factory_to_user_calibration(void)
{
    uint16_t data = 0;
    bool ok = false;
    uint16_t n = 0;
    uint16_t user_points_U = offsetof(FRAM_ADDR_B588, user_points_U);
    uint16_t user_points_I = offsetof(FRAM_ADDR_B588, user_points_I);
    uint16_t factory_points_U = offsetof(FRAM_ADDR_B588, factory_points_U);
    uint16_t factory_points_I = offsetof(FRAM_ADDR_B588, factory_points_I);

    for(n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        ok = get_uint16_t(factory_points_U + (n * 2), &data);
        if(ok)
        {
            set_uint16_t(user_points_U + (n * 2), data);
        }
    }

    for(n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
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
bool B588_Fram::copy_user_to_factory_calibration(void)
{
    uint16_t data = 0;
    bool ok = false;
    uint16_t n = 0;
    uint16_t user_points_U = offsetof(FRAM_ADDR_B588, user_points_U);
    uint16_t user_points_I = offsetof(FRAM_ADDR_B588, user_points_I);
    uint16_t factory_points_U = offsetof(FRAM_ADDR_B588, factory_points_U);
    uint16_t factory_points_I = offsetof(FRAM_ADDR_B588, factory_points_I);

    for(n=0; n<MAX_CALIBRATION_POINTS_B588_U; n++)
    {
        ok = get_uint16_t(user_points_U + (n * 2), &data);
        if(ok)
        {
            set_uint16_t(factory_points_U + (n * 2), data);
        }
    }

    for(n=0; n<MAX_CALIBRATION_POINTS_B588_I; n++)
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
void B588_Fram::clear_all(void)
{
    for(int n=0;n<MAX_ADDRESS_FRAM_B588;n++)
    {
        set_uint8_t(n, 0);
    }
}
//--------------------------------------------------------------------------------
// ТОЛЬКО ДЛЯ ОТЛАДКИ
bool B588_Fram::write_data_8(uint16_t address,
                             uint8_t  data)
{
    return set_uint8_t(address, data);
}
//--------------------------------------------------------------------------------
bool B588_Fram::write_data_16(uint16_t address,
                              uint16_t  data)
{
    return set_uint16_t(address, data);
}
//--------------------------------------------------------------------------------
bool B588_Fram::write_data_32(uint16_t address,
                              uint32_t  data)
{
    return set_uint32_t(address, data);
}
//--------------------------------------------------------------------------------
bool B588_Fram::read_data_8(uint16_t address,
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
bool B588_Fram::read_data_16(uint16_t address,
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
bool B588_Fram::read_data_32(uint16_t address,
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
bool B588_Fram::set_uint8_t(uint16_t address,
                            uint8_t data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    fram_data[address] = data;
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_uint16_t(uint16_t address,
                             uint16_t data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    UINT16 temp;

    temp.value = data;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address+n] = temp.buf[n];
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_uint32_t(uint16_t address,
                             uint32_t data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    UINT32 temp;

    temp.value = data;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address+n] = temp.buf[n];
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_uint8_t(uint16_t address,
                            uint8_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    *data = fram_data[address];
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_uint16_t(uint16_t address,
                             uint16_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    UINT16 temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *data = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_int32_t(uint16_t address,
                            int32_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    UINT32 temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *data = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_uint32_t(uint16_t address,
                             uint32_t *data)
{
    if(address > MAX_ADDRESS_FRAM_B588) return false;

    INT32 temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *data = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool B588_Fram::set_factor_k(double value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, factor_k);
    DOUBLE temp;

    temp.value = value;
    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        fram_data[address+n] = temp.buf[n];
    }

    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_factor_k(double *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, factor_k);
    DOUBLE temp;
    temp.value = 0;

    for(uint16_t n=0; n<sizeof(temp); n++)
    {
        temp.buf[n] = fram_data[address+n];
    }

    *value = temp.value;
    return true;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_disable_factor_k(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, disable_factor_k);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_disable_factor_k(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, disable_factor_k);
    uint8_t temp = 0;
    bool ok = get_uint8_t(address, &temp);
    if(ok)
    {
        *value = temp;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool B588_Fram::set_cal_temp(uint8_t value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, cal_temp);
    return set_uint8_t(address, value);
}
//--------------------------------------------------------------------------------
bool B588_Fram::get_cal_temp(uint8_t *value)
{
    uint16_t address = offsetof(FRAM_ADDR_B588, cal_temp);
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
uint16_t B588_Fram::get_CRC16(uint8_t* pcBlock,
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
