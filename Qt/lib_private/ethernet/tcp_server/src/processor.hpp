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
#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QByteArray;
class QString;
class QWidget;
//--------------------------------------------------------------------------------
class Processor : public MyWidget
{
    Q_OBJECT
public:
    explicit Processor(QWidget *parent = nullptr);
    
signals:
    void output(const QByteArray &);

public slots:
    void input(const QByteArray &data);

private:
    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
