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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#define SIZE 1000

#define MAX_VOLTAGE     400
#define NORMAL_VOLTAGE  311.127f

#define MAX_NOISE       20
#define NORMAL_NOISE    13.25f
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class GrapherBox;
class LogBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = 0);
    ~MainBox();

private slots:
    void calc(double voltage);
    void updateGraphics(void);
    void drawGraphics(void);

private:
    Ui::MainBox *ui;
    GrapherBox *grapher;

    double max_U = 0;
    double current_U = 0;
    double y[SIZE];
    double y2[SIZE];
    double y3[SIZE];

    int curve_voltage = 0;
    int curve_interference = 0;
    int curve_envelope = 0;

    void init(void);
    int getPositionStep(void);
    int getSizeStep(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
