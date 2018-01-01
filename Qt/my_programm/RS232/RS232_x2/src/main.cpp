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
#include <QVBoxLayout>
#include <QTranslator>
#include <QLocale>
#include <QFrame>

#include <QAction>
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    set_codecs();

    QTranslator translator;
    translator.load(":/ru_RU.qm");
    app.installTranslator(&translator);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    QPixmap pixmap(":/logo/pinguin.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    MainWindow *main_window = new MainWindow();

    QFrame *frame = new QFrame;

    splash->showMessage(QObject::tr("init RS-232 (1)..."));
    SerialBox5 *serial = new SerialBox5(main_window->getThis(), "RS-232 (1)");
    serial->add_menu(2, QObject::tr("Настройка RS-232 (1)"));

    splash->showMessage(QObject::tr("init RS-232 (2)..."));
    SerialBox5 *serial2 = new SerialBox5(main_window->getThis(), "RS-232 (2)");
    serial2->add_menu(3, QObject::tr("Настройка RS-232 (2)"));

#if 0
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(serial);
    vbox->addWidget(serial2);
    frame->setLayout(vbox);
#else
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(serial);
    hbox->addWidget(serial2);
    frame->setLayout(hbox);
#endif

    main_window->setCentralWidget(frame);

    main_window->show();

    splash->finish(main_window);

    return app.exec();
}
//--------------------------------------------------------------------------------
