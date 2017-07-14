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
#ifndef GEN_SINUS_BOX_HPP
#define GEN_SINUS_BOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFrame>
//--------------------------------------------------------------------------------
namespace Ui {
    class Gen_sinus_box;
}
//--------------------------------------------------------------------------------
class Gen_sinus_box : public QFrame
{
    Q_OBJECT

public:
    explicit Gen_sinus_box(QWidget *parent);
    ~Gen_sinus_box();

    double get_offset_begin_y(void);
    double get_vert_ampl(void);
    double get_begin_angle(void);
    double get_count_cycles(void);

signals:
    void generate(void);

private:
    Ui::Gen_sinus_box *ui;
};
//--------------------------------------------------------------------------------
#endif // GEN_SINUS_BOX_HPP
