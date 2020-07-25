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
#define MAX_X 10
#define MAX_Y 10
#define CELL_SIZE 50

#define TEST_JSON_X "test_json_x"
#define TEST_JSON_Y "test_json_y"
#define TEST_JSON_W "test_json_w"
#define TEST_JSON_H "test_json_h"
#define TEST_JSON_SPLITTER "test_json_splitter"
//--------------------------------------------------------------------------------
#define ORGNAME "Home"
#define APPNAME "_For_tests"
//--------------------------------------------------------------------------------
#define VERSION                 VER_MAJOR.VER_MINOR.VER_PATCH.VER_BUILD
#define QMAKE_TARGET_COMPANY    ORGNAME
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
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
//--------------------------------------------------------------------------------
#endif
