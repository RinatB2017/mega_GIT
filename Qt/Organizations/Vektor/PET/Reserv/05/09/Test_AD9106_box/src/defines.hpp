#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
//#include <stdint.h>
//--------------------------------------------------------------------------------
#define ORGNAME "НТЦ \"Вектор\""
#define APPNAME "Test_AD9106_box"
//--------------------------------------------------------------------------------
#define ININAME "PETCat.ini"
//--------------------------------------------------------------------------------
#include "version.hpp"
//--------------------------------------------------------------------------------
#define VERSION                     VER_MAJOR,VER_MINOR,VER_PATCH,VER_BUILD
#define QMAKE_TARGET_COMPANY        ORGNAME
#define QMAKE_TARGET_PRODUCT        APPNAME
#define QMAKE_TARGET_DESCRIPTION    "PETCat rules."
#define QMAKE_TARGET_COPYRIGHT      "Copyright (C) 2016-2020 Work, Inc"
#define RC_ICONS                    "/ico/PETCAT.ico"
//--------------------------------------------------------------------------------
#define VER_FILEVERSION             VER_MAJOR,VER_MINOR,VER_PATCH,VER_BUILD
#define VER_FILEVERSION_STR         VER_STR
#define VER_PRODUCTVERSION          VER_MAJOR,VER_MINOR,VER_PATCH,VER_BUILD
#define VER_PRODUCTVERSION_STR      VER_STR
#define VER_FILEDESCRIPTION_STR     APPNAME
#define VER_INTERNALNAME_STR        APPNAME
#define VER_LEGALCOPYRIGHT_STR      QMAKE_TARGET_COPYRIGHT
#define VER_ORIGINALFILENAME_STR    APPNAME
#define VER_PRODUCTNAME_STR         APPNAME
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM               ":/images/PETCAT.png"
//--------------------------------------------------------------------------------
#define FLAG_NO_BLOCK               "no_block"
//--------------------------------------------------------------------------------
#define TODODO                      "Пока не сделано!"
//--------------------------------------------------------------------------------
#endif
