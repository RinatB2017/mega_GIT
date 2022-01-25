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
    $$PWD/Test_CandleStick_Box \
    $$PWD/Test_CandleStick_Box_adv \
    $$PWD/Test_CRC \
    $$PWD/Test_digits \
    $$PWD/Test_dip_widget \
    $$PWD/Test_database \
    $$PWD/Test_drawbuttons \
    $$PWD/Test_draw_line \
    $$PWD/Test_event_filter \
    $$PWD/Test_hsv_color \
    $$PWD/Test_FastTrading \
    $$PWD/Test_GrapherBox \
    $$PWD/Test_HistogramBox \
    $$PWD/Test_HC-05 \
    $$PWD/Test_HID_device \
    $$PWD/Test_HTML \
    $$PWD/Test_QHexEdit \
    $$PWD/Test_QHexEdit2 \
    $$PWD/Test_mesh \
    $$PWD/Test_md5 \
    $$PWD/Test_lcd_knob \
    $$PWD/Test_ledspanel \
    $$PWD/Test_LogBoxs \
    $$PWD/Test_OpenRPT \
    $$PWD/Test_OscilloscopeBox \
    $$PWD/Test_picture \
    $$PWD/Test_PostgreSQL \
    $$PWD/Test_PropertyBrowser \
    $$PWD/Test_QtPropertyBrowser \
    $$PWD/Test_QCustomPlot \
    $$PWD/Test_QGraphicsScene \
    $$PWD/Test_QListWidget \
    $$PWD/Test_QProcess \
    $$PWD/Test_QsLog \
    $$PWD/Test_RadioGarden \
    $$PWD/Test_RGB_display \
    $$PWD/Test_RGB_Matrix \
    $$PWD/Test_SerialBox5 \
    $$PWD/Test_SerialTerm \
    $$PWD/Test_libUSB \
    $$PWD/Test_libVLC \
    $$PWD/Test_model \
    $$PWD/Test_pixmap \
    $$PWD/Test_Polar \
    $$PWD/Test_SQL \
    $$PWD/Test_SMTP \
    $$PWD/Test_thread \
    $$PWD/Test_triangle \
    $$PWD/Test_Widgets \
    $$PWD/Test_QtCharts \
    $$PWD/Test_OpenCL \
    $$PWD/Test_OpenMP \
    $$PWD/Test_QDBus \
    $$PWD/Test_QGraphicsView \
    $$PWD/Test_QML \
    $$PWD/Test_QStateMachine \
    $$PWD/Test_QTreeWidget \
    $$PWD/Test_Game \
    $$PWD/Test_Games \
    $$PWD/Test_OpenGL \
    $$PWD/Test_Mask \
    $$PWD/Test_RSA \
    $$PWD/Test_SDL \
    $$PWD/Test_SDLWidget \
    $$PWD/Test_TCP_UDP_bridge \
    $$PWD/Test_YouTube \
    $$PWD/Tester
    
SUBDIRS += \
    $$PWD/Test_ADB \
    $$PWD/Test_OpenCV
    
unix {
SUBDIRS += \
    $$PWD/Test_Qt3D \
    $$PWD/Tests_QtPlot3D \
    $$PWD/Test_QScintilla \
    $$PWD/Test_PGP \
    $$PWD/Test_syslog \
    $$PWD/Test_Trader \
#    $$PWD/Test_WebKit \
    $$PWD/Test_wwWidgets \
    $$PWD/Test_QWebEngineView \
    $$PWD/Test_QTermWidget \
    $$PWD/Test_mouse 
}
