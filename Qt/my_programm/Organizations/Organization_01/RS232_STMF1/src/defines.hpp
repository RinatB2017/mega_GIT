#ifndef _DEFINES_HPP
#define _DEFINES_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "RS232_STMF1"
#define ID  0x12345678
//--------------------------------------------------------------------------------
#pragma pack(push, 1)
//--------------------------------------------------------------------------------

enum {
    CMD_TEST            = 0,
    CMD_INIT            = 1,
    CMD_GET_ID          = 2,
    CMD_GET_DATA        = 3,
    CMD_CALIBRATION_0	= 4,
    CMD_CALIBRATION_1	= 5,
    CMD_CALIBRATION_2	= 6,
    CMD_CALIBRATION_3	= 7,
    CMD_CALIBRATION_4	= 8,
    CMD_CALIBRATION_5	= 9
};

union CMD_QUESTION
{
    struct {
        unsigned char cmd;
        qint32        param;
        unsigned char crc8;
    } data;
    unsigned char buf[sizeof(data)];
};

union CMD_ANSWER
{
    struct {
        unsigned char cmd;
        unsigned char error;
        unsigned char state;
        qint64        value;
        unsigned char crc8;
    } data;
    unsigned char buf[sizeof(data)];
};

union UINT_TO_BYTES
{
    unsigned int value;
    struct {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
    };
};

union LONGLONG_TO_BYTES
{
    qint64 value;
    struct {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
        unsigned char e;
        unsigned char f;
        unsigned char g;
        unsigned char h;
    };
};

union U_SHORT_BYTES
{
    unsigned short value;
    struct {
        unsigned char byte_0;
        unsigned char byte_1;
    } bytes;
};

enum
{
    ERR_PASS		 		= 0,
    ERR_FAIL_CRC	 		= 1,
    ERR_WRONG_PACKET	        	= 2,
    ERR_ADC_DOWN			= 3,
    ERR_ADC_EMPTY			= 4,
    ERR_CAL_BIGZERO			= 5,
    ERR_CAL_BIGKOEFF	           	= 6,
    ERR_CAL_NOZERO			= 7,
    ERR_OVERLOAD			= 8,
    ERR_WILL_TAKE_TIME		= 9,
    ERR_BUSY				= 10,
    ERR_BAD_LEN				= 11,
    ERR_NO_ANSWER           = 12
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
