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
#include <QMessageBox>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();
#if 1
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Application already running!"));
        return -1;
    }
#else
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));

    QPixmap pixmap(":/logo/pinguin.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    splash->show();

    qApp->processEvents();

    MainWindow main_window(QObject::tr(ORGNAME),
                           QObject::tr(APPNAME),
                           QString("%1.%2.%3").arg(VER_MAJOR).arg(VER_MINOR).arg(VER_BUILD));
    //main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    MainBox *mainBox = new MainBox(main_window.getThis(), splash);
    main_window.setCentralWidget(mainBox);

    main_window.setWindowIcon(QIcon(ICON_PROGRAMM));
    main_window.show();

    splash->finish(&main_window);

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));
    qDebug() << QLocale().name();

    return app.exec();
}
//--------------------------------------------------------------------------------
