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
#ifndef IMITATOR_BRG_OPTIONS_HPP
#define IMITATOR_BRG_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Imitator_BRG_options;
}
//--------------------------------------------------------------------------------
class Imitator_BRG_options : public MyWidget
{
    Q_OBJECT

public:
    explicit Imitator_BRG_options(QWidget *parent = 0);
    ~Imitator_BRG_options();

private slots:
    void block_wheel(void);
    void t_accept(void);

private:
    Ui::Imitator_BRG_options *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // IMITATOR_BRG_OPTIONS_HPP
