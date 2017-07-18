#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_draw_line"
//--------------------------------------------------------------------------------
#define CMD_0x01    0x01
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

typedef struct HEADER
{
    uint8_t addr;
    uint8_t cmd;
    uint8_t len;
} HEADER_t;

union F_01
{
    struct body
    {
        HEADER  header;
        struct data
        {
            uint16_t len_line;      // длина линии
            uint16_t delay_ms;      // задержка
            uint8_t brightness_R;
            uint8_t brightness_G;
            uint8_t brightness_B;
        } data_t;
    } body_t;
    unsigned char buf[sizeof(body_t)];
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
