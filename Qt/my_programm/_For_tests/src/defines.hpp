#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
//  QString("%1").arg(question.buf[n], 2, 16, QChar('0'))
//  строка в HEX с лидирующими нулями
//
//  float c = 1.123456789f;
//  emit info(QString("%1").arg(c, 0, 'f', 3));
//--------------------------------------------------------------------------------
#define P_GEOMETRY_X    "test_geometry_x"
#define P_GEOMETRY_Y    "test_geometry_y"
#define P_GEOMETRY_W    "test_geometry_w"
#define P_GEOMETRY_H    "test_geometry_h"
#define P_ANGLE         "test_angle"

#define M_WIDTH     320
#define M_HEIGHT    200
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
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM               ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
