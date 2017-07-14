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
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class GrapherBox;
class QSpinBox;
class QToolBar;
//--------------------------------------------------------------------------------
const QString breloque_answer("don't worry, be happy");
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void test_start(void);
    void test_stop(void);
    void send_data(void);

    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QByteArray data_multimeter;

    GrapherBox *grapher = 0;

    bool test_is_started;
    bool is_ready;

    QSpinBox *sb_address = 0;
    QSpinBox *sb_channel = 0;
    QSpinBox *sb_time_multimeter = 0;

    QSerialPort serial;

    void init(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);

    void wait_msec(int timeout_msec, int max_len_data);
    bool find_device(void);
    bool check_breloque(void);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
