/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "worker_fake_v786.hpp"
#include "ui_worker_fake_v786.h"
#include "v786_packets.hpp"
//--------------------------------------------------------------------------------
#include "crc.h"
//--------------------------------------------------------------------------------
Worker_fake_v786::Worker_fake_v786(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Worker_fake_v786)
{
    init();
}
//--------------------------------------------------------------------------------
Worker_fake_v786::~Worker_fake_v786()
{
    if(timer)
    {
        timer->stop();
        timer->disconnect();
        delete timer;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::init(void)
{
    ui->setupUi(this);

    connect(this,   &Worker_fake_v786::info,     ui->log_widget, &LogBox::infoLog);
    connect(this,   &Worker_fake_v786::debug,    ui->log_widget, &LogBox::debugLog);
    connect(this,   &Worker_fake_v786::error,    ui->log_widget, &LogBox::errorLog);
    connect(this,   &Worker_fake_v786::trace,    ui->log_widget, &LogBox::traceLog);

    //ui->sb_address->setRange(MIN_ADDRESS, MAX_ADDRESS);

    //---
    address = ui->sb_address->value();
    //---

    QWidgetList wl = qApp->topLevelWidgets();
    foreach (QWidget *w, wl)
    {
        if(strcmp(w->metaObject()->className(), "MainWindow") == 0)
        {
            static_cast<MainWindow *>(w)->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);

            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, &Worker_fake_v786::send_answer);

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::send_answer(void)
{
    emit output(ba_output);
    emit readyRead();
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::input(QByteArray data)
{
    if(data.isEmpty()) return;

    ba_input.clear();
    ba_input.append(data);

    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case V764_CMD_0x01: prepare_answer_01(); break;
    case V764_CMD_0x02: prepare_answer_02(); break;
    case V764_CMD_0x03: prepare_answer_03(); break;
    case V764_CMD_0x04: prepare_answer_04(); break;
    case V764_CMD_0x05: prepare_answer_05(); break;
    case V764_CMD_0x06: prepare_answer_06(); break;
    case V764_CMD_0x07: prepare_answer_07(); break;
    case V764_CMD_0x08: prepare_answer_08(); break;
    case V764_CMD_0x09: prepare_answer_09(); break;
    case V764_CMD_0x0A: prepare_answer_0A(); break;
    case V764_CMD_0x0B: prepare_answer_0B(); break;
    case V764_CMD_0x0C: prepare_answer_0C(); break;
    case V764_CMD_0x0D: prepare_answer_0D(); break;
    case V764_CMD_0x0E: prepare_answer_0E(); break;
    case V764_CMD_0x0F: prepare_answer_0F(); break;

    case V764_CMD_0x10: prepare_answer_10(); break;
    case V764_CMD_0x11: prepare_answer_11(); break;
    case V764_CMD_0x12: prepare_answer_12(); break;
    case V764_CMD_0x13: prepare_answer_13(); break;
    case V764_CMD_0x14: prepare_answer_14(); break;
    case V764_CMD_0x15: prepare_answer_15(); break;
    case V764_CMD_0x16: prepare_answer_16(); break;
    case V764_CMD_0x17: prepare_answer_17(); break;
    case V764_CMD_0x18: prepare_answer_18(); break;
    case V764_CMD_0x19: prepare_answer_19(); break;
    case V764_CMD_0x1A: prepare_answer_1A(); break;
    case V764_CMD_0x1B: prepare_answer_1C(); break;
    case V764_CMD_0x1C: prepare_answer_1C(); break;

    default:
        emit error(QString("unknown code %1").arg(data.at(1), 2, 16, QChar('0')));
        break;
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::run_timer(void)
{
    Q_CHECK_PTR(timer);
    timer->start(FAKE_DELAY_MS);
}
//--------------------------------------------------------------------------------
template<typename T>
bool Worker_fake_v786::check_question(void)
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
bool Worker_fake_v786::check_question_01(void)
{
    return check_question<V764_2_01_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_02(void)
{
    return check_question<V764_2_02_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_03(void)
{
    return check_question<V764_2_03_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_04(void)
{
    return check_question<V764_2_01_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_05(void)
{
    return check_question<V764_2_05_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_06(void)
{
    return check_question<V764_2_06_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_07(void)
{
    return check_question<V764_2_07_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_08(void)
{
    return check_question<V764_2_08_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_09(void)
{
    return check_question<V764_2_09_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_0A(void)
{
    return check_question<V764_2_0A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_0B(void)
{
    return check_question<V764_2_0B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_0C(void)
{
    return check_question<V764_2_0C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_0D(void)
{
    return check_question<V764_2_0D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_0E(void)
{
    return check_question<V764_2_0E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_0F(void)
{
    return check_question<V764_2_0F_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_10(void)
{
    return check_question<V764_2_10_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_11(void)
{
    return check_question<V764_2_11_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_12(void)
{
    return check_question<V764_2_12_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_13(void)
{
    return check_question<V764_2_13_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_14(void)
{
    return check_question<V764_2_14_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_15(void)
{
    return check_question<V764_2_15_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_16(void)
{
    return check_question<V764_2_16_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_17(void)
{
    return check_question<V764_2_17_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_18(void)
{
    return check_question<V764_2_18_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_19(void)
{
    return check_question<V764_2_19_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_1A(void)
{
    return check_question<V764_2_1A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_1B(void)
{
    return check_question<V764_2_1B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_v786::check_question_1C(void)
{
    return check_question<V764_2_1C_QUESTION>();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_01(void)
{
    if(!check_question_01()) return;

    V764_2_01_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x01;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_02(void)
{
    if(!check_question_02()) return;

    V764_2_02_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x02;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_03(void)
{
    if(!check_question_03()) return;

    V764_2_03_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x03;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_04(void)
{
    if(!check_question_04()) return;

    V764_2_04_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x04;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_05(void)
{
    if(!check_question_05()) return;

    V764_2_05_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x05;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_06(void)
{
    if(!check_question_06()) return;

    V764_2_06_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x06;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_07(void)
{
    if(!check_question_07()) return;

    V764_2_07_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x07;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_08(void)
{
    if(!check_question_08()) return;

    V764_2_08_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x08;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_09(void)
{
    if(!check_question_09()) return;

    V764_2_09_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x09;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_0A(void)
{
    if(!check_question_0A()) return;

    V764_2_0A_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x0A;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_0B(void)
{
    if(!check_question_0B()) return;

    V764_2_0B_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x0B;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_0C(void)
{
    if(!check_question_0C()) return;

    V764_2_0C_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x0C;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_0D(void)
{
    if(!check_question_0D()) return;

    V764_2_0D_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x0D;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_0E(void)
{
    if(!check_question_0E()) return;

    V764_2_0E_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x0E;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_0F(void)
{
    if(!check_question_0F()) return;

    V764_2_0F_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x0F;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_10(void)
{
    if(!check_question_10()) return;

    V764_2_10_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x10;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_11(void)
{
    if(!check_question_11()) return;

    V764_2_11_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x11;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_12(void)
{
    if(!check_question_12()) return;

    V764_2_12_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x12;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_13(void)
{
    if(!check_question_13()) return;

    V764_2_13_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x13;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_14(void)
{
    if(!check_question_14()) return;

    V764_2_14_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x14;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_15(void)
{
    if(!check_question_15()) return;

    V764_2_15_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x15;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_16(void)
{
    if(!check_question_16()) return;

    V764_2_16_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x16;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_17(void)
{
    if(!check_question_17()) return;

    V764_2_17_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x17;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_18(void)
{
    if(!check_question_18()) return;

    V764_2_18_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x18;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_19(void)
{
    if(!check_question_19()) return;

    V764_2_19_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x19;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_1A(void)
{
    if(!check_question_1A()) return;

    V764_2_1A_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x1A;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_1B(void)
{
    if(!check_question_1B()) return;

    V764_2_1B_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x1B;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::prepare_answer_1C(void)
{
    if(!check_question_1C()) return;

    V764_2_1C_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = V764_CMD_0x1C;
    answer.body.header.count_data = sizeof(answer.body.data);


    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const uint8_t *>(answer.buf),
                     sizeof(answer));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_v786::port_open(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_v786::port_close(void)
{

}
//--------------------------------------------------------------------------------
