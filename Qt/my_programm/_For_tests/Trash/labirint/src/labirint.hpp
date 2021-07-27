/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef LABIRINT_HPP
#define LABIRINT_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Labirint;
}
//--------------------------------------------------------------------------------
class Labirint : public MyWidget
{
    Q_OBJECT

public:
    explicit Labirint(QWidget *parent = nullptr);
    ~Labirint();

private slots:
    void set_cursor(void);
    void load(void);
    void save(void);

private:
    Ui::Labirint *ui;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // LABIRINT_HPP
