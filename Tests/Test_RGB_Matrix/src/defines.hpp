#ifndef _DEFINES_HPP
#define _DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_RGB_Matrix"
//--------------------------------------------------------------------------------
#define NUM_LEDS_PER_STRIP  6
#define NUM_STRIPS          8

#define MAX_SCREEN_X NUM_LEDS_PER_STRIP * 10
#define MAX_SCREEN_Y NUM_STRIPS
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef struct
{
  unsigned char  address;
  unsigned char  cmd;
  unsigned char  count_data;
}
RGB_HEADER;
//--------------------------------------------------------------------------------
union CMD_0x01_QUESTION
{
  struct BODY{
    RGB_HEADER header;
    char data[NUM_LEDS_PER_STRIP][NUM_STRIPS];
    unsigned short crc16;
  } body;
  unsigned char buf[sizeof(BODY)];
};

union CMD_0x01_ANSWER
{
  struct BODY{
    RGB_HEADER header;
    unsigned short crc16;
  } body;
  unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
#define RGB_CMD_0x01  0x01
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
