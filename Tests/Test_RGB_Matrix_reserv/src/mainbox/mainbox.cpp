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
#include "serialbox5.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "display.hpp"
#include "diod.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    serialBox->deleteLater();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    pos_x = 0;

    createTestBar();
    createSerialBox();
    createGridBox();
    createTimer();
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
    if(btn_run)
    {
        btn_run->setCheckable(true);
        connect(btn_run, SIGNAL(toggled(bool)), this, SLOT(run(bool)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBox(void)
{
    serialBox = new SerialBox5(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createGridBox(void)
{
    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            diod[x][y] = new Diod(this);

            grid->addWidget(diod[x][y], y, x);
        }
    }

    Display *display = new Display(16, 8, this);
    connect(display,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(display,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(display,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(display,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    Display *display2 = new Display(16, 8, this);
    connect(display2,   SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(display2,   SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(display2,   SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(display2,   SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(display);
    box->addWidget(display2);
    box->addLayout(grid);
    box->addStretch(1);

    ui->frame->setLayout(box);
    ui->frame->setFixedWidth(ui->frame->sizeHint().width());

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
                diod[x][y]->set_color(255, 0, 0);
                display->set_color(x, y, 255, 0, 0);
                break;

            case 'G':
                diod[x][y]->set_color(0, 255, 0);
                display->set_color(x, y, 0, 255, 0);
                break;

            case 'B':
                diod[x][y]->set_color(0, 0, 255);
                display->set_color(x, y, 0, 0, 255);
                break;

            default:
                break;
            }

        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTimer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
    CMD_0x01_QUESTION question;
    question.body.header.address = 1;
    question.body.header.cmd = RGB_CMD_0x01;
    question.body.header.count_data = sizeof(question.body.data);

    for(int y=0; y<NUM_STRIPS; y++)
    {
        for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
        {
            question.body.data[x][y].R = diod[x+pos_x][y]->get_R();
            question.body.data[x][y].G = diod[x+pos_x][y]->get_G();
            question.body.data[x][y].B = diod[x+pos_x][y]->get_B();
        }
    }

    question.body.crc16 = 0; //TODO ???
    pos_x++;
    if(pos_x > (MAX_SCREEN_X - NUM_LEDS_PER_STRIP))
    {
        pos_x = 0;
    }

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

    data_rs232.clear();
#ifdef FAKE
    emit trace(QString("[%1]").arg(ba.toHex().data()));
    emit info(QString("send %1 bytes").arg(ba.size()));
#else
    //emit send(ba);
    serialBox->input(ba);

    emit info(QString("send %1 bytes").arg(ba.size()));
#endif
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
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug("read_data");
    data_rs232.append(ba);
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
