#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#include "version.hpp"
//--------------------------------------------------------------------------------
//  QString("%1").arg(question.buf[n], 2, 16, QChar('0'))
//  строка в HEX с лидирующими нулями
//
//  float c = 1.123456789f;
//  emit info(QString("%1").arg(c, 0, 'f', 3));
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
#define ORGNAME "Home"
#define APPNAME "_For_tests"
//--------------------------------------------------------------------------------
#define VERSION                 VER_MAJOR.VER_MINOR.VER_PATCH.VER_BUILD
#define QMAKE_TARGET_COMPANY    ORGNAME
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
#define RC_ICONS                ":/images/computer.ico"
//--------------------------------------------------------------------------------
#define VER_FILEVERSION             VER_MAJOR,VER_MINOR,VER_PATCH,VER_BUILD
#define VER_FILEVERSION_STR         "##VER_MAJOR##VER_MINOR##VER_BUILD\0"
#define VER_PRODUCTVERSION          VER_MAJOR,VER_MINOR,VER_PATCH,VER_BUILD
#define VER_PRODUCTVERSION_STR      "##VER_MAJOR##VER_MINOR##VER_BUILD\0"
#define VER_FILEDESCRIPTION_STR     APPNAME
#define VER_INTERNALNAME_STR        APPNAME
#define VER_LEGALCOPYRIGHT_STR      QMAKE_TARGET_COPYRIGHT
#define VER_ORIGINALFILENAME_STR    APPNAME
#define VER_PRODUCTNAME_STR         APPNAME
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
#define ICON_US         ":/mainwindow/us.png"
//--------------------------------------------------------------------------------
#endif
