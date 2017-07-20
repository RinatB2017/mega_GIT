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
#include <QMessageBox>
#include <QDebug>
#include <QLibraryInfo>
//--------------------------------------------------------------------------------
#if QT_VERSION >= 0x050000
#   include <QtMessageHandler>
#endif
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "myglwidget.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include "posix.hpp"
#endif
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include "test.hpp"
#endif
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    //set_signals();
#endif

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

    MainWindow *main_window = new MainWindow;
    main_window->setAttribute(Qt::WA_DeleteOnClose);

    MyGLWidget *mainBox = new MyGLWidget(main_window->getThis());

    main_window->setCentralWidget(mainBox);
    main_window->show();

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), main_window, SLOT(set_focus(QString)));
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
