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
#include <QDebug>
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

    MainBox *mainBox = new MainBox(main_window->getThis(), splash);
    main_window->setCentralWidget(mainBox);

    main_window->show();

    splash->finish(main_window);

#if 0
    QByteArray ba = QByteArray::fromHex("004F0043005400410054004F004B0020003100300030002E0030003000200070002E00200420043004370434034734D3453001003484403002063000000084082074054044024030060070060030070");
    QString str = QString::fromUtf16(reinterpret_cast<const ushort*>(ba.constData()));
    qDebug() <<"[" << qPrintable(str) << "]";
#endif

    return app.exec();
}
//--------------------------------------------------------------------------------
