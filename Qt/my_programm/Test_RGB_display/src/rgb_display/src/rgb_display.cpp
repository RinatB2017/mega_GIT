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
    //setFixedSize(width(), height());
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

    double left_border = pixelPerMm * LED_BORDER_W_MM;
    double up_border = pixelPerMm * LED_BORDER_H_MM;

    grid = new QGridLayout();
    grid->setMargin(LED_BORDER_SIZE + 0.5);
    grid->setSpacing(LED_BORDER_SIZE + 0.5);

    for(int row=0; row<SCREEN_HEIGTH; row++)
    {
        for(int col=0; col<SCREEN_WIDTH; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->set_size(w_led + left_border * 2,
                          h_led + up_border * 2,
                          left_border,
                          up_border);

            led->setProperty("property_col", col);
            led->setProperty("property_row", row);

            grid->addWidget(led, row, col);

            l_buttons.append(led);
        }
    }

    //---
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
    dsb_led_width->setValue(w_led / pixelPerMm);
    dsb_led_height->setValue(h_led / pixelPerMm);
    dsb_up_border->setValue(up_border / pixelPerMm);
    dsb_left_border->setValue(left_border / pixelPerMm);

    QGridLayout *grid_buttons = new QGridLayout;
    grid_buttons->addWidget(new QLabel("max_x"),        0, 0);    grid_buttons->addWidget(sb_max_x,           0, 1);
    grid_buttons->addWidget(new QLabel("max_y"),        1, 0);    grid_buttons->addWidget(sb_max_y,           1, 1);
    grid_buttons->addWidget(new QLabel("led_width"),    2, 0);    grid_buttons->addWidget(dsb_led_width,      2, 1);
    grid_buttons->addWidget(new QLabel("led_height"),   3, 0);    grid_buttons->addWidget(dsb_led_height,     3, 1);
    grid_buttons->addWidget(new QLabel("up_border"),    4, 0);    grid_buttons->addWidget(dsb_up_border,      4, 1);
    grid_buttons->addWidget(new QLabel("left_border"),  5, 0);    grid_buttons->addWidget(dsb_left_border,    5, 1);

    QPushButton *btn = new QPushButton(this);
    btn->setText("redraw");
    connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(redraw_display()));

    grid_buttons->addWidget(btn,    6,  0,  1, 2);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(grid_buttons);
    vbox->addStretch(1);
    //---

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addLayout(grid);
    vbox2->addStretch(1);

    QHBoxLayout *box = new QHBoxLayout;
    box->addLayout(vbox);
    box->addLayout(vbox2);

    setLayout(box);
}
//--------------------------------------------------------------------------------
void RGB_display::redraw_display(void)
{
    emit trace(Q_FUNC_INFO);

    int max_x = sb_max_x->value();
    int max_y = sb_max_y->value();
    double led_width = dsb_led_width->value();
    double led_height = dsb_led_height->value();
    double up_border = dsb_up_border->value();
    double left_border = dsb_left_border->value();

    emit info(QString("row %1").arg(grid->rowCount()));
    emit info(QString("col %1").arg(grid->columnCount()));

    emit info(QString("count %1").arg(grid->count()));

    max_x = grid->columnCount();
    max_y = grid->rowCount();
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
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

    max_x = sb_max_x->value();
    max_y = sb_max_y->value();

    double pixelPerMm = QApplication::screens().at(0)->logicalDotsPerInch()/2.54/10;
    led_width *= pixelPerMm;
    led_height *= pixelPerMm;
    left_border *= pixelPerMm;
    up_border *= pixelPerMm;

    for(int row=0; row<max_y; row++)
    {
        for(int col=0; col<max_x; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->set_size(led_width + left_border * 2,
                          led_height + up_border * 2,
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
bool RGB_display::load_pic(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName.isEmpty())
    {
        emit info("Файл не выбран");
        return false;
    }

    bool ok = picture.load(fileName);
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
