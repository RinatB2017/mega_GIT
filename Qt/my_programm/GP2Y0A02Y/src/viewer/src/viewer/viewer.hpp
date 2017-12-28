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
#ifndef VIEWER_HPP
#define VIEWER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "device.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Viewer;
}
//--------------------------------------------------------------------------------
class Viewer : public MyWidget
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = 0);
    ~Viewer();

public slots:
    bool set_value(int channel, int value);

private:
    Ui::Viewer *ui;
    QList<Device *> devices;

    void init(void);
    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // VIEWER_HPP
//--------------------------------------------------------------------------------
