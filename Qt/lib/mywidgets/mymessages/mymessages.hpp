/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef MYMESSAGES_HPP
#define MYMESSAGES_HPP
//--------------------------------------------------------------------------------
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
//--------------------------------------------------------------------------------
#define MESSAGEBOX_WIDTH    320
//--------------------------------------------------------------------------------
class MyMessages
{
public:
    MyMessages();

    static int messagebox_noicon(const QString &title,
                                 const QString &text,
                                 unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_info(const QString &title,
                               const QString &text,
                               unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_warning(const QString &title,
                                  const QString &text,
                                  unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_critical(const QString &title,
                                   const QString &text,
                                   unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_question(const QString &title,
                                   const QString &text,
                                   unsigned int width = MESSAGEBOX_WIDTH);
};
//--------------------------------------------------------------------------------
#endif
