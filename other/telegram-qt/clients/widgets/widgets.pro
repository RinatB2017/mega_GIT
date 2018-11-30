QT += core gui widgets network
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += $$PWD/../../TelegramQt

DEFINES += STORE_MEDIA_FILES

LIBS += -lssl -lcrypto -lz
LIBS += -L$$OUT_PWD/../../TelegramQt
LIBS += -lTelegramQt$${QT_MAJOR_VERSION}

TARGET = testApp
TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    CFileManager.cpp \
    CPeerModel.cpp \
    CContactModel.cpp \
    CContactsFilterModel.cpp \
    CChatInfoModel.cpp \
    CDialogModel.cpp \
    CMessageModel.cpp

HEADERS  += MainWindow.hpp \
    CFileManager.hpp \
    CPeerModel.hpp \
    CContactModel.hpp \
    CContactsFilterModel.hpp \
    CChatInfoModel.hpp \
    CDialogModel.hpp \
    CMessageModel.hpp

FORMS    += MainWindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += CMakeLists.txt
