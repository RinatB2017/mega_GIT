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

}
//--------------------------------------------------------------------------------
void RGB_display::init(void)
{
    w_led = LED_SIZE_W_MM;
    h_led = LED_SIZE_H_MM;

    left_border = LED_BORDER_W_MM;
    up_border   = LED_BORDER_H_MM;

    grid = new QGridLayout();
    grid->setMargin(1);
    grid->setSpacing(1);

    max_x = SCREEN_WIDTH;
    max_y = SCREEN_HEIGTH;

    sb_max_x = new QSpinBox(this);
    sb_max_y = new QSpinBox(this);
    dsb_led_width = new QDoubleSpinBox(this);
    dsb_led_height = new QDoubleSpinBox(this);
    dsb_up_border = new QDoubleSpinBox(this);
    dsb_left_border = new QDoubleSpinBox(this);

    sb_max_x->setObjectName("sb_max_x");
    sb_max_y->setObjectName("sb_max_y");
    dsb_led_width->setObjectName("dsb_led_width");
    dsb_led_height->setObjectName("dsb_led_height");
    dsb_up_border->setObjectName("dsb_up_border");
    dsb_left_border->setObjectName("dsb_left_border");

    sb_max_x->setRange(1, 64);
    sb_max_y->setRange(1, 32);
    dsb_led_width->setRange(1.0, 10.0);
    dsb_led_height->setRange(1.0, 10.0);
    dsb_up_border->setRange(0.1, 10.0);
    dsb_left_border->setRange(0.1, 10.8);

    sb_max_x->setValue(max_x);
    sb_max_y->setValue(max_y);
    dsb_led_width->setValue(w_led);
    dsb_led_height->setValue(h_led);
    dsb_up_border->setValue(up_border);
    dsb_left_border->setValue(left_border);

    create_new_display();

    QGridLayout *grid_buttons = new QGridLayout;
    grid_buttons->addWidget(new QLabel("max_x"),        0, 0);    grid_buttons->addWidget(sb_max_x,           0, 1);
    grid_buttons->addWidget(new QLabel("max_y"),        1, 0);    grid_buttons->addWidget(sb_max_y,           1, 1);
    grid_buttons->addWidget(new QLabel("led_width"),    2, 0);    grid_buttons->addWidget(dsb_led_width,      2, 1);
    grid_buttons->addWidget(new QLabel("led_height"),   3, 0);    grid_buttons->addWidget(dsb_led_height,     3, 1);
    grid_buttons->addWidget(new QLabel("up_border"),    4, 0);    grid_buttons->addWidget(dsb_up_border,      4, 1);
    grid_buttons->addWidget(new QLabel("left_border"),  5, 0);    grid_buttons->addWidget(dsb_left_border,    5, 1);

    QPushButton *btn_set = new QPushButton(this);
    QPushButton *btn_get = new QPushButton(this);
    QPushButton *btn_default = new QPushButton(this);

    btn_set->setText("set");
    btn_get->setText("get");
    btn_default->setText("default");

    connect(btn_set,        SIGNAL(clicked(bool)),  this,   SLOT(set_display()));
    connect(btn_get,        SIGNAL(clicked(bool)),  this,   SLOT(get_display()));
    connect(btn_default,    SIGNAL(clicked(bool)),  this,   SLOT(set_default()));

    QHBoxLayout *btn_l = new QHBoxLayout();
    btn_l->addWidget(btn_get);
    btn_l->addWidget(btn_set);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid_buttons);
    vbox->addLayout(btn_l);
    vbox->addWidget(btn_default);
    vbox->addStretch(1);
    //---

    QVBoxLayout *vbox2 = new QVBoxLayout();
    vbox2->addLayout(grid);
    vbox2->addStretch(1);

    QFrame *frame = new QFrame(this);
    frame->setFrameStyle(QFrame::Box | QFrame::Raised);
    frame->setLayout(vbox);
    frame->setFixedWidth(frame->sizeHint().width());

    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(frame);
    box->addLayout(vbox2);

    setLayout(box);

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
    double led_width = dsb_led_width->value();
    double led_height = dsb_led_height->value();
    double up_border = dsb_up_border->value();
    double left_border = dsb_left_border->value();

    emit debug(QString("led_width %1").arg(led_width));
    emit debug(QString("led_height %1").arg(led_height));
    emit debug(QString("up_border %1").arg(up_border));
    emit debug(QString("left_border %1").arg(left_border));

    for(int row=0; row<max_y; row++)
    {
        for(int col=0; col<max_x; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->set_size(led_width,
                          led_height,
                          left_border,
                          up_border);

            led->setProperty("property_col", col);
            led->setProperty("property_row", row);

            grid->addWidget(led, row, col);

            l_buttons.append(led);
        }
    }
}
//--------------------------------------------------------------------------------
void RGB_display::set_display(void)
{
    emit trace(Q_FUNC_INFO);

    max_x = sb_max_x->value();
    max_y = sb_max_y->value();

    double led_width = dsb_led_width->value();
    double led_height = dsb_led_height->value();
    double up_border = dsb_up_border->value();
    double left_border = dsb_left_border->value();

    emit debug(QString("led_width %1").arg(led_width));
    emit debug(QString("led_height %1").arg(led_height));
    emit debug(QString("up_border %1").arg(up_border));
    emit debug(QString("left_border %1").arg(left_border));

    foreach(RGB_dislpay_led *led, l_buttons)
    {
        led->set_size(led_width,
                      led_height,
                      left_border,
                      up_border);
    }
    //create_new_display();

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

    dsb_led_width->setValue(w_value);
    dsb_led_height->setValue(h_value);
    dsb_left_border->setValue(l_border);
    dsb_up_border->setValue(u_border);
}
//--------------------------------------------------------------------------------
void RGB_display::set_default(void)
{
    sb_max_x->setValue(SCREEN_WIDTH);
    sb_max_y->setValue(SCREEN_HEIGTH);
    dsb_led_width->setValue(LED_SIZE_W_MM);
    dsb_led_height->setValue(LED_SIZE_H_MM);
    dsb_left_border->setValue(LED_BORDER_W_MM);
    dsb_up_border->setValue(LED_BORDER_H_MM);
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

    max_x = sb_max_x->value();
    max_y = sb_max_y->value();

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

    int max_x = sb_max_x->value();
    int max_y = sb_max_y->value();

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
        ba_display.append(static_cast<char>(led->get_R()));
        ba_display.append(static_cast<char>(led->get_G()));
        ba_display.append(static_cast<char>(led->get_B()));
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
