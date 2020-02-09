#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    Test_ADC \
    Test_arduino \
    Test_arduino_modbus \
    Test_broadcast_UDP \
    Test_CandleStick_Box \
    Test_CRC \
    Test_digits \
    Test_database \
    Test_drawbuttons \
    Test_draw_line \
    Test_event_filter \
    Test_FastTrading \
    Test_GrapherBox \
    Test_HistogramBox \
    Test_HC-05 \
    Test_HID_device \
    Test_HTML \
    Test_QHexEdit \
    Test_mesh \
    Test_md5 \
    Test_OpenRPT \
    Test_OscilloscopeBox \
    Test_picture \
    Test_PostgreSQL \
    Test_PropertyBrowser \
    Test_QtPropertyBrowser \
    Test_QCustomPlot \
    Test_QGraphicsScene \
    Test_QListWidget \
    Test_QProcess \
    Test_QsLog \
    Test_RGB_display \
    Test_RGB_Matrix \
    Test_RTSP_widget \
    Test_SerialBox5 \
    Test_Simple_PTZ_widget \
    Test_libUSB \
    Test_model \
    Test_pixmap \
    Test_Polar \
    Test_SQL \
    Test_SMTP \
    Test_thread \
    Test_triangle \
    Test_PTZ_widget \
    Test_Widgets \
    Test_QtCharts \
    Test_OpenCL \
    Test_OpenCV \
    Test_OpenMP \
    Test_QDBus \
    Test_QGraphicsView \
    Test_QML \
    Test_QStateMachine \
    Test_QTreeWidget \
    Test_Game \
    Test_Games \
    Test_OpenGL \
    Test_mouse \
    Test_RSA \
    Test_SDL \
    Test_SDLWidget \
    Test_TCP_UDP_bridge \
    Test_YouTube \
    Tester
    
unix {
SUBDIRS += \
    Test_Box2D \
    Test_Qt3D \
    Tests_QtPlot3D \
    Test_QScintilla \
    Test_LibVLC \
    Test_NeuronNet \
    Test_PGP \
    Test_syslog \
    Test_Trader \
    Test_WebKit \
    Test_wwWidgets \
    Test_QWebEngineView
}
