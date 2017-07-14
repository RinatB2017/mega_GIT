/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef TESTBOX_2_HPP
#define TESTBOX_2_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class TestBox_2;
}
//--------------------------------------------------------------------------------
class TestBox_2 : public MyWidget
{
    Q_OBJECT

public:
    explicit TestBox_2(QWidget *parent);
    ~TestBox_2();

private slots:
    void test_Approx(void);
    void test_RMS(void);
    void test(void);

private:
    Ui::TestBox_2 *ui = 0;

    int curve_data = 0;
    int curve_ideal = 0;
    int curve_A_min = 0;
    int curve_A_max = 0;

    double ** getData(int n);
    void getApprox(double **x, double *k, double *b, int n);

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
