#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_RGB_Matrix"
//--------------------------------------------------------------------------------
#define NUM_LEDS_PER_STRIP  32
#define NUM_STRIPS          16

#define LED_SIZE    16

#define MAX_SCREEN_X NUM_LEDS_PER_STRIP * 3
#define MAX_SCREEN_Y NUM_STRIPS
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef struct
{
    uint8_t  address;
    uint8_t  cmd;
    uint16_t count_data;
}
RGB_HEADER;
typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}
LED_COLOR;
//--------------------------------------------------------------------------------
union CMD_0x01_QUESTION
{
    struct BODY {
        RGB_HEADER header;
        uint8_t     brightness;
        LED_COLOR   data[NUM_LEDS_PER_STRIP][NUM_STRIPS];
        uint16_t    crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_0x01_ANSWER
{
    struct BODY{
        RGB_HEADER header;
        uint16_t crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
#pragma pack(pop)
//--------------------------------------------------------------------------------
#define RGB_CMD_0x01  0x01
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
