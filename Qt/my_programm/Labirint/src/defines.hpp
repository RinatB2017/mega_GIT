#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
//#   include <winsock2.h>
#endif
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
#define WALL_ID     13
#define SPACE_ID    5
#define START_ID    30
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
