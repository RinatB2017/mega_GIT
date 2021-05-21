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
#include <QColorDialog>
#include <QApplication>
#include <QToolButton>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
class RGB_dislpay_led : public QToolButton
{
    Q_OBJECT

public:
    explicit RGB_dislpay_led(QWidget *parent = nullptr);
    virtual ~RGB_dislpay_led();

    void set_R(int value);
    void set_G(int value);
    void set_B(int value);

    uint8_t get_R(void);
    uint8_t get_G(void);
    uint8_t get_B(void);

    double get_width(void);
    double get_height(void);

    bool set_size(double w_size_mm,
                  double h_size_mm,
                  double l_border_mm,
                  double u_border_mm);
    void get_size(double *w_size_mm,
                  double *h_size_mm,
                  double *l_border_mm,
                  double *u_border_mm);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    uint8_t color_R = 0;
    uint8_t color_G = 0;
    uint8_t color_B = 0;

    double pixelPerMm = 1.0;

    double full_width = LED_SIZE_W_MM;
    double full_height = LED_SIZE_H_MM;

    double led_width  = 0;
    double led_heigth = 0;

    double left_border_width = 0;
    double up_border_height = 0;

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // RGB_DISLPAY_LED_HPP
