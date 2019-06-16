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
#pragma pack (push, 1)

union QUESTION
{
    struct BODY{
        unsigned short address;
        unsigned char  cmd;
        unsigned short data_len;
        unsigned char  data[3];
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union ANSWER
{
    struct BODY{
        unsigned short address;
        unsigned char  cmd;
        unsigned short data_len;
        unsigned char  data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

#pragma pack (pop)
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5;
class QToolButton;
class QToolBar;
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
    void test(void);
    void read_data(QByteArray ba);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;
    SerialBox5 *serialBox;
    QByteArray data_rs232;
    bool is_ready = false;

    void init(void);
    void wait(int max_time_ms);

    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
