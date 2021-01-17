/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
// === win ===
#define VER_FILEVERSION             VER_MAJOR,VER_MINOR,VER_BUILD,0
#define STR_FILEVERSION             VER_MAJOR ## "." ## VER_MINOR ## "." ## VER_BUILD ## ".0"

#define VER_PRODUCTVERSION          VER_MAJOR,VER_MINOR,VER_BUILD,0
#define STR_PRODUCTVERSION          VER_MAJOR ## "." ## VER_MINOR ## "." ## VER_BUILD ## ".0"
//#define STR_PRODUCTVERSION          "2.0.0.0"

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
#endif
