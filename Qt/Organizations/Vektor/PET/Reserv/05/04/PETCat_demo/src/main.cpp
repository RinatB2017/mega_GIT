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
#include <QApplication>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "pet_demo.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
    #include <QTest>
    #include "test.hpp"
#endif
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();
    QApplication app(argc, argv);

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));

    QPixmap pixmap(":/logo/pinguin.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    splash->show();

    qApp->processEvents();

    MainWindow main_window(QObject::tr(ORGNAME),
                           QObject::tr(APPNAME),
                           QString("%1.%2.%3 (build %4)").arg(VER_MAJOR).arg(VER_MINOR).arg(VER_PATCH).arg(VER_BUILD));
    //main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    PET_demo *mainBox = new PET_demo("PET_demo", main_window.getThis());
    main_window.setCentralWidget(mainBox);
    main_window.setWindowIcon(QIcon(ICON_PROGRAMM));
    main_window.show();

    splash->finish(&main_window);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &main_window, SLOT(set_focus(QString)));
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
