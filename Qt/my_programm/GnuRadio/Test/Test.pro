QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += dial_tone.cpp

PKGCONFIG += \
    gnuradio-audio \
    gnuradio-analog \
    gnuradio-blocks \
    gnuradio-digital \
    gnuradio-filter \
    gnuradio-fft \
    gnuradio-runtime \
    gnuradio-osmosdr
CONFIG += link_pkgconfig

unix:!macx {
    LIBS += -lboost_system$$BOOST_SUFFIX -lboost_program_options$$BOOST_SUFFIX
    LIBS += -lrt  # need to include on some distros
}
