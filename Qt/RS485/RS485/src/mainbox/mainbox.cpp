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
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox5(0),
    test_byte(0),
    cnt(0)
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

    ui->sb_addr_cam->setRange(0, 0xFFFF);
    ui->sb_addr_upu->setRange(0, 0xFFFF);
    ui->sb_time_washout->setRange(0, 0xFFFF);
    ui->sb_preset_washout->setRange(0, 0xFFFF);
    ui->sb_time_interval->setRange(0, 0xFFFF);
    ui->sb_time_preset_washout->setRange(0, 0xFFFF);
    ui->sb_time_pause_washout->setRange(0, 0xFFF);

    ui->sb_addr_cam_2->setRange(0, 0xFFFF);
    ui->sb_addr_upu_2->setRange(0, 0xFFFF);
    ui->sb_time_washout_2->setRange(0, 0xFFFF);
    ui->sb_preset_washout_2->setRange(0, 0xFFFF);
    ui->sb_time_interval_2->setRange(0, 0xFFFF);
    ui->sb_time_preset_washout_2->setRange(0, 0xFFFF);
    ui->sb_time_pause_washout_2->setRange(0, 0xFFF);

    ui->sb_addr_cam->setReadOnly(true);
    ui->sb_addr_upu->setReadOnly(true);
    ui->sb_time_washout->setReadOnly(true);
    ui->sb_preset_washout->setReadOnly(true);
    ui->sb_time_interval->setReadOnly(true);
    ui->sb_time_preset_washout->setReadOnly(true);
    ui->sb_time_pause_washout->setReadOnly(true);

    connect(ui->btn_cmd_34, SIGNAL(clicked(bool)),  this,   SLOT(cmd_34()));
    connect(ui->btn_cmd_36, SIGNAL(clicked(bool)),  this,   SLOT(cmd_36()));
    connect(ui->btn_cmd_37, SIGNAL(clicked(bool)),  this,   SLOT(cmd_37()));

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
    cnt=0;
    test_byte=0;
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
#if 0
        if(is_ready)
            break;
#endif
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug(ba.toHex().data());
    data_rs232.append(ba);
    is_ready = true;
}
//--------------------------------------------------------------------------------
void MainBox::cmd_34(void)
{
    emit info("Чтение из модуля (34)");

    CMD_QUESTION question;

    question.body.prefix = 0xBBAA;                                              // префикс
    question.body.len = 32;                                                     // длина пакета
    question.body.addr = ui->sb_addr_upu->value();                            // адрес модуля
    question.body.reserv_1 = 0;                                                 // резерв
    question.body.cmd = CMD_34;                                                 // команда
    question.body.reserv_2 = 0;                                                 // резерв
    question.body.time_interval = ui->sb_time_interval->value();              // интервал дворника
    question.body.reserv_3 = 0;                                                 // резерв
    question.body.time_washout = ui->sb_time_washout->value();                // время помывки
    question.body.time_pause_washout = ui->sb_time_pause_washout->value();    // время между помывками
    question.body.addr_cam = ui->sb_addr_cam->value();                        // адрес камеры
    question.body.preset_washout = ui->sb_preset_washout->value();            // пресет помывки
    question.body.time_preset_washout = ui->sb_time_preset_washout->value();  // времен помывки

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем [%1]").arg(ba.toHex().data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(ba);
    wait(1000);

    if(data_rs232.size() != 32)
    {
        emit error("Нет данных!");
        return;
    }

    emit info(QString("Получено [%1]").arg(data_rs232.toHex().data()));

    CMD_QUESTION *answer = (CMD_QUESTION *)data_rs232.data();

    ui->sb_addr_upu->setValue(answer->body.addr);                               // адрес модуля
    ui->sb_time_interval->setValue(answer->body.time_interval);                 // интервал дворника
    ui->sb_time_washout->setValue(answer->body.time_washout);                   // время помывки
    ui->sb_time_pause_washout->setValue(answer->body.time_pause_washout);       // время между помывками
    ui->sb_addr_cam->setValue(answer->body.addr_cam);                           // адрес камеры
    ui->sb_preset_washout->setValue(answer->body.preset_washout);               // пресет помывки
    ui->sb_time_preset_washout->setValue(answer->body.time_preset_washout);     // времен помывки
}
//--------------------------------------------------------------------------------
void MainBox::cmd_36(void)
{
    emit info("Запись в модуль (36)");

    CMD_QUESTION question;

    question.body.prefix = 0xBBAA;                                              // префикс
    question.body.len = 32;                                                     // длина пакета
    question.body.addr = ui->sb_addr_upu_2->value();                            // адрес модуля
    question.body.reserv_1 = 0;                                                 // резерв
    question.body.cmd = CMD_36;                                                 // команда
    question.body.reserv_2 = 0;                                                 // резерв
    question.body.time_interval = ui->sb_time_interval_2->value();              // интервал дворника
    question.body.reserv_3 = 0;                                                 // резерв
    question.body.time_washout = ui->sb_time_washout_2->value();                // время помывки
    question.body.time_pause_washout = ui->sb_time_pause_washout_2->value();    // время между помывками
    question.body.addr_cam = ui->sb_addr_cam_2->value();                        // адрес камеры
    question.body.preset_washout = ui->sb_preset_washout_2->value();            // пресет помывки
    question.body.time_preset_washout = ui->sb_time_preset_washout_2->value();  // времен помывки

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем [%1]").arg(ba.toHex().data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(ba);
    wait(1000);

    if(data_rs232.size() != 32)
    {
        emit error("Нет данных!");
        return;
    }
    emit info(QString("Получено [%1]").arg(data_rs232.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::cmd_37(void)
{
    emit info("Запись в модуль (37)");

    CMD_QUESTION question;

    question.body.prefix = 0xBBAA;                                              // префикс
    question.body.len = 32;                                                     // длина пакета
    question.body.addr = ui->sb_addr_upu_2->value();                            // адрес модуля
    question.body.reserv_1 = 0;                                                 // резерв
    question.body.cmd = CMD_37;                                                 // команда
    question.body.reserv_2 = 0;                                                 // резерв
    question.body.time_interval = ui->sb_time_interval_2->value();              // интервал дворника
    question.body.reserv_3 = 0;                                                 // резерв
    question.body.time_washout = ui->sb_time_washout_2->value();                // время помывки
    question.body.time_pause_washout = ui->sb_time_pause_washout_2->value();    // время между помывками
    question.body.addr_cam = ui->sb_addr_cam_2->value();                        // адрес камеры
    question.body.preset_washout = ui->sb_preset_washout_2->value();            // пресет помывки
    question.body.time_preset_washout = ui->sb_time_preset_washout_2->value();  // времен помывки

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit info(QString("Длина пакета = %1").arg(ba.size()));
    emit info(QString("Отправляем [%1]").arg(ba.toHex().data()));

    //---
    data_rs232.clear();
    is_ready = false;
    emit send(ba);
    wait(1000);

    if(data_rs232.size() != 32)
    {
        emit error("Нет данных!");
        return;
    }
    emit info(QString("Получено [%1]").arg(data_rs232.toHex().data()));
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
