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
#include <QMediaPlayer>
#include <QPointer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//#pragma pack (1)

union PELCO_PACKET
{
    struct PELCO_BODY
    {
        uint8_t sync;
        uint8_t address;
        uint8_t cmd1;
        uint8_t cmd2;
        uint8_t data1;
        uint8_t data2;
        uint8_t crc;
    } body;
    uint8_t buf[sizeof(PELCO_BODY)];
};

#pragma pack (pop)
//#pragma pop ()
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
    void send(QByteArray);

public slots:
    void f_move_up(uint8_t speed);
    void f_move_down(uint8_t speed);
    void f_move_left(uint8_t speed);
    void f_move_right(uint8_t speed);
    void f_move_up_left(uint8_t speed);
    void f_move_up_right(uint8_t speed);
    void f_move_down_left(uint8_t speed);
    void f_move_down_right(uint8_t speed);
    void f_zoom_in(void);
    void f_zoom_out(void);
    void f_move_stop(void);

    void f_video(void);

private slots:
    bool test(void);
    void read_data(QByteArray ba);

    void pressed(void);
    void released(void);

    void f_send(void);
    void refresh(void);

    void f_error(QMediaPlayer::Error err);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QMediaPlayer> player;

    void init(void);
    void createTestBar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
