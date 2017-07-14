/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QIcon>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QTest>
#   include "test.hpp"
#endif
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#if 1
    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        if(app.sendMessage("Wake up!")) return 0;
    }
#else
    QApplication app(argc, argv);
#endif

#ifdef UNDER_CONSTRUCTION
#ifndef QT_DEBUG
    MyWidget::messagebox_warning("Warning", "UNDER_CONSTRUCTION");
#endif
#endif

    QCoreApplication::setApplicationName(NAME_PROGRAMM);
    QCoreApplication::setOrganizationName("Work");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(NAME_PROGRAMM);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    QString title = QString("%1 (%2.%3.%4)")
            .arg(APPNAME)
            .arg(VER_MAJOR).arg(VER_MINOR).arg(VER_PATCH);

    MainWindow mainWin;
    mainWin.setWindowTitle(title);
    mainWin.setWindowIcon(QIcon(ICON_PROGRAMM));
    mainWin.show();

#ifdef QT_DEBUG2
    int test_result = QTest::qExec(new Test(), argc, argv);

    if (test_result != EXIT_SUCCESS)
    {
        return test_result;
    }
#endif

    return app.exec();
}
//--------------------------------------------------------------------------------
