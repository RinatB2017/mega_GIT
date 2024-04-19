#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    $$PWD/Sensors_DS16B20 \
    $$PWD/Test_ADXL345 \
    $$PWD/Test_GY-652 \
    $$PWD/Test_HTU21D \
    # $$PWD/Test_MPU-6050   # надо доработать для Qt6
