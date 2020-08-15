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
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_thread_mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
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
#ifdef SINGLE_APP
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Application already running!"));
        return -1;
    }
#else
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/logo.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    splash->show();

    qApp->processEvents();

    MainWindow *main_window = new MainWindow;
    //main_window->setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    MainBox *mainBox = new MainBox(main_window, splash);
    main_window->setCentralWidget(mainBox);
    main_window->setFixedSize(main_window->sizeHint());
    main_window->show();

    splash->finish(main_window);

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));

    return app.exec();
}
//--------------------------------------------------------------------------------
