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
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QWidget>
#include <QLabel>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class TCP_Client;
class Settings;
#ifdef RS232
class SerialBox;
#endif
class LogBox;
class Test_RS232_1;
class Test_RS232_2;
class Test_RS232_3;
class Test_FLBI_1;
class Test_FLBI_2;
class Test_FLBI_3;
//--------------------------------------------------------------------------------
#define CMD_0x10		0x10
#define CMD_0x11		0x11
#define CMD_0x12		0x12
#define CMD_0x13		0x13
#define CMD_0x14		0x14
#define CMD_0x15		0x15
#define CMD_0x16		0x16
#define CMD_0x17		0x17
#define CMD_0x18		0x18
#define CMD_0x19		0x19
#define CMD_0x1A		0x1A
#define CMD_0x1B		0x1B
#define CMD_0x1C		0x1C
#define CMD_0x1D		0x1D
#define CMD_0x1E		0x1E
#define CMD_0x1F		0x1F

#define CMD_0x20		0x20
#define CMD_0x21		0x21
#define CMD_0x22		0x22
#define CMD_0x23		0x23
#define CMD_0x24		0x24
#define CMD_0x25		0x25
#define CMD_0x26		0x26
#define CMD_0x27		0x27
#define CMD_0x28		0x28
#define CMD_0x29		0x29
#define CMD_0x2A		0x2A
#define CMD_0x2B		0x2B
#define CMD_0x2C		0x2C
#define CMD_0x2D		0x2D
#define CMD_0x2E		0x2E
#define CMD_0x2F		0x2F

#define CMD_0x30		0x30
#define CMD_0x31		0x31
#define CMD_0x32		0x32
#define CMD_0x33		0x33
#define CMD_0x34		0x34
#define CMD_0x35		0x35
#define CMD_0x36		0x36
#define CMD_0x37		0x37
#define CMD_0x38		0x38
#define CMD_0x39		0x39
#define CMD_0x3A		0x3A
#define CMD_0x3B		0x3B
#define CMD_0x3C		0x3C
#define CMD_0x3D		0x3D
#define CMD_0x3E		0x3E
#define CMD_0x3F		0x3F

#define CMD_0x40		0x40
#define CMD_0x41		0x41
#define CMD_0x42		0x42
#define CMD_0x43		0x43
#define CMD_0x44		0x44
#define CMD_0x45		0x45
#define CMD_0x46		0x46
#define CMD_0x47		0x47
#define CMD_0x48		0x48
#define CMD_0x49		0x49
#define CMD_0x4A		0x4A
#define CMD_0x4B		0x4B
#define CMD_0x4C		0x4C
#define CMD_0x4D		0x4D
#define CMD_0x4E		0x4E
#define CMD_0x4F		0x4F

#define CMD_0x50		0x50
#define CMD_0x51		0x51

#define CMD_0x52		0x52
#define CMD_0x53		0x53
#define CMD_0x54		0x54
//--------------------------------------------------------------------------------
union U_BYTE
{
    unsigned char value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
    } bits;
};
//--------------------------------------------------------------------------------
#ifdef DEBUG_FRAME
class DebugFrame;
#endif
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent = 0);
    ~MainBox();

protected:
    void changeEvent(QEvent *e);

private slots:
    void updateText(void);
    void updateClient(void);

    void clear_main_checkable(void);
    void clear_lbl_value(void);
    int get_main_checkable(void);

    void clicked_btn_main_01(void);
    void clicked_btn_main_02(void);
    void clicked_btn_main_03(void);
    void clicked_btn_main_04(void);
    void clicked_btn_main_05(void);
    void clicked_btn_main_06(void);
    void clicked_btn_main_07(void);
    void clicked_btn_main_08(void);
    void clicked_btn_main_09(void);
    void clicked_btn_main_10(void);
    void clicked_btn_main_11(void);
    void clicked_btn_main_12(void);
    void clicked_btn_main_13(void);
    void clicked_btn_main_14(void);

    void clicked_btn_clear(void);

    void clicked_btn_out_01(bool state);
    void clicked_btn_out_02(bool state);
    void clicked_btn_out_03(bool state);
    void clicked_btn_out_04(bool state);
    void clicked_btn_out_05(bool state);
    void clicked_btn_out_06(bool state);
    void clicked_btn_out_07(bool state);
    void clicked_btn_out_08(bool state);

    void clicked_btn_in_01(void);
    void clicked_btn_in_02(void);
    void clicked_btn_in_03(void);
    void clicked_btn_in_04(void);
    void clicked_btn_in_05(void);
    void clicked_btn_in_06(void);
    void clicked_btn_in_07(void);
    void clicked_btn_in_08(void);

    void clicked_takt_1(void);
    void clicked_takt_2(void);
    void clicked_takt_3(void);
    void clicked_takt_4(void);
    void clicked_takt_5(void);
    void clicked_takt_6(void);
    void clicked_takt_7(void);
    void clicked_takt_8(void);
    void clicked_takt_9(void);

private:
    Ui::MainBox *ui = 0;
    QWidget *parent = 0;
    TCP_Client *client = 0;
    QByteArray data;
#ifdef RS232
    SerialBox *serialBox = 0;
#endif
    LogBox *logBox = 0;
    Test_RS232_1 *test_rs232_1 = 0;
    Test_RS232_2 *test_rs232_2 = 0;
    Test_RS232_3 *test_rs232_3 = 0;
    Test_FLBI_1  *test_flbi_1 = 0;
    Test_FLBI_2  *test_flbi_2 = 0;
    Test_FLBI_3  *test_flbi_3 = 0;

#ifdef DEBUG_FRAME
    DebugFrame *debug_frame;
#endif

    void init(void);
    void init_log(void);
    void init_tcp_client(void);
    void connects(void);
    void build_data(unsigned char cmd);
    void build_data_ext(unsigned char cmd, unsigned char cmd_ext);
    void load_setting(void);
    void save_setting(void);
    void message_error(const QString &);
    bool check(uchar test_byte,
               uchar true_byte,
               uchar number_table,
               uchar number_takt);
    bool check_xF(uchar test_byte,
                  uchar number_table,
                  uchar number_takt);
    bool check_x0(uchar test_byte,
                  uchar number_table,
                  uchar number_takt);
    bool check_bits(uchar test_byte,
                    uchar first_bit_pos,
                    uchar first_bit_value,
                    uchar second_bit_pos,
                    uchar second_bit_value,
                    uchar number_table,
                    uchar number_takt);

    void check_in(uchar test_byte,
                  uchar true_byte,
                  QLabel *label);

    void print(const QString&, int = 0, bool = false);

protected:
    void customEvent(QEvent*);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
