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
#include <QMessageBox>
#include <QLocale>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mainwindow.hpp"
#include "b590_imitator.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();

    QtSingleApplication app(argc, argv);
    if(app.isRunning())
    {
        QMessageBox::critical(0, QObject::tr("Error"), QObject::tr("Application already running!"));
        return -1;
    }

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    MainWindow *main_window = new MainWindow();

    B590_imitator *mainBox = new B590_imitator(main_window->getThis());
    main_window->setCentralWidget(mainBox);

    main_window->set_status1_text(QString(QObject::tr("1 попугай = %1 uV")).arg(2500000.0f / pow(2, 23)));
    main_window->show();

    return app.exec();
}
//--------------------------------------------------------------------------------
