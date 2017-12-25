/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QSpinBox>
#include <QTimer>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "serialbox5_lite.hpp"
//--------------------------------------------------------------------------------
#include "palette.hpp"
#include "display.hpp"
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
    main_serialBox->deleteLater();
    control_serialBox->deleteLater();

    display->deleteLater();
    control_display->deleteLater();
    palette->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    pos_x = 0;

    createTestBar();
    createSerialBox();
    createDisplayBox();
    createTimer();

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    if(mw == nullptr)
    {
        return;
    }

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    sb_interval = new QSpinBox(this);
    sb_interval->setMinimum(100);
    sb_interval->setMaximum(60000);

    toolBar->addWidget(new QLabel("interval in msec "));
    toolBar->addWidget(sb_interval);
    QToolButton *btn_run = add_button(toolBar,
                                      new QToolButton(this),
                                      qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                      "run",
                                      "run");
    QToolButton *btn_update = add_button(toolBar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                         "update",
                                         "update");
    if(btn_run)
    {
        btn_run->setCheckable(true);
        connect(btn_run,    SIGNAL(toggled(bool)),  this,   SLOT(run(bool)));
        connect(btn_update, SIGNAL(clicked(bool)),  this,   SLOT(update()));
    }
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBox(void)
{
    main_serialBox = new SerialBox5_lite(this);
    main_serialBox->add_menu(2);

    control_serialBox = new SerialBox5_lite(this);

    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(main_serialBox);
    vbox1->addStretch(1);

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(control_serialBox);
    vbox2->addStretch(1);

    ui->gridLayout->addLayout(vbox1,    0, 0);
    ui->gridLayout->addLayout(vbox2,    1, 0);

    connect(this,           SIGNAL(send(QByteArray)),   main_serialBox, SLOT(input(QByteArray)));
    connect(main_serialBox, SIGNAL(output(QByteArray)), this,           SLOT(read_data(QByteArray)));

    connect(control_serialBox, SIGNAL(output(QByteArray)), this,   SLOT(read_display_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createDisplayBox(void)
{
    display = new Display(MAX_SCREEN_X, MAX_SCREEN_Y, this);
    display->setObjectName("display");
    display->load_setting();
    display->set_left_btn_active(true);
    display->set_right_btn_active(false);

    control_display = new Display(NUM_LEDS_PER_STRIP, NUM_STRIPS, this);
    control_display->setObjectName("control_display");
    control_display->load_setting();
    control_display->set_left_btn_active(false);
    control_display->set_right_btn_active(false);

    //clear control_display
    control_display->clear();
    //---

    palette = new MyPalette(4, 4, this);
    palette->setObjectName("MyPalette");
    palette->load_setting();
    palette->set_left_btn_active(true);
    palette->set_right_btn_active(true);
    palette->setTitle("Палитра");
    connect(palette,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(palette,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(palette,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(palette,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(palette);
    hbox->addWidget(display);
    hbox->addStretch(1);

    ui->gridLayout->addLayout(hbox,             0,  1);
    ui->gridLayout->addWidget(control_display,  1,  1);
}
//--------------------------------------------------------------------------------
#if 0
    const char *buf[MAX_SCREEN_Y] = {
        ".............................R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.R",
        "RRRR.GGG..B..B.RRR..GGGG.BBB..R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.",
        "R..R.G..G.B.BB.R..R.G.....B..R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.R",
        "R..R.GGG..BB.B.RRR..GGGG..B...R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.",
        "R..R.G....B..B.R..R.G.....B..R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.R",
        "R..R.G....B..B.R..R.G.....B...R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.",
        "R..R.G....B..B.RRR..GGGG..B..R.R.R.R.R.R.R.R.R.R.R.R.R.R.R.R",
        "..............................R.R.R.R.R.R.R.R.R.R.R.R.R.R.R."
    };
    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            switch(buf[y][x])
            {
            case 'R':
                display->set_color(x, y, 255, 0, 0);
                break;

            case 'G':
                display->set_color(x, y, 0, 255, 0);
                break;

            case 'B':
                display->set_color(x, y, 0, 0, 255);
                break;

            default:
                break;
            }

        }
    }
#endif
//--------------------------------------------------------------------------------
void MainBox::createTimer(void)
{
    timer = new QTimer(this);
    connect(timer,  SIGNAL(timeout()),  this,   SLOT(update()));
    if(main_serialBox)
    {
        connect(main_serialBox,  SIGNAL(not_working()), timer,  SLOT(stop()));
    }
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
    if(is_busy)
    {
        return;
    }
    is_busy = true;

    CMD_0x01_QUESTION question;
    question.body.header.address = 1;
    question.body.header.cmd = RGB_CMD_0x01;
    question.body.header.count_data = sizeof(question.body.data);

    //question.body.brightness = 128; //TODO brightness

    uint8_t value_R = 0;
    uint8_t value_G = 0;
    uint8_t value_B = 0;

    for(int y=0; y<NUM_STRIPS; y++)
    {
        for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
        {
            int temp_x = x + pos_x;
            if(temp_x >= MAX_SCREEN_X)
            {
                temp_x -= MAX_SCREEN_X;
            }
            display->get_R(temp_x, y, &value_R);

            display->get_R(temp_x, y, &value_R);
            display->get_G(temp_x, y, &value_G);
            display->get_B(temp_x, y, &value_B);

            question.body.data[x][y].R = value_R;
            question.body.data[x][y].G = value_G;
            question.body.data[x][y].B = value_B;
        }
    }

    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

#ifdef MODBUS
    QByteArray ba;
    ba.clear();
    ba.append(":");
    for(unsigned int n=0; n<sizeof(CMD_0x01_QUESTION); n++)
    {
        ba.append(QString("%1")
                  .arg(question.buf[n], 2, 16, QChar('0'))
                  .toUpper());
    }
    ba.append(0x0D);
#else
    QByteArray ba;
    for(unsigned int n=0; n<sizeof(CMD_0x01_QUESTION); n++)
    {
        ba.append(question.buf[n]);
    }
#endif

#if 0
    for(int y=0; y<NUM_STRIPS; y++)
    {
        for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
        {
            control_display->set_color(x, y,
                                       question.body.data[x][y].R,
                                       question.body.data[x][y].G,
                                       question.body.data[x][y].B);
        }
    }
#endif

#ifdef FAKE
    emit trace(QString("[%1]").arg(ba.data()));
    emit info(QString("send %1 bytes").arg(ba.size()));
#else

    data_rs232.clear();
    is_ready = false;
    emit send(ba);

    emit trace(ba);
    emit debug(QString("send %1 bytes").arg(ba.size()));

    wait(1000);

    if(is_ready == false)
    {
        emit error("data is empty!");
    }
    else
    {
        pos_x++;
        if(pos_x >= MAX_SCREEN_X)
        {
            pos_x = 0;
        }
    }
#endif

    is_busy = false;
}
//--------------------------------------------------------------------------------
void MainBox::run(bool state)
{
    if(state)
    {
        emit info(tr("timer ON"));
        timer->start(sb_interval->value());
    }
    else
    {
        emit info(tr("timer OFF"));
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        if(is_ready)
        {
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_display_data(QByteArray ba)
{
    //emit debug("read_display_data");
    emit debug(ba);

    QByteArray data;

    for(int n=0; n<ba.length(); n++)
    {
        char s = ba.at(n);
        switch (s) {
        case ':':
            display_data_rs232.clear();
            break;

        case '\r':
        case '\n':
            data = QByteArray::fromHex(display_data_rs232);
            if(data.length() == 5)
            {
                control_display->set_color((int)data.at(0),
                                           (int)data.at(1),
                                           (uint8_t)data.at(2),
                                           (uint8_t)data.at(3),
                                           (uint8_t)data.at(4));
            }
            else
            {
                emit error(QString("len = %1").arg(data.length()));
            }
            break;

        default:
            display_data_rs232.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    //emit debug("read_data");
    emit debug(ba);

    for(int n=0; n<ba.length(); n++)
    {
        char s = ba.at(n);
        switch (s) {
        case ':':
            data_rs232.clear();
            break;

        case '\r':
        case '\n':
            is_ready = true;
            break;

        default:
            data_rs232.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
