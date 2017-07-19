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
#include "serialbox.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
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
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

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
    btn_run->setCheckable(true);

    connect(btn_run, SIGNAL(toggled(bool)), this, SLOT(run(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBox(void)
{
    serialBox = new SerialBox(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createGridBox(void)
{
    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            btn[x][y] = new Diod(this);

            grid->addWidget(btn[x][y], y, x);
        }
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addStretch(1);

    ui->frame->setLayout(vbox);

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
                btn[x][y]->set_state(DIOD_R);
                break;

            case 'G':
                btn[x][y]->set_state(DIOD_G);
                break;

            case 'B':
                btn[x][y]->set_state(DIOD_B);
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
            switch(btn[x+pos_x][y]->property("ID").toInt())
            {
            case DIOD_R:
                question.body.data[x][y] = 'R';
                break;
            case DIOD_G:
                question.body.data[x][y] = 'G';
                break;
            case DIOD_B:
                question.body.data[x][y] = 'B';
                break;

            default:
                question.body.data[x][y] = 0;
                break;
            }
        }
    }

    question.body.crc16 = 0; //???
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
        ba.append(QString("%1").arg(question.buf[n], 2, 16, QChar('0')));
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
    emit trace(ba.toHex());
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
