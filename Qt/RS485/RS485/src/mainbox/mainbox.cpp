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
#include "defines.hpp"
#include "mainbox.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox5(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets("rs485");

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    //createTestBar();

    serialBox5 = new SerialBox5(this, "RS485", "RS485");
    serialBox5->add_menu(2);

    ui->serial_layout->addWidget(serialBox5);

    connect(this,       SIGNAL(send(QByteArray)),   serialBox5, SLOT(input(QByteArray)));
    connect(serialBox5, SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));

    connect(ui->pelco_d,    SIGNAL(send(QByteArray)),   serialBox5, SLOT(input(QByteArray)));

    ui->sb_addr_cam->setRange(0, 0xFFFF);
    ui->sb_addr_upu->setRange(0, 0xFFFF);
    ui->sb_time_washout->setRange(0, 0xFFFF);
    ui->sb_preset_washout->setRange(0, 0xFFFF);
    ui->sb_time_interval->setRange(0, 0xFFFF);
    ui->sb_time_preset_washout->setRange(0, 0xFFFF);
    ui->sb_time_pause_washout->setRange(0, 0xFFF);

    connect(ui->btn_read,   SIGNAL(clicked(bool)),  this,   SLOT(cmd_read()));
    connect(ui->btn_write,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_write()));
    connect(ui->btn_reset,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_reset()));
    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(cmd_test()));

    connect(ui->pelco_d,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(ui->pelco_d,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(ui->pelco_d,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(ui->pelco_d,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    load_widgets("rs485");
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
    toolBar->setObjectName("testbar");

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit debug(tr("reset"));
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
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    if(ba.isEmpty())
    {
        return;
    }

    emit debug(QString("read_data [%1] %2 bytes")
               .arg(ba.toHex().data())
               .arg(ba.size()));
    data_rs232.append(ba);
    is_ready = true;
}
//--------------------------------------------------------------------------------
QByteArray MainBox::convert(QByteArray ba)
{
    QByteArray temp;
    temp.append(':');
    temp.append(ba.toHex().toUpper());
    temp.append('\n');

    return temp;
}
//--------------------------------------------------------------------------------
void MainBox::cmd_read(void)
{
    emit info("Чтение");

    QUESTION_RW question;

    question.body.header.prefix_16 = 0xBBAA;                                     // префикс
    question.body.header.len_16 = sizeof(question);                              // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value();                      // адрес модуля
    question.body.header.cmd_8 = CMD_READ;                                       // команда

    question.body.addr_cam_32 = ui->sb_addr_cam->value();                        // адрес камеры
    question.body.time_washout_32 = ui->sb_time_washout->value();                // время помывки
    question.body.time_interval_16 = ui->sb_time_interval->value();              // интервал дворника
    question.body.time_pause_washout_32 = ui->sb_time_pause_washout->value();    // время между помывками
    question.body.preset_washout_32 = ui->sb_preset_washout->value();            // пресет помывки
    question.body.time_preset_washout_32 = ui->sb_time_preset_washout->value();  // времен помывки
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    if(data_rs232.size() != sizeof(ANSWER_RW))
    {
        emit error("Нет данных!");
        return;
    }

    emit info(QString("Получено [%1]").arg(data_rs232.toHex().toUpper().data()));

    ANSWER_RW *answer = (ANSWER_RW *)data_rs232.data();
    emit info(QString("prefix_16 %1").arg(answer->body.header.prefix_16));
    emit info(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit info(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit info(QString("len_16 %1").arg(answer->body.header.len_16));

    emit info(QString("addr_cam_32 %1").arg(answer->body.addr_cam_32));
    emit info(QString("time_interval_16 %1").arg(answer->body.time_interval_16));
    emit info(QString("time_washout_32 %1").arg(answer->body.time_washout_32));
    emit info(QString("time_pause_washout_32 %1").arg(answer->body.time_pause_washout_32));
    emit info(QString("preset_washout_32 %1").arg(answer->body.preset_washout_32));
    emit info(QString("time_preset_washout_32 %1").arg(answer->body.time_preset_washout_32));

}
//--------------------------------------------------------------------------------
void MainBox::cmd_write(void)
{
    emit info("Запись");

    QUESTION_RW question;

    question.body.header.prefix_16 = 0xBBAA;                                       // префикс
    question.body.header.len_16 = sizeof(question);                                // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value();                        // адрес модуля
    question.body.header.cmd_8 = CMD_WRITE;                                        // команда

    question.body.addr_cam_32 = ui->sb_addr_cam->value();                          // адрес камеры
    question.body.time_washout_32 = ui->sb_time_washout->value();                  // время помывки
    question.body.time_interval_16 = ui->sb_time_interval->value();                // интервал дворника
    question.body.time_pause_washout_32 = ui->sb_time_pause_washout->value();      // время между помывками
    question.body.preset_washout_32 = ui->sb_preset_washout->value();              // пресет помывки
    question.body.time_preset_washout_32 = ui->sb_time_preset_washout->value();    // времен помывки
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    if(data_rs232.size() != sizeof(ANSWER_RW))
    {
        emit error("Нет данных!");
        return;
    }
    emit info(QString("Получено [%1]").arg(data_rs232.toHex().toUpper().data()));

    ANSWER_RW *answer = (ANSWER_RW *)data_rs232.data();
    emit info(QString("prefix_16 %1").arg(answer->body.header.prefix_16));
    emit info(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit info(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit info(QString("len_16 %1").arg(answer->body.header.len_16));

    emit info(QString("addr_cam_32 %1").arg(answer->body.addr_cam_32));
    emit info(QString("time_interval_16 %1").arg(answer->body.time_interval_16));
    emit info(QString("time_washout_32 %1").arg(answer->body.time_washout_32));
    emit info(QString("time_pause_washout_32 %1").arg(answer->body.time_pause_washout_32));
    emit info(QString("preset_washout_32 %1").arg(answer->body.preset_washout_32));
    emit info(QString("time_preset_washout_32 %1").arg(answer->body.time_preset_washout_32));
}
//--------------------------------------------------------------------------------
void MainBox::cmd_test(void)
{
    emit info("Test");

    QUESTION_TEST question;

    question.body.header.prefix_16 = 0xBBAA;                // префикс
    question.body.header.len_16 = sizeof(question);         // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value(); // адрес модуля
    question.body.header.cmd_8 = CMD_TEST;                  // команда

    question.body.data = 0;
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    if(data_rs232.size() != sizeof(ANSWER_TEST))
    {
        emit error("Нет данных!");
        return;
    }
    emit info(QString("Получено [%1]").arg(data_rs232.toHex().toUpper().data()));

    ANSWER_TEST *answer = (ANSWER_TEST *)data_rs232.data();
    emit info(QString("prefix_16 %1").arg(answer->body.header.prefix_16));
    emit info(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit info(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit info(QString("len_16 %1").arg(answer->body.header.len_16));

    emit info(QString("data %1").arg(answer->body.data));
}
//--------------------------------------------------------------------------------
void MainBox::cmd_reset(void)
{
    emit info("Reset");

    QUESTION_RESET question;

    question.body.header.prefix_16 = 0xBBAA;                // префикс
    question.body.header.len_16 = sizeof(question);         // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value(); // адрес модуля
    question.body.header.cmd_8 = CMD_RESET;                 // команда

    question.body.data = 0;
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    if(data_rs232.size() != sizeof(ANSWER_RESET))
    {
        emit error("Нет данных!");
        return;
    }
    emit info(QString("Получено [%1]").arg(data_rs232.toHex().toUpper().data()));

    ANSWER_RESET *answer = (ANSWER_RESET *)data_rs232.data();
    emit info(QString("prefix_16 %1").arg(answer->body.header.prefix_16));
    emit info(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit info(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit info(QString("len_16 %1").arg(answer->body.header.len_16));

    emit info(QString("data %1").arg(answer->body.data));
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
