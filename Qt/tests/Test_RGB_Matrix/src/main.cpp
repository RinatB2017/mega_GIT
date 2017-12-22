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
#include <QLocale>
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
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

    QPixmap pixmap(":/logo/pinguin.png");
    MySplashScreen *splash = new MySplashScreen(pixmap);
    splash->show();
    splash->showMessage(QObject::tr("Подождите ..."));
    qApp->processEvents();

    MainWindow *main_window = new MainWindow();

#if 0
    //Light
    QPalette palette;

    palette.setBrush(QPalette::WindowText,  QBrush(QColor((QRgb)0x000000)));
    palette.setBrush(QPalette::Button,      QBrush(QColor((QRgb)0xd4d4d4)));
    palette.setBrush(QPalette::Light,       QBrush(QColor((QRgb)0xffffff)));
    palette.setBrush(QPalette::Text,        QBrush(QColor((QRgb)0x000000)));
    palette.setBrush(QPalette::BrightText,  QBrush(QColor((QRgb)0xffffff)));
    palette.setBrush(QPalette::ButtonText,  QBrush(QColor((QRgb)0x000000)));
    palette.setBrush(QPalette::Base,        QBrush(QColor((QRgb)0xffffff)));
    palette.setBrush(QPalette::Window,      QBrush(QColor((QRgb)0xd4d4d4)));
    palette.setBrush(QPalette::Shadow,      QBrush(QColor((QRgb)0x404040)));

    main_window->setPalette(palette);
#endif

    MainBox *mainBox = new MainBox(main_window->getThis(), splash);
    main_window->setCentralWidget(mainBox);

    //qApp->setPalette(qApp->style()->standardPalette());
    main_window->show();

    splash->finish(main_window);

    return app.exec();
}
//--------------------------------------------------------------------------------
