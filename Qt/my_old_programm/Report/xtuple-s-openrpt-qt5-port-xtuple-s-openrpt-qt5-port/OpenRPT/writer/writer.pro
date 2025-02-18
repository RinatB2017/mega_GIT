#
# OpenRPT report writer and rendering engine
# Copyright (C) 2001-2011 by OpenMFG, LLC
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# Please contact info@openmfg.com with any questions on this license.
#

include( ../../global.pri )

TEMPLATE = app
TARGET   = OpenRPT
unix:TARGET = openrpt

CONFIG += qt warn_on

DESTDIR     = ../../bin
MOC_DIR     = tmp
UI_DIR      = tmp
OBJECTS_DIR = tmp

INCLUDEPATH += ../../common ../common ../images
LIBS += -L../../lib -lwrtembed -lcommon -lrenderer -lDmtx_Library

win32-msvc* {
  PRE_TARGETDEPS += ../../lib/wrtembed.lib \
                    ../../lib/common.lib   \
                    ../../lib/renderer.lib
} else {
  PRE_TARGETDEPS += ../../lib/libwrtembed.a \
                    ../../lib/libcommon.a   \
                    ../../lib/librenderer.a
}

RC_FILE = writer.rc
macx:RC_FILE = ../images/OpenRPT.icns

HEADERS += reportwriterwindow.h \
           ../common/builtinSqlFunctions.h	# MANU

SOURCES += rptwrt.cpp\
           ../common/builtinSqlFunctions.cpp \	# MANU
           reportwriterwindow.cpp

QT += xml sql widgets printsupport
#QT += qt3support

RESOURCES += writer.qrc

TRANSLATIONS = writer_fr.ts writer_it.ts writer_ru.ts writer_es.ts
