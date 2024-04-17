#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    $$PWD/sensors \
    $$PWD/Test_ADC \
    $$PWD/Test_AT \
    $$PWD/Test_arduino \
    $$PWD/Test_arduino_modbus \
    $$PWD/Test_broadcast_UDP \
    $$PWD/Test_QtCharts \
    $$PWD/Test_CandleStick_Box \
    $$PWD/Test_CandleStick_Box_adv \
    $$PWD/Test_CRC \
    $$PWD/Test_digits \
    $$PWD/Test_dip_widget \
    $$PWD/Test_database \
    $$PWD/Test_drawbuttons \
    $$PWD/Test_draw_line \
    # $$PWD/Test_event_filter \             #надо исправить
    $$PWD/Test_hsv_color \
    $$PWD/Test_FastTrading \
    $$PWD/Test_GrapherBox \
    # $$PWD/Test_HistogramBox \             #надо исправить
    $$PWD/Test_HC-05 \
    $$PWD/Test_HID_device \
    $$PWD/Test_HTML \
    # $$PWD/Test_QHexEdit \                 #надо исправить
    $$PWD/Test_mesh \
    $$PWD/Test_md5 \
    $$PWD/Test_lcd_knob \
    $$PWD/Test_ledspanel \
    $$PWD/Test_LogBoxs \
    # $$PWD/Test_OpenRPT \                  #надо исправить
    $$PWD/Test_OscilloscopeBox \
    $$PWD/Test_picture \
    $$PWD/Test_PostgreSQL \
    # $$PWD/Test_PropertyBrowser \          #надо исправить
    # $$PWD/Test_QtPropertyBrowser \        #надо исправить
    # $$PWD/Test_QCustomPlot \              #надо исправить
    $$PWD/Test_QGraphicsScene \
    $$PWD/Test_QListWidget \
    $$PWD/Test_QProcess \
    # $$PWD/Test_QsLog \                    #надо исправить
    $$PWD/Test_RadioGarden \
    $$PWD/Test_RGB_display \
    # $$PWD/Test_RGB_Matrix \               #надо исправить
    $$PWD/Test_SerialBox5 \
    $$PWD/Test_libFTDI \
    $$PWD/Test_libUSB \
    $$PWD/Test_libVLC \
    $$PWD/Test_model \
    $$PWD/Test_pixmap \
    $$PWD/Test_Polar \
    # $$PWD/Test_SQL \                      #надо исправить
    $$PWD/Test_thread \
    $$PWD/Test_triangle \
    $$PWD/Test_Widgets \
    $$PWD/Test_QDBus \
    $$PWD/Test_QGraphicsView \
    $$PWD/Test_QML \
    # $$PWD/Test_QStateMachine \            #надо исправить
    $$PWD/Test_QTreeWidget \
    $$PWD/Test_Game \
    $$PWD/Test_Games \
    $$PWD/Test_Mask \
    $$PWD/Test_TCP_UDP_bridge \
    $$PWD/Test_YouTube \
    $$PWD/Tester \
    $$PWD/Test_WebSDR

win {
SUBDIRS += \
    $$PWD/Test_libMPSSE \
    $$PWD/Test_PGP \
    $$PWD/Test_RSA
}

unix {
SUBDIRS += \
    $$PWD/Test_ADB \
    $$PWD/Test_ffmpeg \
    $$PWD/Test_mouse \
    $$PWD/Test_OpenCL \
    $$PWD/Test_OpenCV \
    $$PWD/Test_OpenMP \
    $$PWD/Test_syslog \
    # $$PWD/Test_SerialTerm \               #Qt5
    $$PWD/Test_SDL \
    $$PWD/Test_SDLWidget \
    # $$PWD/Test_SMTP \                     #надо исправить
    $$PWD/Test_Trader \
    # $$PWD/Test_Qt3D \                     #надо исправить
    $$PWD/Test_QWebEngineView \
    $$PWD/Test_QTermWidget \
    # $$PWD/Test_QHexEdit2 \                #надо исправить
    # $$PWD/Test_QScintilla \               #Qt5
    # $$PWD/Tests_QtPlot3D \                #Qt5
    # $$PWD/Test_wwWidgets                  #надо исправить
}
