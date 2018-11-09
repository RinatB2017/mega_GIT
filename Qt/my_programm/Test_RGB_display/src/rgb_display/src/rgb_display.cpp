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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "rgb_display_led.hpp"
#include "rgb_display.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
RGB_display::RGB_display(QWidget *parent) :
    MyWidget(parent)
{
    init();

    adjustSize();
    setFixedSize(width(), height());
}
//--------------------------------------------------------------------------------
RGB_display::~RGB_display()
{

}
//--------------------------------------------------------------------------------
void RGB_display::init(void)
{
    double pixelPerMm = QApplication::screens().at(0)->logicalDotsPerInch()/2.54/10;
    double w_led = pixelPerMm * LED_SIZE_W_MM;
    double h_led = pixelPerMm * LED_SIZE_H_MM;

    double border = pixelPerMm * LED_BORDER_MM;

    grid = new QGridLayout();
    grid->setMargin(border);
    grid->setSpacing(border);

    for(int row=0; row<SCREEN_HEIGTH; row++)
    {
        for(int col=0; col<SCREEN_WIDTH; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->set_size(w_led, h_led);

            led->setProperty("property_col", col);
            led->setProperty("property_row", row);

            grid->addWidget(led, row, col);

            l_buttons.append(led);
        }
    }

    setLayout(grid);
}
//--------------------------------------------------------------------------------
bool RGB_display::load_ico(void)
{
    bool ok = picture.load(":/mainwindow/computer.png");
    if(!ok)
    {
        emit error("can't load picture");
        return false;
    }
    max_x = picture.width();
    max_y = picture.height();

    for(int y=0; y<SCREEN_HEIGTH; y++)
    {
        for(int x=0; x<SCREEN_WIDTH; x++)
        {
            QRgb color = picture.pixel(x, y);
            foreach(RGB_dislpay_led *led, l_buttons)
            {
                int p_x = led->property("property_col").toInt();
                int p_y = led->property("property_row").toInt();
                if((p_x == x) && (p_y == y))
                {
                    led->set_R(qRed(color));
                    led->set_G(qGreen(color));
                    led->set_B(qBlue(color));
                    led->repaint();
                    break;
                }
            }
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void RGB_display::show_picture(int begin_x, int begin_y)
{
    if(picture.isNull())
    {
        emit error("picture not loaded!");
        return;
    }

    for(int y=0; y<SCREEN_HEIGTH; y++)
    {
        for(int x=0; x<SCREEN_WIDTH; x++)
        {
            QRgb color = picture.pixel(begin_x + x, begin_y + y);
            foreach(RGB_dislpay_led *led, l_buttons)
            {
                int p_x = led->property("property_col").toInt();
                int p_y = led->property("property_row").toInt();
                if((p_x == x) && (p_y == y))
                {
                    led->set_R(qRed(color));
                    led->set_G(qGreen(color));
                    led->set_B(qBlue(color));
                    led->repaint();
                    break;
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
void RGB_display::load_leds(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif

    settings->beginGroup("Display");
    QByteArray ba_display = settings->value("ba_display").toByteArray();
    settings->endGroup();

    int count_led = 0;
    if(ba_display.isEmpty() == false)
    {
        if(ba_display.count() == (l_buttons.count() * 3))
        {
            for(int n=0; n<ba_display.count(); n+=3)
            {
                l_buttons[count_led]->set_R(ba_display.at(n));
                l_buttons[count_led]->set_G(ba_display.at(n+1));
                l_buttons[count_led]->set_B(ba_display.at(n+2));
                count_led++;
            }
        }
    }

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void RGB_display::save_leds(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif

    QByteArray ba_display;
    foreach (RGB_dislpay_led *led, l_buttons)
    {
        ba_display.append(led->get_R());
        ba_display.append(led->get_G());
        ba_display.append(led->get_B());
    }

    settings->beginGroup("Display");
    settings->setValue("ba_display", ba_display);
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
int RGB_display::get_max_x(void)
{
    return max_x;
}
//--------------------------------------------------------------------------------
int RGB_display::get_max_y(void)
{
    return max_y;
}
//--------------------------------------------------------------------------------
void RGB_display::updateText(void)
{

}
//--------------------------------------------------------------------------------
