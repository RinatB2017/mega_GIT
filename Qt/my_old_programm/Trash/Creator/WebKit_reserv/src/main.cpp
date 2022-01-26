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
#include "mainwindow.hpp"
#include "webkit_mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type)
    {
    case QtDebugMsg:
        //qDebug() << QString("Debug: %1").arg(msg);
        break;

    case QtWarningMsg:
        qDebug() << QString("Warning: %1").arg(msg);
        break;

    case QtCriticalMsg:
        qDebug() << QString("Critical: %1").arg(msg);
        break;

    case QtFatalMsg:
        qDebug() << QString("Fatal: %1").arg(msg);
        abort();
    }
}
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    //qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    set_codecs();

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));

    MainWindow *main_window = new MainWindow; //(QObject::tr(APPNAME), QString("%1.%2.%3").arg(VER_MAJOR).arg(VER_MINOR).arg(VER_BUILD));

    main_window->setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);
    main_window->showMaximized();

    MainBox *mainBox = new MainBox(main_window->getThis());

    main_window->setCentralWidget(mainBox);
    main_window->setWindowIcon(QIcon(QLatin1String(":/mainwindow/computer.png")));

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));

    return app.exec();
}
//--------------------------------------------------------------------------------
