/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QGridLayout>
#include <QSettings>
//--------------------------------------------------------------------------------
#include "display.hpp"
#include "diod.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
Display::Display(unsigned int max_x,
                 unsigned int max_y,
                 unsigned int led_width,
                 unsigned int led_height,
                 QWidget *parent) :
    MyWidget(parent)
{
    for(int y=0; y<MAX_DISPLAY_Y; y++)
    {
        for(int x=0; x<MAX_DISPLAY_X; x++)
        {
            diod[x][y] = nullptr;
        }
    }

    create_display(max_x, max_y, led_width, led_height);

    emit info(QString("max_x = %1").arg(max_x));
    emit info(QString("max_y = %1").arg(max_y));
}
//--------------------------------------------------------------------------------
bool Display::create_display(unsigned int w,
                             unsigned int h,
                             unsigned int led_width,
                             unsigned int led_height)
{
    if(w > MAX_DISPLAY_X)   w = MAX_DISPLAY_X;
    if(w <= 0)              w = DEFAULT_X;
    if(h > MAX_DISPLAY_Y)   h = MAX_DISPLAY_Y;
    if(h <= 0)              h = DEFAULT_Y;

    this->max_x = w;
    this->max_y = h;

    emit debug(QString("create_display(%1, %2)").arg(w).arg(h));

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);
    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            diod[x][y] = new Diod(static_cast<int>(led_width),
                                  static_cast<int>(led_height),
                                  this);
            diod[x][y]->set_left_btn_active(true);
            diod[x][y]->set_right_btn_active(false);

            grid->addWidget(diod[x][y],
                            static_cast<int>(y),
                            static_cast<int>(x));
        }
    }

    setLayout(grid);
    adjustSize();
    //setFixedSize(sizeHint());

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    update();
    return true;
}
//--------------------------------------------------------------------------------
void Display::set_left_btn_active(bool value)
{
    flag_active = value;
    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            diod[x][y]->set_left_btn_active(value);
        }
    }
}
//--------------------------------------------------------------------------------
void Display::set_right_btn_active(bool value)
{
    flag_active = value;
    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            diod[x][y]->set_right_btn_active(value);
        }
    }
}
//--------------------------------------------------------------------------------
void Display::set_data(QByteArray data)
{
    if(static_cast<uint>(data.length()) != (max_x * max_y * 3))
    {
        emit error("Display::set_data bad data size!");
        return;
    }

    int index = 0;
    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            diod[x][y]->set_color(static_cast<uint8_t>(data[index]),
                                  static_cast<uint8_t>(data[index+1]),
                    static_cast<uint8_t>(data[index+2]));
            index+=3;
        }
    }
}
//--------------------------------------------------------------------------------
QByteArray Display::get_data(void)
{
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
    QByteArray ba;

    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            R = diod[x][y]->get_R();
            G = diod[x][y]->get_G();
            B = diod[x][y]->get_B();

            ba.append(static_cast<char>(R));
            ba.append(static_cast<char>(G));
            ba.append(static_cast<char>(B));
        }
    }
    return ba;
}
//--------------------------------------------------------------------------------
Display::~Display()
{
    save_setting();
}
//--------------------------------------------------------------------------------
bool Display::set_color(unsigned int x,
                        unsigned int y,
                        QColor color)
{
    if(x > max_x)
    {
        emit error("Display::set_color: x > MAX_DISPLAY_X");
        return false;
    }
    if(y > max_y)
    {
        emit error("Display::set_color: y > MAX_DISPLAY_Y");
        return false;
    }

    if(diod[x][y])
    {
        diod[x][y]->set_color(color);
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Display::set_color(unsigned int x,
                        unsigned int y,
                        uint8_t R_value,
                        uint8_t G_value,
                        uint8_t B_value)
{
    if(x > max_x)
    {
        emit error("Display::set_color: x > MAX_DISPLAY_X");
        return false;
    }
    if(y > max_y)
    {
        emit error("Display::set_color: y > MAX_DISPLAY_Y");
        return false;
    }

    if(diod[x][y])
    {
        diod[x][y]->set_color(R_value,
                              G_value,
                              B_value);
    }
    return true;
}
//--------------------------------------------------------------------------------
uint Display::get_max_x(void)
{
    return max_x;
}
//--------------------------------------------------------------------------------
uint Display::get_max_y(void)
{
    return max_y;
}
//--------------------------------------------------------------------------------
bool Display::get_R(unsigned int x,
                    unsigned int y,
                    uint8_t *value)
{
    if(x > max_x)
    {
        emit error(QString("Display::set_color: x = %1").arg(x));
        return false;
    }
    if(y > max_y)
    {
        emit error(QString("Display::set_color: y = %1").arg(y));
        return false;
    }

    if(diod[x][y] != nullptr)
    {
        *value = diod[x][y]->get_R();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool Display::get_G(unsigned int x,
                    unsigned int y,
                    uint8_t *value)
{
    if(x > max_x)
    {
        emit error("Display::set_color: x > MAX_DISPLAY_X");
        return false;
    }
    if(y > max_y)
    {
        emit error("Display::set_color: y > MAX_DISPLAY_Y");
        return false;
    }

    *value = diod[x][y]->get_G();
    return true;
}
//--------------------------------------------------------------------------------
bool Display::get_B(unsigned int x,
                    unsigned int y,
                    uint8_t *value)
{
    if(x > max_x)
    {
        emit error("Display::set_color: x > MAX_DISPLAY_X");
        return false;
    }
    if(y > max_y)
    {
        emit error("Display::set_color: y > MAX_DISPLAY_Y");
        return false;
    }

    *value = diod[x][y]->get_B();
    return true;
}
//--------------------------------------------------------------------------------
void Display::clear(void)
{
    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            set_color(x, y, QColor(Qt::black));
        }
    }
}
//--------------------------------------------------------------------------------
void Display::load_setting(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(objectName());
    set_data(settings->value("value").toByteArray());
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void Display::save_setting(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(objectName());
    settings->setValue("value", get_data());
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
bool Display::resize(unsigned int w, unsigned int h)
{
    if(w > MAX_DISPLAY_X)
    {
        emit error("Display::resize: w > MAX_DISPLAY_X");
        return false;
    }
    if(h > MAX_DISPLAY_Y)
    {
        emit error("Display::resize: h > MAX_DISPLAY_Y");
        return false;
    }

    qDeleteAll(children());

    bool ok = create_display(w, h,
                             static_cast<uint>(diod[0][0]->width()),
            static_cast<uint>(diod[0][0]->height()));
    if(ok)
    {
        adjustSize();
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool Display::resize_led(unsigned int w, unsigned int h)
{
    if(w == 0)  return false;
    if(h == 0)  return false;
    if(w > MAX_LED_SIZE_W)  return false;
    if(h > MAX_LED_SIZE_H)  return false;

    for(unsigned int y=0; y<max_y; y++)
    {
        for(unsigned int x=0; x<max_x; x++)
        {
            if(diod[x][y])
            {
                diod[x][y]->resize(static_cast<int>(w),
                                   static_cast<int>(h));
            }
        }
    }
    adjustSize();
    return true;
}
//--------------------------------------------------------------------------------
void Display::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Display::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
