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
#if QT_VERSION >= 0x050000
#   include <QtMessageHandler>
#endif
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_polar.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include "posix.hpp"
#endif
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include "test.hpp"
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();
#ifdef SINGLE_APP
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        if(app.sendMessage("Wake up!")) return 0;
    }
#else
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/logo.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    Q_ASSERT(splash);
    splash->show();

    

    MainWindow *main_window = new MainWindow();

    MainBox *mainBox = new MainBox(main_window, splash);

    main_window->setCentralWidget(mainBox);
    main_window->show();

    splash->finish(main_window);

#ifdef SINGLE_APP
    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), main_window, SLOT(set_focus(QString)));
#endif
    qDebug() << qPrintable(QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME)));

#ifdef QT_DEBUG
    int test_result = QTest::qExec(new Test(), argc, argv);
    if (test_result != EXIT_SUCCESS)
    {
        return test_result;
    }
#endif

    return app.exec();
}
//--------------------------------------------------------------------------------
