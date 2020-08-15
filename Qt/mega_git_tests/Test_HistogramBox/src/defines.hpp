#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#if defined(Q_CC_MSVC)
# define QT_STATIC_CONST static
# define QT_STATIC_CONST_IMPL
#else
# define QT_STATIC_CONST static const
# define QT_STATIC_CONST_IMPL const
#endif
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Test_HistogramBox"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
