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
#include <QDebug>
//--------------------------------------------------------------------------------
#if QT_VERSION >= 0x050000
#   include <QtMessageHandler>
#endif
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include "posix.hpp"
#endif
#include "log_to_file.hpp"
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include "test.hpp"
#endif
//--------------------------------------------------------------------------------
//  rm /usr/lib/libGL.so.1
//--------------------------------------------------------------------------------
//#include <QGLFormat>

#include <QScriptEngine>
int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    //set_signals();
#endif
#if QT_VERSION >= 0x050000
    //qInstallMessageHandler(log_to_file);
#else
    //qInstallMsgHandler(log_to_file);
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
    QApplication app(argc, argv);
#endif

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setApplicationVersion(VER_STR);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

#if 0
    QLabel lbl;

    QScriptEngine scriptEngine;
    QScriptValue scriptLbl = scriptEngine.newQObject(&lbl);
    scriptEngine.globalObject().setProperty("lbl", scriptLbl);
    scriptEngine.evaluate("lbl.text = 'Hello, world !'");
    scriptEngine.evaluate("lbl.show()");

    return app.exec();
#endif

    QPixmap pixmap(":/logo/pinguin.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    Q_CHECK_PTR(splash);
    splash->show();

    //---
#if 0
    QPushButton *btn = new QPushButton;
    btn->setFixedSize(320, 200);
    btn->setText("test");
    btn->show();
#endif
    //---
#if 0
    QFile file(":/images/style.css");
    if(file.open(QFile::ReadOnly))
    {
        QString StyleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(StyleSheet);
    }
#endif
    //---

    //MainWindow *main_window = new MainWindow;
    MyMainWindow *main_window = new MyMainWindow();
    main_window->setAttribute(Qt::WA_DeleteOnClose);

    qDebug() << main_window->windowFlags();
    //main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    // отключает заголовок
    //main_window->setWindowFlags(Qt::FramelessWindowHint);

    //---
#if 0
    QPalette palette;

    palette.setBrush(QPalette::WindowText,  QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::Button,      QBrush(QColor(32, 64, 96),      Qt::SolidPattern));
    palette.setBrush(QPalette::Light,       QBrush(QColor(24, 48, 44),      Qt::SolidPattern));
    palette.setBrush(QPalette::Text,        QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor(24, 48, 64),      Qt::SolidPattern));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor(65, 113, 145),    Qt::SolidPattern));
    palette.setBrush(QPalette::Base,        QBrush(QColor(24, 48, 64),      Qt::SolidPattern));
    palette.setBrush(QPalette::Window,      QBrush(QColor(12, 24, 32),      Qt::SolidPattern));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor(65, 113, 145),    Qt::SolidPattern));

    main_window->setPalette(palette);
#endif
    //---

    MainBox *mainBox = new MainBox(main_window->getThis(), splash);

    //---
#if 0
    qDebug() << "OpenGL Versions Supported: " << QGLFormat::openGLVersionFlags();
    QGLFormat format;
    format.setVersion(4, 3);
    QGLFormat::setDefaultFormat(format);
#endif
    //---

    main_window->setCentralWidget(mainBox);
    main_window->show();

    splash->finish(main_window);

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
