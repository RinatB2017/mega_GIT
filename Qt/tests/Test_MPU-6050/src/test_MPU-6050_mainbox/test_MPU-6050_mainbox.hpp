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
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
enum CURVE {
    DOTS = 0,
    LINES,
    SPLINE_LINES
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class PlotPicker;
class QSplitter;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void test(void);

    void data_mpu6050(QByteArray data);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;

    QByteArray dirty_array;

    int curve_x_accel = 0;
    int curve_y_accel = 0;
    int curve_z_accel = 0;
    int curve_temperature = 0;
    int curve_x_gyro = 0;
    int curve_y_gyro = 0;
    int curve_z_gyro = 0;

    void init(void);
    void createTestBar(void);
    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
