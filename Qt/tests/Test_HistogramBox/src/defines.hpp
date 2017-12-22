#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
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
