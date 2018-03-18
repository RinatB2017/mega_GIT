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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "serialbox5_lite.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include "test.hpp"
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
    MyApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setApplicationVersion(VER_STR);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    MainWindow *main_window = new MainWindow();

    QPixmap pixmap(":/logo/pinguin.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    Q_CHECK_PTR(splash);

    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    splash->showMessage("init RS-232_5 (1)...");
    SerialBox5 *serial = new SerialBox5(main_window->getThis(), "RS-232_5 (1)", "RS-232_1");
    serial->add_menu(2);

    splash->showMessage("init RS-232_5 (2)...");
    SerialBox5 *serial2 = new SerialBox5(main_window->getThis(), "RS-232_5 (2)", "RS-232_2");
    serial2->add_menu(4);

    main_window->add_dock_widget("RS232_1", "rs232_1", Qt::LeftDockWidgetArea,  serial);
    main_window->add_dock_widget("RS232_2", "rs232_2", Qt::RightDockWidgetArea, serial2);
    main_window->load_setting();
    main_window->show();

    splash->finish(main_window);

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
