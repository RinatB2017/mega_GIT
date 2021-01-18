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
#define ORGNAME "Home"
#define APPNAME "_For_tests"
//--------------------------------------------------------------------------------
#define QMAKE_TARGET_COMPANY        ORGNAME
#define QMAKE_TARGET_DESCRIPTION    "My app"
#define QMAKE_TARGET_COPYRIGHT      "Copyright © 2020-2025"
#define QMAKE_TARGET_PRODUCT        APPNAME
#define QMAKE_MANIFEST              ""
#define RC_CODEPAGE                 866
#define RC_ICONS                    ico/computer.ico
#define RC_LANG                     ru_RU
//#define VERSION                     VER_MAJOR.VER_MINOR.VER_BUILD.0
//--------------------------------------------------------------------------------
//#define VER_PRODUCTVERSION_STR      VER_STR
//#define VER_FILEDESCRIPTION_STR     APPNAME
//#define VER_INTERNALNAME_STR        APPNAME
//#define VER_LEGALCOPYRIGHT_STR      QMAKE_TARGET_COPYRIGHT
//#define VER_ORIGINALFILENAME_STR    APPNAME
//#define VER_PRODUCTNAME_STR         APPNAME
////--------------------------------------------------------------------------------
//#define Q(x) #x
//#define QUOTE(x) Q(x)
////--------------------------------------------------------------------------------
//#define VER_FILEVERSION             VER_MAJOR,VER_MINOR,VER_BUILD,0
//#define STR_FILEVERSION             QUOTE(VER_MAJOR) "." QUOTE(VER_MINOR) "." QUOTE(VER_BUILD) ".0"

//#define VER_PRODUCTVERSION          VER_MAJOR,VER_MINOR,VER_BUILD,0
//#define STR_PRODUCTVERSION          QUOTE(VER_MAJOR) "." QUOTE(VER_MINOR) "." QUOTE(VER_BUILD) ".0"

//#define STR_COMPANYNAME             ORGNAME
//#define STR_FILEDESCRIPTION         "My app"
//#define STR_INTERNALNAME            APPNAME
//#define STR_LEGALCOPYRIGHT          VER_LEGALCOPYRIGHT_STR ORGNAME
//#define STR_LEGALTRADEMARKS1        "All Rights Reserved"
//#define STR_LEGALTRADEMARKS2        STR_LEGALTRADEMARKS1
//#define STR_ORIGINALFILENAME        APPNAME
//#define STR_PRODUCTNAME             APPNAME
//#define STR_COMPANYDOMAIN           "mywebsite.com"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM               ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
