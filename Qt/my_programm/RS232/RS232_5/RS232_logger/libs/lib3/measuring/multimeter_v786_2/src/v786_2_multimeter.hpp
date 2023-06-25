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
#ifndef V786_2_MULTIMETER_HPP
#define V786_2_MULTIMETER_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QScrollArea>
//--------------------------------------------------------------------------------
#include <QElapsedTimer>
#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QMenu>
#include <QList>
//--------------------------------------------------------------------------------
#include "v786_2_serialport_test.hpp"
#include "serialbox5_wo_form.hpp"
#include "v786_2_packets.hpp"
#include "hexspinbox.hpp"
//--------------------------------------------------------------------------------
// просто число, нужное для уникальности адреса свистка
#define MAGIC_NUMBER    1000

#define P_MULTIMETER_PORTNAME   "multimeter_portname"
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)

union BYTE_MEASURING
{
    uint8_t value;
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
    uint8_t value;
    struct
    {
        unsigned limit:3;
        unsigned auto_limit:1;
        unsigned reserved:4;
    } data;
};

union BYTE_DEVIATION_RETENTION
{
    uint8_t value;
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
#define ERROR_02 0x02
#define ERROR_03 0x03
#define ERROR_04 0x04
#define ERROR_05 0x05
#define ERROR_06 0x06
#define ERROR_07 0x07
#define ERROR_08 0x08
#define ERROR_09 0x09
#define ERROR_0A 0x0A
#define ERROR_0B 0x0B
#define ERROR_0C 0x0C
#define ERROR_0D 0x0D
#define ERROR_0E 0x0E
#define ERROR_0F 0x0F
#define ERROR_10 0x10
#define ERROR_11 0x11
#define ERROR_12 0x12
#define ERROR_13 0x13
#define ERROR_14 0x14
#define ERROR_15 0x15
#define ERROR_16 0x16
#define ERROR_17 0x17
#define ERROR_18 0x18
#define ERROR_19 0x19
#define ERROR_1A 0x1A
#define ERROR_1B 0x1B
#define ERROR_1C 0x1C

#define ERROR_01_TEXT   "Команда не опознана"
#define ERROR_02_TEXT   "Ошибка CRC"
#define ERROR_03_TEXT   "Значение в поле данных не является допустимым для данной команды."
#define ERROR_04_TEXT   "Устройство занято"
#define ERROR_05_TEXT   "Прибор принял команду, но будет выполнять ее длительное время (предотвращает ошибку по таймауту)"
#define ERROR_06_TEXT   "Длина посылки меньше возможной"
#define ERROR_07_TEXT   "Длина посылки больше допустимой"
#define ERROR_08_TEXT   "Длина области данных не совпадает с указанной"
#define ERROR_09_TEXT   "Удаленное управление не включено"
#define ERROR_0A_TEXT   "Ошибка записи во flash память прибора"
#define ERROR_0B_TEXT   "RF_BUSY_BY_CHARGE"
#define ERROR_0C_TEXT   "RF_CHARGE_OVERVOLTAGE"
#define ERROR_0D_TEXT   "RF_CHARGE_UNDERVOLTAGE"
#define ERROR_0E_TEXT   "Батарея не заряжаемая или неисправна"
#define ERROR_0F_TEXT   "Нагрузочная способность источника недостаточна для заряда"
#define ERROR_10_TEXT   "Не проведена калибровка запрашиваемого режима измерения"
#define ERROR_11_TEXT   "Величина, подаваемая на вход при калибровке, больше или меньше допустимой (± 25%)"
#define ERROR_12_TEXT   "Ошибка доступа."
#define ERROR_13_TEXT   "Перегрузка (в бОльшую сторону)"
#define ERROR_14_TEXT   "Перегрузка (в меньшую сторону)"
#define ERROR_15_TEXT   "Недостаточная частота или постоянное напряжение на входе"
#define ERROR_16_TEXT   "Не выполнена калибровка 0 требуемого режима"
#define ERROR_17_TEXT   "RF_CHARGE_IS_DISABLED"
#define ERROR_18_TEXT   "RF_RX_ERR_FIFO_OVERFLOW"
#define ERROR_19_TEXT   "RF_RX_ERR_LENGTH_SO_SMALL"
#define ERROR_1A_TEXT   "RF_RX_ERR_CRC_FAIL"
#define ERROR_1B_TEXT   "RF_RX_ERR_ACK_CRC_FAIL"
#define ERROR_1C_TEXT   "Отсутствует калибровка нулей"
//--------------------------------------------------------------------------------
#define L_200mV     0
#define L_2V        1
#define L_20V       2
#define L_200V      3
#define L_750V      4
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
#define L_C_100pF   1
#define L_C_1000pF  2
#define L_C_0_1mkF  3
#define L_C_1mkF    4
#define L_C_10mkF   5
#define L_C_1000mkF 6

#define L_L_0mH     0
#define L_L_1mH     1
#define L_L_10mH    2
#define L_L_100mH   3

//--------------------------------------------------------------------------------
#define VALUE_U 0
#define VALUE_I 1
#define VALUE_R 2

#define VALUE_U_TEXT "U"
#define VALUE_I_TEXT "I"
#define VALUE_R_TEXT "R"
//--------------------------------------------------------------------------------
#define E_ERROR_MULTIMETR_TEXT          "Ошибка мультиметра!"
#define E_ERROR_INDEX_NOT_CORRECT_TEXT  "Неверный индекс!"
#define E_ERROR_PORT_NOT_OPEN_TEXT      "Порт мультиметра В7-86 не открыт!"
#define E_ERROR_BREAK_TEXT              "Выполнение команды прервано!"
#define E_ERROR_MAX_REPEAT_TEXT         "Превышено количество повторов!"
#define E_ERROR_NO_ANSWER_TEXT          "Мультиметр не отвечает!"
#define E_ERROR_BIG_ANSWER_TEXT         "Длина пакета слишком большая!"
#define E_ERROR_SMALL_ANSWER_TEXT       "Длина пакета слишком маленькая!"
#define E_ERROR_SMALL_PACKET_TEXT       "Длина пакета слишком маленькая!"
#define E_ERROR_BAD_SERNO_TEXT          "Неверный серийный номер или год изготовления!"
#define E_ERROR_BAD_ADDRESS_TEXT        "Неверный адрес прибора!"
#define E_ERROR_BAD_CRC_TEXT            "Плохая контрольная сумма!"
#define E_ERROR_BAD_CMD_TEXT            "Неверная команда!"
#define E_ERROR_BAD_LEN_TEXT            "Неверная длина пакета!"
#define E_ERROR_UNKNOWN_CMD_TEXT        "Неизвестная команда!"
#define E_ERROR_SIZE_NOT_CORRECT_TEXT   "Длина данных не соответствует заявленной!"
#define E_UNKNOWN_ERROR_TEXT            "Неизвестная ошибка!"
#define E_INVALID_FLOAT_TEXT            "Неверное число!"
#define E_INVALID_LIMIT_TEXT            "Неверный предел!"
#define E_INVALID_DELTA_TEXT            "Неверная дельта!"
#define E_BREAK_MEASURING_TEXT          "Измерения прерваны!"
//--------------------------------------------------------------------------------
class Multimeter_V786_2 : public MyWidget
{
    Q_OBJECT

signals:
    void update_serno(ushort);
    void update_year(ushort);

    void update_battery(uint16_t);
    void update_rssi(int);

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

    explicit Multimeter_V786_2(QWidget *parent = nullptr);
    virtual ~Multimeter_V786_2();

    QList<QPushButton *> toolbar_buttons;

    QMenu *add_menu(void);
    QToolBar *add_test_toolbar(void);

    void toolbar_btn_click(void);

    bool find_device(void);
    bool open_device(const QString &port_name);

    bool close_device(void);

    bool find_cmd(int cmd);

    void set_retention(char value);
    void set_deviation(char value);
    void set_limit(uint8_t value);
    void set_auto_limit(bool state);
    void set_timeout(unsigned int value);

    void set_address(unsigned int value);
    void set_channel(unsigned int value);
    void set_serno(unsigned int value);
    void set_year(unsigned int value);
    void set_remote_address(unsigned int value);

#ifdef TEST
    void set_test_address(uint8_t value);
    void set_test_channel(uint8_t value);
    void set_test_serno(uint16_t value);
    void set_test_year(uint16_t value);
#endif

    void set_silence(bool state);

    bool service_on(void);
    bool service_off(void);

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
    bool buzzer_test(void);

    bool calibration_UDC(void);
    bool calibration_UAC(void);
    bool calibration_IDC(void);
    bool calibration_IAC(void);
    bool calibration_R(void);

    bool get_state(void);
    void check_set_new_addres(void);

    bool test_diod(void);
    bool udc_cal_zeroes(void);
    bool idc_cal_zeroes(void);
    bool res_cal_zeroes(void);
    bool cap_cal_zeroes(void);
    bool ind_cal_zeroes(void);
    bool flash_write_zeroes(void);

    //TODO added 28.07.22
    bool read_point_from_memory(uint8_t table,
                                uint8_t index_0,
                                uint8_t index_1,
                                float *value);
    bool write_point_to_memory(uint8_t table,
                               uint8_t index_0,
                               uint8_t index_1,
                               float value);

    bool read_firmware_version(QString *f_version);
    bool get_rssi(int8_t *rssi);
    bool set_new_address(void);
    bool set_new_address(unsigned int new_address);

    bool fw_set_default_calibration_tables(void);
    bool fw_get_cnt_e21(uint16_t *cnt_e21);
    bool fw_test(void);
    //---

    void set_step_a(uint8_t value);
    void set_step_b(uint8_t value);
    void set_step_c(uint8_t value);

    uint8_t get_step_a(void);
    uint8_t get_step_b(void);
    uint8_t get_step_c(void);

    void set_data_len(uint8_t value);
    void set_data_0(uint8_t value);
    void set_data_1(uint8_t value);

    uint8_t get_data_len(void);
    uint8_t get_data_0(void);
    uint8_t get_data_1(void);

    uint8_t get_deviation_retention(bool *ok);
    uint8_t get_limit(uint8_t cmd, bool *ok);
    int get_deviation(bool *ok);
    QString get_deviation_string(void);
    QString get_retention_string(void);

    bool port_is_open(void);

    int print_last_error(void);
    int get_last_error(void);

    void print_cmd_text(int cmd);
    QString get_cmd_text(int cmd);

    bool set_new_param(RF_set_param param);
    bool set_new_param(RF_set_param_2 param, QByteArray *ba_res);

    void test(void);
    int send_raw_data(const QByteArray &data);

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
                                 int count_data);

signals:
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
    void get_battery_value(uint16_t);

public slots:
    bool check_breloque(void);
    bool check_version(void);
    bool get_version(int *version);

    bool set_breloque_address(uint16_t address);
    bool get_breloque_address(int *address);

    uint8_t get_retention(void);
    uint8_t get_deviation(void);
    uint8_t get_limit(void);
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

    QByteArray prepare_V764_2_RF_STATUS_REQUEST(void);
    QByteArray prepare_V764_2_RF_REMOTE_CONTROL_ENABLE(void);
    QByteArray prepare_V764_2_RF_REMOTE_CONTROL_DISABLE(void);
    QByteArray prepare_V764_2_RF_SERVICE_ENABLE(void);
    QByteArray prepare_V764_2_RF_POWER_OFF(void);
    QByteArray prepare_V764_2_RF_DC_VOLTAGE_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_AC_VOLTAGE_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_DC_CURRENT_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_AC_CURRENT_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_RESISTANCE_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_CAPACITANCE_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_INDUCTANCE_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_BUZZER_TEST(void);
    QByteArray prepare_V764_2_RF_FREQUENCY_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_ACTIVE_POWER_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_REACTIVE_POWER_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_FULL_POWER_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_COS_FI_MEASUREMENT(void);
    QByteArray prepare_V764_2_RF_UDC_CAL(void);
    QByteArray prepare_V764_2_RF_UAC_CAL(void);
    QByteArray prepare_V764_2_RF_IDC_CAL(void);
    QByteArray prepare_V764_2_RF_IAC_CAL(void);
    QByteArray prepare_V764_2_RF_RES_CAL(void);
    QByteArray prepare_V764_2_RF_FLASH_WRITE(void);
    QByteArray prepare_V764_2_RF_SERVICE_DISABLE(void);
    QByteArray prepare_V764_2_RF_DIODE_TEST(void);
    QByteArray prepare_V764_2_RF_UDC_CAL_ZEROES(void);
    QByteArray prepare_V764_2_RF_IDC_CAL_ZEROES(void);
    QByteArray prepare_V764_2_RF_RES_CAL_ZEROES(void);
    QByteArray prepare_V764_2_RF_CAP_CAL_ZEROES(void);
    QByteArray prepare_V764_2_RF_IND_CAL_ZEROES(void);
    QByteArray prepare_V764_2_RF_FLASH_WRITE_ZEROES(void);
    //TODO added 28.07.22
    QByteArray prepare_V764_2_RF_READ_POINT_FROM_MEMORY(void);
    QByteArray prepare_V764_2_RF_WRITE_POINT_TO_MEMORY(void);
    QByteArray prepare_V764_2_RF_READ_FIRMWARE_VERSION(void);
    QByteArray prepare_V764_2_RF_SET_NEW_ADDRESS(void);
    QByteArray prepare_V764_2_RF_GET_RSSI(void);
    QByteArray prepare_V764_2_RF_TEST(void);

    QByteArray prepare_V764_2_RF_GET_CNT_E21(void);
    QByteArray prepare_V764_2_RF_SET_DEFAULT_CALIBRATION_TABLES(void);

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
    int run_dialog_0x1E(void);
    int run_dialog_0x1F(void);
    int run_dialog_0x20(void);
    int run_dialog_0x21(void);
    int run_dialog_0x22(void);
    int run_dialog_0x23(void);

    //TODO added 28.07.22
    int run_dialog_0x50(void);
    int run_dialog_0x51(void);
    int run_dialog_0x52(void);
    int run_dialog_0x53(void);
    int run_dialog_0x54(void);

    //TODO added 27.09.22
    int run_dialog_0x55(void);
    //---

    int run_dialog_0x56(void);
    int run_dialog_0x57(void);

#ifdef TEST
    void receive(const QVariant &data);
#endif

private slots:
    void read_data(QByteArray data);
    //void data_ready(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifdef TEST
    SerialPort_V786_2_test *serial = nullptr;
#else
    SerialBox5_wo_form *serial = nullptr;
#endif

    uint8_t multimeter_retention = 0;
    uint8_t multimeter_deviation = 0;
    uint8_t multimeter_limit = 0;
    bool multimeter_auto_limit = false;

    uint multimeter_serno = 0;
    uint multimeter_year = 0;
    uint multimeter_address = 0;
    uint multimeter_channel = 0;

    uint16_t remote_address = 0;

    uint8_t point_table = 0;
    uint8_t point_index_0 = 0;
    uint8_t point_index_1 = 0;
    float point_value = 0;

    QSpinBox *sb_serno   = nullptr;
    QSpinBox *sb_year    = nullptr;
    QSpinBox *sb_address = nullptr;
    QSpinBox *sb_channel = nullptr;

    uint multimeter_timeout = 0;

    uint8_t multimeter_step_a = 0;
    uint8_t multimeter_step_b = 0;
    uint8_t multimeter_step_c = 0;

    uint8_t multimeter_data_len = 0;
    uint8_t multimeter_data_0 = 0;
    uint8_t multimeter_data_1 = 0;

    bool is_ready = false;
    QByteArray data_radiomodule;

    int last_error = 0;

    bool is_silence = false;

    // старый свисток "don't worry, be happy";
    // новый свисток  "use linux, bro!"

    QString breloque_answer = "don't worry, be happy";

    void init(void);

    int check_packet_answer(uint8_t cmd,
                            int packet_len);

    void wait(int timeout_msec);
    void wait(int max_len,
              int min_time_msec,
              int max_time_msec);

    QByteArray convert_ascii_to_data(QString input, bool *ok);
    QByteArray convert_data_to_ascii(QByteArray data);

    int send(int command,
             QByteArray data,
             unsigned int len_data,
             unsigned int serialno,
             unsigned int year);

    QByteArray get_raw_data(void);

    int measuring(uint8_t cmd,
                  double *value,
                  bool *ok);
    bool get_value(int cmd, double *value);
    QString get_string_value(bool *ok);

    QString get_limit_string(unsigned int cmd,
                             uint8_t byte_options,
                             bool is_russian = true);
    QString get_limit_string(bool is_russian = true);
    void send_values(double measuring_value);
    uint8_t prepare_byte_measuring(void);

    int get_size_answer(int command);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MULTIMETER_V786_2_HPP
