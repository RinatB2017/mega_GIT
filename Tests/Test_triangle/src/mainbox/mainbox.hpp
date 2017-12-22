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
//--------------------------------------------------------------------------------
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = 0);
    ~MainBox();

protected:
    void changeEvent(QEvent *e);

private slots:
    void test(void);

private:
    Ui::MainBox *ui = 0;
    QWidget *parent = 0;

    void init(void);
    void createTestBar(void);

    void load_setting(void);
    void save_setting(void);

    bool check_triangle(double A, double B, double C);
    bool check_angle(const QString &angle_name, double angle_grad);
    bool check_trianle_120(double A, double B, double C);

    void test_triangle(double a, double b, double c);

    double grad_to_rad(double angle_grad);
    double rad_to_grad(double angle_rad);

    bool calc(double A, double B, double C, double angle_grad);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
