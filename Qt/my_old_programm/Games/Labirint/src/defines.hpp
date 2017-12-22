#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Labirint"
//--------------------------------------------------------------------------------
#define QMAKE_TARGET_COMPANY    "Work"
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
#define RC_ICONS                "computer.ico"
#define VERSION                 VER_MAJOR.VER_MINOR.VER_BUILD
//--------------------------------------------------------------------------------
#define PLAYER_ID   31
#define SPACE_ID    5
#define EXIT_ID     1
//--------------------------------------------------------------------------------
#define MIN_WIDTH   5
#define MAX_WIDTH   25

#define MIN_HEIGHT   5
#define MAX_HEIGHT   25
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
