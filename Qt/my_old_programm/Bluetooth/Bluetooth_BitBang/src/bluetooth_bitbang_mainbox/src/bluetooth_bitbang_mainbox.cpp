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
#include "ui_bluetooth_bitbang_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "bluetooth_bitbang_mainbox.hpp"
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

#ifdef QT_DEBUG
    createTestBar();
#endif

    serialBox = new SerialBox5(this, "RS232", "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch(1);

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

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

    CMD_0x01_QUESTION *question = reinterpret_cast<CMD_0x01_QUESTION *>(packet.data());
#if 1
    emit trace(QString("address %1").arg(question->body.header.address));
    emit trace(QString("cmd %1").arg(question->body.header.cmd));
    emit trace(QString("count_data %1").arg(question->body.header.count_data));
    emit trace(QString("led_index %1").arg(question->body.data.led_index));
    emit trace(QString("led_state %1").arg(question->body.data.led_state));
#endif

    bool state = question->body.data.led_state;
    switch (question->body.data.led_index)
    {
    case LED_2:  state ? ui->led_2->setStyleSheet(LED_ON)  : ui->led_2->setStyleSheet(LED_OFF);   break;
    case LED_3:  state ? ui->led_3->setStyleSheet(LED_ON)  : ui->led_3->setStyleSheet(LED_OFF);   break;
    case LED_4:  state ? ui->led_4->setStyleSheet(LED_ON)  : ui->led_4->setStyleSheet(LED_OFF);   break;
    case LED_5:  state ? ui->led_5->setStyleSheet(LED_ON)  : ui->led_5->setStyleSheet(LED_OFF);   break;
    case LED_6:  state ? ui->led_6->setStyleSheet(LED_ON)  : ui->led_6->setStyleSheet(LED_OFF);   break;
    case LED_7:  state ? ui->led_7->setStyleSheet(LED_ON)  : ui->led_7->setStyleSheet(LED_OFF);   break;
    case LED_8:  state ? ui->led_8->setStyleSheet(LED_ON)  : ui->led_8->setStyleSheet(LED_OFF);   break;
    case LED_9:  state ? ui->led_9->setStyleSheet(LED_ON)  : ui->led_9->setStyleSheet(LED_OFF);   break;
    case LED_10: state ? ui->led_10->setStyleSheet(LED_ON) : ui->led_10->setStyleSheet(LED_OFF);  break;
    case LED_11: state ? ui->led_11->setStyleSheet(LED_ON) : ui->led_11->setStyleSheet(LED_OFF);  break;
    case LED_12: state ? ui->led_12->setStyleSheet(LED_ON) : ui->led_12->setStyleSheet(LED_OFF);  break;
    case LED_13: state ? ui->led_13->setStyleSheet(LED_ON) : ui->led_13->setStyleSheet(LED_OFF);  break;

    default:
        break;
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
    ba.append(reinterpret_cast<char *>(answer.buf), sizeof(CMD_0x01_ANSWER));

    QByteArray output;

    output.clear();
    output.append(':');
    output.append(ba.toHex());
    output.append('\n');
    emit info(QString("send [%1]").arg(output.data()));

    emit send(output);
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
