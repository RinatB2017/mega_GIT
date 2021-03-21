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
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox4.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    set_codecs();

    QApplication app(argc, argv);

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/logo.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));

    MainWindow *main_window = new MainWindow();

    QFrame *frame = new QFrame;

    splash->showMessage(QObject::tr("init RS-232 (1)..."));
    SerialBox4 *serial = new SerialBox4(main_window, "RS-232 (1)");
    serial->add_menu(2, QObject::tr("Настройка RS-232 (1)"));

    splash->showMessage(QObject::tr("init RS-232 (2)..."));
    SerialBox4 *serial2 = new SerialBox4(main_window, "RS-232 (2)");
    serial2->add_menu(3, QObject::tr("Настройка RS-232 (2)"));

#ifdef SINGLE_APP
    QBoxLayout *box = new QBoxLayout(QBoxLayout::LeftToRight);
#else
    QBoxLayout *box = new QBoxLayout(QBoxLayout::TopToBottom);
#endif
    box->addWidget(serial);
    box->addWidget(serial2);
    frame->setLayout(box);

    main_window->setCentralWidget(frame);

    main_window->show();

    splash->finish(main_window);

    return app.exec();
}
//--------------------------------------------------------------------------------
