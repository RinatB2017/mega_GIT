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
#include "mainwindow.hpp"
#include "parportbox.hpp"
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
    set_codecs();

    QApplication app(argc, argv);
    app.setOrganizationName(ORGNAME);
    app.setApplicationName(APPNAME);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    MainWindow *main_window = new MainWindow();
    Q_CHECK_PTR(main_window);

    ParportBox *mainBox = new ParportBox(main_window->getThis());
    Q_CHECK_PTR(mainBox);
    main_window->setCentralWidget(mainBox);
    main_window->show();

    qDebug() << QString("Starting application \"%1\"").arg(APPNAME);

    return app.exec();
}
//--------------------------------------------------------------------------------
