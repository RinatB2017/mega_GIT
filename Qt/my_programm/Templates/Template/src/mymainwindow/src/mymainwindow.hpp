/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MYMAINWINDOW_HPP
#define MYMAINWINDOW_HPP
//--------------------------------------------------------------------------------
#include <QTextStream>
#include <QFile>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
//--------------------------------------------------------------------------------
class MyMainWindow : public MainWindow
{
    Q_OBJECT

public:
    MyMainWindow(MainWindow *parent = nullptr);
    virtual ~MyMainWindow();

    void install_handler(void);

#ifdef HAVE_QT5
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);
#else
    static void messageHandler(QtMsgType type,
                               const char *msg);
#endif

private:
    static QFile *m_logFile;

};
//--------------------------------------------------------------------------------
#endif // MYMAINWINDOW_HPP
