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
class PlotPicker;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void test(void);

    void data_mpu6050(QByteArray data);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;

    QList<QLCDNumber *> display_widgets;

    QByteArray dirty_array;

    int curve_x_accel = 0;
    int curve_y_accel = 0;
    int curve_z_accel = 0;
    int curve_temperature = 0;
    int curve_x_gyro = 0;
    int curve_y_gyro = 0;
    int curve_z_gyro = 0;

    int curve_x_angle = 0;
    int curve_y_angle = 0;
    int curve_z_angle = 0;

    qreal x_angle = 0;
    qreal y_angle = 0;
    qreal z_angle = 0;

    QString convert(qreal value);

    void init(void);
    void createTestBar(void);
    void init_gl_widget(void);
    void init_serial_widget(void);
    void init_grapher_widget(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
