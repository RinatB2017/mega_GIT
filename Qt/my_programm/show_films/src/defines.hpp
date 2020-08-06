#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#include <QString>
//--------------------------------------------------------------------------------
#define ORGNAME "Home"
#define APPNAME "Смотрелка фильмов"
//--------------------------------------------------------------------------------
struct FILM
{
    QString name;
    QString filename;
    QString picture;
};
//--------------------------------------------------------------------------------
#define ROLE_IS_FOLDER  Qt::UserRole + 0
#define ROLE_NAME       Qt::UserRole + 1
#define ROLE_FILENAME   Qt::UserRole + 2
#define ROLE_PICTURE    Qt::UserRole + 3
//--------------------------------------------------------------------------------
#define FILMS_XML "films.xml"
//--------------------------------------------------------------------------------
#define MAX_WIDTH  300.0
#define MAX_HEIGHT 420.0
//--------------------------------------------------------------------------------
#define SPACE_FILE  ":/no_picture.png"
#define FOLDER_FILE ":/folder.png"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM   ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#endif
