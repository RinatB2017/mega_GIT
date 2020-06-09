/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QLibraryInfo>
#include <QMessageBox>
//--------------------------------------------------------------------------------
#if QT_VERSION >= 0x050000
#   include <QtMessageHandler>
#endif
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "for_tests_mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include "posix.hpp"
#endif
//--------------------------------------------------------------------------------
#include "log_to_file.hpp"
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
//#define SINGLE_APP
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include "test.hpp"
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
//  rm /usr/lib/libGL.so.1
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    set_signals();
#endif
#if QT_VERSION >= 0x050000
    //qInstallMessageHandler(log_to_file);
#else
    //qInstallMsgHandler(log_to_file);
#endif

    set_codecs();
#ifdef SINGLE_APP
    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtSingleApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        //QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Application already running!"));
        if(app.sendMessage("Wake up!")) return 0;
    }
#else
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(ORGNAME);
    app.setApplicationName(APPNAME);
    app.setApplicationVersion(VER_STR);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/logo.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    Q_CHECK_PTR(splash);

    splash->show();

    MyMainWindow *main_window = new MyMainWindow();
    // MainWindow *main_window = new MainWindow();
    Q_CHECK_PTR(main_window);

    // отключает заголовок
    // main_window->setWindowFlags(Qt::FramelessWindowHint);

    MainBox *mainBox = new MainBox(main_window->getThis(), splash);
    Q_CHECK_PTR(mainBox);

    main_window->setCentralWidget(mainBox);
    main_window->show();

    splash->finish(main_window);

#ifdef SINGLE_APP
    QObject::connect(&app, &QtSingleApplication::messageReceived, main_window, &MyMainWindow::set_focus);
#endif

#ifdef QT_DEBUG
    qDebug() << qPrintable(QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME)));

    int test_result = QTest::qExec(new Test(), argc, argv);
    if (test_result != EXIT_SUCCESS)
    {
        return test_result;
    }
#endif

    return app.exec();
}
//--------------------------------------------------------------------------------
