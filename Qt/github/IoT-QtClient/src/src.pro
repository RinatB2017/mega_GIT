#
# The MIT License (MIT)
#
# Copyright (c) 2015 Wapice Ltd.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

QT       += network
QT       -= gui
CONFIG   += c++11

TARGET = iot
TEMPLATE = lib

DEFINES += IOT_LIBRARY

HEADERS += device.h \
    device_p.h \
    requesthandler.h \
    response.h \
    error.h \
    devicelist.h \
    devicelist_p.h \
    datanode.h \
    datanode_p.h \
    item_p.h \
    connection.h \
    connection_p.h \
    requesthandlerprovider.h

SOURCES += device.cpp \
    device_p.cpp \
    devicelist.cpp \
    devicelist_p.cpp \
    requesthandlerprovider.cpp \
    requesthandler.cpp \
    response.cpp \
    item_p.cpp \
    error.cpp \
    datanode.cpp \
    datanode_p.cpp \
    connection.cpp \
    connection_p.cpp

