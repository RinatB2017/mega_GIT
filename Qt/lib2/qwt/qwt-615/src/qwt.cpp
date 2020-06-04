/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt.h"
#include <qapplication.h>

QSize qwtExpandedToGlobalStrut( const QSize &size )
{
#if QT_DEPRECATED_SINCE(5, 15)
    return size.expandedTo( QApplication::globalStrut() );
#else
    return size;
#endif
}
