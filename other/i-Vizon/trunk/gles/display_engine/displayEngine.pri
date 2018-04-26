########################################################################
#                   i-Vizon Display Engine Section                     #
########################################################################

IVIZON_TRUNK    = /opt/i-vizon/trunk
GLES            = $$IVIZON_TRUNK/gles
GLES_WRAPPER    = $$GLES/wrapper
GLES_DE         = $$GLES/display_engine
UTILS           = $$IVIZON_TRUNK/utils
COMMON          = $$IVIZON_TRUNK/common
DEFINES         += IVIZON_DEBUGr


SOURCES += $$GLES_DE/src/iv_display_engine.cpp
HEADERS += $$GLES_DE/inc/iv_display_engine.h
