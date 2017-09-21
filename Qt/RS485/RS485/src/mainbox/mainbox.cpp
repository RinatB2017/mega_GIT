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

    connect(ui->btn_cmd_31, SIGNAL(clicked(bool)),  this,   SLOT(cmd_31()));
    connect(ui->btn_cmd_34, SIGNAL(clicked(bool)),  this,   SLOT(cmd_34()));
    connect(ui->btn_cmd_36, SIGNAL(clicked(bool)),  this,   SLOT(cmd_36()));
    connect(ui->btn_cmd_37, SIGNAL(clicked(bool)),  this,   SLOT(cmd_37()));
    connect(ui->btn_cmd_42, SIGNAL(clicked(bool)),  this,   SLOT(cmd_42()));

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
#if 0
        if(is_ready)
            break;
#endif
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    if(ba.isEmpty())
    {
        return;
    }

    emit info(QString("read_data [%1] %2 bytes")
              .arg(ba.toHex().data())
              .arg(ba.size()));
    data_rs232.append(ba);
    is_ready = true;
}
//--------------------------------------------------------------------------------
uint16_t MainBox::convert16(uint16_t prefix)
{
#ifdef CONVERT
    return htons(prefix);
#else
    return prefix;
#endif
}
//--------------------------------------------------------------------------------
uint32_t MainBox::convert32(uint32_t prefix)
{
#ifdef CONVERT
    return htonl(prefix);
#else
    return prefix;
#endif
}
//--------------------------------------------------------------------------------
void MainBox::cmd_31(void)
{
    emit info("Чтение состояния (31)");

    CMD_QUESTION question;

    question.body.prefix_16 = convert16(0xAABB);                                            // префикс
    question.body.len_16 = convert16(32);                                                   // длина пакета
    question.body.addr_8 = ui->sb_addr_upu->value();                                        // адрес модуля
    question.body.reserv_3_16 = 0;                                                          // резерв
    question.body.cmd_8 = CMD_31;                                                           // команда
    question.body.reserv_2_8 = 0;                                                           // резерв
    question.body.time_interval_16 = convert16(ui->sb_time_interval->value());              // интервал дворника
    question.body.reserv_3_16 = convert16(0);                                               // резерв
    question.body.time_washout_32 = convert32(ui->sb_time_washout->value());                // время помывки
    question.body.time_pause_washout_32 = convert32(ui->sb_time_pause_washout->value());    // время между помывками
    question.body.addr_cam_32 = convert32(ui->sb_addr_cam->value());                        // адрес камеры
    question.body.preset_washout_32 = convert32(ui->sb_preset_washout->value());            // пресет помывки
    question.body.time_preset_washout_32 = convert32(ui->sb_time_preset_washout->value());  // времен помывки

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
void MainBox::cmd_34(void)
{
    emit info("Чтение из модуля (34)");

    CMD_QUESTION question;

    question.body.prefix_16 = convert16(0xAABB);                                            // префикс
    question.body.len_16 = convert16(32);                                                   // длина пакета
    question.body.addr_8 = ui->sb_addr_upu->value();                                        // адрес модуля
    question.body.reserv_3_16 = 0;                                                          // резерв
    question.body.cmd_8 = CMD_34;                                                           // команда
    question.body.reserv_2_8 = 0;                                                           // резерв
    question.body.time_interval_16 = convert16(ui->sb_time_interval->value());              // интервал дворника
    question.body.reserv_3_16 = convert16(0);                                               // резерв
    question.body.time_washout_32 = convert32(ui->sb_time_washout->value());                // время помывки
    question.body.time_pause_washout_32 = convert32(ui->sb_time_pause_washout->value());    // время между помывками
    question.body.addr_cam_32 = convert32(ui->sb_addr_cam->value());                        // адрес камеры
    question.body.preset_washout_32 = convert32(ui->sb_preset_washout->value());            // пресет помывки
    question.body.time_preset_washout_32 = convert32(ui->sb_time_preset_washout->value());  // времен помывки

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

    ui->sb_addr_upu->setValue(answer->body.addr_8);                                         // адрес модуля
    ui->sb_time_interval->setValue(convert16(answer->body.time_interval_16));               // интервал дворника
    ui->sb_time_washout->setValue(convert32(answer->body.time_washout_32));                 // время помывки
    ui->sb_time_pause_washout->setValue(convert32(answer->body.time_pause_washout_32));     // время между помывками
    ui->sb_addr_cam->setValue(convert32(answer->body.addr_cam_32));                         // адрес камеры
    ui->sb_preset_washout->setValue(convert32(answer->body.preset_washout_32));             // пресет помывки
    ui->sb_time_preset_washout->setValue(convert32(answer->body.time_preset_washout_32));   // времен помывки
}
//--------------------------------------------------------------------------------
void MainBox::cmd_36(void)
{
    emit info("Запись в модуль (36)");

    CMD_QUESTION question;

    question.body.prefix_16 = convert16(0xAABB);                                                // префикс
    question.body.len_16 = convert16(32);                                                       // длина пакета
    question.body.addr_8 = ui->sb_addr_upu_2->value();                                          // адрес модуля
    question.body.reserv_3_16 = 0;                                                              // резерв
    question.body.cmd_8 = CMD_36;                                                               // команда
    question.body.reserv_2_8 = 0;                                                               // резерв
    question.body.time_interval_16 = convert16(ui->sb_time_interval_2->value());                // интервал дворника
    question.body.reserv_3_16 = convert16(0);                                                   // резерв
    question.body.time_washout_32 = convert32(ui->sb_time_washout_2->value());                  // время помывки
    question.body.time_pause_washout_32 = convert32(ui->sb_time_pause_washout_2->value());      // время между помывками
    question.body.addr_cam_32 = convert32(ui->sb_addr_cam_2->value());                          // адрес камеры
    question.body.preset_washout_32 = convert32(ui->sb_preset_washout_2->value());              // пресет помывки
    question.body.time_preset_washout_32 = convert32(ui->sb_time_preset_washout_2->value());    // времен помывки

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

    question.body.prefix_16 = convert16(0xAABB);                                                // префикс
    question.body.len_16 = convert16(32);                                                       // длина пакета
    question.body.addr_8 = ui->sb_addr_upu_2->value();                                          // адрес модуля
    question.body.reserv_3_16 = 0;                                                              // резерв
    question.body.cmd_8 = CMD_37;                                                               // команда
    question.body.reserv_2_8 = 0;                                                               // резерв
    question.body.time_interval_16 = convert16(ui->sb_time_interval_2->value());                // интервал дворника
    question.body.reserv_3_16 = convert16(0);                                                   // резерв
    question.body.time_washout_32 = convert32(ui->sb_time_washout_2->value());                  // время помывки
    question.body.time_pause_washout_32 = convert32(ui->sb_time_pause_washout_2->value());      // время между помывками
    question.body.addr_cam_32 = convert32(ui->sb_addr_cam_2->value());                          // адрес камеры
    question.body.preset_washout_32 = convert32(ui->sb_preset_washout_2->value());              // пресет помывки
    question.body.time_preset_washout_32 = convert32(ui->sb_time_preset_washout_2->value());    // времен помывки

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
void MainBox::cmd_42(void)
{
    emit info("TEST (42)");

    CMD_QUESTION question;

    question.body.prefix_16 = convert16(0xAABB);                                                // префикс
    question.body.len_16 = convert16(32);                                                       // длина пакета
    question.body.addr_8 = ui->sb_addr_upu_2->value();                                          // адрес модуля
    question.body.reserv_8 = 0;                                                                 // резерв
    question.body.cmd_8 = CMD_42;                                                               // команда
    question.body.reserv_2_8 = 0;                                                               // резерв
    question.body.time_interval_16 = convert16(ui->sb_time_interval_2->value());                // интервал дворника
    question.body.reserv_3_16 = convert16(0);                                                   // резерв
    question.body.time_washout_32 = convert32(ui->sb_time_washout_2->value());                  // время помывки
    question.body.time_pause_washout_32 = convert32(ui->sb_time_pause_washout_2->value());      // время между помывками
    question.body.addr_cam_32 = convert32(ui->sb_addr_cam_2->value());                          // адрес камеры
    question.body.preset_washout_32 = convert32(ui->sb_preset_washout_2->value());              // пресет помывки
    question.body.time_preset_washout_32 = convert32(ui->sb_time_preset_washout_2->value());    // времен помывки

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
