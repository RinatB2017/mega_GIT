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
#include <QTextCodec>
#include <QString>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "version.hpp"
#include "server.hpp"
//--------------------------------------------------------------------------------
void codec(void)
{
    QTextCodec *unixcodec = QTextCodec::codecForName("UTF8");
    QTextCodec *wincodec  = QTextCodec::codecForName("CP866");
#ifdef Q_OS_LINUX
    Q_UNUSED(wincodec);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(unixcodec);
    QTextCodec::setCodecForCStrings(unixcodec);
#endif
    QTextCodec::setCodecForLocale(unixcodec);
#endif
#ifdef Q_OS_WIN
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(unixcodec);
    QTextCodec::setCodecForCStrings(wincodec);
#endif
    QTextCodec::setCodecForLocale(wincodec);
#endif
}
//--------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    codec();

    app.setOrganizationName(QObject::tr(ORGNAME));
    app.setApplicationName(QObject::tr(APPNAME));

    Server *server = new Server();

    app.setActiveWindow(server);

    qDebug() << QString(QObject::tr("Starting application %1")).arg(QObject::tr(APPNAME));

    return app.exec();
}
//--------------------------------------------------------------------------------
