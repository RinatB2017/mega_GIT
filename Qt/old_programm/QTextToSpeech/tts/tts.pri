
DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    $$PWD/src/qtexttospeech.h \
    $$PWD/src/qtexttospeechplugin.h \
    $$PWD/src/qtexttospeechengine.h \
    $$PWD/src/qtexttospeech_p.h \
    $$PWD/src/qtexttospeech_global.h \
    $$PWD/src/qvoice.h \
    $$PWD/src/qvoice_p.h \

SOURCES += \
    $$PWD/src/qtexttospeech.cpp \
    $$PWD/src/qtexttospeechplugin.cpp \
    $$PWD/src/qtexttospeechengine.cpp \
    $$PWD/src/qvoice.cpp \
