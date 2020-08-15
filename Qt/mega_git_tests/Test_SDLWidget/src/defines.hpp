#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include <winsock2.h>
#endif
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_SDLWidget"
//--------------------------------------------------------------------------------
#define QMAKE_TARGET_COMPANY    "Work"
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
#define RC_ICONS                "computer.ico"
#define VERSION                 VER_MAJOR.VER_MINOR.VER_BUILD
//--------------------------------------------------------------------------------
#define TEST_0  1000
#define TEST_0_TEXT  "test 0"

#define TEST_1  1001
#define TEST_1_TEXT  "test 1"

#define TEST_2  1002
#define TEST_2_TEXT  "test 2"

#define TEST_3  1003
#define TEST_3_TEXT  "test 3"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
