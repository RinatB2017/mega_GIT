#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_NewMoonLight"
//--------------------------------------------------------------------------------
#define CMD_SET    0x01
#define CMD_GET    0x02
//--------------------------------------------------------------------------------
#define MAX_SCREEN_X    6
#define MAX_SCREEN_Y    6
#define MAX_LED     (3 * 6)
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

union NewMoonLightPacket
{
    struct BODY
    {
        uint8_t address;
        uint8_t command;
        uint8_t cnt_data;
        uint8_t data[MAX_SCREEN_X][MAX_SCREEN_Y];
    } body;
    unsigned char buf[sizeof(BODY)];
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
