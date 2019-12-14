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
}
//--------------------------------------------------------------------------------
RGB_display::~RGB_display()
{
    save_leds();
}
//--------------------------------------------------------------------------------
bool RGB_display::set_param(int cnt_led_x,
                            int cnt_led_y,
                            double width_led,
                            double height_led,
                            double l_border,
                            double u_border)
{
    if(cnt_led_x <= 0)  return false;
    if(cnt_led_y <= 0)  return false;
    if(width_led <= 0)  return false;
    if(height_led <= 0) return false;
    if(l_border <= 0)   return false;
    if(u_border <= 0)   return false;

    max_x = cnt_led_x;
    max_y = cnt_led_y;
    w_led = width_led;
    h_led = height_led;
    left_border = l_border;
    up_border = u_border;

    create_new_display();

    return true;
}
//--------------------------------------------------------------------------------
bool RGB_display::get_param(int *cnt_led_x,
                            int *cnt_led_y,
                            double *width_led,
                            double *height_led,
                            double *l_border,
                            double *u_border)
{
    (*cnt_led_x) = max_x;
    (*cnt_led_y) = max_y;
    (*width_led) = w_led;
    (*height_led) = h_led;
    (*l_border) = left_border;
    (*u_border) = up_border;

    return true;
}
//--------------------------------------------------------------------------------
void RGB_display::init(void)
{
    max_x = SCREEN_WIDTH;
    max_y = SCREEN_HEIGTH;

    w_led = LED_SIZE_W_MM;
    h_led = LED_SIZE_H_MM;

    left_border = LED_BORDER_W_MM;
    up_border   = LED_BORDER_H_MM;

    grid = new QGridLayout();
    grid->setMargin(1);
    grid->setSpacing(1);

    QVBoxLayout *vbox2 = new QVBoxLayout();
    vbox2->addLayout(grid);
    vbox2->addStretch(1);

    QHBoxLayout *box = new QHBoxLayout();
    box->addLayout(vbox2);

    setLayout(box);

    load_leds();
    //create_new_display();

    adjustSize();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //setFixedSize(width(), height());
}
//--------------------------------------------------------------------------------
void RGB_display::clean_grid(void)
{
    for(int y=0; y<grid->rowCount(); y++)
    {
        for(int x=0; x<grid->columnCount(); x++)
        {
            QLayoutItem *item = grid->itemAtPosition(y, x);
            if(item)
            {
                QWidget *w = item->widget();
                if(w)
                {
                    grid->removeWidget(w);
                    w->deleteLater();
                }
            }
        }
    }
    l_buttons.clear();
}
//--------------------------------------------------------------------------------
void RGB_display::create_new_display(void)
{
    emit trace(Q_FUNC_INFO);

    clean_grid();
    for(int row=0; row<max_y; row++)
    {
        for(int col=0; col<max_x; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->set_size(w_led,
                          h_led,
                          left_border,
                          up_border);

            led->setProperty("property_col", col);
            led->setProperty("property_row", row);

            grid->addWidget(led, row, col);

            l_buttons.append(led);
        }
    }
    emit debug(QString("created %1 leds").arg(l_buttons.count()));
}
//--------------------------------------------------------------------------------
void RGB_display::set_display(void)
{
    emit trace(Q_FUNC_INFO);

    foreach(RGB_dislpay_led *led, l_buttons)
    {
        led->set_size(w_led,
                      h_led,
                      left_border,
                      up_border);
    }
    adjustSize();
    //setFixedSize(width(), height());
}
//--------------------------------------------------------------------------------
void RGB_display::get_display(void)
{
    if(l_buttons.count() == 0)
    {
        emit error("l_buttons.count() == 0");
        return;
    }

    double w_value = -1;
    double h_value = -1;
    double l_border = -1;
    double u_border = -1;

    l_buttons.at(0)->get_size(&w_value, &h_value, &l_border, &u_border);

    emit debug(QString("w_value %1").arg(w_value));
    emit debug(QString("h_value %1").arg(h_value));
    emit debug(QString("l_border %1").arg(l_border));
    emit debug(QString("u_border %1").arg(u_border));
}
//--------------------------------------------------------------------------------
void RGB_display::set_default(void)
{
    emit trace(Q_FUNC_INFO);

    max_x = SCREEN_WIDTH;
    max_y = SCREEN_HEIGTH;
    w_led = LED_SIZE_W_MM;
    h_led = LED_SIZE_H_MM;
    left_border = LED_BORDER_W_MM;
    up_border = LED_BORDER_H_MM;

    create_new_display();
}
//--------------------------------------------------------------------------------
bool RGB_display::load_ico(void)
{
    return load_picture(":/mainwindow/computer.png");
}
//--------------------------------------------------------------------------------
bool RGB_display::load_pic(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName.isEmpty())
    {
        emit error("Файл не выбран");
        return false;
    }

    return load_picture(fileName);
}
//--------------------------------------------------------------------------------
bool RGB_display::load_picture(QString fileName)
{
    if(fileName.isEmpty())
    {
        emit error("fileName is empty");
        return false;
    }

    emit info(QString("load %1").arg(fileName));
    bool ok = picture.load(fileName);
    if(!ok)
    {
        emit error("can't load picture");
        return false;
    }

    //    max_x = sb_max_x->value();
    //    max_y = sb_max_y->value();

    emit debug(QString("max_x %1").arg(max_x));
    emit debug(QString("max_y %1").arg(max_y));

    if(max_x <= 0)  return false;
    if(max_y <= 0)  return false;
    if(max_x > MAX_SCREEN_X)    return false;
    if(max_y > MAX_SCREEN_Y)    return false;

    show_picture(0, 0);
    return true;
}
//--------------------------------------------------------------------------------
void RGB_display::show_picture(int begin_x, int begin_y)
{
    PACKET packet;

    if(picture.isNull())
    {
        emit error("picture not loaded!");
        return;
    }

    packet.body.brightness = static_cast<uint8_t>(brightness);

    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
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

                    LED led;
                    led.color_R = static_cast<uint8_t>(qRed(color));
                    led.color_G = static_cast<uint8_t>(qGreen(color));
                    led.color_B = static_cast<uint8_t>(qBlue(color));
                    packet.body.leds[x][y] = led;
                    break;
                }
            }
        }
    }

    QString packet_str;
    packet_str.append(":");
    for(unsigned int n=0; n<sizeof(PACKET); n++)
    {
        QString temp;
        temp = QString("%1").arg(packet.buf[n], 2, 16, QChar('0')).toUpper();
        packet_str.append(temp);
    }
    packet_str.append("\n");

    emit debug(QString("len %1").arg(packet_str.length()));
    emit send(packet_str);
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
    w_led = settings->value("w_led").toDouble();
    h_led = settings->value("h_led").toDouble();
    left_border = settings->value("left_border").toDouble();
    up_border = settings->value("up_border").toDouble();
    brightness = settings->value("brightness").toInt();
    QByteArray ba_display = settings->value("ba_display").toByteArray();
    settings->endGroup();

    if(w_led == 0.0)        w_led = LED_DEFAULT_W;
    if(h_led == 0.0)        h_led = LED_DEFAULT_H;
    if(left_border == 0.0)  left_border = LED_BORDER_W_MM;
    if(up_border == 0.0)    up_border = LED_BORDER_H_MM;

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
        ba_display.append(static_cast<char>(led->get_R()));
        ba_display.append(static_cast<char>(led->get_G()));
        ba_display.append(static_cast<char>(led->get_B()));
    }

    settings->beginGroup("Display");
    settings->setValue("w_led", w_led);
    settings->setValue("h_led", h_led);
    settings->setValue("left_border", left_border);
    settings->setValue("up_border", up_border);
    settings->setValue("brightness", brightness);
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
int RGB_display::get_picture_w(void)
{
    return picture.width();
}
//--------------------------------------------------------------------------------
int RGB_display::get_picture_h(void)
{
    return picture.height();
}
//--------------------------------------------------------------------------------
void RGB_display::send_test_data(void)
{
    PACKET packet;

    packet.body.brightness = static_cast<uint8_t>(brightness);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            foreach(RGB_dislpay_led *led, l_buttons)
            {
                int p_x = led->property("property_col").toInt();
                int p_y = led->property("property_row").toInt();
                if((p_x == x) && (p_y == y))
                {
                    LED temp_l;
                    temp_l.color_R = led->get_R();
                    temp_l.color_G = led->get_G();
                    temp_l.color_B = led->get_B();

                    packet.body.leds[x][y] = temp_l;
                }
            }
        }
    }

    QString packet_str;
    packet_str.append(":");
    for(unsigned int n=0; n<sizeof(PACKET); n++)
    {
        QString temp;
        temp = QString("%1").arg(packet.buf[n], 2, 16, QChar('0')).toUpper();
        packet_str.append(temp);
    }
    packet_str.append("\n");

    //emit debug(packet_str);
    emit debug(QString("len %1").arg(packet_str.length()));

    emit send(packet_str);
}
//--------------------------------------------------------------------------------
bool RGB_display::set_brightness(int value)
{
    if(value < 0)
    {
        return false;
    }
    brightness = value;
    return true;
}
//--------------------------------------------------------------------------------
void RGB_display::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool RGB_display::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void RGB_display::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void RGB_display::save_setting(void)
{

}
//--------------------------------------------------------------------------------
