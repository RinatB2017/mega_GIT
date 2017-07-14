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
#ifndef FLOODMAP_OPTIONS_HPP
#define FLOODMAP_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Floodmap_options;
}
//--------------------------------------------------------------------------------
class Floodmap_options : public MyWidget
{
    Q_OBJECT

public:
    explicit Floodmap_options(QWidget *parent = 0);
    ~Floodmap_options();

    void init(void);

private slots:
    void block_wheel(void);
    void t_accept(void);

private:
    Ui::Floodmap_options *ui;

protected:
    bool eventFilter(QObject*, QEvent* event);

};
//--------------------------------------------------------------------------------
#endif
