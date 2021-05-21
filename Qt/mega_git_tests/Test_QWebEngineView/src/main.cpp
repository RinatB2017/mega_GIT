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
#include "mainwindow.hpp"
#include "test_qwebengineview_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    set_codecs();

    app.setOrganizationName(ORGNAME);
    app.setApplicationName(APPNAME);
#ifdef Q_OS_LINUX
    app.setApplicationVersion(QString("%1.%2.%3.%4")
                              .arg(VER_MAJOR)
                              .arg(VER_MINOR)
                              .arg(VER_PATCH)
                              .arg(VER_BUILD));
#endif
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    MainWindow *main_window = new MainWindow();
    Q_ASSERT(main_window);
    // main_window->setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    MainBox *mainBox = new MainBox(main_window);
    Q_ASSERT(mainBox);

    main_window->setCentralWidget(mainBox);
    main_window->show();

#ifdef QT_DEBUG
    qDebug() << QString(QObject::tr("Starting application %1")).arg(APPNAME);
#endif

    return app.exec();
}
//--------------------------------------------------------------------------------
