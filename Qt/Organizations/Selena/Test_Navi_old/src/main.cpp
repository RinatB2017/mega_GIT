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
#include <QString>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
#ifdef Q_OS_LINUX
    #include "posix.hpp"
#endif
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    // set_signals();
#endif
    QApplication app(argc, argv);
    set_codecs();

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));

    MainWindow main_window;
    // main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    MainBox *mainBox = new MainBox(main_window.getThis());

    main_window.setCentralWidget(mainBox);

    main_window.setWindowIcon(QIcon(ICON_PROGRAMM));
    main_window.show();

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));

    return app.exec();
}
//--------------------------------------------------------------------------------
