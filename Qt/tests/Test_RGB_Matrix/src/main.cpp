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
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "Test_RGB_Matrix_mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();

    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        if(app.sendMessage("Wake up!")) return 0;
    }

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setApplicationVersion(VER_STR);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/logo.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    Q_CHECK_PTR(splash);
    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    MainWindow *main_window = new MainWindow();

    MainBox *mainBox = new MainBox(main_window->getThis(), splash);
    main_window->setCentralWidget(mainBox);

    main_window->show();

    splash->finish(main_window);

    return app.exec();
}
//--------------------------------------------------------------------------------
