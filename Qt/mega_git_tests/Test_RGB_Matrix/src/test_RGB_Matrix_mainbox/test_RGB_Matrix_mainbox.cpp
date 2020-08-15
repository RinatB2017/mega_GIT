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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_RGB_Matrix_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_RGB_Matrix_mainbox.hpp"
#include "defines.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "serialbox5_lite.hpp"
//--------------------------------------------------------------------------------
#include "palette.hpp"
#include "display.hpp"
//--------------------------------------------------------------------------------
//#include "font-5x7.hpp"
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
    if(main_serialBox) main_serialBox->deleteLater();
    if(control_serialBox) control_serialBox->deleteLater();

    if(display) display->deleteLater();
    if(control_display) control_display->deleteLater();
    if(palette) palette->deleteLater();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    pos_x = 0;

    createSerialBox();
    createDisplayBox();
    createDockWidgets();

    createTimer();

    setFixedSize(0, 0);
    //setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    sb_interval = new QSpinBox(this);
    sb_interval->setMinimum(100);
    sb_interval->setMaximum(60000);

    testbar->addWidget(new QLabel("interval in msec "));
    testbar->addWidget(sb_interval);
    QToolButton *btn_run = add_button(testbar,
                                      new QToolButton(this),
                                      qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                      "run",
                                      "run");
    QToolButton *btn_update = add_button(testbar,
                                         new QToolButton(this),
                                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                         "update",
                                         "update");
    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon(),
                                       "test",
                                       "test");
    Q_CHECK_PTR(btn_run);
    Q_CHECK_PTR(btn_update);
    

    btn_run->setCheckable(true);
    connect(btn_run,    SIGNAL(toggled(bool)),  this,   SLOT(run(bool)));
    connect(btn_update, SIGNAL(clicked(bool)),  this,   SLOT(update()));
    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBox(void)
{
    main_serialBox = new SerialBox5_lite(this);
    main_serialBox->set_caption("RS232(1)");
    main_serialBox->add_menu(2);

    control_serialBox = new SerialBox5_lite(this);
    control_serialBox->set_caption("RS232(2)");
    control_serialBox->add_menu(3);

    connect(this,           SIGNAL(send(QByteArray)),   main_serialBox, SLOT(input(QByteArray)));
    connect(main_serialBox, SIGNAL(output(QByteArray)), this,           SLOT(read_data(QByteArray)));

    connect(control_serialBox, SIGNAL(output(QByteArray)), this,   SLOT(read_display_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createDisplayBox(void)
{
    display = new Display(MAX_SCREEN_X, MAX_SCREEN_Y, LED_SIZE, LED_SIZE, this);
    display->setObjectName("display");
    display->set_left_btn_active(true);
    display->set_right_btn_active(false);

    control_display = new Display(NUM_LEDS_PER_STRIP, NUM_STRIPS, LED_SIZE, LED_SIZE, this);
    control_display->setObjectName("control_display");
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
}
//--------------------------------------------------------------------------------
void MainBox::createDockWidgets(void)
{
    QHBoxLayout *hbox_serial1 = new QHBoxLayout();
    hbox_serial1->addWidget(main_serialBox);
    hbox_serial1->addStretch(1);

    QHBoxLayout *hbox_serial2 = new QHBoxLayout();
    hbox_serial2->addWidget(control_serialBox);
    hbox_serial2->addStretch(1);

    QHBoxLayout *hbox_display = new QHBoxLayout();
    hbox_display->addWidget(palette);
    hbox_display->addWidget(display);
    hbox_display->addStretch(1);

    QVBoxLayout *vbox1 = new QVBoxLayout();
    vbox1->addLayout(hbox_serial1);
    vbox1->addLayout(hbox_display);

    QVBoxLayout *vbox2 = new QVBoxLayout();
    vbox2->addLayout(hbox_serial2);
    vbox2->addWidget(control_display);

    QWidget *w1 = new QWidget(this);
    w1->setLayout(vbox1);

    QWidget *w2 = new QWidget(this);
    w2->setLayout(vbox2);

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    mw->add_dock_widget("Main",     "main_control",     Qt::LeftDockWidgetArea,  reinterpret_cast<QWidget *>(w1));
    mw->add_dock_widget("Control",  "control_control",  Qt::RightDockWidgetArea, reinterpret_cast<QWidget *>(w2));
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

    question.body.brightness = 128; //TODO brightness

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
            display->get_R(static_cast<unsigned int>(temp_x), static_cast<unsigned int>(y), &value_R);

            display->get_R(static_cast<unsigned int>(temp_x), static_cast<unsigned int>(y), &value_R);
            display->get_G(static_cast<unsigned int>(temp_x), static_cast<unsigned int>(y), &value_G);
            display->get_B(static_cast<unsigned int>(temp_x), static_cast<unsigned int>(y), &value_B);

            question.body.data[x][y].R = value_R;
            question.body.data[x][y].G = value_G;
            question.body.data[x][y].B = value_B;
        }
    }

    question.body.crc16 = CRC::crc16(reinterpret_cast<uint8_t *>(&question.buf), sizeof(question) - 2);

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

    emit debug(QString("[%1]").arg(ba.data()));
    emit debug(QString("send %1 bytes").arg(ba.size()));

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

    is_busy = false;
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QFont *font = new QFont("Terminal", 5);
    //QFont *font = new QFont("Monospace", 5);
    Q_CHECK_PTR(font);

    QString text = "Hello, world! ПРИВЕТ, МИР!";
    QFontMetrics fm(*font);
    int w = fm.horizontalAdvance(text);
    int h = fm.height();

    QImage picture(w, h, QImage::Format_Mono);

    QPainter painter;
    painter.begin(&picture);
    painter.setPen(QPen(Qt::white));
    painter.fillRect(0, 0, w, h, QBrush(Qt::black));
    painter.setFont(*font);
    painter.drawText(0, 0, w, h, Qt::AlignCenter, text);
    painter.end();

    emit info(QString("w = %1").arg(picture.width()));
    emit info(QString("h = %1").arg(picture.height()));

#if 0
    display->resize(w, h);
    display->clear();
    for(int y=1; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            display->set_color(x, y, picture.pixelColor(x, y));
        }
    }
#else
    control_display->resize(static_cast<unsigned int>(w), static_cast<unsigned int>(h));
    control_display->resize_led(8, 8);
    control_display->clear();
    for(int y=1; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            control_display->set_color(static_cast<unsigned int>(x),
                                       static_cast<unsigned int>(y),
                                       picture.pixelColor(x, y));
        }
    }
#endif
    emit info("OK");
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
    QElapsedTimer time;
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
    emit debug("read_display_data");
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
                int x = static_cast<int>(data.at(0));
                int y = static_cast<int>(data.at(1));
                uint8_t r = static_cast<uint8_t>(data.at(2));
                uint8_t g = static_cast<uint8_t>(data.at(3));
                uint8_t b = static_cast<uint8_t>(data.at(4));
                if((r!=0) && (g!=0) && (b!=0))
                {
                    emit info("found");
                }
                control_display->set_color(static_cast<unsigned int>(x),
                                           static_cast<unsigned int>(y),
                                           r, g, b);
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
    emit debug("read_data");
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
