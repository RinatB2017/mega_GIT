#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************
DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

CONFIG  += qt warn_on thread static
QT      += opengl

linux-g++:QMAKE_CXXFLAGS += -fno-exceptions

HEADERS += $$PWD/src/*.h
SOURCES += $$PWD/src/*.cpp

unix:!macx {
    LIBS += -lGLU
}

macx: LIBS += -framework OpenGL

win32 {
  !build_pass {
    win32-msvc | win32-msvc2002 {
      error(Unsupported Visual Studio version ( < 2003 ))
    }
  }

  win32-msvc2008 | win32-msvc2010 | win32-msvc2012 | win32-msvc2013 {
    QMAKE_CXXFLAGS += -MP
    QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_STL
    QMAKE_CXXFLAGS_RELEASE += /fp:fast /arch:SSE2
  }

    LIBS += -lqwtplot3d -lopengl32 -lglu32 -lgdi32
}

unix:VERSION = 0.3.1

#HEADERS += \
#    $$PWD/src/qwt3d_appearance.h \
#    $$PWD/src/qwt3d_autoscaler.h \
#    $$PWD/src/qwt3d_axis.h \
#    $$PWD/src/qwt3d_color.h \
#    $$PWD/src/qwt3d_color_std.h \
#    $$PWD/src/qwt3d_colorlegend.h \
#    $$PWD/src/qwt3d_coordsys.h \
#    $$PWD/src/qwt3d_data.h \
#    $$PWD/src/qwt3d_drawable.h \
#    $$PWD/src/qwt3d_enrichment.h \
#    $$PWD/src/qwt3d_enrichment_std.h \
#    $$PWD/src/qwt3d_extglwidget.h \
#    $$PWD/src/qwt3d_function.h \
#    $$PWD/src/qwt3d_global.h \
#    $$PWD/src/qwt3d_graphplot.h \
#    $$PWD/src/qwt3d_gridmapping.h \
#    $$PWD/src/qwt3d_gridplot.h \
#    $$PWD/src/qwt3d_helper.h \
#    $$PWD/src/qwt3d_io.h \
#    $$PWD/src/qwt3d_io_reader.h \
#    $$PWD/src/qwt3d_label.h \
#    $$PWD/src/qwt3d_mapping.h \
#    $$PWD/src/qwt3d_meshplot.h \
#    $$PWD/src/qwt3d_openglhelper.h \
#    $$PWD/src/qwt3d_parametricsurface.h \
#    $$PWD/src/qwt3d_plot3d.h \
#    $$PWD/src/qwt3d_portability.h \
#    $$PWD/src/qwt3d_scale.h \
#    $$PWD/src/qwt3d_surfaceplot.h \
#    $$PWD/src/qwt3d_types.h \
#    $$PWD/src/qwt3d_valueptr.h \
#    $$PWD/src/qwt3d_volumeplot.h

#SOURCES += \
#    $$PWD/src/qwt3d_appearance.cpp \
#    $$PWD/src/qwt3d_autoscaler.cpp \
#    $$PWD/src/qwt3d_axis.cpp \
#    $$PWD/src/qwt3d_color_std.cpp \
#    $$PWD/src/qwt3d_colorlegend.cpp \
#    $$PWD/src/qwt3d_coordsys.cpp \
#    $$PWD/src/qwt3d_data.cpp \
#    $$PWD/src/qwt3d_drawable.cpp \
#    $$PWD/src/qwt3d_enrichment_std.cpp \
#    $$PWD/src/qwt3d_extglwidget.cpp \
#    $$PWD/src/qwt3d_function.cpp \
#    $$PWD/src/qwt3d_graphplot.cpp \
#    $$PWD/src/qwt3d_gridmapping.cpp \
#    $$PWD/src/qwt3d_gridplot.cpp \
#    $$PWD/src/qwt3d_io.cpp \
#    $$PWD/src/qwt3d_io_reader.cpp \
#    $$PWD/src/qwt3d_label.cpp \
#    $$PWD/src/qwt3d_lighting.cpp \
#    $$PWD/src/qwt3d_meshplot.cpp \
#    $$PWD/src/qwt3d_parametricsurface.cpp \
#    $$PWD/src/qwt3d_plot3d.cpp \
#    $$PWD/src/qwt3d_scale.cpp \
#    $$PWD/src/qwt3d_surfaceplot.cpp \
#    $$PWD/src/qwt3d_types.cpp \
#    $$PWD/src/qwt3d_volumeplot.cpp
