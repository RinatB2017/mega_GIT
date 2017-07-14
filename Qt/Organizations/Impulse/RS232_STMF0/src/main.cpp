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
#include <QTranslator>
#include <QMessageBox>
#include <QLocale>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qtsingleapplication.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "version.hpp"
#include "math.h"
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
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

    QTranslator translator;
    translator.load(":/ru_RU.qm");
    app.installTranslator(&translator);

    QPixmap pixmap(":/logo/pinguin.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    MainWindow main_window;

    MainBox *mainBox = new MainBox(main_window.getThis(), splash);
    main_window.setCentralWidget(mainBox);

    main_window.set_status1_text(QString(QObject::tr("1 попугай = %1 uV")).arg(2500000.0f / pow(2, 23)));

    main_window.setWindowIcon(QIcon(ICON_PROGRAMM));
    main_window.show();

    splash->finish(&main_window);

    return app.exec();
}
//--------------------------------------------------------------------------------
