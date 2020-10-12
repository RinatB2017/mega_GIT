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
#include "test_widget.hpp"
//--------------------------------------------------------------------------------
#define SINGLE_APP
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();
#ifdef SINGLE_APP
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        //QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Application already running!"));
        if(app.sendMessage("Wake up!")) return 0;
    }
#else
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setApplicationVersion(VER_STR);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    MainWindow *main_window = new MainWindow();

    QPixmap pixmap(":/logo/logo.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    Q_CHECK_PTR(splash);

    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    TestWidget *cw = new TestWidget(main_window);
//    QObject::connect(cw, &TestWidget::info,  main_window, &MainWindow::info);

    main_window->setCentralWidget(cw);
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
