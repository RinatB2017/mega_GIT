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

TEMPLATE = lib
CONFIG  += qt warn_on staticlib
DEFINES += MAKELIB

DESTDIR = ../../lib
MOC_DIR = tmp
OBJECTS_DIR = tmp
UI_DIR = tmp


HEADERS = openreports.h \
          barcodes.h \
          crosstab.h \
          graph.h \
          orcrosstab.h \
          orutils.h \
          orprerender.h \
          orprintrender.h \
          renderobjects.h \
          previewdialog.h \
          ../../MetaSQL/metasql.h \
          ../common/builtinformatfunctions.h \
          ../common/builtinSqlFunctions.h \			# MANU
          ../common/labelsizeinfo.h \
          ../common/pagesizeinfo.h 
SOURCES = openreports.cpp \
          3of9.cpp \
          ext3of9.cpp \
          code128.cpp \
          codeean.cpp \
          crosstab.cpp \
          graph.cpp \
          orutils.cpp \
          orprerender.cpp \
          orprintrender.cpp \
          renderobjects.cpp \
          previewdialog.cpp \ 
          ../../MetaSQL/metasql.cpp \
          ../common/builtinformatfunctions.cpp \
          ../common/builtinSqlFunctions.cpp \		# MANU
          ../common/labelsizeinfo.cpp \
          ../common/pagesizeinfo.cpp 

INCLUDEPATH += ../common ../../common

QT += xml sql

TRANSLATIONS    = renderer_fr.ts renderer_it.ts renderer_ru.ts renderer_es.ts
