/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
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
#include <QElapsedTimer>
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "packets.h"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void send(const QByteArray&);

private slots:
    void choice_test();
    void choice_programm();

    void read_data(QByteArray ba);
    bool test();

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)();
    } CMD_t;
    QList<CMD> test_commands;
    QList<CMD> programm_commands;

    QPointer<QToolBar> test_bar;
    QPointer<QToolBar> programm_bar;
    QPointer<QComboBox> cb_test;
    QPointer<QComboBox> cb_programm;

    QByteArray input_ba;
    QByteArray packet_ba;
    bool data_is_ready = false;
    bool is_silence = false;

    void init_widgets();
    void connects();
    void init_serial_fix();

    bool get_ID();
    void f_send(const QByteArray &data);
    void wait_msec(int msec, bool waiting = false);

    template<typename T>
    bool check_answer(int cmd);

    uint8_t crc8(uint8_t *buf,uint8_t offset, uint8_t length);

    void init();
    void create_test_bar();
    void create_programm_bar();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();

    friend class Test; // Для работы QTest
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
