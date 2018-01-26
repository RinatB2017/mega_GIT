/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QApplication>
#include <QMessageBox>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#ifdef USE_B588
#   include "b588.hpp"
#endif
#ifdef USE_B590
#   include "b590.hpp"
#endif
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();
#if 1
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        //QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Application already running!"));
        if(app.sendMessage("Wake up!")) return 0;
    }
#else
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/pinguin.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    splash->show();

    qApp->processEvents();

    MainWindow *main_window = new MainWindow();

#ifdef USE_B588
    B588 *mainBox = new B588(main_window->getThis());
#endif
#ifdef USE_B590
    B590 *mainBox = new B590(main_window->getThis());
#endif

    main_window->setCentralWidget(mainBox);
    main_window->setMinimumWidth(1024);
    main_window->show();

    splash->finish(main_window);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), main_window, SLOT(set_focus(QString)));

    qDebug() << qPrintable(QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME)));
    

    return app.exec();
}
//--------------------------------------------------------------------------------
