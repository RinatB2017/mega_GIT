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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5_lite;
class QSpinBox;
class QTimer;
class Display;
class MyPalette;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void run(bool state);
    void read_display_data(QByteArray ba);
    void read_data(QByteArray ba);
    void update(void);

    void test(void);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    SerialBox5_lite *main_serialBox;
    SerialBox5_lite *control_serialBox;

    QByteArray display_data_rs232;
    QByteArray data_rs232;

    QSpinBox *sb_interval;
    QTimer *timer;

    Display *display;
    Display *control_display;
    MyPalette *palette;
    int pos_x;

    bool is_busy = false;
    bool is_ready = false;

    void init(void);
    void wait(int max_time_ms);

    void createTestBar(void);
    void createSerialBox(void);
    void createDisplayBox(void);
    void createDockWidgets(void);
    void createTimer(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
