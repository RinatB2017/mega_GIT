/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QLibraryInfo>
//--------------------------------------------------------------------------------
#if QT_VERSION >= 0x050000
    #include <QtMessageHandler>
#endif
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_UNIX
    #include "posix.hpp"
#endif
#include "log_to_file.hpp"
//--------------------------------------------------------------------------------
#include "lib/codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
    #include "test.hpp"
#endif
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#ifdef Q_OS_UNIX
    set_signals();
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

    QPixmap pixmap(":/logo/pinguin.png");

    MySplashScreen *splash = new MySplashScreen(pixmap, 10);
    splash->show();

    qApp->processEvents();

    MainWindow main_window(QObject::tr(ORGNAME),
                           QObject::tr(APPNAME),
                           QString("%1.%2.%3 (build %4)").arg(VER_MAJOR).arg(VER_MINOR).arg(VER_PATCH).arg(VER_BUILD));
    //main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    //---
    // Workaround: Removing the /usr/lib/libGL.so.1 file fixes the problem.
    // qDebug() << "OpenGL Versions Supported: " << QGLFormat::openGLVersionFlags();
    // QGLFormat format;
    // format.setVersion(4, 3);
    // QGLFormat::setDefaultFormat(format);
    //---

    MainBox *mainBox = new MainBox(main_window.getThis(), splash);
    main_window.setCentralWidget(mainBox);
    main_window.setWindowIcon(QIcon(ICON_PROGRAMM));
    main_window.show();

    splash->finish(&main_window);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &main_window, SLOT(set_focus(QString)));
    qDebug() << qPrintable(QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME)));

#ifdef QT_DEBUG
    int test_result = QTest::qExec(new Test(&main_window), argc, argv);

    if (test_result != EXIT_SUCCESS)
    {
        return test_result;
    }
#endif

    return app.exec();
}
//--------------------------------------------------------------------------------
