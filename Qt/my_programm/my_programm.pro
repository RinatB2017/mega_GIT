#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    $$PWD/_For_tests \
    $$PWD/auto_inc_version \
    $$PWD/CalcMd5 \
    $$PWD/Check_Network \
    $$PWD/hello_speak \
    $$PWD/Get_acceleration_data \
    $$PWD/Get_MyIP \
    $$PWD/GP2Y0A02Y \
    $$PWD/GPS \
    $$PWD/imageviewer \
    $$PWD/Icons \
    $$PWD/PelcoD_485 \
    $$PWD/Reader_SAAT_T505 \
    $$PWD/Simple_HTTP_reader \
    $$PWD/Splitter \
    $$PWD/neural/QNeuralNet \
    $$PWD/Labyrinth \
    $$PWD/MCP4725 \
    $$PWD/MODBUS \
    $$PWD/show_films \
    $$PWD/Templates \
    $$PWD/WiFi \

unix {
SUBDIRS += \
    $$PWD/MyBot \    #надо нормально собрать в новой версии opencv
    $$PWD/QRCodeGenerator \
    $$PWD/Teacher \
    $$PWD/Teacher_light \
    $$PWD/Walkers \
}
    
lessThan(QT_MAJOR_VERSION, 5) {
SUBDIRS += \
    $$PWD/RS232/RS232_4
} else {
SUBDIRS += \
    $$PWD/RS232/RS232_5
}
