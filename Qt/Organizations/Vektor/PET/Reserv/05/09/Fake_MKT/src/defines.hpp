#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "Fake_MKT"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
