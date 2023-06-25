/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                      **
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
#include "mainwindow.hpp"
#include "worker_fake_v764.hpp"
#include "ui_worker_fake_v764.h"
#include "v764_packets.hpp"
//--------------------------------------------------------------------------------
#include "crc.h"
//--------------------------------------------------------------------------------
Worker_fake_v764::Worker_fake_v764(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Worker_fake_v764)
{
    init();
}
//--------------------------------------------------------------------------------
Worker_fake_v764::~Worker_fake_v764()
{
    if(timer)
    {
        timer->stop();
        disconnect(timer, &QTimer::timeout, this, &Worker_fake_v764::send_answer);

        delete timer;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::init(void)
{
    ui->setupUi(this);

    connect(this,   &Worker_fake_v764::info,     ui->log_widget, &LogBox::infoLog);
    connect(this,   &Worker_fake_v764::debug,    ui->log_widget, &LogBox::debugLog);
    connect(this,   &Worker_fake_v764::error,    ui->log_widget, &LogBox::errorLog);
    connect(this,   &Worker_fake_v764::trace,    ui->log_widget, &LogBox::traceLog);

    //---
    ui->sb_address->setRange(MIN_ADDRESS, MAX_ADDRESS);
    address = ui->sb_address->value();
    //---

    QWidgetList wl = qApp->topLevelWidgets();
    foreach (QWidget *w, wl)
    {
        if(strcmp(w->metaObject()->className(), "MainWindow") == 0)
        {
            static_cast<MainWindow *>(w)->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);

            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, &Worker_fake_v764::send_answer);

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::send_answer(void)
{
    emit output(ba_output);
    emit readyRead();
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::input(QByteArray data)
{
    if(data.isEmpty()) return;

    ba_input.clear();
    ba_input.append(data);

    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case V764_CMD_0x41: prepare_answer_41(); break;
    case V764_CMD_0x42: prepare_answer_42(); break;
    case V764_CMD_0x45: prepare_answer_45(); break;
    case V764_CMD_0x46: prepare_answer_46(); break;
    case V764_CMD_0x47: prepare_answer_47(); break;
    case V764_CMD_0x48: prepare_answer_48(); break;
    case V764_CMD_0x53: prepare_answer_53(); break;

    default:
        emit error(QString("unknown code %1").arg(data.at(1), 2, 16, QChar('0')));
        break;
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::run_timer(void)
{
    Q_ASSERT(timer);
    timer->start(FAKE_DELAY_MS);
}
//--------------------------------------------------------------------------------
template<typename T>
bool Worker_fake_v764::check_question(void)
{
    T *question = reinterpret_cast<T *>(ba_input.data());

    uint16_t crc = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&question->buf),
                   sizeof(question) - sizeof(question.crc16));
    uint16_t crc16 = question->body.crc16;

    if(crc != crc16)
    {
#ifdef Q_DEBUG
        qDebug() << Q_FUNC_INFO << "bad crc";
        qDebug() << "crc: " << QString::number(crc, 16);
        qDebug() << "crc16: " << QString::number(crc16, 16);
#endif
        return false;
    }
    return true;

}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_41(void)
{
    return check_question<CMD_V764_0x41_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_42(void)
{
    return check_question<CMD_V764_0x42_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_45(void)
{
    return check_question<CMD_V764_0x45_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_46(void)
{
    return check_question<CMD_V764_0x46_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_47(void)
{
    return check_question<CMD_V764_0x47_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_48(void)
{
    return check_question<CMD_V764_0x48_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v764::check_question_53(void)
{
    return check_question<CMD_V764_0x53_QUESTION>();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_41(void)
{
    if(!check_question_41()) return;

    CMD_V764_0x41_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x41;
    answer.body.header.count_data = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x41_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x41_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_42(void)
{
    if(!check_question_42()) return;

    CMD_V764_0x42_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x42;
    answer.body.header.count_data = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x42_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x42_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_45(void)
{
    if(!check_question_45()) return;

    CMD_V764_0x45_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x45;
    answer.body.header.count_data = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x45_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x45_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_46(void)
{
    if(!check_question_46()) return;

    CMD_V764_0x46_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x46;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x46_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x46_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_47(void)
{
    if(!check_question_47()) return;

    CMD_V764_0x47_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x47;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x47_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x47_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_48(void)
{
    if(!check_question_48()) return;

    CMD_V764_0x48_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x48;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x48_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x41_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::prepare_answer_53(void)
{
    if(!check_question_53()) return;

    CMD_V764_0x53_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x53;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<unsigned char *>(&answer.buf),
                                               sizeof(CMD_V764_0x53_ANSWER) - sizeof(question.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(CMD_V764_0x53_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v764::port_open(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_v764::port_close(void)
{

}
//--------------------------------------------------------------------------------
