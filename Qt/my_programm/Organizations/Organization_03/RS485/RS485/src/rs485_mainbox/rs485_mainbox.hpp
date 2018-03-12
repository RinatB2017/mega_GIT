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
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class SerialPort_test;
class PelcoD;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

    enum Errors {
        E_NO_ERROR = 0,
        E_ANSWER_EMPTY,
        E_ANSWER_BAD_SIZE,
        E_BAD_PREFIX,
        E_BAD_CMD,
        E_BAD_CRC16
    };

signals:
    void send(QByteArray);

private slots:
    void read_data(QByteArray ba);
    void test(void);

    void cmd_test(void);
    void cmd_reset(void);
    void cmd_read(void);
    void cmd_write(void);

    void cmd_pump_on(void);
    void cmd_pump_off(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QByteArray data_rs232_dirty;
    QByteArray data_rs232_clean;
    bool is_ready = false;

    PelcoD *pelco_d = 0;

    void init(void);
    void wait(int max_time_ms);

    void createTestBar(void);

    QByteArray convert(QByteArray ba);
    int check_answer_test(QByteArray data);
    int check_answer_reset(QByteArray data);
    int check_answer_read(QByteArray data);
    int check_answer_write(QByteArray data);
    int check_answer_pump_on(QByteArray data);
    int check_answer_pump_off(QByteArray data);

    void print_err(int code);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
