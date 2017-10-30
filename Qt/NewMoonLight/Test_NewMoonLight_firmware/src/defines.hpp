#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_NewMoonLight_firmware"
//--------------------------------------------------------------------------------
#define MAX_SCREEN_X    6
#define MAX_SCREEN_Y    6
//--------------------------------------------------------------------------------
#define CMD_0x01    0x01
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#define PROPERTY_X   "property_x"
#define PROPERTY_Y   "property_y"
#define PROPERTY_X1  "property_x1"
#define PROPERTY_Y1  "property_y1"
#define PROPERTY_X2  "property_x2"
#define PROPERTY_Y2  "property_y2"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef struct HEADER
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
} HEADER_t;
//--------------------------------------------------------------------------------
union F_01
{
    struct BODY
    {
        HEADER  header;
        struct data
        {
            uint8_t leds[MAX_SCREEN_X][MAX_SCREEN_Y];
        } data_t;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
