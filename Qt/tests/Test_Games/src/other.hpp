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
#ifndef OTHER_HPP
#define OTHER_HPP
//--------------------------------------------------------------------------------
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
//--------------------------------------------------------------------------------
Window* findWindows( Display* display, ulong* winCount )
{
    Atom actualType;
    int format;
    ulong bytesAfter;
    uchar* list = nullptr;
    Status status = XGetWindowProperty( display,
                                        DefaultRootWindow( display ),
                                        XInternAtom( display, "_NET_CLIENT_LIST", False ),
                                        0L,
                                        ~0L,
                                        False,
                                        XA_WINDOW,
                                        &actualType,
                                        &format,
                                        winCount,
                                        &bytesAfter,
                                        &list
                                        );

    if( status != Success )
    {
        *winCount = 0;
        return nullptr;
    }

    return reinterpret_cast< Window* >( list );
}
//--------------------------------------------------------------------------------
char* getWindowName( Display* display, Window win )
{
    Atom actualType;
    int format;
    ulong count, bytesAfter;
    uchar* name = nullptr;
    Status status = XGetWindowProperty( display,
                                        win,
                                        XInternAtom( display, "_NET_WM_NAME", False ),
                                        0L,
                                        ~0L,
                                        False,
                                        XInternAtom( display, "UTF8_STRING", False ),
                                        &actualType,
                                        &format,
                                        &count,
                                        &bytesAfter,
                                        &name
                                        );

    if( status != Success )
    {
        return nullptr;
    }

    if( name == NULL ) {
        Status status = XGetWindowProperty( display,
                                            win,
                                            XInternAtom( display, "WM_NAME", False ),
                                            0L,
                                            ~0L,
                                            False,
                                            AnyPropertyType,
                                            &actualType,
                                            &format,
                                            &count,
                                            &bytesAfter,
                                            &name
                                            );

        if( status != Success )
        {
            return nullptr;
        }
    }

    return reinterpret_cast< char* >( name );
}
//--------------------------------------------------------------------------------
#endif // OTHER_HPP
