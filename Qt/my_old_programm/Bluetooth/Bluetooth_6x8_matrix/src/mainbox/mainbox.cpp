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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
#include "led.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    serialBox = new SerialBox5(this, "RS232", "RS232");
    serialBox->add_menu(2);

    QFrame *frame = new QFrame(this);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(0);
    grid->setMargin(0);
    for(int y=0; y<NUM_STRIPS; y++)
    {
        for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
        {
            buf_led[y][x] = new Led();
            grid->addWidget(buf_led[y][x], y, x);
        }
    }
    frame->setLayout(grid);
    frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    frame->setFixedSize(frame->sizeHint());

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(serialBox);
    vbox->addStretch(1);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(vbox);
    hbox->addWidget(frame);

    setLayout(hbox);

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
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

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info(tr("test"));
    send_answer_data();

}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    for(int n=0; n<ba.length(); n++)
    {
        char s = ba.at(n);
        switch(s)
        {
        case ':':
            clean_data.clear();
            break;

        case '\n':
            read_modbus();
            break;

        default:
            clean_data.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_modbus(void)
{
    QByteArray packet = QByteArray::fromHex(clean_data);
    if(packet.length() != sizeof(CMD_0x01_QUESTION))
    {
        emit error(QString("packet size %1").arg(packet.length()));
        emit error(QString("size %1 need %2")
                   .arg(clean_data.length())
                   .arg(sizeof(CMD_0x01_QUESTION)));
        return;
    }

    CMD_0x01_QUESTION *question = (CMD_0x01_QUESTION *)packet.data();
    emit debug(QString("address %1").arg(question->body.header.address));
    emit debug(QString("cmd %1").arg(question->body.header.cmd));
    emit debug(QString("count_data %1").arg(question->body.header.count_data));

    if(question->body.header.count_data != NUM_STRIPS*NUM_LEDS_PER_STRIP)
    {
        emit error(QString("bad len data %1 != %2")
                   .arg(question->body.header.count_data)
                   .arg(NUM_STRIPS*NUM_LEDS_PER_STRIP));
        return;
    }

    for(int y=0; y<NUM_STRIPS; y++)
    {
        for(int x=0; x<NUM_LEDS_PER_STRIP; x++)
        {
            buf_led[y][x]->set_color(question->body.data.led[x][y]);
        }
    }

    send_answer_data();
}
//--------------------------------------------------------------------------------
void MainBox::send_answer_data(void)
{
    CMD_0x01_ANSWER answer;
    QByteArray ba;

    answer.body.header.address = 1;
    answer.body.header.cmd = 1;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.data.result = 1;
    answer.body.crc8 = 0;

    ba.clear();
    ba.append((char *)answer.buf, sizeof(CMD_0x01_ANSWER));

    QByteArray output;

    output.clear();
    output.append(':');
    output.append(ba.toHex());
    output.append('\n');
    emit info(QString("send [%1]").arg(output.data()));

    emit send(output);
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
