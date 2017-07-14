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

    mainWin.setPalette(palette);
#endif

    mainWin.show();

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
