/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#ifndef B588_FRAM_HPP
#define B588_FRAM_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
#include <QObject>
//--------------------------------------------------------------------------------
#include "b588_powersupply.hpp"
//--------------------------------------------------------------------------------
#define MAX_ADDRESS_FRAM_B588    8192
#define MAX_BRIGHT_B588          9
#define MAX_VOLUME_SOUND_B588    10
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef union
{
    uint16_t buf_16[MAX_CALIBRATION_POINTS_B588_U];
    uint8_t  buf_08[MAX_CALIBRATION_POINTS_B588_U*2];
} TEMP_U;
typedef union
{
    uint16_t buf_16[MAX_CALIBRATION_POINTS_B588_I];
    uint8_t  buf_08[MAX_CALIBRATION_POINTS_B588_I*2];
} TEMP_I;

typedef union
{
    uint16_t value;
    uint8_t  buf[sizeof(value)];
} UINT16;

typedef union
{
    int16_t value;
    uint8_t buf[sizeof(value)];
} INT16;

typedef union
{
    uint32_t value;
    uint8_t  buf[sizeof(value)];
} UINT32;

typedef union
{
    double value;
    uint8_t buf[sizeof(value)];
} DOUBLE;

typedef union
{
    int32_t value;
    uint8_t buf[sizeof(value)];
} INT32;

typedef struct
{
    bool active;
    uint16_t voltage;
    uint16_t current;
    uint16_t time;
} s_point;

typedef struct
{
    s_point point[MAX_POINT_B588];
    uint16_t repeat_count;
    uint16_t crc16;
} s_profile;
//=============================================================================
typedef struct
{
  int32_t  Value_ADC_Zero;
  int32_t  Value_ADC_Delta;
  uint16_t Value_DAC_MIN;
  uint16_t Value_DAC_MAX;
} s_zero_b588_U;

typedef struct
{
  int32_t  Value_ADC_Zero;
  int32_t  Value_ADC_Delta;
  uint16_t Value_DAC_MIN;
  uint16_t Value_DAC_MAX;
} s_zero_b588_I;
//=============================================================================
typedef struct
{
    uint16_t signature;   //0x55AA

    uint16_t user_points_U[MAX_CALIBRATION_POINTS_B588_U];
    uint16_t user_points_crc16_U;

    uint16_t user_points_I[MAX_CALIBRATION_POINTS_B588_I];
    uint16_t user_points_crc16_I;

    uint16_t factory_points_U[MAX_CALIBRATION_POINTS_B588_U];
    uint16_t factory_points_crc16_U;

    uint16_t factory_points_I[MAX_CALIBRATION_POINTS_B588_I];
    uint16_t factory_points_crc16_I;

    s_profile profiles[MAX_PROFILE_B588];

    s_zero_b588_U Value_K_Zero_U;
    s_zero_b588_I Value_K_Zero_I;

    int8_t  Bright;
    int8_t  Sound;
    int8_t  Adr_GPIB;
    uint8_t Count_Last_U_I;
    uint8_t Save_Counts_U_I;
    uint8_t Speed_UART;
    uint8_t Adr_ModBus;

    uint32_t  moto_time;

    uint16_t Year_Factory;      // Год выпуска устройства
    uint8_t  Month_Factory;     // Месяц выпуска устройства (1-12)
    uint16_t Serial_Factory;    // Заводской номер устройства

    double  factor_k;
    uint8_t disable_factor_k;
    uint8_t cal_temp;

} FRAM_ADDR_B588;

#pragma pack(pop)
//--------------------------------------------------------------------------------
class B588_Fram : public QObject
{
    Q_OBJECT

public:
    explicit B588_Fram(QObject *parent = 0);

    void init(void);
    bool read_fram_from_file(const QString &filename);
    bool read_fram_from_default_data(void);

    bool write_user_U(void);
    bool write_user_I(void);
    bool write_factory_U(void);
    bool write_factory_I(void);

    bool set_user_calibration_point_U(uint16_t index_point, uint16_t value);
    bool set_user_calibration_point_I(uint16_t index_point, uint16_t value);
    bool set_factory_calibration_point_U(uint16_t index_point, uint16_t value);
    bool set_factory_calibration_point_I(uint16_t index_point, uint16_t value);

    bool get_user_calibration_point_U(uint16_t index_point, uint16_t *value);
    bool get_user_calibration_point_I(uint16_t index_point, uint16_t *value);
    bool get_factory_calibration_point_U(uint16_t index_point, uint16_t *value);
    bool get_factory_calibration_point_I(uint16_t index_point, uint16_t *value);

    bool calc_and_save_CRC16_user_calibration_points_U(void);
    bool calc_and_save_CRC16_user_calibration_points_I(void);

    bool calc_and_save_CRC16_factory_calibration_points_U(void);
    bool calc_and_save_CRC16_factory_calibration_points_I(void);

    bool get_CRC16_user_calibration_points_U(uint16_t *value);
    bool get_CRC16_user_calibration_points_I(uint16_t *value);

    bool get_CRC16_factory_calibration_points_U(uint16_t *value);
    bool get_CRC16_factory_calibration_points_I(uint16_t *value);

    bool set_moto_time(uint32_t value);
    bool get_moto_time(uint32_t *value);

    bool set_Bright(int8_t value);
    bool set_Sound(int8_t value);
    bool set_Adr_GPIB(int8_t value);
    bool set_Count_Last_U_I(uint8_t value);
    bool set_Save_Counts_U_I(uint8_t value);
    bool set_Speed_UART(uint8_t value);
    bool set_Adr_ModBus(uint8_t value);

    bool get_Bright(uint8_t *value);
    bool get_Sound(uint8_t *value);
    bool get_Adr_GPIB(uint8_t *value);
    bool get_Count_Last_U_I(uint8_t *value);
    bool get_Save_Counts_U_I(uint8_t *value);
    bool get_Speed_UART(uint8_t *value);
    bool get_Adr_ModBus(uint8_t *value);

    bool set_Year_Factory(uint16_t value);
    bool set_Month_Factory(uint8_t value);
    bool set_Serial_Factory(uint16_t value);

    bool get_Year_Factory(uint16_t *value);
    bool get_Month_Factory(uint8_t *value);
    bool get_Serial_Factory(uint16_t *value);

    bool set_factor_k(double value);
    bool get_factor_k(double *value);

    bool set_disable_factor_k(uint8_t value);
    bool get_disable_factor_k(uint8_t *value);

    bool set_cal_temp(uint8_t value);
    bool get_cal_temp(uint8_t *value);

    bool set_profile_point(uint16_t index_profile,
                           uint16_t index_point,
                           s_point point);
    bool get_profile_point(uint16_t index_profile,
                           uint16_t index_point,
                           s_point *point);

    bool set_Value_K_Zero_U(int32_t  new_Value_ADC_Zero,
                            int32_t  new_Value_ADC_Delta,
                            uint16_t new_Value_DAC_MIN,
                            uint16_t new_Value_DAC_MAX);
    bool set_Value_K_Zero_I(int32_t  new_Value_ADC_Zero,
                            int32_t  new_Value_ADC_Delta,
                            uint16_t new_Value_DAC_MIN,
                            uint16_t new_Value_DAC_MAX);

    bool get_Value_K_Zero_U(s_zero_b588_U *local_calib_U);
    bool get_Value_K_Zero_I(s_zero_b588_I *local_calib_I);

    bool get_U_I(uint16_t index_last_value,
                 uint16_t *U,
                 uint16_t *I);

    bool check_CRC16_user_calibration_points_U(void);
    bool check_CRC16_user_calibration_points_I(void);

    bool copy_factory_to_user_calibration(void);
    bool copy_user_to_factory_calibration(void);

    void clear_all(void);

    // ТОЛЬКО ДЛЯ ОТЛАДКИ
    bool write_data_8(uint16_t address,
                      uint8_t  data);
    bool write_data_16(uint16_t address,
                       uint16_t data);
    bool write_data_32(uint16_t address,
                       uint32_t data);
    bool read_data_8(uint16_t address,
                     uint8_t  *data);
    bool read_data_16(uint16_t address,
                      uint16_t *data);
    bool read_data_32(uint16_t address,
                      uint32_t *data);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    bool set_uint8_t(uint16_t address,  uint8_t data);
    bool set_uint16_t(uint16_t address, uint16_t data);
    bool set_uint32_t(uint16_t address, uint32_t data);

    bool get_uint8_t(uint16_t address,  uint8_t *data);
    bool get_uint16_t(uint16_t address, uint16_t *data);
    bool get_int32_t(uint16_t address,  int32_t *data);
    bool get_uint32_t(uint16_t address, uint32_t *data);

    uint16_t get_max_address(void);

    uint16_t get_CRC16(uint8_t* pcBlock,
                       uint16_t len);

    uint8_t  fram_data[MAX_ADDRESS_FRAM_B588];
};
//--------------------------------------------------------------------------------
#endif
