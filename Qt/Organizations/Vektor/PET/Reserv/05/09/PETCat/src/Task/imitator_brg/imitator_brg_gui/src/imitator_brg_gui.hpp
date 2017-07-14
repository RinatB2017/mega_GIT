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
#ifndef IMITATOR_BRG_GUI_HPP
#define IMITATOR_BRG_GUI_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "imitator_brg.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Imitator_BRG_GUI;
}
//--------------------------------------------------------------------------------
class Imitator_BRG_GUI : public MyWidget
{
    Q_OBJECT

public:
    explicit Imitator_BRG_GUI(QWidget *parent);
    ~Imitator_BRG_GUI();

private slots:
    void test(void);

    void start(void);
    void stop(void);
    void refresh(void);

    bool set_values(void);

private:
    Ui::Imitator_BRG_GUI *ui = 0;

    Imitator_BRG *brg = 0;

    void block_interface(bool state);

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
