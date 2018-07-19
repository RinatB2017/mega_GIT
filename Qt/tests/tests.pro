#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    Test_arduino \
    Test_arduino_modbus \
    Test_CandleStick_Box \
    Test_curvetracker \
    Test_digits \
    Test_database \
    Test_drawbuttons \
    Test_event_filter \
    Test_GrapherBox \
    Test_GY-652 \
    Test_HistogramBox \
    Test_HID_device \
    Test_QHexEdit \
    Test_md5 \
    Test_OpenRPT \
    Test_OscilloscopeBox \
    Test_PostgreSQL \
    Test_PropertyBrowser \
    Test_QtPropertyBrowser \
    Test_QCustomPlot \
    Test_QGraphicsScene \
    Test_QListWidget \
    Test_QsLog \
    Test_libUSB \
    Test_model \
    Test_Polar \
    Test_SQL \
    Test_SMTP \
    Test_thread \
    Test_triangle \
    Test_RGB_Matrix \
    Test_PTZ_widget \
    Test_RTSP_widget \
    Test_Widgets \
    Test_QtCharts \
#
    Test_OpenCL \
    Test_OpenMP \
    Test_QDBus \
    Test_QGraphicsView \
    Test_QML \
    Test_QStateMachine \
    Test_QTreeWidget \
#
    Test_Game \
    Test_OpenGL \
    Test_mouse \
    Test_draw_line \
#
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
}
