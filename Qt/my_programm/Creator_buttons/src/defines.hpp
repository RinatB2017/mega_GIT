/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#include "version.hpp"
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Creator_buttons"
//--------------------------------------------------------------------------------
#define MIN_WIDTH   10
#define MAX_WIDTH   256
#define MIN_HEIGHT  10
#define MAX_HEIGHT  256

#define S_SIZE_W        "size_w"
#define S_SIZE_H        "size_h"
#define S_SIZE_BORDER   "size_border"
#define S_BORDER_COLOR  "border_color"
#define S_BACKGROUND_COLOR  "background_color"
#define S_MARGIN_T  "margin_t"
#define S_MARGIN_B  "margin_b"
#define S_MARGIN_L  "margin_l"
#define S_MARGIN_R  "margin_r"
#define S_TRANSPARENT_BORDER    "transparent_border"
#define S_TRANSPARENT_BACKGROUND    "transparent_background"
#define S_RB_ECLIPSE        "rb_eclipse"
#define S_RB_RECTANGLE      "rb_rectangle"
#define S_RB_ARROW_LEFT     "rb_arrow_left"
#define S_RB_ARROW_RIGHT    "rb_arrow_right"

#define S_ELLIPSE_BORDER_COLOR          "ellipse_border_color"
#define S_ELLIPSE_BACKGROUND_COLOR      "ellipse_background_color"
#define S_RECTANGLE_BORDER_COLOR        "rectangle_border_color"
#define S_RECTANGLE_BACKGROUND_COLOR    "rectangle_background_color"
#define S_ARROW_LEFT_BORDER_COLOR       "arrow_left_border_color"
#define S_ARROW_LEFT_BACKGROUND_COLOR   "arrow_left_background_color"
#define S_ARROW_RIGHT_BORDER_COLOR      "arrow_right_border_color"
#define S_ARROW_RIGHT_BACKGROUND_COLOR  "arrow_right_background_color"
//--------------------------------------------------------------------------------
#define VERSION                 VER_MAJOR.VER_MINOR.VER_PATCH.VER_BUILD
#define QMAKE_TARGET_COMPANY    ORGNAME
#define QMAKE_TARGET_PRODUCT    APPNAME
#define QMAKE_TARGET_COPYRIGHT  "Copyright 2015-2020"
#define RC_ICONS                ":/images/computer.ico"
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
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
