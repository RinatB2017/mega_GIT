QT           += network

HEADERS = sender.h
SOURCES = \
    sender.cpp \
    main.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}

