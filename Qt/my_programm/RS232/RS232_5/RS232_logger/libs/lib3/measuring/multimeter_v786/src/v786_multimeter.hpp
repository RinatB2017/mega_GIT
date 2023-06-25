/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef V786_MULTIMETER_HPP
#define V786_MULTIMETER_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "serialbox5_wo_form.hpp"
//--------------------------------------------------------------------------------
#ifdef TEST
#   include "v786_serialport_test.hpp"
#endif
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)

union BYTE_MEASURING
{
    unsigned char value;
    struct
    {
        unsigned limit:3;
        unsigned auto_limit:1;
        unsigned deviation:2; // отклонение
        unsigned retention:2; // удержание
    } data;
};

union BYTE_LIMITS
{
    unsigned char value;
    struct
    {
        unsigned limit:3;
        unsigned auto_limit:1;
        unsigned reserved:4;
    } data;
};

union BYTE_DEVIATION_RETENTION
{
    unsigned char value;
    struct
    {
        unsigned deviation:2;
        unsigned retention:2;
        unsigned reserved:4;
    } data;
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
#define SIZE_CRC 3 // 2 байта контрольной суммы + 1 байт канала
//--------------------------------------------------------------------------------
#define ERROR_01 0x01
#define ERROR_01_TEXT   tr("Команда не опознана")

#define ERROR_02 0x02
#define ERROR_02_TEXT   tr("Ошибка CRC")

#define ERROR_03 0x03
#define ERROR_03_TEXT   tr("Значение в поле данных не является допустимым для данной команды.")

#define ERROR_04 0x04
#define ERROR_04_TEXT   tr("Выполнение текущей команды прервано.")

#define ERROR_05 0x05
#define ERROR_05_TEXT   tr("Прибор принял команду, но будет выполнять ее длительное время (предотвращает ошибку по таймауту)")

#define ERROR_06 0x06
#define ERROR_06_TEXT   tr("Длина посылки меньше возможной")

#define ERROR_07 0x07
#define ERROR_07_TEXT   tr("Длина посылки больше допустимой")

#define ERROR_08 0x08
#define ERROR_08_TEXT   tr("Длина области данных не совпадает с указанной")

#define ERROR_09 0x09
#define ERROR_09_TEXT   tr("Удаленное управление не включено")

#define ERROR_0A 0x0A
#define ERROR_0A_TEXT   tr("Ошибка записи во flash память прибора")

#define ERROR_0B 0x0B
#define ERROR_0B_TEXT   tr("Прибор занят зарядом АКБ")

#define ERROR_0C 0x0C
#define ERROR_0C_TEXT   tr("Напряжение заряда выше допустимого")

#define ERROR_0D 0x0D
#define ERROR_0D_TEXT   tr("Напряжение заряда ниже допустимого")

#define ERROR_0E 0x0E
#define ERROR_0E_TEXT   tr("Батарея не заряжаемая или неисправна")

#define ERROR_0F 0x0F
#define ERROR_0F_TEXT   tr("Нагрузочная способность источника недостаточна для заряда")

#define ERROR_10 0x10
#define ERROR_10_TEXT   tr("Не проведена калибровка запрашиваемого режима измерения")

#define ERROR_11 0x11
#define ERROR_11_TEXT   tr("Величина, подаваемая на вход при калибровке, больше или меньше допустимой (± 25%)")

#define ERROR_12 0x12
#define ERROR_12_TEXT   tr("Ошибка доступа.")

#define ERROR_13 0x13
#define ERROR_13_TEXT   tr("Перегрузка (в бОльшую сторону)")

#define ERROR_14 0x14
#define ERROR_14_TEXT   tr("Перегрузка (в меньшую сторону)")

#define ERROR_15 0x15
#define ERROR_15_TEXT   tr("Недостаточная частота или постоянное напряжение на входе")

#define ERROR_16 0x16
#define ERROR_16_TEXT   tr("Не выполнена калибровка 0 требуемого режима")

#define ERROR_17 0x17
#define ERROR_17_TEXT   tr("Режим заряда не включен")
//--------------------------------------------------------------------------------
#define L_200mV     0
#define L_2V        1
#define L_20V       2
#define L_200V      3
#define L_1000V     4

#define L_200mV_TEXT "200 mV"
#define L_2V_TEXT    "2 V"
#define L_20V_TEXT   "20 V"
#define L_200V_TEXT  "200 V"
#define L_1000V_TEXT "1000 V"

#define L_200mkA    0
#define L_2mA       1
#define L_20mA      2
#define L_200mA     3
#define L_2000mA    4
#define L_20A       5

#define L_200mkA_TEXT   "200 mkA"
#define L_2mA_TEXT      "2 mA"
#define L_20mA_TEXT     "20 mA"
#define L_200mA_TEXT    "200 mA"
#define L_2000mA_TEXT   "2000 mA"
#define L_20A_TEXT      "20 A"

#define L_200o      0
#define L_2kOm      1
#define L_20kOm     2
#define L_200kOm    3
#define L_2Mom      4

#define L_200o_TEXT   "200"
#define L_2kOm_TEXT   "2 kOm"
#define L_20kOm_TEXT  "20 kOm"
#define L_200kOm_TEXT "200 kOm"
#define L_2Mom_TEXT   "2 MOm"

#define L_C_0pF     0
#define L_C_100pF   0
#define L_C_1000pF  0
#define L_C_0_1mkF  0
#define L_C_1mkF    0
#define L_C_10mkF   0
#define L_C_1000mkF 0

#define L_L_0mH     0
#define L_L_1mH     0
#define L_L_10mH    0
#define L_L_100mH   0

//--------------------------------------------------------------------------------
#define VALUE_U 0
#define VALUE_I 1
#define VALUE_R 2

#define VALUE_U_TEXT "U"
#define VALUE_I_TEXT "I"
#define VALUE_R_TEXT "R"
//--------------------------------------------------------------------------------
#define E_ERROR_MULTIMETR_TEXT          tr("Ошибка мультиметра!")
#define E_ERROR_INDEX_NOT_CORRECT_TEXT  tr("Неверный индекс!")
#define E_ERROR_PORT_NOT_OPEN_TEXT      tr("Порт мультиметра В7-86 не открыт!")
#define E_ERROR_BREAK_TEXT              tr("Выполнение команды прервано!")
#define E_ERROR_MAX_REPEAT_TEXT         tr("Превышено количество повторов!")
#define E_ERROR_NO_ANSWER_TEXT          tr("Мультиметр не отвечает!")
#define E_ERROR_BIG_ANSWER_TEXT         tr("Длина пакета слишком большая!")
#define E_ERROR_SMALL_ANSWER_TEXT       tr("Длина пакета слишком маленькая!")
#define E_ERROR_SMALL_PACKET_TEXT       tr("Длина пакета слишком маленькая!")
#define E_ERROR_BAD_SERNO_TEXT          tr("Неверный серийный номер или год изготовления!")
#define E_ERROR_BAD_ADDRESS_TEXT        tr("Неверный адрес прибора!")
#define E_ERROR_BAD_CRC_TEXT            tr("Плохая контрольная сумма!")
#define E_ERROR_BAD_CMD_TEXT            tr("Неверная команда!")
#define E_ERROR_BAD_LEN_TEXT            tr("Неверная длина пакета!")
#define E_ERROR_UNKNOWN_CMD_TEXT        tr("Неизвестная команда!")
#define E_ERROR_SIZE_NOT_CORRECT_TEXT   tr("Длина данных не соответствует заявленной!")
#define E_UNKNOWN_ERROR_TEXT            tr("Неизвестная ошибка!")
#define E_INVALID_FLOAT_TEXT            tr("Неверное число!")
#define E_INVALID_LIMIT_TEXT            tr("Неверный предел!")
#define E_INVALID_DELTA_TEXT            tr("Неверная дельта!")
#define E_BREAK_MEASURING_TEXT          tr("Измерения прерваны!")
//--------------------------------------------------------------------------------
const QString breloque_answer("don't worry, be happy");
//--------------------------------------------------------------------------------
class Multimeter_V786 : public QObject
{
    Q_OBJECT

public:
    enum
    {
        E_NO_ERROR = 0x1000,
        E_ERROR_MULTIMETR,
        E_ERROR_INDEX_NOT_CORRECT,
        E_ERROR_PORT_NOT_OPEN,
        E_ERROR_BREAK,
        E_ERROR_MAX_REPEAT,
        E_ERROR_NO_ANSWER,
        E_ERROR_BIG_ANSWER,
        E_ERROR_SMALL_ANSWER,
        E_ERROR_SMALL_PACKET,
        E_ERROR_BAD_SERNO,
        E_ERROR_BAD_ADDRESS,
        E_ERROR_BAD_CRC,
        E_ERROR_BAD_CMD,
        E_ERROR_BAD_LEN,
        E_ERROR_UNKNOWN_CMD,
        E_ERROR_SIZE_NOT_CORRECT,
        E_UNKNOWN_ERROR,
        E_INVALID_FLOAT,
        E_INVALID_LIMIT,
        E_INVALID_DELTA,
        E_BREAK_MEASURING,
    };
    enum
    {
        RETENTION_0 = 0,
        RETENTION_1 = 1,
        RETENTION_2 = 2,
        RETENTION_3 = 3
    };
    enum
    {
        DEVIATION_0 = 0,
        DEVIATION_1 = 1,
        DEVIATION_2 = 2
    };

    Multimeter_V786(QObject *parent = nullptr);
    ~Multimeter_V786();

    bool find_device(QString *port_name);

    bool find_cmd(int cmd);

    void set_retention(char value);
    void set_deviation(char value);
    void set_limit(char value);
    void set_auto_limit(bool state);
    void set_timeout(unsigned int value);

    void set_address(unsigned int value);
    void set_channel(unsigned int value);
    void set_serno(unsigned int value);
    void set_year(unsigned int value);

#ifdef TEST
    void set_test_address(uint8_t value);
    void set_test_channel(uint8_t value);
    void set_test_serno(uint16_t value);
    void set_test_year(uint16_t value);
#endif

    void set_silence(bool state);

    bool service_on(int timeout);

    bool remote_control_on(void);
    bool remote_control_off(void);
    bool write_to_flash(void);
    bool measuring_UDC(double *value);
    bool measuring_UAC(double *value);
    bool measuring_IDC(double *value);
    bool measuring_IAC(double *value);
    bool measuring_F(double *value);
    bool measuring_R(double *value);
    bool measuring_L(double *value);
    bool measuring_C(double *value);

    bool measuring_P(double *value);
    bool measuring_Q(double *value);
    bool measuring_S(double *value);
    bool measuring_PF(double *value);

    int measuring_battery(bool *ok);

    bool power_off(void);
    bool continuity(void);

    bool calibration_UDC(void);
    bool calibration_UAC(void);
    bool calibration_IDC(void);
    bool calibration_IAC(void);
    bool calibration_R(void);

    bool get_ID(void);
    bool get_state(void);

    void set_step_a(unsigned char value);
    void set_step_b(unsigned char value);
    void set_step_c(unsigned char value);

    unsigned char get_step_a(void);
    unsigned char get_step_b(void);
    unsigned char get_step_c(void);

    void set_data_len(unsigned char value);
    void set_data_0(unsigned char value);
    void set_data_1(unsigned char value);

    unsigned char get_data_len(void);
    unsigned char get_data_0(void);
    unsigned char get_data_1(void);

    unsigned char get_deviation_retention(bool *ok);
    unsigned char get_limit(unsigned char cmd, bool *ok);
    int get_deviation(bool *ok);
    QString get_deviation_string(void);
    QString get_retention_string(void);

    bool port_is_open(void);

    int print_last_error(void);
    int get_last_error(void);

    void print_cmd_text(int cmd);
    QString get_cmd_text(int cmd);

    void test(void);

    QByteArray get_test_state_answer(int cmd,
                                     int count_data,
                                     int serial_no,
                                     int serial_year,
                                     int range,
                                     int mode,
                                     int math_hold,
                                     int batt_voltage);
    QByteArray get_test_service_on_answer(int cmd,
                                          int count_data,
                                          int serial_no,
                                          int serial_year);
    QByteArray get_test_power_off_answer(int cmd,
                                         int count_data,
                                         int serial_no,
                                         int serial_year);
    QByteArray get_get_id_answer(int id,
                                 int count_data,
                                 int c1,
                                 int c2,
                                 int c3);

    QMenu *add_menu(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void change_retention(const QString &);
    void change_deviation(const QString &);
    void change_limit(const QString &);

    void change_auto_limit(bool);
    void change_timeout(unsigned int);
    void change_address(unsigned int);
    void change_channel(unsigned int);
    void change_serno(unsigned int);
    void change_year(unsigned int);

    void get_measuring_value(const QString &);
    void get_battery_value(unsigned short);

public slots:
    bool check_breloque(void);

    unsigned char get_retention(void);
    unsigned char get_deviation(void);
    unsigned char get_limit(void);
    bool get_auto_limit(void);
    unsigned int get_timeout(void);
    unsigned int get_address(void);
    unsigned int get_channel(void);
    unsigned int get_serno(void);
    unsigned int get_year(void);

    bool choice_options(int cmd,
                        int *serno_value,
                        int *year_value,
                        int *address_value,
                        int *channel_value);
    int send_cmd(int cmd);

    int run_dialog_0x01(void);
    int run_dialog_0x02(void);
    int run_dialog_0x03(void);
    int run_dialog_0x04(void);
    int run_dialog_0x05(void);
    int run_dialog_0x06(void);
    int run_dialog_0x07(void);
    int run_dialog_0x08(void);
    int run_dialog_0x09(void);
    int run_dialog_0x0A(void);
    int run_dialog_0x0B(void);
    int run_dialog_0x0C(void);
    int run_dialog_0x0D(void);
    int run_dialog_0x0E(void);
    int run_dialog_0x0F(void);
    int run_dialog_0x10(void);
    int run_dialog_0x11(void);
    int run_dialog_0x12(void);
    int run_dialog_0x13(void);
    int run_dialog_0x14(void);
    int run_dialog_0x15(void);
    int run_dialog_0x16(void);
    int run_dialog_0x17(void);
    int run_dialog_0x1B(void);
    int run_dialog_0x1C(void);
    int run_dialog_0x1D(void);

#ifdef TEST
    void receive(const QVariant &data);
#endif

private slots:
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef TEST
    SerialPort_V786_test *serial = nullptr;
#else
    SerialBox5_wo_form *serial = nullptr;
#endif

    unsigned char multimeter_retention;
    unsigned char multimeter_deviation;
    unsigned char multimeter_limit;
    bool multimeter_auto_limit;

    unsigned int multimeter_serno;
    unsigned int multimeter_year;
    unsigned int multimeter_address;
    unsigned int multimeter_channel;
    unsigned int multimeter_timeout;

    unsigned char multimeter_step_a;
    unsigned char multimeter_step_b;
    unsigned char multimeter_step_c;

    unsigned char multimeter_data_len;
    unsigned char multimeter_data_0;
    unsigned char multimeter_data_1;

    bool is_ready;
    QByteArray data_radiomodule;

    int last_error;

    bool is_silence;

    void init(void);

    int check_packet_answer(unsigned char cmd,
                            int packet_len);

    void wait(int timeout_msec);
    void wait(int max_len,
              int min_time_msec,
              int max_time_msec);

    QByteArray convert_ascii_to_data(QString input, bool *ok);
    QByteArray convert_data_to_ascii(QByteArray data);

    int send_raw_data(const QByteArray &data);

    int send(int command,
             QByteArray data,
             unsigned int serialno,
             unsigned int year,
             int timeout);

    QByteArray get_raw_data(void);

    int measuring(unsigned char cmd,
                  double *value,
                  bool *ok);
    bool get_value(int cmd, double *value);
    QString get_string_value(bool *ok);

    QString get_limit_string(unsigned int cmd,
                             unsigned char byte_options,
                             bool is_russian = true);
    QString get_limit_string(bool is_russian = true);
    void send_values(double measuring_value);
    unsigned char prepare_byte_measuring(void);

    int get_size_answer(int command);
};
//--------------------------------------------------------------------------------
#endif // MULTIMETER_V786_HPP
