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
#include <QElapsedTimer>
#include <QPointer>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
//#   define htons(n) static_cast<uint16_t>((static_cast<uint16_t>(n) << 8)  | (static_cast<uint16_t>(n) >> 8))
//#   define htonl(n) static_cast<uint32_t>((static_cast<uint32_t>(n) << 16) | (static_cast<uint32_t>(n) >> 16))
#   define htons(n) (uint16_t)((((uint16_t) (n)) << 8) | (((uint16_t) (n)) >> 8))
#   define htonl(n) (uint32_t)((((uint32_t) (n)) << 16) | (((uint32_t) (n)) >> 16))
#endif
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

union QUESTION
{
    struct BODY{
        ushort address;
        uchar  cmd;
        ushort data_len;
        uchar  data[3];
        ushort crc16;
    } body;
    uchar buf[sizeof(BODY)];
};

union ANSWER
{
    struct BODY{
        ushort address;
        uchar  cmd;
        ushort data_len;
        uchar  data;
        ushort crc16;
    } body;
    uchar buf[sizeof(BODY)];
};

#pragma pack (pop)
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

private slots:
    void test(void);
    void read_data(QByteArray ba);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
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
