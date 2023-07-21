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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QColorDialog>
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
//#   define htons(n) static_cast<uint16_t>((static_cast<uint16_t>(n) << 8)  | (static_cast<uint16_t>(n) >> 8))
//#   define htonl(n) static_cast<uint32_t>((static_cast<uint32_t>(n) << 16) | (static_cast<uint32_t>(n) >> 16))
#   include <arpa/inet.h>
#endif
#ifdef Q_OS_WIN
#   define htons(n) (uint16_t)((((uint16_t) (n)) << 8) | (((uint16_t) (n)) >> 8))
#   define htonl(n) (uint32_t)((((uint32_t) (n)) << 16) | (((uint32_t) (n)) >> 16))
#endif
//--------------------------------------------------------------------------------
#define MAX_EEPROM_BYTES    512
#define BEGIN_BYTE          0x55
#define END_BYTE            0xAA

#define MOTOR_MAX_ANGLE     255
#define LED_MAX_VALUE       255
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
enum {
    CMD_ZERO        = 0,

    CMD_SET_COLOR_R = 1,
    CMD_SET_COLOR_G = 2,
    CMD_SET_COLOR_B = 3,
    CMD_SET_COLOR_W = 4,
    CMD_SET_ALL_COLOR = 5,

    CMD_RUN_MOTOR_1 = 6,
    CMD_RUN_MOTOR_2 = 7,
    CMD_RUN_ALL_MOTOR = 8,

    CMD_SET_ALL = 9
};

//--------------------------------------------------------------------------------
#pragma pack (push, 1)

union MODBUS_FULL
{
    struct {
        unsigned char  address_slave;
        unsigned char  modbus_cmd;
        unsigned short address;
        unsigned short count;
        unsigned char  count_bytes;

        unsigned short command;
        unsigned short value_R;
        unsigned short value_G;
        unsigned short value_B;
        unsigned short value_W;
        unsigned short motor_1;
        unsigned short motor_2;
    } data;
    unsigned char buf[sizeof(data)];
};

#pragma pack (pop)
//--------------------------------------------------------------------------------
class MySplashScreen;
class QHexEdit;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void send(const QByteArray &);

private slots:
    void read_data(const QByteArray &data);

    void set_color(void);
    void update_frame_color(void);

    void test(void);

    void command_set_color_R(void);
    void command_set_color_G(void);
    void command_set_color_B(void);
    void command_set_color_W(void);
    void command_set_color_all(void);

    void command_run_motor_1(void);
    void command_run_motor_2(void);
    void command_run_motors(void);

    void color_changed(int);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
    QWidget *parent;

    uchar eeprom_buffer[MAX_EEPROM_BYTES+1];
    QByteArray sending_array;
    QByteArray received_array;
    bool data_is_ready = false;

    void init(void);

    void createTestBar(void);

    void clear_data(void);

    void append_data(char *buf, int len);
    void append_end_byte(void);

    void append_crc(void);

    void clear_sending_array(void);
    void clear_receive_array(void);
    void wait_msec(int timeout_msec);

    void send_data(void);

    unsigned char calculateCRC_8(char *pcBlock, uint len);
    unsigned int  calculateCRC_16(uchar *frame, uchar bufferSize);

    void prepare_modbus_command(unsigned short cmd);

    void prepare_modbus_color_R(void);
    void prepare_modbus_color_G(void);
    void prepare_modbus_color_B(void);
    void prepare_modbus_color_W(void);
    void prepare_modbus_color_all(void);

    void prepare_modbus_motor_1(void);
    void prepare_modbus_motor_2(void);
    void prepare_modbus_motor_all(void);

    void prepare_modbus_set_all(void);

    void set_color_frame(int color_R,
                         int color_G,
                         int color_B);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
