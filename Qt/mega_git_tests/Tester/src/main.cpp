/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QApplication>
//--------------------------------------------------------------------------------
#include "tester.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName(ORGNAME);
    app.setApplicationName(APPNAME);
#ifdef Q_OS_LINUX
    app.setApplicationVersion(QString("%1.%2.%3.%4")
                              .arg(VER_MAJOR)
                              .arg(VER_MINOR)
                              .arg(VER_PATCH)
                              .arg(VER_BUILD));
#endif
    //app.setWindowIcon(QIcon(ICON_PROGRAMM));

    Tester *main_window = new Tester;
    Q_ASSERT(main_window);
    main_window->setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    main_window->move(0, 0);
    main_window->show();

    main_window->setWindowIcon(QIcon(QLatin1String(":/programm.png")));

    return app.exec();
}
//--------------------------------------------------------------------------------
