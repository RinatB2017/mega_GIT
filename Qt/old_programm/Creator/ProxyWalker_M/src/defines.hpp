#ifndef DEFINES_H
#define DEFINES_H
//--------------------------------------------------------------------------------
enum errors{
    E_NO_ERROR = 0,
    E_ERROR_TIMEOUT,
    E_ERROR_NO_PAGE,
    E_ERROR_NOT_LOADED,
    E_ERROR_IS_BLOCK
};
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "ProxyWalker_M"

#define URL_MAIN_PAGE "http://psixpom.jimdo.com/"

#define CHECK_PAGE "https://www.google.ru"

#define PROXIES_XML "proxies.xml"
#define WEBSETTINGS_XML "websettings.xml"
#define MAX_TIMEOUT 30000
#define MAX_TAB 15
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
