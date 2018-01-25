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
#ifndef B590_POWERSUPPLY_HPP
#define B590_POWERSUPPLY_HPP
//--------------------------------------------------------------------------------
#include <QSerialPort>
#include <QObject>
#include <QFrame>
#include "B590_fram.hpp"
//--------------------------------------------------------------------------------
#ifdef FAKE
#include "b590_serialport_test.hpp"
#endif
//--------------------------------------------------------------------------------
#define FAKTOR_K_NONE   0
#define FAKTOR_K_NONE_TEXT  "none"

#define FAKTOR_K_MINUS  1
#define FAKTOR_K_MINUS_TEXT "minus"

#define FAKTOR_K_PLUS   2
#define FAKTOR_K_PLUS_TEXT  "plus"
//--------------------------------------------------------------------------------
enum
{
    E_B590_NO_ERROR = 0,
    E_B590_ERROR_INDEX_NOT_CORRECT,
    E_B590_ERROR_PORT_NOT_OPEN,
    E_B590_ERROR_BREAK,
    E_B590_ERROR_MAX_REPEAT,
    E_B590_ERROR_NO_ANSWER,
    E_B590_ERROR_BIG_ANSWER,
    E_B590_ERROR_SMALL_ANSWER,
    E_B590_ERROR_BAD_SERNO,
    E_B590_ERROR_BAD_ADDRESS,
    E_B590_ERROR_BAD_CRC,
    E_B590_ERROR_BAD_COMMAND,
    E_B590_ERROR_BAD_LEN,
    E_B590_ERROR_UNKNOWN_COMMAND,
    E_B590_INVALID_FLOAT,
    E_B590_INVALID_LIMIT,
    E_B590_INVALID_DELTA,
    E_B590_BREAK_MEASURING,
    E_B590_UNKNOWN_PROFILE,
    E_B590_UNKNOWN_POINT,
    E_B590_PROFILE_EMPTY,
    E_B590_ERROR_SIGNATURE,
    E_B590_NUMBER_PROFILE_ERROR,
    E_B590_NUMBER_POINT_ERROR,
    E_B590_NUMBER_STRING_ERROR,
    E_B590_NUMBER_REPEAT_ERROR,
    E_B590_BUSY,
    E_B590_LO_VOLTAGE,
    E_B590_HI_VOLTAGE,
    E_B590_LO_CURRENT,
    E_B590_HI_CURRENT,
    E_B590_NO_REVERSE,
    E_B590_MAX_POWER,
    E_B590_LO_TIME,
    E_B590_HI_TIME,
    E_B590_ERROR_POINT,
    E_B590_PROFILE_IS_RUN,
    E_B590_CALIBRATION_RUNNING,
    E_B590_NO_RC,
    E_B590_COMMAND_ON_CONSTRUCTION
};

enum {
    PACKET_B590_OK = 0,
    PACKET_B590_ERROR,
    PACKET_B590_FAIL
};
//--------------------------------------------------------------------------------
#define ERROR_E00_B590  (1 << 0)
#define ERROR_E01_B590  (1 << 1)
#define ERROR_E02_B590  (1 << 2)
#define ERROR_E03_B590  (1 << 3)
#define ERROR_E04_B590  (1 << 4)
#define ERROR_E05_B590  (1 << 5)
#define ERROR_E06_B590  (1 << 6)
#define ERROR_E07_B590  (1 << 7)
#define ERROR_E08_B590  (1 << 8)
#define ERROR_E09_B590  (1 << 9)
#define ERROR_E10_B590  (1 << 10)
#define ERROR_E11_B590  (1 << 11)
#define ERROR_E12_B590  (1 << 12)
#define ERROR_E13_B590  (1 << 13)
#define ERROR_E14_B590  (1 << 14)
#define ERROR_E15_B590  (1 << 15)
#define ERROR_E16_B590  (1 << 16)
#define ERROR_E17_B590  (1 << 17)
#define ERROR_E18_B590  (1 << 18)
#define ERROR_E19_B590  (1 << 19)
#define ERROR_E20_B590  (1 << 20)
#define ERROR_E21_B590  (1 << 21)
#define ERROR_E22_B590  (1 << 22)
#define ERROR_E23_B590  (1 << 23)
#define ERROR_E24_B590  (1 << 24)
#define ERROR_E25_B590  (1 << 25)
#define ERROR_E26_B590  (1 << 26)
#define ERROR_E27_B590  (1 << 27)
#define ERROR_E28_B590  (1 << 28)
#define ERROR_E29_B590  (1 << 29)
#define ERROR_E30_B590  (1 << 30)
#define ERROR_E31_B590  (1 << 31)
//--------------------------------------------------------------------------------
#define ERROR_E00_B590_TEXT      tr("E0 - низкая температура (температура < 5 градусов)")
#define ERROR_E01_B590_TEXT      tr("E1 - перегрев (температура > 75 градусов)")
#define ERROR_E02_B590_TEXT      tr("E2 - нет ответа АЦП (неответ 10 раз после 10 перезапусков каждые 160 мс) ")
#define ERROR_E03_B590_TEXT      tr("E3 - нет опорного напряжения на АЦП (5 раз проверки status)")
#define ERROR_E04_B590_TEXT      tr("E4 - ошибка АЦП  (5 раз проверки status)")
#define ERROR_E05_B590_TEXT      tr("E5 - ошибка контрольной суммы калибровочных данных в АЦП напряжения")
#define ERROR_E06_B590_TEXT      tr("E6 - ошибка контрольной суммы калибровочных данных в АЦП тока")
#define ERROR_E07_B590_TEXT      tr("E7 - ошибка контрольной суммы калибровочных данных в ЦАП напряжения")
#define ERROR_E08_B590_TEXT      tr("E8 - ошибка контрольной суммы калибровочных данных в ЦАП тока")
#define ERROR_E09_B590_TEXT      tr("E9 - неверна калибровка АЦП напряжения")
#define ERROR_E10_B590_TEXT      tr("E10 - неверна калибровка АЦП тока")
#define ERROR_E11_B590_TEXT      tr("E11 - неверна калибровка ЦАП напряжения")
#define ERROR_E12_B590_TEXT      tr("E12 - неверна калибровка ЦАП тока")
#define ERROR_E13_B590_TEXT      tr("E13 - нет заводской калибровки напряжения или она неверна")
#define ERROR_E14_B590_TEXT      tr("E14 - нет заводской калибровки тока или она неверна")
#define ERROR_E15_B590_TEXT      tr("E15 - пропадание питания - после чтение функцией 0x46 - обнуляется")
#define ERROR_E16_B590_TEXT      tr("E16 - попытка записи в исполняемый профиль")
#define ERROR_E17_B590_TEXT      tr("E17 - неверный номер профиля")
#define ERROR_E18_B590_TEXT      tr("E18 - профиль пуст")
#define ERROR_E19_B590_TEXT      tr("E19 - неверный номер точки")
#define ERROR_E20_B590_TEXT      tr("E20 - превышено максимальное значение напряжения")
#define ERROR_E21_B590_TEXT      tr("E21 - превышено максимальное значение тока")
#define ERROR_E22_B590_TEXT      tr("E22 - превышено максимальное значение времени")
#define ERROR_E23_B590_TEXT      tr("E23 - попытка установить неверную скорость обмена")
#define ERROR_E24_B590_TEXT      tr("E24 - попытка включить реверс при его отсутствии")
#define ERROR_E25_B590_TEXT      tr("E25 - reserved")
#define ERROR_E26_B590_TEXT      tr("E26 - reserved")
#define ERROR_E27_B590_TEXT      tr("E27 - reserved")
#define ERROR_E28_B590_TEXT      tr("E28 - reserved")
#define ERROR_E29_B590_TEXT      tr("E29 - reserved")
#define ERROR_E30_B590_TEXT      tr("E30 - reserved")
#define ERROR_E31_B590_TEXT      tr("E31 - reserved")
//--------------------------------------------------------------------------------
class QSerialPort;
class QTextEdit;
class QMenu;
//--------------------------------------------------------------------------------
class Powersupply_B590 : public QObject
{
    Q_OBJECT

public:
    Powersupply_B590(QObject *parent = 0);
    ~Powersupply_B590();

    bool port_is_open(void);

    int print_last_error(void);
    int get_last_error(void);

    void print_adv_error(uint32_t bits, uint16_t code);

    void set_delay_ms(int delay_ms);
    int get_delay_ms(void);

    int check_dirty_packet(const QByteArray &data);

    bool get_temperature(uint8_t *temp,
                         bool *flag_zummer);
    bool get_accurate_temperature(double *temp);
    void set_state_silence(bool state);
    bool set_UI(int32_t  voltage_mV,
                uint16_t current_mA);
    bool set_UI_parrot(uint16_t voltage,
                       uint16_t current);
    bool set_vent_speed(uint16_t speed);
    bool get_UI(int32_t  *in_voltage,
                uint16_t *in_current,
                int32_t  *out_voltage,
                uint16_t *out_current,
                int8_t   *temp,
                uint8_t  *active_profile);
    bool get_ADC(uint32_t *U_parrot,
                 uint32_t *I_parrot,
                 uint32_t *U_input_parrot);
    bool run_calibration(char code);
    bool get_calibration_state(uint8_t *state);
    bool get_current_calibration_point(uint8_t *point, uint8_t *state);

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

    bool get_calibration_zero(s_zero_b590_U *zero_U,
                              s_zero_b590_I *zero_I);

    bool copy_user_calibration_point_to_factory(void);

    bool get_info(QTextEdit *log);

    //---
    void set_ignore_bad_cmd(bool state);

    bool test(void);

    //---
    bool Set_DAC_U_I(signed int U,signed int I);
    bool Set_Auto_Send_Power_Unit_Get_real_U(void);
    bool Set_Auto_Send_Power_Unit_Get_ADC_U(void);
    bool Save_Calib_Point_DAC(uint8_t number_point,
                              uint16_t data_point);
    bool Send_Command_To_Calib_ADC_U(void);
    bool Send_Command_To_Calib_ADC_I(void);
    bool Send_Command_To_Calib_DAC_U(void);
    bool Send_Command_To_Calib_DAC_I(void);
    bool Send_Command_To_Next_Point(void);
    bool Test_Calib_Point_DAC(uint8_t number_point);
    bool Set_Auto_Send_Power_Unit_Get_Setting_DAC_U(void);
    bool Set_Ventilator_MAX_Speed(void);
    //---

    QWidget *add_frame_find_powersupply(void);

    bool get_error(uint32_t *bits, uint16_t *code);

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

    void close_connect(void);

    void run_dialog(int index);

    void run_dialog_0x11(void);
    void run_dialog_0x12(void);
    void run_dialog_0x15(void);
    void run_dialog_0x16(void);
    void run_dialog_0x17(void);

    void run_dialog_0x41(void);
    void run_dialog_0x42(void);
    void run_dialog_0x46(void);
    void run_dialog_0x47(void);
    void run_dialog_0x48(void);
    void run_dialog_0x49(void);
    void run_dialog_0x4A(void);

    void run_dialog_0x54(void);
    void run_dialog_0x55(void);
    void run_dialog_0x56(void);
    void run_dialog_0x57(void);
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

    void run_dialog_0x71(void);
    void run_dialog_0x72(void);
    void run_dialog_0x73(void);
    void run_dialog_0x75(void);

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
#define B590_CMD_0x54_TEXT tr("Б5-90 функция 0x54: Запрос данных по профилям.")
#define B590_CMD_0x56_TEXT tr("Б5-90 функция 0x56: Чтение профиля по точкам.")
#define B590_CMD_0x5E_TEXT tr("Б5-90 функция 0x5E: Запись данных точки в профиль для исполнения.")
#define B590_CMD_0x5F_TEXT tr("Б5-90 функция 0x5F: Запуск профиля на исполнение.")
#define B590_CMD_0x6E_TEXT tr("Б5-90 функция 0x6E: Запись количества повторов в профиль.")
#define B590_CMD_0x6F_TEXT tr("Б5-90 функция 0x6F: Очистка профиля")
#endif

    bool get_data_profile(int number_profile,
                          uint8_t *count_point,
                          uint8_t *count_repeat_profile);
    bool get_point_profile(uint8_t number_profile,
                           uint8_t number_point,
                           uint8_t *r_profile,
                           uint8_t *r_point,
                           uint16_t *r_voltage,
                           uint16_t *r_current,
                           uint16_t *r_time);
    bool add_point_profile(uint8_t number_profile,
                           uint16_t voltage,
                           uint16_t current,
                           uint16_t time);
    bool run_profile(uint8_t number_profile,
                     uint8_t *r_profile);
    bool add_count_repeat_profile(int number_profile,
                                  int repeat_count);
    bool clear_profile(int number_profile);

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
    SerialPort_B590_test serial;
#else
    QSerialPort serial;
#endif

    bool is_ready;
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

    int check_packet_answer(uint8_t cmd,
                            int packet_len);

    int send_cmd_0x11(void);
    int send_cmd_0x12(void);
    int send_cmd_0x15(void);
    int send_cmd_0x16(void);
    int send_cmd_0x17(void);

    int send_cmd_0x41(void);
    int send_cmd_0x42(uint8_t code_speed);
    int send_cmd_0x46(uint8_t  *type,
                      uint16_t *year,
                      uint8_t  *month,
                      uint16_t *serno);
    int send_cmd_0x47(int32_t  *in_voltage,
                      uint16_t *in_current,
                      int32_t  *out_voltage,
                      uint16_t *out_current,
                      int8_t   *temp,
                      uint8_t  *active_profile,
                      bool *flag_zummer);
    int send_cmd_0x48(uint8_t new_address_MODBUS,
                      uint8_t *r_new_address_MODBUS);
    int send_cmd_0x49(int32_t voltage,
                      uint16_t current);
    int send_cmd_0x4A(uint32_t *bits, uint16_t *code);

    int send_cmd_0x54(uint8_t profile,
                      uint8_t *count_point,
                      uint8_t *count_repeat_profile);
    int send_cmd_0x55(unsigned int *mototime_min);
    int send_cmd_0x56(uint8_t  profile,
                      uint8_t  point,
                      uint8_t  *r_profile,
                      uint8_t  *r_point,
                      uint16_t *r_voltage,
                      uint16_t *r_current,
                      uint16_t *r_time);
    int send_cmd_0x57(uint16_t U,
                      uint16_t I);
    int send_cmd_0x5A(uint16_t *setting_U,
                      uint16_t *setting_I,
                      uint16_t *current_U,
                      uint16_t *current_I,
                      uint8_t *temperature,
                      uint8_t *active_profile,
                      uint8_t *point);
    int send_cmd_0x5B(uint32_t *U,
                      uint32_t *I,
                      uint32_t *U_input);
    int send_cmd_0x5C(s_zero_b590_U zero_U,
                      s_zero_b590_I zero_I,
                      uint8_t       *r_error);
    int send_cmd_0x5D(s_zero_b590_U *zero_U,
                      s_zero_b590_I *zero_I);
    int send_cmd_0x5E(uint8_t profile,
                      uint16_t voltage_mV,
                      uint16_t current_mA,
                      uint16_t time,
                      uint16_t *r_error,
                      uint8_t  *r_profile,
                      uint16_t *r_voltage,
                      uint16_t *r_current,
                      uint16_t *r_time);
    int send_cmd_0x5F(uint8_t profile,
                      uint8_t *number_profile);

    int send_cmd_0x60(void);
    int send_cmd_0x61(int32_t  *Value_ADC_Zero_U, double *factor_k_DAC, double *factor_k_ADC);
    int send_cmd_0x62(int32_t  *Value_ADC_Zero_I,
                      double *factor_k_DAC, double *factor_k_ADC);
    int send_cmd_0x63(uint8_t code,
                      uint8_t number_point,
                      uint16_t data_point,
                      uint8_t *r_number_point,
                      uint16_t *r_data_point);
    int send_cmd_0x64(char cmd);
    int send_cmd_0x65(uint8_t *number_current_point,
                      uint8_t *state);
    int send_cmd_0x66(uint16_t code,
                      uint16_t index,
                      uint16_t *value);
    int send_cmd_0x67(uint16_t speed_cooler,
                      uint16_t *r_speed_cooler);
    int send_cmd_0x68(uint16_t data_PWM,
                      uint16_t *r_data_PWM);
    int send_cmd_0x69(uint16_t *OCR3A,
                      uint16_t *OCR3B);
    int send_cmd_0x6A(void);
    int send_cmd_0x6B(void);
    int send_cmd_0x6C(unsigned int number_string);
    int send_cmd_0x6D(uint8_t secret_cmd,
                      uint8_t input_data[],
                      uint8_t *r_secret_cmd,
                      uint8_t *result,
                      QByteArray *output_data);

    int send_cmd_0x6E(uint8_t profile,
                      uint8_t repeat,
                      uint8_t *r_profile,
                      uint8_t *r_repeat);
    int send_cmd_0x6F(uint8_t profile,
                      uint8_t *r_profile);

    int send_cmd_0x71(uint8_t *state_ADC_U,
                      uint8_t *state_ADC_I,
                      uint8_t *state_ADC_Ui);
    int send_cmd_0x72(uint16_t conf_ADC,
                      uint16_t *r_conf_ADC);
    int send_cmd_0x73(uint16_t *real_DAC_U,
                      uint16_t *real_DAC_I,
                      uint8_t *revers,
                      uint16_t *temperature);

    int send_cmd_0x75(uint8_t *count_ReStart_ADC_1,
                      uint8_t *count_ReStart_ADC_0,
                      uint8_t *count_ReStart_ADC_2,
                      uint16_t *count_Error_Answer_ADC,
                      unsigned int *count_Error_ADC,
                      uint16_t *MCUCSR);

#ifdef NEEDED_PAUSE
    void pause(unsigned int msec = 3000);
#endif

protected:
    bool event(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif
