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
#include "ui_test_arduino_modbus_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "test_arduino_modbus_mainbox.hpp"
#include "defines.hpp"
#include "sleeper.h"
#include "crc.h"
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

    serialBox = new SerialBox5(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this,       SIGNAL(send(QByteArray)),   serialBox,  SLOT(input(QByteArray)));
    connect(serialBox,  SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
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

    QUESTION question;
    question.body.address = htons(0x1);
    question.body.cmd = 2;
    question.body.data_len = htons(sizeof(question.body.data));
    question.body.data[0] = 4;
    question.body.data[1] = 5;
    question.body.data[2] = 6;
    question.body.crc16 = htons(CRC::java_crc16(question.buf, sizeof(QUESTION) - sizeof(question.body.crc16)));

    QByteArray ba;
    ba.clear();
    ba.append((const char *)question.buf, sizeof(QUESTION));

    if(!serialBox->isOpen())
    {
        emit error("serial not open!");
        return;
    }

    data_rs232.clear();
    serialBox->input(ba);
    emit info(QString("sending %1 bytes").arg(sizeof(QUESTION)));
    wait(1000);

    if(data_rs232.isEmpty())
    {
        emit error("no data");
        return;
    }
    if(data_rs232.length() != sizeof(ANSWER))
    {
        emit error(QString("received %1 needed %2 bytes")
                   .arg(data_rs232.length())
                   .arg(sizeof(ANSWER)));
        return;
    }

    ANSWER *answer = (ANSWER *)data_rs232.constData();
    unsigned short calc_crc16 = CRC::java_crc16(answer->buf, sizeof(ANSWER) - sizeof(answer->body.crc16));
    if(calc_crc16 != answer->body.crc16)
    {
        emit error(QString("CRC16: received %1 needed %2")
                   .arg(answer->body.crc16)
                   .arg(calc_crc16));
        return;
    }

    emit info(data_rs232.toHex());
    emit info(QString("address %1").arg(answer->body.address));
    emit info(QString("cmd %1").arg(answer->body.cmd));
    emit info(QString("data_len %1").arg(answer->body.data_len));
    emit info(QString("data %1").arg(answer->body.data));
    emit info(QString("crc16 %1").arg(answer->body.crc16, 0, 16));
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        //if(is_ready) break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug("read_data");
    data_rs232.append(ba);
    is_ready = true;
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
