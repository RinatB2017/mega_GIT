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
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class SerialBox5;
class QTimer;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = 0);
    ~MainBox();

signals:
    void send_data(const QString &);

private slots:
    void tick(void);
    void change_timer_interval(double value);
    void power_timer(bool state);

    void load_data(void);

private:
    Ui::MainBox *ui = 0;
    QWidget *parent = 0;
    SerialBox5 *serial = 0;
    QTimer *timer = 0;
    int index = 0;

    void init(void);
    void createSerial(void);
    void createTimer(void);

    void load_file(const QString &filename);

    void load_setting(void);
    void save_setting(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
