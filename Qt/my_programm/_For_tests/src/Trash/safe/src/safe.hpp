/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef SAFE_HPP
#define SAFE_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Safe;
}
//--------------------------------------------------------------------------------
class Safe : public MyWidget
{
    Q_OBJECT

public:
    Safe(QWidget *parent = nullptr);
    ~Safe();

    int get_value(void);

private slots:
    void click(void);
    void s_back(void);
    void s_clear(void);

private:
    Ui::Safe *ui;
    int value = 0;
    QList<QAbstractButton *> widgets;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // SAFE_HPP
