
#include <QCoreApplication>

#include </usr/include/X11/Xlib.h>
#include </usr/include/X11/Xatom.h>

#include <iostream>

Window* findWindows( Display* display, ulong* winCount )
{
    Atom actualType;
    int format;
    ulong bytesAfter;
    uchar* list = NULL;
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
                                        &list );

    if( status != Success )
    {
        *winCount = 0;
        return NULL;
    }

    return reinterpret_cast< Window* >( list );
}

char* getWindowName( Display* display, Window win )
{
    Atom actualType;
    int format;
    ulong count, bytesAfter;
    uchar* name = NULL;
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
                                        &name );

    if( status != Success )
    {
        return NULL;
    }

    if( name == NULL )
    {
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
                                            &name );

        if( status != Success )
        {
            return NULL;
        }
    }

    return reinterpret_cast< char* >( name );
}

int main()
{
    setlocale( LC_ALL, "" );

    if( Display* display = XOpenDisplay( NULL ) )
    {
        ulong count = 0;
        Window* wins = findWindows( display, &count );
        for( ulong i = 0; i < count; ++i )
        {
            Window w = wins[ i ];
            std::wcout << w;
            if( char* name = getWindowName( display, w ) )
            {
                std::wcout << ": " << QString::fromUtf8( name ).toStdWString();
                XFree( name );
            }
            std::wcout << std::endl;

            XWindowAttributes attrs;
            if( XGetWindowAttributes( display, w, &attrs ) )
            {
                Window child;
                if( XTranslateCoordinates(
                            display,
                            w, attrs.root,
                            0, 0,
                            &attrs.x, &attrs.y,
                            &child
                            ) ) {
                    std::wcout << QString( "  > Rect: [%1, %2, %3x%4]" )
                                  .arg( attrs.x )
                                  .arg( attrs.y )
                                  .arg( attrs.width )
                                  .arg( attrs.height )
                                  .toStdWString();
                    std::wcout << std::endl;
                }
            }
        }

        if( wins )
        {
            XFree( wins );
        }

        XCloseDisplay( display );
    }

    return 0;
}
