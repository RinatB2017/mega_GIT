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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QImage>
//--------------------------------------------------------------------------------
#include "ui_test_rgb_display_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_rgb_display_mainbox.hpp"
#include "rgb_dislpay_led.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    create_display();

    connect(this,   SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,   SLOT(read_data(QByteArray)));

    connect(ui->btn_load,   SIGNAL(clicked(bool)),  this,   SLOT(load_ico()));

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

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

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    mw->add_windowsmenu_action(testbar->toggleViewAction());    //TODO странно
}
//--------------------------------------------------------------------------------
void MainBox::create_display(void)
{
    ui->grid->setSpacing(0);
    ui->grid->setMargin(0);

    double pixelPerMm = QApplication::screens().at(0)->logicalDotsPerInch()/2.54/10;
    w_led = pixelPerMm * 3.5;    // Ширина 3.5 mm
    h_led = pixelPerMm * 3.5;    // Высота 3.5 mm

    for(int row=0; row<SCREEN_HEIGTH; row++)
    {
        for(int col=0; col<SCREEN_WIDTH; col++)
        {
            RGB_dislpay_led *led = new RGB_dislpay_led(w_led, h_led, this);
            connect(led,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
            connect(led,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
            connect(led,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
            connect(led,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

            led->setProperty("property_col", col);
            led->setProperty("property_row", row);

            ui->grid->addWidget(led, row, col);

            l_buttons.append(led);
        }
    }

#if 1
    int state = 0;
    foreach(RGB_dislpay_led *led, l_buttons)
    {
        switch(state)
        {
        case 0:
        case 3:
        case 6:
            led->set_R(255);
            led->set_G(0);
            led->set_B(0);
            led->repaint();
            break;

        case 1:
        case 4:
        case 7:
            led->set_R(0);
            led->set_G(255);
            led->set_B(0);
            led->repaint();
            break;

        case 2:
        case 5:
        case 8:
            led->set_R(0);
            led->set_G(0);
            led->set_B(255);
            led->repaint();
            break;

        default:
            state = 0;
            led->set_R(255);
            led->set_G(0);
            led->set_B(0);
            led->repaint();
            break;
        }
        state++;
    }
#endif
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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_ico(void)
{
    emit info("Test_0()");

    QImage picture;
    bool ok = picture.load(":/mainwindow/computer.png");
    if(!ok)
    {
        emit error("can't load picture");
        return;
    }

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
}
//--------------------------------------------------------------------------------
bool MainBox::prepare_data(QByteArray input, QByteArray *output)
{
    Q_CHECK_PTR(output);

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
    emit debug(data.data());
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    emit info(QString("header %1").arg(sizeof(P_HEADER)));
    emit info(QString("data %1").arg(sizeof(P_DATA)));

    P_HEADER header;
    header.addr = 0;
    header.cmd = CMD_01;
    header.len = sizeof(P_DATA);

    P_DATA data;
    //data.brightness = 128;
    int n = 0;
    foreach (RGB_dislpay_led *led, l_buttons)
    {
        //int p_x = led->property("property_col").toInt();
        //int p_y = led->property("property_row").toInt();

        LED p_led;
        p_led.color_R = led->get_R();
        p_led.color_G = led->get_G();
        p_led.color_B = led->get_B();

        //data.leds[p_x][p_y] = p_led;

        data.leds[n++] = p_led;
        if(n > NUM_LEDS)
        {
            break;
        }
    }

#if 0
    for(unsigned int n=0; n<sizeof(P_DATA); n++)
    {
        emit debug(QString("n RGB: %1 %2 %3 %4")
                   .arg(n)
                   .arg(data.leds[n].color_R)
                   .arg(data.leds[n].color_G)
                   .arg(data.leds[n].color_B));
    }
#endif

    QByteArray input;
    input.append((char *)&header, sizeof(P_HEADER));
    input.append((char *)&data,   sizeof(P_DATA));

    QByteArray output;

    bool ok = prepare_data(input, &output);
    if(!ok)
    {
        return false;
    }

    emit debug(output.data());
    emit send(output);

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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
