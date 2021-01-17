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
//#define MAX_X 10
//#define MAX_Y 10
//#define CELL_SIZE 50

//#define TEST_JSON_X "test_json_x"
//#define TEST_JSON_Y "test_json_y"
//#define TEST_JSON_W "test_json_w"
//#define TEST_JSON_H "test_json_h"
//#define TEST_JSON_SPLITTER "test_json_splitter"

//#define PROXIES_XML "proxies.xml"
//--------------------------------------------------------------------------------
#define ORGNAME "Home"
#define APPNAME "_For_tests"
//--------------------------------------------------------------------------------
#define VERSION                 VER_MAJOR.VER_MINOR.VER_PATCH.VER_BUILD
#define QMAKE_TARGET_COMPANY    ORGNAME
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
//--------------------------------------------------------------------------------
#define VER_PRODUCTVERSION_STR      VER_STR
#define VER_FILEDESCRIPTION_STR     APPNAME
#define VER_INTERNALNAME_STR        APPNAME
#define VER_LEGALCOPYRIGHT_STR      QMAKE_TARGET_COPYRIGHT
#define VER_ORIGINALFILENAME_STR    APPNAME
#define VER_PRODUCTNAME_STR         APPNAME
//--------------------------------------------------------------------------------
// === win ===
#define VER_FILEVERSION             VER_MAJOR,VER_MINOR,VER_BUILD,0
//#define STR_FILEVERSION             VER_MAJOR ## "." ## VER_MINOR ## "." ## VER_BUILD ## ".0"
#define STR_FILEVERSION             "1.2.3.0" //VER_MAJOR"."VER_MINOR"."VER_BUILD".0"

#define VER_PRODUCTVERSION          VER_MAJOR,VER_MINOR,VER_BUILD,0
//#define STR_PRODUCTVERSION          VER_MAJOR ## "." ## VER_MINOR ## "." ## VER_BUILD ## "\0"
#define STR_PRODUCTVERSION          "1.2.3.0" //VER_MAJOR"."VER_MINOR"."VER_BUILD".0"

#define STR_COMPANYNAME             ORGNAME
#define STR_FILEDESCRIPTION         APPNAME
#define STR_INTERNALNAME            APPNAME
#define STR_LEGALCOPYRIGHT          "Copyright © 2021 " ORGNAME
#define STR_LEGALTRADEMARKS1        "All Rights Reserved"
#define STR_LEGALTRADEMARKS2        STR_LEGALTRADEMARKS1
#define STR_ORIGINALFILENAME        APPNAME
#define STR_PRODUCTNAME             APPNAME
#define STR_COMPANYDOMAIN           "mywebsite.com"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
