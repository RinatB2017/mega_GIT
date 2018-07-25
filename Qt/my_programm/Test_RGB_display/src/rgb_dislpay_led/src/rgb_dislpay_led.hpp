/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef RGB_DISLPAY_LED_HPP
#define RGB_DISLPAY_LED_HPP
//--------------------------------------------------------------------------------
#include <QToolButton>
#include <QMouseEvent>
//--------------------------------------------------------------------------------
class RGB_dislpay_led : public QToolButton
{
    Q_OBJECT

public:
    explicit RGB_dislpay_led(int fix_width,
                             int fix_heigth,
                             QWidget *parent = nullptr);
    ~RGB_dislpay_led();

    void set_R(int value);
    void set_G(int value);
    void set_B(int value);

    int get_R(void);
    int get_G(void);
    int get_B(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    int color_R = 0;
    int color_G = 0;
    int color_B = 0;

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // RGB_DISLPAY_LED_HPP
