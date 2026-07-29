/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QColorDialog>
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class Diod;
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
    void read_data(QByteArray ba);
    void test();
    void set_color();

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
    QByteArray data_rs232;

    int color_R = 0;
    int color_G = 0;
    int color_B = 0;

    void init();
    void init_widgets();

    void createTestBar();
    void createSerialBox();

    void block_this_button(bool state);
    void block_interface(bool state);
    void block_widget(const QString &name, bool state);

    QString convert_data_to_ascii(uint8_t data);
    uint8_t convert_ascii_to_value(char hi, char lo);

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
