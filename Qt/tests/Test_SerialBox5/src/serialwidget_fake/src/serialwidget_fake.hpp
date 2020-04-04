/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
// FAKE
//--------------------------------------------------------------------------------
#ifndef SERIALWIDGET_FAKE_HPP
#define SERIALWIDGET_FAKE_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QSerialPort>
//--------------------------------------------------------------------------------
// это шаблон, не надо его переписывать
#include "worker_fake.hpp"

#include "serialwidget.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class SerialWidget_fake : public SerialWidget
{
    Q_OBJECT

public:
    explicit SerialWidget_fake(QWidget *parent = nullptr);
    ~SerialWidget_fake();

private:
    Worker_fake *worker_fake = nullptr;
};
//--------------------------------------------------------------------------------
#endif // SERIALWIDGET_HPP
