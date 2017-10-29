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
class SerialBox5;
class QToolButton;
class QSpinBox;
class QToolBar;
class Diod;
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
    void on(void);
    void off(void);

    void test(void);
    void test2(void);

    void read_data(QByteArray ba);

    void click(void);

    void btn_click(bool state);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    SerialBox5 *serialBox = 0;
    QByteArray data_rs232;

    uint8_t buf_leds[MAX_SCREEN_X][MAX_SCREEN_Y] = { \
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0
    };

    void init(void);
    void init_widgets(void);
    void connect_log(void);

    QToolButton *btn[MAX_SCREEN_X][MAX_SCREEN_Y];

    QSpinBox *sb_min = 0;
    QSpinBox *sb_max = 0;

    QSlider  *sl_cold = 0;
    QSpinBox *sb_cold = 0;
    QSlider  *sl_hot = 0;
    QSpinBox *sb_hot = 0;

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void createTestBar(void);
    void createSerialBox(void);
    void createGridBox(void);

    void block_this_button(bool state);
    void block_interface(bool state);
    void block_widget(const QString name, bool state);

    QString convert_data_to_ascii(uint8_t data);
    uint8_t convert_ascii_to_value(char hi, char lo);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
