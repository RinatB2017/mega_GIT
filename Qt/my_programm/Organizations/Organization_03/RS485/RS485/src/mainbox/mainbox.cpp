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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "defines.hpp"
#include "mainbox.hpp"
#include "packet.hpp"
#include "pelcod.hpp"
#include "crc.h"
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
    save_widgets("w_rs485");
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    ui->serial_widget->set_caption("RS-485");
    ui->serial_widget->add_menu(2);
    ui->serial_widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));

#ifdef UNDER_CONSTRUCTION
    pelco_d = new PelcoD(this);
    connect(pelco_d,    SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(pelco_d,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(pelco_d,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(pelco_d,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(pelco_d,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    layout()->addWidget(pelco_d);
#endif

    ui->sb_addr_cam->setRange(0, 0xFFFF);
    ui->sb_addr_upu->setRange(0, 0xFFFF);
    ui->sb_time_washout->setRange(5, 0xFFFF);
    ui->sb_preset_washout->setRange(0, 0xFFFF);
    ui->sb_time_interval->setRange(3, 0xFFFF);

    connect(ui->btn_read,   SIGNAL(clicked(bool)),  this,   SLOT(cmd_read()));
    connect(ui->btn_write,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_write()));
    connect(ui->btn_reset,  SIGNAL(clicked(bool)),  this,   SLOT(cmd_reset()));
    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(cmd_test()));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    load_widgets("w_rs485");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

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
    emit info("test");

    ui->serial_widget->setProperty("p_test", true);
    bool ok = ui->serial_widget->property("p_test").toBool();
    emit error(QString("%1").arg(ok ? "true" : "false"));
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

    //emit debug(QString("read_data: [%1]").arg(ba.data()));
    emit debug(QString("read_data: [%1]").arg(ba.toHex().toUpper().data()));
    for(int n=0; n<ba.size(); n++)
    {
        char s = ba.at(n);
        switch(s)
        {
        case ':':
            data_rs232_dirty.clear();
            is_ready = false;
            break;
        case '\r':
        case '\n':
            data_rs232_clean.clear();
            data_rs232_clean.append(QByteArray::fromHex(data_rs232_dirty));
            is_ready = true;
            break;
        default:
            data_rs232_dirty.append(s);
            break;
        }
    }
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
int MainBox::check_answer_test(QByteArray data)
{
    if(data.isEmpty())
    {
        emit error("Ответ пуст!");
        return ERR_ANSWER_EMPTY;
    }

    ANSWER_TEST *answer = (ANSWER_TEST *)data.data();
    Q_CHECK_PTR(answer);
    if(data.size() != (int)(sizeof(HEADER) + answer->body.header.len_16 + sizeof(answer->body.crc16)))
    {
        emit error("Размер пакета не корректен");
        return ERR_ANSWER_BAD_SIZE;
    }

    uint16_t prefix = answer->body.header.prefix_16;
    if(prefix != PREFIX)
    {
        emit error(QString("Префикс не корректен! [0x%1]").arg(prefix, 0, 16));
        return ERR_BAD_PREFIX;
    }

    uint8_t cmd = answer->body.header.cmd_8;
    if(cmd != CMD_TEST)
    {
        emit error(QString("Неверная команда! [0x%1]").arg(cmd, 0, 16));
        return ERR_BAD_CMD;
    }

    uint16_t crc16 = CRC::crc16((uint8_t *)&answer->buf, sizeof(ANSWER_TEST) - sizeof(answer->body.crc16));
    if(crc16 != answer->body.crc16)
    {
        emit error(QString("Контрольная сумма не корректнa! 0x%1 != 0x%2")
                   .arg(answer->body.crc16, 2, 16, QChar('0'))
                   .arg(crc16, 2, 16, QChar('0')));
        return ERR_BAD_CRC16;
    }

    emit debug(QString("prefix_16 %1").arg(prefix, 0, 16));
    emit debug(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit debug(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit debug(QString("len_16 %1").arg(answer->body.header.len_16));

    emit debug(QString("data %1").arg(answer->body.data.result));

    return NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_answer_reset(QByteArray data)
{
    if(data.isEmpty())
    {
        emit error("Ответ пуст!");
        return ERR_ANSWER_EMPTY;
    }

    ANSWER_RESET *answer = (ANSWER_RESET *)data.data();
    Q_CHECK_PTR(answer);
    if(data.size() != (int)(sizeof(HEADER) + answer->body.header.len_16 + sizeof(answer->body.crc16)))
    {
        emit error("Размер пакета не корректен");
        return ERR_ANSWER_BAD_SIZE;
    }

    uint16_t prefix = answer->body.header.prefix_16;
    if(prefix != PREFIX)
    {
        emit error(QString("Префикс не корректен! [0x%1]").arg(prefix, 0, 16));
        return ERR_BAD_PREFIX;
    }

    uint8_t cmd = answer->body.header.cmd_8;
    if(cmd != CMD_RESET)
    {
        emit error(QString("Неверная команда! [0x%1]").arg(cmd, 0, 16));
        return ERR_BAD_CMD;
    }

    uint16_t crc16 = CRC::crc16((uint8_t *)&answer->buf, sizeof(ANSWER_RESET) - sizeof(answer->body.crc16));
    if(crc16 != answer->body.crc16)
    {
        emit error(QString("Контрольная сумма не корректнa! 0x%1 != 0x%2")
                   .arg(answer->body.crc16, 2, 16, QChar('0'))
                   .arg(crc16, 2, 16, QChar('0')));
        return ERR_BAD_CRC16;
    }

    uint32_t    addr_upu = answer->body.header.addr_8;                              // адрес upu
    uint32_t    addr_cam_32 = answer->body.data.addr_cam_32;                        // адрес камеры
    uint16_t    time_interval_16 = answer->body.data.time_interval_16;              // интервал дворника
    uint32_t    time_washout_32 = answer->body.data.time_washout_32;                // время помывки
    uint32_t    time_pause_washout_32 = answer->body.data.time_pause_washout_32;    // время между помывками
    uint32_t    preset_washout_32 = answer->body.data.preset_washout_32;            // пресет помывки

    QTime tpw(0,0,0);
    QTime tpw2;
    tpw2 = tpw.addSecs(time_pause_washout_32);
    emit debug(QString("time_pause_washout_32 %1").arg(time_pause_washout_32));
    emit debug(QString("[%1 %2 %3]")
               .arg(tpw2.hour())
               .arg(tpw2.minute())
               .arg(tpw2.second()));

    ui->sb_addr_upu->setValue(addr_upu);                // адрес upu
    ui->sb_addr_cam->setValue(addr_cam_32);             // адрес камеры
    ui->sb_time_interval->setValue(time_interval_16);   // интервал дворника
    ui->sb_time_washout->setValue(time_washout_32);     // время помывки
    ui->te_time_pause_washout->setTime(tpw2);           // время между помывками
    ui->sb_preset_washout->setValue(preset_washout_32); // пресет помывки

    emit debug(QString("prefix_16 %1").arg(prefix, 0, 16));
    emit debug(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit debug(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit debug(QString("len_16 %1").arg(answer->body.header.len_16));

    return NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_answer_read(QByteArray data)
{
    if(data.isEmpty())
    {
        emit error("Ответ пуст!");
        return ERR_ANSWER_EMPTY;
    }

    ANSWER_READ *answer = (ANSWER_READ *)data.data();
    if(data.size() != (int)(sizeof(HEADER) + answer->body.header.len_16 + sizeof(answer->body.crc16)))
    {
        emit error("Размер пакета не корректен");
        return ERR_ANSWER_BAD_SIZE;
    }

    uint16_t prefix = answer->body.header.prefix_16;
    if(prefix != PREFIX)
    {
        emit error(QString("Префикс не корректен! [0x%1]").arg(prefix, 0, 16));
        return ERR_BAD_PREFIX;
    }

    uint8_t cmd = answer->body.header.cmd_8;
    if(cmd != CMD_READ)
    {
        emit error(QString("Неверная команда! [0x%1]").arg(cmd, 0, 16));
        return ERR_BAD_CMD;
    }

    uint16_t crc16 = CRC::crc16((uint8_t *)&answer->buf, sizeof(ANSWER_READ) - sizeof(answer->body.crc16));
    if(crc16 != answer->body.crc16)
    {
        emit error(QString("Контрольная сумма не корректнa! 0x%1 != 0x%2")
                   .arg(answer->body.crc16, 2, 16, QChar('0'))
                   .arg(crc16, 2, 16, QChar('0')));
        return ERR_BAD_CRC16;
    }

    uint32_t    addr_upu = answer->body.header.addr_8;                              // адрес upu
    uint32_t    addr_cam_32 = answer->body.data.addr_cam_32;                        // адрес камеры
    uint16_t    time_interval_16 = answer->body.data.time_interval_16;              // интервал дворника
    uint32_t    time_washout_32 = answer->body.data.time_washout_32;                // время помывки
    uint32_t    time_pause_washout_32 = answer->body.data.time_pause_washout_32;    // время между помывками
    uint32_t    preset_washout_32 = answer->body.data.preset_washout_32;            // пресет помывки

    QTime tpw(0,0,0);
    QTime tpw2;
    tpw2 = tpw.addSecs(time_pause_washout_32);
    emit debug(QString("time_pause_washout_32 %1").arg(time_pause_washout_32));
    emit debug(QString("[%1 %2 %3]")
               .arg(tpw2.hour())
               .arg(tpw2.minute())
               .arg(tpw2.second()));

    ui->sb_addr_upu->setValue(addr_upu);                // адрес upu
    ui->sb_addr_cam->setValue(addr_cam_32);             // адрес камеры
    ui->sb_time_interval->setValue(time_interval_16);   // интервал дворника
    ui->sb_time_washout->setValue(time_washout_32);     // время помывки
    ui->te_time_pause_washout->setTime(tpw2);           // время между помывками
    ui->sb_preset_washout->setValue(preset_washout_32); // пресет помывки

    emit debug(QString("prefix_16 %1").arg(prefix, 0, 16));
    emit debug(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit debug(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit debug(QString("len_16 %1").arg(answer->body.header.len_16));

    emit debug(QString("addr_cam_32 %1").arg(addr_cam_32));
    emit debug(QString("time_interval_16 %1").arg(time_interval_16));
    emit debug(QString("time_washout_32 %1").arg(time_washout_32));
    emit debug(QString("time_pause_washout_32 %1").arg(time_pause_washout_32));
    emit debug(QString("preset_washout_32 %1").arg(preset_washout_32));

    return NO_ERROR;
}
//--------------------------------------------------------------------------------
int MainBox::check_answer_write(QByteArray data)
{
    if(data.isEmpty())
    {
        emit error("Ответ пуст!");
        return ERR_ANSWER_EMPTY;
    }

    ANSWER_WRITE *answer = (ANSWER_WRITE *)data.data();
    if(data.size() != (int)(sizeof(HEADER) + answer->body.header.len_16 + sizeof(answer->body.crc16)))
    {
        emit error("Размер пакета не корректен");
        return ERR_ANSWER_BAD_SIZE;
    }

    uint16_t prefix = answer->body.header.prefix_16;
    if(prefix != PREFIX)
    {
        emit error(QString("Префикс не корректен! [0x%1]").arg(prefix, 0, 16));
        return ERR_BAD_PREFIX;
    }

    uint8_t cmd = answer->body.header.cmd_8;
    if(cmd != CMD_WRITE)
    {
        emit error(QString("Неверная команда! [0x%1]").arg(cmd, 0, 16));
        return ERR_BAD_CMD;
    }

    uint16_t crc16 = CRC::crc16((uint8_t *)&answer->buf, sizeof(ANSWER_WRITE) - sizeof(answer->body.crc16));
    if(crc16 != answer->body.crc16)
    {
        emit error(QString("Контрольная сумма не корректнa! 0x%1 != 0x%2")
                   .arg(answer->body.crc16, 2, 16, QChar('0'))
                   .arg(crc16, 2, 16, QChar('0')));
        return ERR_BAD_CRC16;
    }

    uint32_t    addr_cam_32 = answer->body.data.addr_cam_32;                         // адрес камеры
    uint16_t    time_interval_16 = answer->body.data.time_interval_16;               // интервал дворника
    uint32_t    time_washout_32 = answer->body.data.time_washout_32;                 // время помывки
    uint32_t    time_pause_washout_32 = answer->body.data.time_pause_washout_32;     // время между помывками
    uint32_t    preset_washout_32 = answer->body.data.preset_washout_32;             // пресет помывки

    QTime tpw(0,0,0);
    QTime tpw2;
    tpw2 = tpw.addSecs(time_pause_washout_32);
    emit debug(QString("time_pause_washout_32 %1").arg(time_pause_washout_32));
    emit debug(QString("[%1 %2 %3]")
               .arg(tpw2.hour())
               .arg(tpw2.minute())
               .arg(tpw2.second()));

    ui->sb_addr_cam->setValue(addr_cam_32);                         // адрес камеры
    ui->sb_time_interval->setValue(time_interval_16);               // интервал дворника
    ui->sb_time_washout->setValue(time_washout_32);                 // время помывки
    ui->te_time_pause_washout->setTime(tpw2);                        // время между помывками
    ui->sb_preset_washout->setValue(preset_washout_32);             // пресет помывки

    emit debug(QString("prefix_16 %1").arg(prefix, 0, 16));
    emit debug(QString("addr_8 %1").arg(answer->body.header.addr_8));
    emit debug(QString("cmd_8 %1").arg(answer->body.header.cmd_8));
    emit debug(QString("len_16 %1").arg(answer->body.header.len_16));

    emit debug(QString("addr_cam_32 %1").arg(addr_cam_32));
    emit debug(QString("time_interval_16 %1").arg(time_interval_16));
    emit debug(QString("time_washout_32 %1").arg(time_washout_32));
    emit debug(QString("time_pause_washout_32 %1").arg(time_pause_washout_32));
    emit debug(QString("preset_washout_32 %1").arg(preset_washout_32));

    return NO_ERROR;
}
//--------------------------------------------------------------------------------
void MainBox::cmd_read(void)
{
    emit info("Чтение");

    QUESTION_READ question;

    question.body.header.prefix_16 = 0xBBAA;                                     // префикс
    question.body.header.len_16 = sizeof(question);                              // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value();                      // адрес модуля
    question.body.header.cmd_8 = CMD_READ;                                       // команда
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit debug(QString("Длина пакета = %1").arg(ba.size()));
    emit debug(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232_clean.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    emit debug(QString("Получено [%1]").arg(data_rs232_clean.toHex().data()));
    int err = check_answer_read(data_rs232_clean);
    if(err != MainBox::NO_ERROR)
    {
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::cmd_write(void)
{
    emit info("Запись");

    QUESTION_WRITE question;

    question.body.header.prefix_16 = 0xBBAA;                                       // префикс
    question.body.header.len_16 = sizeof(question);                                // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value();                        // адрес модуля
    question.body.header.cmd_8 = CMD_WRITE;                                        // команда

    uint32_t t = ui->te_time_pause_washout->time().hour() * 3600 +
            ui->te_time_pause_washout->time().minute() * 60 +
            ui->te_time_pause_washout->time().second();

    question.body.data.addr_cam_32 = ui->sb_addr_cam->value();                          // адрес камеры
    question.body.data.time_washout_32 = ui->sb_time_washout->value();                  // время помывки
    question.body.data.time_interval_16 = ui->sb_time_interval->value();                // интервал дворника
    question.body.data.time_pause_washout_32 = t;                                       // время между помывками
    question.body.data.preset_washout_32 = ui->sb_preset_washout->value();              // пресет помывки
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit debug(QString("Длина пакета = %1").arg(ba.size()));
    emit debug(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232_clean.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    emit debug(QString("Получено [%1]").arg(data_rs232_clean.toHex().data()));
    int err = check_answer_write(data_rs232_clean);
    if(err != MainBox::NO_ERROR)
    {
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::cmd_test(void)
{
    emit info("Test");
    emit trace("cmd_test");

    QUESTION_TEST question;

    question.body.header.prefix_16 = 0xBBAA;                // префикс
    question.body.header.len_16 = sizeof(question);         // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value(); // адрес модуля
    question.body.header.cmd_8 = CMD_TEST;                  // команда
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit debug(QString("Длина пакета = %1").arg(ba.size()));
    emit debug(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232_clean.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    emit debug(QString("Получено [%1]").arg(data_rs232_clean.data()));
    emit debug(QString("Получено (hex) [%1]").arg(data_rs232_clean.toHex().toUpper().data()));
    int err = check_answer_test(data_rs232_clean);
    if(err != MainBox::NO_ERROR)
    {
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::cmd_reset(void)
{
    emit info("Reset");
    emit trace("cmd_reset");

    QUESTION_RESET question;

    question.body.header.prefix_16 = 0xBBAA;                // префикс
    question.body.header.len_16 = sizeof(question);         // длина пакета
    question.body.header.addr_8 = ui->sb_addr_upu->value(); // адрес модуля
    question.body.header.cmd_8 = CMD_RESET;                 // команда
    question.body.crc16 = CRC::crc16((uint8_t *)&question.buf, sizeof(question) - 2);

    QByteArray ba;
    ba.append((char *)&question.buf, sizeof(question.buf));

    emit debug(QString("Длина пакета = %1").arg(ba.size()));
    emit debug(QString("Отправляем %1").arg(convert(ba).replace("\n", "").data()));

    //---
    data_rs232_clean.clear();
    is_ready = false;
    emit send(convert(ba));
    wait(1000);

    emit debug(QString("Получено [%1]").arg(data_rs232_clean.toHex().data()));
    int err = check_answer_reset(data_rs232_clean);
    if(err != MainBox::NO_ERROR)
    {
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::print_err(int code)
{
    switch(code)
    {
    case NO_ERROR:              emit info("Нет ошибки!");                   break;
    case ERR_ANSWER_EMPTY:      emit error("Ответ пуст!");                  break;
    case ERR_ANSWER_BAD_SIZE:   emit error("Неверный размер пакета");       break;
    case ERR_BAD_PREFIX:        emit error("Неверный префикс");             break;
    case ERR_BAD_CMD:           emit error("Неверная команда");             break;
    case ERR_BAD_CRC16:         emit error("Неверная контрольная сумма");   break;
    default:
        emit error(QString("unknown err code [%1]").arg(code));
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
