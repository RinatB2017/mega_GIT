#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    ADC_powersupply \
    AFC \
    B590_imitator \
    Calibration \
    Externed_calibration \
    Factory_Calibration \
    GDS_840C \
    Multimeter_calc_time \
    Multimeter_V786 \
    Multimeter_V7XX \
    PowerSupply \
    PowerSupply_FRAM \
    PowerSupply_RC \
    PowerSupply_Temperature \
    RS232_STMF0 \
    RS232_STMF1 \
    Test_PowerSupply \
    Test_radiochannel \

unix {
SUBDIRS += \
    FT2232 \
    FT2232_old \
    FT2232_reserv
}
