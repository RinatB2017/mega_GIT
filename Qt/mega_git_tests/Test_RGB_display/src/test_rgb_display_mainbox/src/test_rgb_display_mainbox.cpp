/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QImage>
//--------------------------------------------------------------------------------
#include "ui_test_rgb_display_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_rgb_display_mainbox.hpp"
#include "rgb_display_led.hpp"
#include "rgb_display.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    ui->sb_max_x->setRange(1, 128);
    ui->sb_max_y->setRange(1, 64);
    ui->sb_led_width->setRange(1.0, 10.0);
    ui->sb_led_height->setRange(1.0, 10.0);
    ui->sb_led_up_border->setRange(0.1, 10.0);
    ui->sb_led_left_border->setRange(0.1, 10.8);

    ui->sb_brightness->setRange(1, 0xFF);

    ui->serial_widget->set_fix_baudrate(9600);

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,   SLOT(read_data(QByteArray)));

    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_u,  &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_d,  &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_l,  &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_r,  &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_ul, &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_ur, &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_dl, &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_dr, &QToolButton::setEnabled);

    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_send_test_data, &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_load_pic,       &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->btn_load_ico,       &QToolButton::setEnabled);
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::port_is_active,   ui->sb_brightness,      &QToolButton::setEnabled);

    connect(ui->btn_ul,         &QToolButton::clicked,  this,   &MainBox::move_ul);
    connect(ui->btn_u,          &QToolButton::clicked,  this,   &MainBox::move_u);
    connect(ui->btn_ur,         &QToolButton::clicked,  this,   &MainBox::move_ur);

    connect(ui->btn_l,          &QToolButton::clicked,  this,   &MainBox::move_l);
    connect(ui->btn_r,          &QToolButton::clicked,  this,   &MainBox::move_r);

    connect(ui->btn_dl,         &QToolButton::clicked,  this,   &MainBox::move_dl);
    connect(ui->btn_d,          &QToolButton::clicked,  this,   &MainBox::move_d);
    connect(ui->btn_dr,         &QToolButton::clicked,  this,   &MainBox::move_dr);

    connect(ui->btn_load_ico,   &QPushButton::clicked,  ui->rgb_display,   &RGB_display::load_ico);
    connect(ui->btn_load_pic,   &QPushButton::clicked,  ui->rgb_display,   &RGB_display::load_pic);

    connect(this,               &MainBox::send,
            ui->serial_widget,  static_cast<int (SerialBox5_fix_baudrate::*)(const QByteArray&)>(&SerialBox5_fix_baudrate::input));

    connect(ui->serial_widget, &SerialBox5_fix_baudrate::output,  this,   &MainBox::get_data);

    connect(ui->rgb_display,    &RGB_display::send,     this,   &MainBox::send_data);

    connect(ui->btn_send_test_data, &QPushButton::clicked,  this,   &MainBox::send_test_data);

    connect(ui->btn_get,        &QPushButton::clicked,  this,   &MainBox::get_param);
    connect(ui->btn_set,        &QPushButton::clicked,  this,   &MainBox::set_param);
    connect(ui->btn_default,    &QPushButton::clicked,  this,   &MainBox::set_default);

    //setFixedSize(sizeHint());

    QTimer::singleShot(0, [this]{
        load_widgets();
    });
    QTimer::singleShot(100, [this]{
        set_param();
    });
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test 0", &MainBox::test_0 });
        commands.append({ id++, "test 1", &MainBox::test_1 });
        commands.append({ id++, "test 2", &MainBox::test_2 });
        commands.append({ id++, "test 3", &MainBox::test_3 });
        commands.append({ id++, "test 4", &MainBox::test_4 });
        commands.append({ id++, "test 5", &MainBox::test_5 });

        QToolBar *testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, commands)
        {
            cb_test->addItem(command.cmd_text, QVariant(command.cmd));
        }

        testbar->addWidget(cb_test);
        QToolButton *btn_choice_test = add_button(testbar,
                                                  new QToolButton(this),
                                                  qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                  "choice_test",
                                                  "choice_test");
        btn_choice_test->setObjectName("btn_choice_test");

        connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::prepare_data(QByteArray input, QByteArray *output)
{
    Q_ASSERT(output);

    if(input.isEmpty())
    {
        emit error("Input is empty!");
        return false;
    }
    output->clear();

    output->append(':');
    output->append(input.toHex().toUpper());
    output->append('\n');

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray data)
{
    emit trace("read_data");
#if 0
    emit debug(data.data());
#else
    Q_UNUSED(data);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::move_ul(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_x > 0)
    {
        begin_x--;
    }
    if(begin_y > 0)
    {
        begin_y--;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_u(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_y > 0)
    {
        begin_y--;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_ur(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_x < ui->rgb_display->get_max_x())  && ((begin_x + ui->rgb_display->get_max_x()) < ui->rgb_display->get_picture_w()))
    {
        begin_x++;
    }
    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_l(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_x > 0)
    {
        begin_x--;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_r(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_x < ui->rgb_display->get_max_x()) && ((begin_x + ui->rgb_display->get_max_x()) < ui->rgb_display->get_picture_w()))
    {
        begin_x++;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_dl(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_x > 0)
    {
        begin_x--;
    }
    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_d(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_dr(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_x < ui->rgb_display->get_max_x())  && ((begin_x + ui->rgb_display->get_max_x()) < ui->rgb_display->get_picture_w()))
    {
        begin_x++;
    }
    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::get_data(QByteArray data)
{
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::send_data(QString data)
{
    emit debug(data);
    emit send(data.toLatin1());
}
//--------------------------------------------------------------------------------
void MainBox::send_test_data(void)
{
    ui->rgb_display->set_brightness(ui->sb_brightness->value());
    ui->rgb_display->send_test_data();
}
//--------------------------------------------------------------------------------
void MainBox::set_param(void)
{
    int cnt_led_x = ui->sb_max_x->value();
    int cnt_led_y = ui->sb_max_y->value();
    double width_led = ui->sb_led_width->value();
    double height_led = ui->sb_led_height->value();
    double l_border = ui->sb_led_left_border->value();
    double u_border = ui->sb_led_up_border->value();

    bool ok = ui->rgb_display->set_param(cnt_led_x,
                                         cnt_led_y,
                                         width_led,
                                         height_led,
                                         l_border,
                                         u_border);
    if(!ok)
    {
        emit error("bad param");
    }
}
//--------------------------------------------------------------------------------
void MainBox::get_param(void)
{
    int cnt_led_x = 0;
    int cnt_led_y = 0;
    double width_led = 0;
    double height_led = 0;
    double l_border = 0;
    double u_border = 0;

    bool ok = ui->rgb_display->get_param(&cnt_led_x,
                                         &cnt_led_y,
                                         &width_led,
                                         &height_led,
                                         &l_border,
                                         &u_border);
    if(!ok)
    {
        return;
    }

    ui->sb_max_x->setValue(cnt_led_x);
    ui->sb_max_y->setValue(cnt_led_y);
    ui->sb_led_width->setValue(width_led);
    ui->sb_led_height->setValue(height_led);
    ui->sb_led_left_border->setValue(l_border);
    ui->sb_led_up_border->setValue(u_border);
}
//--------------------------------------------------------------------------------
void MainBox::set_default(void)
{
    ui->sb_max_x->setValue(SCREEN_WIDTH);
    ui->sb_max_y->setValue(SCREEN_HEIGTH);
    ui->sb_led_width->setValue(LED_SIZE_W_MM);
    ui->sb_led_height->setValue(LED_SIZE_H_MM);
    ui->sb_led_left_border->setValue(LED_BORDER_W_MM);
    ui->sb_led_up_border->setValue(LED_BORDER_H_MM);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
