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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "grapherbox.hpp"
#include "defines.hpp"
#include "version.hpp"
//--------------------------------------------------------------------------------
#include "../lib/codecs.h"
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    set_codecs();

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));

    MainWindow main_window;
    // main_window.setWindowFlags(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowContextHelpButtonHint);

    GrapherBox *mainBox = new GrapherBox(main_window.getThis());
    mainBox->set_title("xxx");
    mainBox->set_axis_scale_x(0, 400);
    mainBox->set_axis_scale_y(-220.0f, 220.0f);
    mainBox->add_curve("signal");

    main_window.setCentralWidget(mainBox);

    main_window.setWindowIcon(QIcon(ICON_PROGRAMM));
    main_window.show();

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));

    return app.exec();
}
//--------------------------------------------------------------------------------
