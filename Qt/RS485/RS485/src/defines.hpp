#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "RS485"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#define CMD_31  0x31
#define CMD_34  0x34
#define CMD_36  0x36
#define CMD_37  0x37
#define CMD_42  0x42
//--------------------------------------------------------------------------------
#define CONVERT
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

union CMD_QUESTION
{
    struct BODY{
        uint16_t prefix_16;                 // префикс
        uint16_t len_16;                    // длина пакета
        uint8_t  addr_8;                    // адрес модуля
        uint8_t  reserv_8;                  // резерв
        uint8_t  cmd_8;                     // команда
        uint8_t  reserv_2_8;                // резерв
        uint16_t time_interval_16;          // интервал дворника
        uint16_t reserv_3_16;               // резерв
        uint32_t time_washout_32;           // время помывки
        uint32_t time_pause_washout_32;     // время между помывками
        uint32_t addr_cam_32;               // адрес камеры
        uint32_t preset_washout_32;         // пресет помывки
        uint32_t time_preset_washout_32;    // времен помывки
    } body;
    unsigned char buf[sizeof(BODY)];
};

#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
