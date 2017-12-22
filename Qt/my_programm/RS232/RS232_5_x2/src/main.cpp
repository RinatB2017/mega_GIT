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
#include "serialbox5_lite.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    set_codecs();

    QTranslator translator;
    translator.load(":/ru_RU.qm");
    app.installTranslator(&translator);
    app.setWindowIcon(QIcon(ICON_PROGRAMM));

    MainWindow *main_window = new MainWindow();

    QPixmap pixmap(":/logo/pinguin.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    Q_CHECK_PTR(splash);
    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    QFrame *frame = new QFrame;
    Q_CHECK_PTR(frame);
    splash->showMessage("init RS-232_5 (1)...");
    SerialBox5 *serial = new SerialBox5(main_window->getThis(), "RS-232_5 (1)", "RS-232_1");
    serial->add_menu(2);

    splash->showMessage("init RS-232_5 (2)...");
    SerialBox5_lite *serial2 = new SerialBox5_lite(main_window->getThis(), "RS-232_5 (2)", "RS-232_2");
    serial2->setFixedSize(serial2->sizeHint());
    serial2->add_menu(4);

#if 1
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

#if 1
    main_window->setCentralWidget(frame);
#else
    main_window->setCentralWidget(frame, false, Qt::Horizontal);
#endif

    main_window->show();

    splash->finish(main_window);

    return app.exec();
}
//--------------------------------------------------------------------------------
