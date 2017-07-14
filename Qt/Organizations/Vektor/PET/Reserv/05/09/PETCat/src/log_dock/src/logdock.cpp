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
#include <QMetaMethod>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "logdock.hpp"
//#include "logdock_options.hpp"
//--------------------------------------------------------------------------------
#include "logbox.hpp"
//--------------------------------------------------------------------------------
LogDock::LogDock(const QString &title,
                 QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    le = new LogBox(this);
    if(!le) return;

    setWidget(le);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::infoLog(const QString &text)
{
    le->infoLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::debugLog(const QString &text)
{
    le->debugLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::errorLog(const QString &text)
{
    le->errorLog(text);
}
//--------------------------------------------------------------------------------
void LogDock::messageLog(const QString &text)
{
    le->messageLog(text);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void LogDock::load_settings(void)
{
    le->load_settings();
}
//--------------------------------------------------------------------------------
void LogDock::save_settings(void)
{
    le->save_settings();
}
//--------------------------------------------------------------------------------
void LogDock::load_html(void)
{
    le->load_html();
}
//--------------------------------------------------------------------------------
void LogDock::save_html(void)
{
    le->save_html();
}
//--------------------------------------------------------------------------------
void  LogDock::set_font(QFont font)
{
    le->set_font(font);
}
//--------------------------------------------------------------------------------
QFont LogDock::get_font(void)
{
    return le->get_font();
}
//--------------------------------------------------------------------------------
