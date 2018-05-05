TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

########################################################################
#                   i-Vizon Template Section                           #
########################################################################

IVIZON_TRUNK    = /opt/i-vizon/trunk
UTILS           = $$IVIZON_TRUNK/utils
COMMON          = $$IVIZON_TRUNK/common

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Common

HEADERS += $$COMMON/inc/common.h

SOURCES += $$COMMON/src/common.cpp

INCLUDEPATH += $$COMMON/inc/

########################################################################
#                          Project Section                             #
########################################################################

HEADERS += ../../file_operation.h

SOURCES += main.cpp \
../../file_operation.cpp


