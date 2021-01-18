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
#ifndef DEFINES_WIN_HPP
#define DEFINES_WIN_HPP
//--------------------------------------------------------------------------------
#include "version.hpp"
//--------------------------------------------------------------------------------
#define Q(x) #x
#define QUOTE(x) Q(x)
//--------------------------------------------------------------------------------
#define VER_FILEVERSION             VER_MAJOR,VER_MINOR,VER_BUILD,0
#define STR_FILEVERSION             QUOTE(VER_MAJOR) "." QUOTE(VER_MINOR) "." QUOTE(VER_BUILD) ".0"

#define VER_PRODUCTVERSION          VER_MAJOR,VER_MINOR,VER_BUILD,0
#define STR_PRODUCTVERSION          QUOTE(VER_MAJOR) "." QUOTE(VER_MINOR) "." QUOTE(VER_BUILD) ".0"

#define STR_COMPANYNAME             ORGNAME
#define STR_FILEDESCRIPTION         "My template for app"
#define STR_INTERNALNAME            APPNAME
#define STR_LEGALCOPYRIGHT          "Copyright © 2021 " ORGNAME
#define STR_LEGALTRADEMARKS1        "All Rights Reserved"
#define STR_LEGALTRADEMARKS2        STR_LEGALTRADEMARKS1
#define STR_ORIGINALFILENAME        APPNAME
#define STR_PRODUCTNAME             APPNAME
#define STR_COMPANYDOMAIN           "mywebsite.com"
//--------------------------------------------------------------------------------
#endif
