TEMPLATE = app
TARGET = btchat

QT += core bluetooth widgets
#QT += core gui widgets

SOURCES = \
    main.cpp \
    chat.cpp \
    remoteselector.cpp \
    chatserver.cpp \
    chatclient.cpp

HEADERS = \
    chat.h \
    remoteselector.h \
    chatserver.h \
    chatclient.h

FORMS = \
    chat.ui \
    remoteselector.ui

#target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/btchat
#INSTALLS += target
