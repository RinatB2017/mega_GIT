/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef V764_1_MULTIMETER_HPP
#define V764_1_MULTIMETER_HPP
//--------------------------------------------------------------------------------
#include <QDialogButtonBox>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QHBoxLayout>
#include <QVariant>
#include <QMenu>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#define MULTIMETER_NAME    "Мультиметр В7-64/1"
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
enum
{
    E_V764_1_NO_ERROR = 0,
    E_V764_1_ERROR_INDEX_NOT_CORRECT,
    E_V764_1_ERROR_PORT_NOT_OPEN,
    E_V764_1_ERROR_BREAK,
    E_V764_1_ERROR_MAX_REPEAT,
    E_V764_1_ERROR_NO_ANSWER,
    E_V764_1_ERROR_BIG_ANSWER,
    E_V764_1_ERROR_SMALL_ANSWER,
    E_V764_1_ERROR_BAD_SERNO,
    E_V764_1_ERROR_BAD_ADDRESS,
    E_V764_1_ERROR_BAD_COMMAND,
    E_V764_1_ERROR_BAD_CRC,
    E_V764_1_ERROR_BAD_LEN,
    E_V764_1_ERROR_UNKNOWN_COMMAND,
    E_V764_1_ERROR_SIZE_NOT_CORRECT,
    E_V764_1_UNKNOWN_ERROR,
    E_V764_1_INVALID_FLOAT,
    E_V764_1_INVALID_LIMIT,
    E_V764_1_INVALID_DELTA,
    E_V764_1_BREAK_MEASURING,
    E_V764_1_ILLEGAL_FUNCTION,
    E_V764_1_ILLEGAL_DATA_VALUE,
    E_V764_1_OVERLOAD,
    E_V764_1_LO_FREQ
};
enum MODE {
    UDC = 1000,
    UAC,
    IDC,
    IAC,
    R,
    HZ,
    MHZ
};
enum STATE {
    WAIT_CR = 0,
    WAIT_LF
};
//--------------------------------------------------------------------------------
#define CR  0x0D
#define LF  0x0A
//--------------------------------------------------------------------------------
#define BUTTON_NUMBER_UDC  5
#define BUTTON_NUMBER_IDC  6
#define BUTTON_NUMBER_R    7
#define BUTTON_NUMBER_UAC  8
#define BUTTON_NUMBER_IAC  9
#define BUTTON_NUMBER_HZ   10
#define BUTTON_NUMBER_MHZ  11
#define BUTTON_NUMBER_AVR  12

#define BUTTON_CODE_UDC  0x35
#define BUTTON_CODE_IDC  0x36
#define BUTTON_CODE_R    0x37
#define BUTTON_CODE_UAC  0x38
#define BUTTON_CODE_IAC  0x39
#define BUTTON_CODE_HZ   0x41
#define BUTTON_CODE_MHZ  0x42
#define BUTTON_CODE_AVR  0x43

#define MODE_UDC  0
#define MODE_IDC  1
#define MODE_R    2
#define MODE_UAC  3
#define MODE_IAC  4
#define MODE_HZ   5
#define MODE_MHZ  6
//--------------------------------------------------------------------------------
#define BUTTON_1    0x31
#define BUTTON_2    0x32
#define BUTTON_3    0x33
#define BUTTON_4    0x34
#define BUTTON_5    0x35
#define BUTTON_6    0x36
#define BUTTON_7    0x37
#define BUTTON_8    0x38
#define BUTTON_9    0x39
#define BUTTON_10   0x41
#define BUTTON_11   0x42
#define BUTTON_12   0x43
#define BUTTON_13   0x44
#define BUTTON_14   0x45
#define BUTTON_15   0x46
#define BUTTON_16   0x47
//--------------------------------------------------------------------------------
#define MIN_TIME    1
#define MAX_TIME    0xFFFF

#define MIN_ADDRESS 1
#define MAX_ADDRESS 255

#define MIN_CODE_BUTTON 1
#define MAX_CODE_BUTTON 16
//--------------------------------------------------------------------------------
union U_BYTE
{
    unsigned char value;
    struct
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    } bites;
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
class SerialBox5_wo_form;
//--------------------------------------------------------------------------------
class Multimeter_V764_1 : public QWidget
{
    Q_OBJECT

public:
    explicit Multimeter_V764_1(QWidget *parent = nullptr);
    virtual ~Multimeter_V764_1();

    int init_modbus(void);
    int init_terminal(void);

    int print_last_error(void);
    int get_last_error(void);

    bool port_is_open(void);

    bool find_device(void);
    bool find_address(int begin_address, int end_address, int *r_address);

    void set_delay_ms(int delay_ms);
    void set_state_silence(bool state);

    //---
    bool set_state_measuring_UDC(void);
    bool set_state_measuring_UAC(void);
    bool set_state_measuring_IDC(void);
    bool set_state_measuring_IAC(void);
    bool set_state_measuring_R(void);
    bool set_state_measuring_HZ(void);
    bool set_state_measuring_MHZ(void);
    bool set_state_AVR(void);

    bool send_cmd(const char btn);

    bool get_value(int *value_uV);
    //---

    int send_cmd_0x41(void);
    int send_cmd_0x42(unsigned char baud,
                      unsigned char time);
    int send_cmd_0x45(void);
    int send_cmd_0x46(void);
    int send_cmd_0x47(int *value_uV);
    int send_cmd_0x48(unsigned char new_address);
    int send_cmd_0x53(unsigned char number_button_1_16);

    QMenu *add_menu(void);

    void close_connect(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void measurement_mode(MODE);
    void measurement_mode(int);
    void current_limit(int);
    void current_sign(int);

public slots:
    void check_multimeter_v762_2(void);

    void set_address(int new_address);
    unsigned char get_address(void);

    void run_dialog_0x41(void);
    void run_dialog_0x42(void);
    void run_dialog_0x45(void);
    void run_dialog_0x46(void);
    void run_dialog_0x47(void);
    void run_dialog_0x48(void);
    void run_dialog_0x53(void);

#ifdef TEST
    void receive(const QVariant &data);
#endif

private slots:
    void port_read(const QByteArray &data);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
#ifndef TEST
    SerialBox5_wo_form *serial = nullptr;
#else
    SerialPort_V764_2_test *serial = nullptr;
#endif

    bool is_ready = false;
    QByteArray data_multimeter;
    int delay_ms = 0;
    int last_error = 0;
    int address = 0;

    bool flag_closed = false;
    bool is_silence = false;

    void init(void);

    void wait(int time_msec);
    void wait(int max_len,
              int min_time_msec,
              int max_time_msec);

    void msgError(const QString &message);

    int check_packet_answer(unsigned char cmd,
                            int packet_len);

protected:
    bool event(QEvent *e);
};
//--------------------------------------------------------------------------------
#endif //
