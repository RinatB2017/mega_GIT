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
#ifndef POWERSUPPLY_B588_HPP
#define POWERSUPPLY_B588_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
#include <QSerialPort>
#include <QToolBar>
#include <QObject>
//--------------------------------------------------------------------------------
#ifdef FAKE
    #include "b588_serialport_test.hpp"
#endif
//--------------------------------------------------------------------------------
enum
{
    E_B588_NO_ERROR = 0,
    E_B588_ERROR_INDEX_NOT_CORRECT,
    E_B588_ERROR_PORT_NOT_OPEN,
    E_B588_ERROR_BREAK,
    E_B588_ERROR_MAX_REPEAT,
    E_B588_ERROR_NO_ANSWER,
    E_B588_ERROR_BIG_ANSWER,
    E_B588_ERROR_SMALL_ANSWER,
    E_B588_ERROR_BAD_SERNO,
    E_B588_ERROR_BAD_ADDRESS,
    E_B588_ERROR_BAD_CRC,
    E_B588_ERROR_BAD_COMMAND,
    E_B588_ERROR_BAD_LEN,
    E_B588_ERROR_UNKNOWN_COMMAND,
    E_B588_INVALID_FLOAT,
    E_B588_INVALID_LIMIT,
    E_B588_INVALID_DELTA,
    E_B588_BREAK_MEASURING,
    E_B588_UNKNOWN_PROFILE,
    E_B588_UNKNOWN_POINT,
    E_B588_PROFILE_EMPTY,
    E_B588_ERROR_SIGNATURE,
    E_B588_NUMBER_PROFILE_ERROR,
    E_B588_NUMBER_POINT_ERROR,
    E_B588_NUMBER_STRING_ERROR,
    E_B588_NUMBER_REPEAT_ERROR,
    E_B588_BUSY,
    E_B588_LO_VOLTAGE,
    E_B588_HI_VOLTAGE,
    E_B588_LO_CURRENT,
    E_B588_HI_CURRENT,
    E_B588_NO_REVERSE,
    E_B588_MAX_POWER,
    E_B588_LO_TIME,
    E_B588_HI_TIME,
    E_B588_ERROR_POINT,
    E_B588_PROFILE_IS_RUN,
    E_B588_CALIBRATION_RUNNING,
    E_B588_NO_RC,
    E_B588_COMMAND_ON_CONSTRUCTION
};

enum {
    PACKET_B588_OK = 0,
    PACKET_B588_ERROR,
    PACKET_B588_FAIL
};
//--------------------------------------------------------------------------------
#define MAX_CALIBRATION_POINTS_B588_U    61
#define MAX_CALIBRATION_POINTS_B588_I    61

#define MIN_VOLTAGE_B588 1
#define MAX_VOLTAGE_B588 60000

#define MIN_CURRENT_B588 1
#define MAX_CURRENT_B588 30000

#define MIN_PROFILE_B588 1
#define MAX_PROFILE_B588 9

#define MIN_POINT_B588   1
#define MAX_POINT_B588   30

#define MIN_REPEAT_B588  1
#define MAX_REPEAT_B588  250

#define MIN_ADDRESS_B588 1
#define MAX_ADDRESS_B588 251

#define MIN_DELAY_B588   1000
#define MAX_DELAY_B588   0xFFFF

#define MIN_ID_B588      1
#define MAX_ID_B588      0xFF

#define MIN_TIME_B588    1
#define MAX_TIME_B588    0xFFFF

#define MIN_DATA_B588    1
#define MAX_DATA_B588    0xFFFF

#define MIN_SPEED_B588   1
#define MAX_SPEED_B588   0xFFFF

#define MIN_PWM_B588     1
#define MAX_PWM_B588     0xFFFF

#define MIN_COUNTER_B588 1
#define MAX_COUNTER_B588 0xFF

#define MIN_CONF_ADC_B588    1
#define MAX_CONF_ADC_B588    0xFF

#define MIN_CODE_BUTTON_B588 1
#define MAX_CODE_BUTTON_B588 16

#define MIN_CMD_B588 1
#define MAX_CMD_B588 0xFF

#define MIN_NUMBER_STRING_B588   1
#define MAX_NUMBER_STRING_B588   6
//--------------------------------------------------------------------------------
#define MAX_B588_VALUE_U       MAX_VOLTAGE_B588
#define MAX_B588_VALUE_I       MAX_CURRENT_B588

#define MIN_B588_VALUE_U 1
#define MIN_B588_VALUE_I 1

#define MINIMUM_B588_I   10

// Число калибровочных точек для ЦАП
#define COUNT_B588_CALIB_POINTS_DAC_U MAX_CALIBRATION_POINTS_B588_U
#define COUNT_B588_CALIB_POINTS_DAC_I MAX_CALIBRATION_POINTS_B588_I

#define POINT_B588_U_1 1000     // Нижняя граница калибровки 1В
#define POINT_B588_U_2 60000	// Верхняя граница калибровки 59В

#define POINT_B588_I_1 20       // Нижняя граница калибровки 0,02А
#define POINT_B588_I_4 30000	// Верхняя граница калибровки 28А
//--------------------------------------------------------------------------------
//было 1024L
//#define MAGIC_B588_U  (MAX_B588_VALUE_U / COUNT_B588_CALIB_POINTS_DAC_U)
#define MAGIC_B588_U    ((MAX_B588_VALUE_U - MIN_B588_VALUE_U) / COUNT_B588_CALIB_POINTS_DAC_U)
//#define MAGIC_B588_U  ((POINT_B588_U_2 - POINT_B588_U_1) / COUNT_B588_CALIB_POINTS_DAC_U)

//было 852L
//#define MAGIC_B588_I  (MAX_B588_VALUE_I / COUNT_CALIB_POINTS_DAC_I)
#define MAGIC_B588_I    ((MAX_B588_VALUE_I - MIN_B588_VALUE_I) / COUNT_B588_CALIB_POINTS_DAC_I)
//#define MAGIC_B588_I  ((POINT_B588_I_2 - POINT_B588_I_1) / COUNT_B588_CALIB_POINTS_DAC_I)
//--------------------------------------------------------------------------------
#define ERROR_E00_B588_TEXT      tr("E0 не используется")
#define ERROR_E01_B588_TEXT      tr("E01 ошибка контрольной суммы FLASH платы индикации")
#define ERROR_E02_B588_TEXT      tr("E02 нет ответа от контроллера платы преобразователя")
#define ERROR_E03_B588_TEXT      tr("E03 низкая температура (температура меньше 5 градусов)")
#define ERROR_E04_B588_TEXT      tr("E04 перегрев (температура больше 80 градусов)")
#define ERROR_E05_B588_TEXT      tr("E05 нет ответа ЦАП (неответ 10 раз после 10 перезапусков каждые 160 мс")
#define ERROR_E06_B588_TEXT      tr("E06 нет опорного напряжения на АЦП (5 раз проверки status)")
#define ERROR_E07_B588_TEXT      tr("E07 ошибка ЦАП (5 раз проверки status)")
#define ERROR_E08_B588_TEXT      tr("E08 ошибка контрольной суммы калибровочных данных в АЦП напряжения")
#define ERROR_E09_B588_TEXT      tr("E09 ошибка контрольной суммы калибровочных данных в АЦП тока")
#define ERROR_E10_B588_TEXT      tr("E10 ошибка контрольной суммы калибровочных данных в ЦАП напряжения")
#define ERROR_E11_B588_TEXT      tr("E11 ошибка контрольной суммы калибровочных данных в АЦП тока")
#define ERROR_E12_B588_TEXT      tr("E12 неверна калибровка АЦП напряжения - выход за допустимые пределы")
#define ERROR_E13_B588_TEXT      tr("E13 неверна калибровка АЦП тока - выход за допустимые пределы")
#define ERROR_E14_B588_TEXT      tr("E14 неверна калибровка ЦАП напряжения - (следуюшее значение меньше предыдущего)")
#define ERROR_E15_B588_TEXT      tr("E15 неверна калибровка ЦАП тока - (следуюшее значение меньше предыдущего)")
#define ERROR_E16_B588_TEXT      tr("E16 нет заводской калибровки напряжения, или она неверна")
#define ERROR_E17_B588_TEXT      tr("E17 нет заводской калибровки тока, или она неверна")
#define ERROR_E18_B588_TEXT      tr("E18 нет ответа от FRAM (после 5 попыток)")
#define ERROR_E19_B588_TEXT      tr("E19 пропадание питания - после чтение функцией 0x46 - обнуляется")
#define ERROR_E20_B588_TEXT      tr("E20 присутствует реверс")
#define ERROR_E21_B588_TEXT      tr("E21 присутсвует плата интерфейса")
#define ERROR_E22_B588_TEXT      tr("E22 дистанционное управление E22")
#define ERROR_E23_B588_TEXT      tr("E23 дистанционное управление E23")
#define ERROR_E24_B588_TEXT      tr("E24 попытка записи в исполняемый профиль")
#define ERROR_E25_B588_TEXT      tr("E25 неверный номер профиля")
#define ERROR_E26_B588_TEXT      tr("E26 профиль пуст")
#define ERROR_E27_B588_TEXT      tr("E27 неверный номер точки")
#define ERROR_E28_B588_TEXT      tr("E28 недопустимое значение напряжения (превышено максимальное)")
#define ERROR_E29_B588_TEXT      tr("E29 недопустимое значение тока")
#define ERROR_E30_B588_TEXT      tr("E30 недопустимое значение времени")
#define ERROR_E31_B588_TEXT      tr("E31 попытка установить неверную скорость обмена")
//--------------------------------------------------------------------------------
class QSerialPort;
class QMenu;
//--------------------------------------------------------------------------------
class Powersupply_B588 : public QObject
{
    Q_OBJECT

public:
    explicit Powersupply_B588(QObject *parent = nullptr);
    ~Powersupply_B588();

    bool port_is_open(void);

    int  print_last_error(void);
    int  get_last_error(void);

    void set_delay_ms(int delay_ms);
    int  get_delay_ms(void);

    int check_dirty_packet(const QByteArray &data);

    bool get_temperature(unsigned char *temp);
    bool get_accurate_temperature(double *temp);
    void set_state_silence(bool state);
    bool set_UI(unsigned int voltage_mV,
                unsigned int current_mA);
    bool set_UI_parrot(unsigned short voltage,
                       unsigned int current);
    bool set_vent_speed(unsigned int speed);
    bool set_PWM(uint16_t PWM_value);
    bool get_UI(unsigned short *in_voltage,
                unsigned short *in_current,
                unsigned short *out_voltage,
                unsigned short *out_current,
                unsigned char  *temp,
                unsigned char  *current_point,
                unsigned char  *active_profile);
    bool get_ADC(uint32_t *U_parrot,
                 uint32_t *I_parrot,
                 uint32_t *U_input_parrot);
    bool get_DAC_ADC(unsigned short *setting_U,
                     unsigned int *setting_I,
                     unsigned short *current_U,
                     unsigned int *current_I,
                     unsigned char *temperature,
                     unsigned char *active_profile,
                     unsigned char *point);
    bool get_adv_ADC(uint32_t *U_parrot,
                     uint32_t *I_parrot,
                     uint32_t *U_input_parrot);
    bool run_calibration(char code);
    bool get_current_calibration_point(char *point);

    bool get_calibration_point_U(uint8_t number_point,
                                 uint16_t *value);
    bool get_calibration_point_I(uint8_t number_point,
                                 uint16_t *value);

    bool set_calibration_point_U(uint8_t number_point,
                                 uint16_t value);
    bool set_calibration_point_I(uint8_t number_point,
                                 uint16_t value);

    bool get_information(uint8_t  *type,
                         uint16_t *year,
                         uint8_t  *month,
                         uint16_t *serno);

    bool set_date_and_serno(uint16_t year,
                            uint8_t  month,
                            uint16_t serno);

    bool copy_user_calibration_point_to_factory(void);

    bool test_3_channel(int PWM,
                        int set_voltage_mV,
                        int get_voltage_mV,
                        int *result_mV,
                        int max_deviation_mV);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    bool find_device(void);
    void set_address(int new_address);

    bool rc_on(void);
    bool rc_off(void);

    bool corridor_on(void);
    bool corridor_off(void);

    void close_connect(void);

    void set_ignore_bad_cmd(bool state);

    bool test(void);

    //---
    bool Set_DAC_U_I(signed int U,signed int I);
    bool Set_Auto_Send_Power_Unit_Get_real_U(void);
    bool Set_Auto_Send_Power_Unit_Get_ADC_U(void);
    bool Save_Calib_Point_DAC(unsigned char number_point,
                              unsigned short data_point);
    bool Send_Command_To_Calib_ADC_U(void);
    bool Send_Command_To_Calib_ADC_I(void);
    bool Send_Command_To_Calib_DAC_U(void);
    bool Send_Command_To_Calib_DAC_I(void);
    bool Send_Command_To_Next_Point(void);
    bool Test_Calib_Point_DAC(unsigned char number_point);
    bool Set_Auto_Send_Power_Unit_Get_Setting_DAC_U(void);
    bool Set_Ventilator_MAX_Speed(unsigned short *r_speed_cooler);
    //---

    void run_dialog(int index);

    void run_dialog_0x01(void);
    void run_dialog_0x02(void);
    void run_dialog_0x03(void);
    void run_dialog_0x04(void);
    void run_dialog_0x05(void);
    void run_dialog_0x06(void);
    void run_dialog_0x07(void);
    void run_dialog_0x08(void);
    void run_dialog_0x09(void);
    void run_dialog_0x0A(void);
    void run_dialog_0x0B(void);
    void run_dialog_0x0C(void);
    void run_dialog_0x0D(void);
    void run_dialog_0x0E(void);
    void run_dialog_0x0F(void);

    void run_dialog_0x10(void);
    void run_dialog_0x11(void);
    void run_dialog_0x12(void);
    void run_dialog_0x13(void);
    void run_dialog_0x14(void);
    void run_dialog_0x15(void);
    void run_dialog_0x16(void);
    void run_dialog_0x17(void);

    void run_dialog_0x20(void);
    void run_dialog_0x21(void);
    void run_dialog_0x22(void);
    void run_dialog_0x23(void);
    void run_dialog_0x24(void);
    void run_dialog_0x25(void);
    void run_dialog_0x26(void);
    void run_dialog_0x27(void);
    void run_dialog_0x28(void);
    void run_dialog_0x29(void);
    void run_dialog_0x2A(void);
    void run_dialog_0x2B(void);
    void run_dialog_0x2C(void);
    void run_dialog_0x2D(void);
    void run_dialog_0x2E(void);
    void run_dialog_0x2F(void);

    void run_dialog_0x30(void);
    void run_dialog_0x31(void);
    void run_dialog_0x32(void);
    void run_dialog_0x33(void);
    void run_dialog_0x34(void);
    void run_dialog_0x35(void);
    void run_dialog_0x36(void);
    void run_dialog_0x37(void);
    void run_dialog_0x38(void);
    void run_dialog_0x39(void);
    void run_dialog_0x3A(void);
    void run_dialog_0x3B(void);
    void run_dialog_0x3C(void);
    void run_dialog_0x3D(void);
    void run_dialog_0x3E(void);
    void run_dialog_0x3F(void);

    void run_dialog_0x41(void);
    void run_dialog_0x42(void);
    void run_dialog_0x43(void);
    void run_dialog_0x44(void);
    void run_dialog_0x46(void);
    void run_dialog_0x47(void);
    void run_dialog_0x48(void);
    void run_dialog_0x49(void);
    void run_dialog_0x4A(void);

    void run_dialog_0x54(void);
    void run_dialog_0x55(void);
    void run_dialog_0x56(void);
    void run_dialog_0x57(void);
    void run_dialog_0x58(void);
    void run_dialog_0x59(void);
    void run_dialog_0x5A(void);
    void run_dialog_0x5B(void);
    void run_dialog_0x5C(void);
    void run_dialog_0x5D(void);
    void run_dialog_0x5E(void);
    void run_dialog_0x5F(void);

    void run_dialog_0x60(void);
    void run_dialog_0x61(void);
    void run_dialog_0x62(void);
    void run_dialog_0x63(void);
    void run_dialog_0x64(void);
    void run_dialog_0x65(void);
    void run_dialog_0x66(void);
    void run_dialog_0x67(void);
    void run_dialog_0x68(void);
    void run_dialog_0x69(void);
    void run_dialog_0x6A(void);
    void run_dialog_0x6B(void);
    void run_dialog_0x6C(void);
    void run_dialog_0x6D(void);
    void run_dialog_0x6E(void);
    void run_dialog_0x6F(void);

    void run_dialog_0x70(void);
    void run_dialog_0x71(void);
    void run_dialog_0x72(void);
    void run_dialog_0x73(void);
    void run_dialog_0x75(void);
    void run_dialog_0x7B(void);

    bool fram_write(uint16_t fram_address,
                    uint8_t  len_data,
                    QByteArray fram_data);
    bool fram_write_8(uint16_t fram_address,
                      uint8_t  fram_data);
    bool fram_write_16(uint16_t fram_address,
                       uint16_t  fram_data);
    bool fram_write_32(uint16_t fram_address,
                       uint32_t  fram_data);

    bool fram_read(uint16_t fram_address,
                   uint8_t  len_data,
                   QByteArray *fram_data);
    bool fram_read_8(uint16_t fram_address,
                     uint8_t *fram_data);
    bool fram_read_16(uint16_t fram_address,
                      uint16_t *fram_data);
    bool fram_read_32(uint16_t fram_address,
                      uint32_t *fram_data);

    //---
#if 0
    #define B588_CMD_0x54_TEXT tr("Б5-90 функция 0x54: Запрос данных по профилям.")
    #define B588_CMD_0x56_TEXT tr("Б5-90 функция 0x56: Чтение профиля по точкам.")
    #define B588_CMD_0x5E_TEXT tr("Б5-90 функция 0x5E: Запись данных точки в профиль для исполнения.")
    #define B588_CMD_0x5F_TEXT tr("Б5-90 функция 0x5F: Запуск профиля на исполнение.")
    #define B588_CMD_0x6E_TEXT tr("Б5-90 функция 0x6E: Запись количества повторов в профиль.")
    #define B588_CMD_0x6F_TEXT tr("Б5-90 функция 0x6F: Очистка профиля")
#endif

    bool get_data_profile(int number_profile,
                          unsigned char *count_point,
                          unsigned char *count_repeat_profile);
    bool get_point_profile(uint8_t  number_profile,
                           uint8_t  number_point,
                           uint8_t  *r_profile,
                           uint8_t  *r_point,
                           uint16_t *r_voltage,
                           uint16_t *r_current,
                           uint16_t *r_time);
    bool add_point_profile(int number_profile,
                           int number_point,
                           unsigned short voltage,
                           unsigned int current,
                           unsigned short time);
    bool run_profile(int number_profile);
    bool add_count_repeat_profile(int number_profile,
                                  int repeat_count);
    bool clear_profile(int number_profile);
    //---

    bool get_ID(uint8_t *ID);

    // тестовые функции
    unsigned int get_Correct_value_U(unsigned int new_setting_U);
    unsigned int get_Correct_value_I(unsigned long new_setting_I);
    //

private slots:
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef FAKE
    SerialPort_B588_test serial;
#else
    QSerialPort serial;
#endif

    bool is_ready = false;
    QByteArray data_powersupply;
    int last_error;
    bool busy;
    int address;
    int delay_ms;

    bool flag_closed;
    bool is_silence;

    bool ignore_bad_cmd;

    void init(void);

    void wait(int time_msec);
    void wait(int max_len,
              int min_time_msec,
              int max_time_msec);

    void msgError(const QString &message);

    int check_packet_answer(unsigned char cmd,
                            int packet_len);

    int send_cmd_0x01(void);
    int send_cmd_0x02(void);
    int send_cmd_0x03(void);
    int send_cmd_0x04(void);
    int send_cmd_0x05(void);
    int send_cmd_0x06(void);
    int send_cmd_0x07(void);
    int send_cmd_0x08(void);
    int send_cmd_0x09(void);
    int send_cmd_0x0A(void);
    int send_cmd_0x0B(void);
    int send_cmd_0x0C(void);
    int send_cmd_0x0D(void);
    int send_cmd_0x0E(void);
    int send_cmd_0x0F(void);

    int send_cmd_0x10(void);
    int send_cmd_0x11(void);
    int send_cmd_0x12(void);
    int send_cmd_0x13(void);
    int send_cmd_0x14(void);
    int send_cmd_0x15(void);
    int send_cmd_0x16(void);
    int send_cmd_0x17(void);

    int send_cmd_0x20(void);
    int send_cmd_0x21(void);
    int send_cmd_0x22(void);
    int send_cmd_0x23(void);
    int send_cmd_0x24(void);
    int send_cmd_0x25(void);
    int send_cmd_0x26(void);
    int send_cmd_0x27(void);
    int send_cmd_0x28(void);
    int send_cmd_0x29(void);
    int send_cmd_0x2A(void);
    int send_cmd_0x2B(void);
    int send_cmd_0x2C(void);
    int send_cmd_0x2D(void);
    int send_cmd_0x2E(void);
    int send_cmd_0x2F(void);

    int send_cmd_0x30(void);
    int send_cmd_0x31(void);
    int send_cmd_0x32(void);
    int send_cmd_0x33(void);
    int send_cmd_0x34(void);
    int send_cmd_0x35(void);
    int send_cmd_0x36(void);
    int send_cmd_0x37(void);
    int send_cmd_0x38(void);
    int send_cmd_0x39(void);
    int send_cmd_0x3A(void);
    int send_cmd_0x3B(void);
    int send_cmd_0x3C(void);
    int send_cmd_0x3D(void);
    int send_cmd_0x3E(void);
    int send_cmd_0x3F(void);

    int send_cmd_0x41(void);
    int send_cmd_0x42(unsigned char code_speed);
    int send_cmd_0x43(void);
    int send_cmd_0x44(void);
    int send_cmd_0x46(uint8_t *type,
                      uint16_t *year,
                      uint8_t *month,
                      uint16_t *serno);
    int send_cmd_0x47(unsigned short *in_voltage,
                      unsigned short *in_current,
                      unsigned short *out_voltage,
                      unsigned short *out_current,
                      unsigned char *temp,
                      unsigned char *current_point,
                      unsigned char *active_profile);
    int send_cmd_0x48(unsigned char new_address_MODBUS,
                      unsigned char *r_new_address_MODBUS);
    int send_cmd_0x49(unsigned int voltage_mV,
                      unsigned int current_mA,
                      unsigned short *r_voltage,
                      unsigned short *r_current);
    int send_cmd_0x4A(uint32_t *bits_error);

    int send_cmd_0x54(unsigned char profile,
                      unsigned char *r_count_point,
                      unsigned char *r_count_repeat_profile);
    int send_cmd_0x55(unsigned int *mototime_min);
    int send_cmd_0x56(uint8_t profile,
                      uint8_t point,
                      uint8_t *r_profile,
                      uint8_t *r_point,
                      uint16_t *r_voltage,
                      uint16_t *r_current,
                      uint16_t *r_time);
    int send_cmd_0x57(unsigned short U,
                      short I);
    int send_cmd_0x58(unsigned short U,
                      short I,
                      unsigned short *r_U,
                      short *r_I);
    int send_cmd_0x59(unsigned short U,
                      short I,
                      unsigned short *r_U,
                      short *r_I);
    int send_cmd_0x5A(unsigned short *setting_U,
                      unsigned int *setting_I,
                      unsigned short *current_U,
                      unsigned int *current_I,
                      unsigned char *temperature,
                      unsigned char *active_profile,
                      unsigned char *point);
    int send_cmd_0x5B(uint32_t *U_parrot,
                      uint32_t *I_parrot,
                      uint32_t *U_input_parrot);
    int send_cmd_0x5C(int32_t Value_Zero_ADC_U,
                      int32_t Value_delta_ADC_U,
                      uint16_t Value_DAC_MIN,
                      uint16_t Value_DAC_MAX,
                      int32_t *r_Value_Zero_ADC_U,
                      int32_t *r_Value_delta_ADC_U,
                      uint16_t *r_Value_DAC_MIN,
                      uint16_t *r_Value_DAC_MAX);
    int send_cmd_0x5D(int32_t Value_Zero_ADC_I,
                      int32_t Value_delta_ADC_I,
                      uint16_t Value_DAC_MIN,
                      uint16_t Value_DAC_MAX,
                      int32_t *r_Value_Zero_ADC_I,
                      int32_t *r_Value_delta_ADC_I,
                      uint16_t *r_Value_DAC_MIN,
                      uint16_t *r_Value_DAC_MAX);
    int send_cmd_0x5E(unsigned char profile,
                      unsigned char point,
                      unsigned short voltage_mV,
                      unsigned int current_mA,
                      unsigned short time, unsigned short *r_error,
                      unsigned char *r_profile,
                      unsigned char *r_point,
                      unsigned short *r_voltage,
                      signed int *r_current,
                      unsigned short *r_time);
    int send_cmd_0x5F(unsigned char profile,
                      unsigned char *number_profile);

    int send_cmd_0x60(void);
    int send_cmd_0x61(int32_t  *Value_ADC_Zero_U,
                      double *factor_K);
    int send_cmd_0x62(int32_t  *Value_ADC_Zero_I,
                      double *factor_K);
    int send_cmd_0x63(uint8_t  code,
                      uint8_t  number_point,
                      uint16_t data_point,
                      uint8_t *r_number_point,
                      uint16_t *r_data_point);
    int send_cmd_0x64(char cmd);
    int send_cmd_0x65(char *number_current_point);
    int send_cmd_0x66(uint16_t code,
                      uint16_t index,
                      uint16_t *value);
    int send_cmd_0x67(unsigned short speed_cooler,
                      unsigned short *r_speed_cooler);
    int send_cmd_0x68(unsigned short data_PWM,
                      unsigned short *r_data_PWM);
    int send_cmd_0x69(unsigned short *OCR3A,
                      unsigned short *OCR3B);
    int send_cmd_0x6A(void);
    int send_cmd_0x6B(void);
    int send_cmd_0x6D(uint8_t secret_cmd,
                      uint8_t input_data[],
                      uint8_t *r_secret_cmd,
                      uint8_t *result,
                      QByteArray *output_data);
    int send_cmd_0x6C(unsigned int number_string);

    int send_cmd_0x6E(unsigned char profile,
                      unsigned char repeat,
                      unsigned char *r_profile,
                      unsigned char *r_repeat);
    int send_cmd_0x6F(unsigned char profile,
                      unsigned char *r_profile);

    int send_cmd_0x71(unsigned char *r_state_ADC_U,
                      unsigned char *r_state_ADC_I,
                      unsigned char *r_state_ADC_Ui);
    int send_cmd_0x72(unsigned short conf_ADC,
                      unsigned short *r_conf_ADC);
    int send_cmd_0x73(unsigned short *real_DAC_U,
                      unsigned short *real_DAC_I,
                      unsigned char *revers,
                      unsigned short *temperature);

    int send_cmd_0x75(unsigned char  *count_ReStart_ADC_1,
                      unsigned char  *count_ReStart_ADC_0,
                      unsigned char  *count_ReStart_ADC_2,
                      unsigned short *count_Error_Answer_ADC,
                      unsigned int   *count_Error_ADC,
                      unsigned short *MCUCSR);
    int send_cmd_0x7B(void);
    int send_cmd_0x7C(void);

    bool set_data_32(QStringList text,
                     uint32_t value,
                     uint32_t *r_value);

#ifdef NEEDED_PAUSE
    void pause(unsigned int msec = 3000);
#endif

protected:
    bool event(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif // POWERSUPPLY_B588_HPP
