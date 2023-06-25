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
#include "worker_fake_b590.hpp"
#include "ui_worker_fake_b590.h"
//--------------------------------------------------------------------------------
#include "b590_packets.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
Worker_fake_b590::Worker_fake_b590(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Worker_fake_b590)
{
    init();
}
//--------------------------------------------------------------------------------
Worker_fake_b590::~Worker_fake_b590()
{
    if(timer)
    {
        timer->stop();
        disconnect(timer,  &QTimer::timeout,   this,   &Worker_fake_b590::send_answer);

        delete timer;
    }
    if(fram)
    {
        delete fram;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::init(void)
{
    ui->setupUi(this);

    connect(this,   &Worker_fake_b590::info,     ui->log_widget, &LogBox::infoLog);
    connect(this,   &Worker_fake_b590::debug,    ui->log_widget, &LogBox::debugLog);
    connect(this,   &Worker_fake_b590::error,    ui->log_widget, &LogBox::errorLog);
    connect(this,   &Worker_fake_b590::trace,    ui->log_widget, &LogBox::traceLog);

    ui->sb_address->setRange(MIN_ADDRESS_B590, MAX_ADDRESS_B590);

    //---
    address = ui->sb_address->value();

    ui->sb_noise->setMinimum(0);
    ui->sb_noise->setMaximum(5);
    ui->sb_noise->setObjectName("sb_noise");

    ui->sb_R->setMinimum(0.0000001);
    ui->sb_R->setMaximum(65535);
    ui->sb_R->setSingleStep(0.0000001);
    ui->sb_R->setDecimals(7);
    ui->sb_R->setObjectName("sb_R");

    ui->dsb_temperature->setRange(0, 100);

    ui->sb_R->setValue(0.0100091);

    ui->sb_voltage->setMinimum(MIN_VOLTAGE_B590);
    ui->sb_voltage->setMaximum(MAX_VOLTAGE_B590);
    ui->sb_voltage->setObjectName("sb_voltage");

    ui->sb_current->setMinimum(MIN_CURRENT_B590);
    ui->sb_current->setMaximum(MAX_CURRENT_B590);
    ui->sb_current->setObjectName("sb_current");

    connect(ui->btn_set_noice, &QToolButton::pressed, this, &Worker_fake_b590::regenerate_all);

    regenerate_dac_U_values(0);
    regenerate_dac_I_values(0);
    //---

    QWidgetList wl = qApp->topLevelWidgets();
    foreach (QWidget *w, wl)
    {
        if(strcmp(w->metaObject()->className(), "MainWindow") == 0)
        {
            static_cast<MainWindow *>(w)->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);

            timer = new QTimer(this);
            connect(timer,  &QTimer::timeout,   this,   &Worker_fake_b590::send_answer);

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::send_answer(void)
{
    emit output(ba_output);
    emit readyRead();
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::input(QByteArray data)
{
    if(data.isEmpty()) return;

    ba_input.clear();
    ba_input.append(data);

    if(ba_input.length() < 2) return;
    switch(ba_input.at(1))
    {
    case B590_CMD_0x11: prepare_answer_11(); break;
    case B590_CMD_0x12: prepare_answer_12(); break;
    case B590_CMD_0x15: prepare_answer_15(); break;
    case B590_CMD_0x16: prepare_answer_16(); break;
    case B590_CMD_0x17: prepare_answer_17(); break;

    case B590_CMD_0x41: prepare_answer_41(); break;
    case B590_CMD_0x42: prepare_answer_42(); break;
    case B590_CMD_0x46: prepare_answer_46(); break;
    case B590_CMD_0x47: prepare_answer_47(); break;
    case B590_CMD_0x48: prepare_answer_48(); break;
    case B590_CMD_0x49: prepare_answer_49(); break;
    case B590_CMD_0x4A: prepare_answer_4A(); break;

    case B590_CMD_0x54: prepare_answer_54(); break;
    case B590_CMD_0x55: prepare_answer_55(); break;
    case B590_CMD_0x56: prepare_answer_56(); break;
    case B590_CMD_0x57: prepare_answer_57(); break;
    case B590_CMD_0x5A: prepare_answer_5A(); break;
    case B590_CMD_0x5B: prepare_answer_5B(); break;
    case B590_CMD_0x5C: prepare_answer_5C(); break;
    case B590_CMD_0x5D: prepare_answer_5D(); break;
    case B590_CMD_0x5E: prepare_answer_5E(); break;
    case B590_CMD_0x5F: prepare_answer_5F(); break;

    case B590_CMD_0x61: prepare_answer_61(); break;
    case B590_CMD_0x62: prepare_answer_62(); break;
    case B590_CMD_0x63: prepare_answer_63(); break;
    case B590_CMD_0x64: prepare_answer_64(); break;
    case B590_CMD_0x65: prepare_answer_65(); break;
    case B590_CMD_0x66: prepare_answer_66(); break;
    case B590_CMD_0x67: prepare_answer_67(); break;
    case B590_CMD_0x68: prepare_answer_68(); break;
    case B590_CMD_0x69: prepare_answer_69(); break;
    case B590_CMD_0x6A: prepare_answer_6A(); break;
    case B590_CMD_0x6B: prepare_answer_6B(); break;
    case B590_CMD_0x6C: prepare_answer_6C(); break;
    case B590_CMD_0x6D: prepare_answer_6D(); break;
    case B590_CMD_0x6E: prepare_answer_6E(); break;
    case B590_CMD_0x6F: prepare_answer_6F(); break;

    case B590_CMD_0x71: prepare_answer_71(); break;
    case B590_CMD_0x72: prepare_answer_72(); break;
    case B590_CMD_0x73: prepare_answer_73(); break;
    case B590_CMD_0x75: prepare_answer_75(); break;
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::run_timer(void)
{
    Q_ASSERT(timer);
    timer->start(FAKE_DELAY_MS);
}
//--------------------------------------------------------------------------------
template<typename T>
bool Worker_fake_b590::check_question(void)
{
    T *question = reinterpret_cast<T *>(ba_input.data());

    uint16_t crc = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&question->buf),
                                          sizeof(T) - 2);
    if(crc != question->body.crc16)
    {
        emit error(QString("%1:bad crc").arg(Q_FUNC_INFO));
        emit error(QString("crc 0x%1").arg(crc, 2, 16, QChar('0')));
        emit error(QString("question.crc 0x%1").arg(question->body.crc16, 4, 16, QChar('0')));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_11(void)
{
    return check_question<B590_CMD_11_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_12(void)
{
    return check_question<B590_CMD_12_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_13(void)
{
    return check_question<B590_CMD_13_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_14(void)
{
    return check_question<B590_CMD_14_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_15(void)
{
    return check_question<B590_CMD_15_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_16(void)
{
    return check_question<B590_CMD_16_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_17(void)
{
    return check_question<B590_CMD_17_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_41(void)
{
    return check_question<B590_CMD_41_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_42(void)
{
    return check_question<B590_CMD_42_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_46(void)
{
    return check_question<B590_CMD_46_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_47(void)
{
    return check_question<B590_CMD_47_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_48(void)
{
    return check_question<B590_CMD_48_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_49(void)
{
    return check_question<B590_CMD_49_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_4A(void)
{
    return check_question<B590_CMD_4A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_54(void)
{
    return check_question<B590_CMD_54_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_55(void)
{
    return check_question<B590_CMD_55_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_56(void)
{
    return check_question<B590_CMD_56_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_57(void)
{
    return check_question<B590_CMD_57_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_5A(void)
{
    return check_question<B590_CMD_5A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_5B(void)
{
    return check_question<B590_CMD_5B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_5C(void)
{
    return check_question<B590_CMD_5C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_5D(void)
{
    return check_question<B590_CMD_5D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_5E(void)
{
    return check_question<B590_CMD_5E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_5F(void)
{
    return check_question<B590_CMD_5F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_61(void)
{
    return check_question<B590_CMD_61_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_62(void)
{
    return check_question<B590_CMD_62_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_63(void)
{
    return check_question<B590_CMD_63_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_64(void)
{
    return check_question<B590_CMD_64_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_65(void)
{
    return check_question<B590_CMD_65_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_66(void)
{
    return check_question<B590_CMD_66_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_67(void)
{
    return check_question<B590_CMD_67_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_68(void)
{
    return check_question<B590_CMD_68_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_69(void)
{
    return check_question<B590_CMD_69_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_6A(void)
{
    return check_question<B590_CMD_6A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_6B(void)
{
    return check_question<B590_CMD_6B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_6C(void)
{
    return check_question<B590_CMD_6C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_6D(void)
{
    return check_question<B590_CMD_6D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_6E(void)
{
    return check_question<B590_CMD_6E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_6F(void)
{
    return check_question<B590_CMD_6F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_71(void)
{
    return check_question<B590_CMD_71_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_72(void)
{
    return check_question<B590_CMD_72_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_73(void)
{
    return check_question<B590_CMD_73_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::check_question_75(void)
{
    return check_question<B590_CMD_75_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_11(void)
{
    if(!check_question_11()) return;

    B590_CMD_11_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x11;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_11_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(B590_CMD_11_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_12(void)
{
    if(!check_question_12()) return;

    B590_CMD_12_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x12;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_12_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(B590_CMD_12_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_15(void)
{
    if(!check_question_15()) return;

    B590_CMD_15_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x15;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_15_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_15_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_16(void)
{
    if(!check_question_16()) return;

    B590_CMD_16_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x16;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_16_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_16_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_17(void)
{
    if(!check_question_17()) return;

    B590_CMD_17_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x17;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_17_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_17_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_41(void)
{
    if(!check_question_41()) return;

    B590_CMD_41_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x41;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_41_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_41_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_42(void)
{
    if(!check_question_42()) return;

    B590_CMD_42_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x42;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_42_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_42_ANSWER));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_46(void)
{
    if(!check_question_46()) return;

    B590_CMD_46_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x46;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.type  = 4;
    answer.body.data.year  = 1;
    answer.body.data.month = 1;
    answer.body.data.serno = 123;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_46_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_46_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_47(void)
{
    if(!check_question_47()) return;

    B590_CMD_47_ANSWER answer;

    calculate(ui->sb_voltage->value(),
              static_cast<uint16_t>(ui->sb_current->value()));
    Connection::set_value(ui->sb_voltage->value());

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x47;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.in_voltage  = ui->sb_voltage->value();
    answer.body.data.in_current  = static_cast<uint16_t>(ui->sb_current->value());
    answer.body.data.out_voltage = ui->sb_voltage->value();
    answer.body.data.out_current = static_cast<uint16_t>(ui->sb_current->value());
    answer.body.data.temperature = static_cast<int8_t>(ui->dsb_temperature->value());
    answer.body.data.current_point = static_cast<uint8_t>(dac_point);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_47_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_47_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_48(void)
{
    if(!check_question_48()) return;

    B590_CMD_48_QUESTION *question = reinterpret_cast<B590_CMD_48_QUESTION *>(ba_input.data());
    B590_CMD_48_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x48;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.new_address_modbus = question->body.data.new_address_modbus;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_48_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_48_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_49(void)
{
    if(!check_question_49()) return;

    B590_CMD_49_QUESTION *question = reinterpret_cast<B590_CMD_49_QUESTION *>(ba_input.data());
    B590_CMD_49_ANSWER answer;

    calculate(question->body.data.voltage,
              question->body.data.current);
    Connection::set_value(ui->sb_voltage->value());

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x49;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.data.error = 0;
    answer.body.data.voltage = ui->sb_voltage->value();
    answer.body.data.current = static_cast<uint16_t>(ui->sb_current->value());
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_49_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_49_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_4A(void)
{
    if(!check_question_4A()) return;

    B590_CMD_4A_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x4A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_bits = 0;
    answer.body.data.error_code = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_4A_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                     sizeof(B590_CMD_4A_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_54(void)
{
    if(!check_question_54()) return;

    B590_CMD_54_QUESTION *question = reinterpret_cast<B590_CMD_54_QUESTION *>(ba_input.data());
    B590_CMD_54_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x54;
    answer.body.header.count_data = sizeof(answer.body.data);

    int number_profile = question->body.data.profile;
    uint8_t count_point = 0;
    uint8_t count_repeat_profile = 0;
    bool ok = fram->get_profile(static_cast<uint16_t>(number_profile),
                                &count_point,
                                &count_repeat_profile);

    answer.body.data.profile = static_cast<uint8_t>(number_profile);
    if(ok)
    {
        answer.body.data.count_point = count_point;
        answer.body.data.count_repeat_profile = count_repeat_profile;
    }

    answer.body.data.error = ok ? 0 : 1;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_54_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_54_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_55(void)
{
    if(!check_question_55()) return;

    B590_CMD_55_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x55;
    answer.body.header.count_data = sizeof(answer.body.data);

    uint32_t time = 0;
    bool ok = fram->get_moto_time(&time);
    if(ok)
    {
        answer.body.data.mototime_min = time;
    }

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_55_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_55_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_56(void)
{
    if(!check_question_56()) return;

    B590_CMD_56_QUESTION *question = reinterpret_cast<B590_CMD_56_QUESTION *>(ba_input.data());
    B590_CMD_56_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x56;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;

    answer.body.data.point = question->body.data.point;
    answer.body.data.profile = question->body.data.profile;

    s_point point;
    bool ok = fram->get_profile_point(question->body.data.profile,
                                      question->body.data.point,
                                      &point);

    if(ok)
    {
        answer.body.data.voltage = static_cast<uint16_t>(point.voltage);
        answer.body.data.current = static_cast<uint16_t>(point.current);
        answer.body.data.time    = point.time;
    }

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_56_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_56_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_57(void)
{
    if(!check_question_57()) return;

    B590_CMD_57_QUESTION *question = reinterpret_cast<B590_CMD_57_QUESTION *>(ba_input.data());
    B590_CMD_57_ANSWER answer;

    calculate(question->body.data.U,
              question->body.data.I);
    Connection::set_value(ui->sb_voltage->value());

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x57;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.I = static_cast<uint16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_57_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_57_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_5A(void)
{
    if(!check_question_5A()) return;

    B590_CMD_5A_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x5A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.active_point = 0;
    answer.body.data.active_profile = 0;
    answer.body.data.current_ADC_I = 0;
    answer.body.data.current_ADC_U = 0;
    answer.body.data.setting_DAC_I = 0;
    answer.body.data.setting_DAC_U = 0;
    answer.body.data.temperature = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_5A_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_5A_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_5B(void)
{
    if(!check_question_5B()) return;

    B590_CMD_5B_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x5B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.ADC_I = 0;
    answer.body.data.ADC_U = 0;
    answer.body.data.ADC_U_input = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_5B_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_5B_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_5C(void)
{
    if(!check_question_5C()) return;

    B590_CMD_5C_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x5C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_5C_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_5C_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_5D(void)
{
    if(!check_question_5D()) return;

    B590_CMD_5D_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x5D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.zero_U = zero_U;
    answer.body.data.zero_I = zero_I;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_5D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_5D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_5E(void)
{
    if(!check_question_5E()) return;

    B590_CMD_5E_QUESTION *question = reinterpret_cast<B590_CMD_5E_QUESTION *>(ba_input.data());
    B590_CMD_5E_ANSWER answer;
    s_point point;

    point.voltage = question->body.data.voltage;
    point.current = question->body.data.current;
    point.time    = question->body.data.time;

    bool ok = fram->add_profile_point(question->body.data.profile, point);

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x5E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = ok ? 0 : 1;

    answer.body.data.profile = question->body.data.profile;

    answer.body.data.voltage = static_cast<uint16_t>(point.voltage);
    answer.body.data.current = point.current;
    answer.body.data.time    = point.time;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_5E_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_5E_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_5F(void)
{
    if(!check_question_5F()) return;

    B590_CMD_5F_QUESTION *question = reinterpret_cast<B590_CMD_5F_QUESTION *>(ba_input.data());
    B590_CMD_5F_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x5F;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.number_profile = question->body.data.number_profile;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_5F_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_5F_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_61(void)
{
    if(!check_question_61()) return;

    B590_CMD_61_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x61;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_ADC_U_0 = zero_U.Value_ADC_U[0];
    answer.body.data.factor_k_DAC = static_cast<float>(factor_k_DAC);
    answer.body.data.factor_k_ADC = static_cast<float>(factor_k_ADC);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_61_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_61_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_62(void)
{
    if(!check_question_62()) return;

    B590_CMD_62_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x62;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_ADC_I_0  = zero_I.Value_ADC_I[0];
    answer.body.data.factor_k_DAC = static_cast<float>(factor_k_DAC);
    answer.body.data.factor_k_ADC = static_cast<float>(factor_k_ADC);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_62_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_62_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_63(void)
{
    if(!check_question_63()) return;

    B590_CMD_63_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x63;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_point = 0;
    answer.body.data.number_point = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_63_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_63_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
/*
  TASK_CALIBRATION_ADC_U = 1,   // Запуск калибровки АЦП по напряжению
  TASK_CALIBRATION_ADC_I = 2,   // Запуск калибровки АЦП по току
  TASK_CALIBRATION_DAC_U = 3,   // Запуск калибровки ЦАП по напряжению
  TASK_CALIBRATION_DAC_I = 4,   // Запуск калибровки ЦАП по току
  ASK_SLP =  5,                 // Режим SLP
*/
void Worker_fake_b590::prepare_answer_64(void)
{
    if(!check_question_64()) return;

    B590_CMD_64_QUESTION *question = reinterpret_cast<B590_CMD_64_QUESTION *>(ba_input.data());
    B590_CMD_64_ANSWER answer;

    dac_point = 0;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x64;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.code = question->body.data.code;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_64_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_64_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_65(void)
{
    if(!check_question_65()) return;

    B590_CMD_65_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x65;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.number_current_point = static_cast<uint8_t>(dac_point);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_65_ANSWER) - 2);

    dac_point++;

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_65_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_66(void)
{
    if(!check_question_66()) return;

    B590_CMD_66_QUESTION *question = reinterpret_cast<B590_CMD_66_QUESTION *>(ba_input.data());
    B590_CMD_66_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x66;
    answer.body.header.count_data = sizeof(answer.body.data);

    uint8_t index = question->body.data.index_point;
    uint16_t value = 0;
    switch(question->body.data.code)
    {
    case 0:
        value = dac_U[index];
        break;

    case 1:
        value = dac_I[index];
        break;
    }

    answer.body.data.points_crc16 = 0;
    answer.body.data.point_value  = value;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_66_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_66_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_67(void)
{
    if(!check_question_67()) return;

    B590_CMD_67_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x67;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.speed_cooler = 1600;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_67_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_67_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_68(void)
{
    if(!check_question_68()) return;

    B590_CMD_68_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x68;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_PWM = 1600;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_68_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_68_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_69(void)
{
    if(!check_question_69()) return;

    B590_CMD_69_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x69;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.OCR3A = 0;
    answer.body.data.OCR3B = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_69_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_69_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_6A(void)
{
    if(!check_question_6A()) return;

    B590_CMD_6A_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_6A_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6A_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_6B(void)
{
    if(!check_question_6B()) return;

    B590_CMD_6B_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_6B_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6B_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_6C(void)
{
    if(!check_question_6C()) return;

    B590_CMD_6C_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_compile_day = 0;
    answer.body.data.data_compile_month = 0;
    answer.body.data.data_compile_year = 0;
    answer.body.data.time_compile_hour = 0;
    answer.body.data.time_compile_min = 0;
    answer.body.data.time_compile_sec = 0;
    answer.body.data.version = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_6C_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6C_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_6D(void)
{
    if(!check_question_6D()) return;

    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());

    //---
    switch(question->body.data.secret_cmd)
    {
    case SECRET_CMD_GET_ID:                 secret_CMD_GET_ID();                break;
    case SECRET_CMD_RESET:                  secret_CMD_RESET();                 break;
    case SECRET_CMD_CLEAR_MOTOHOUR:         secret_CMD_CLEAR_MOTOHOUR();        break;
    case SECRET_CMD_SAVE_DATE_AND_SERNO:    secret_CMD_SAVE_DATE_AND_SERNO();   break;
    case SECRET_CMD_COPY_USER_TO_FACTORY:   secret_CMD_COPY_USER_TO_FACTORY();  break;

    case SECRET_CMD_CLEAR_FRAM:             secret_CMD_CLEAR_FRAM();            break;

    case SECRET_CMD_READ_FRAM_8BIT:         secret_CMD_READ_FRAM_8BIT();        break;
    case SECRET_CMD_READ_FRAM_16BIT:        secret_CMD_READ_FRAM_16BIT();       break;
    case SECRET_CMD_READ_FRAM_32BIT:        secret_CMD_READ_FRAM_32BIT();       break;
    case SECRET_CMD_READ_FRAM_MORE_BYTES:   secret_CMD_READ_FRAM_MORE_BYTES();  break;

    case SECRET_CMD_SAVE_FRAM_8BIT:         secret_CMD_SAVE_FRAM_8BIT();        break;
    case SECRET_CMD_SAVE_FRAM_16BIT:        secret_CMD_SAVE_FRAM_16BIT();       break;
    case SECRET_CMD_SAVE_FRAM_32BIT:        secret_CMD_SAVE_FRAM_32BIT();       break;
    case SECRET_CMD_SAVE_FRAM_MORE_BYTES:   secret_CMD_SAVE_FRAM_MORE_BYTES();  break;

    default:
        emit error(QString("unknown secret_cmd 0x%1").arg(question->body.data.secret_cmd, 0, 16));
        break;
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_6E(void)
{
    if(!check_question_6E()) return;
    B590_CMD_6E_ANSWER answer;

    //---
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_6E_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6E_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_6F(void)
{
    if(!check_question_6F()) return;

    B590_CMD_6F_QUESTION *question = reinterpret_cast<B590_CMD_6F_QUESTION *>(ba_input.data());
    B590_CMD_6F_ANSWER answer;

    //---
    clear_profile(question->body.data.profile);
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6F;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.profile = question->body.data.profile;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6F_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6F_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_71(void)
{
    if(!check_question_71()) return;

    B590_CMD_71_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x71;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.state_ADC_I  = 0;
    answer.body.data.state_ADC_U  = 0;
    answer.body.data.state_ADC_Ui = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_71_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_71_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_72(void)
{
    if(!check_question_72()) return;

    B590_CMD_72_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x72;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.conf_ADC = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_72_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_72_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_73(void)
{
    if(!check_question_73()) return;

    B590_CMD_73_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x73;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.real_DAC_I = 0;
    answer.body.data.real_DAC_U = 0;
    answer.body.data.revers = 0;

    //*temp = (float)((float)temperature / (float)16.0f - 200.0f) / (float)4.0f;
    answer.body.data.temperature = static_cast<uint16_t>((ui->dsb_temperature->value() * 4.0 + 200.0) * 16.0);  //???

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_73_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_73_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::prepare_answer_75(void)
{
    if(!check_question_75()) return;

    B590_CMD_75_ANSWER answer;

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x75;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.count_Error_ADC = 0;
    answer.body.data.count_Error_Answer_ADC = 0;
    answer.body.data.count_ReStart_ADC_0 = 0;
    answer.body.data.count_ReStart_ADC_1 = 0;
    answer.body.data.count_ReStart_ADC_2 = 0;
    answer.body.data.MCUCSR = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_75_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_75_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_GET_ID(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_GET_ID fram_id;
    fram_id.data.ID = 0x55;

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = fram_id.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_RESET(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_CLEAR_MOTOHOUR(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_CLEAR_FRAM(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_READ_FRAM_8BIT(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_8BIT fram_8bit;
    unsigned int len = sizeof(DATA_FRAM_8BIT);
    for(unsigned int n=0; n<len; n++)
    {
        fram_8bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->read_data_8(fram_8bit.data.address, &fram_8bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_8bit.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf), sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_READ_FRAM_16BIT(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_16BIT fram_16bit;
    unsigned int len = sizeof(DATA_FRAM_16BIT);
    for(unsigned int n=0; n<len; n++)
    {
        fram_16bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->read_data_16(fram_16bit.data.address, &fram_16bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_16bit.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_READ_FRAM_32BIT(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_32BIT fram_32bit;
    unsigned int len = sizeof(DATA_FRAM_32BIT);
    for(unsigned int n=0; n<len; n++)
    {
        fram_32bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->read_data_32(fram_32bit.data.address, &fram_32bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_32bit.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_READ_FRAM_MORE_BYTES(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_MORE_BYTES fram_more_bytes;
    unsigned int len = sizeof(DATA_FRAM_MORE_BYTES);
    for(unsigned int n=0; n<len; n++)
    {
        fram_more_bytes.buf[n] = question->body.data.secret_data[n];
    }

    uint16_t address = fram_more_bytes.data.address;
    unsigned int len_data = fram_more_bytes.data.data_len;
    uint8_t value = 0;
    for(unsigned int n=0; n<len_data; n++)
    {
        bool ok = fram->read_data_8(static_cast<uint16_t>(address+n),
                                    &value);
        if(ok)
        {
            fram_more_bytes.data.fram_data[n] = value;
        }
    }

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = fram_more_bytes.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_SAVE_FRAM_8BIT(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_8BIT save_fram_8bit;
    unsigned int len = sizeof(DATA_FRAM_8BIT);
    for(unsigned int n=0; n<len; n++)
    {
        save_fram_8bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->write_data_8(save_fram_8bit.data.address, save_fram_8bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_8bit.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_SAVE_FRAM_16BIT(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_16BIT save_fram_16bit;
    unsigned int len = sizeof(DATA_FRAM_16BIT);
    for(unsigned int n=0; n<len; n++)
    {
        save_fram_16bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->write_data_16(save_fram_16bit.data.address, save_fram_16bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_16bit.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_SAVE_FRAM_32BIT(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_32BIT save_fram_32bit;
    unsigned int len = sizeof(DATA_FRAM_32BIT);
    for(unsigned int n=0; n<len; n++)
    {
        save_fram_32bit.buf[n] = question->body.data.secret_data[n];
    }

    fram->write_data_32(save_fram_32bit.data.address, save_fram_32bit.data.value);

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_32bit.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_SAVE_FRAM_MORE_BYTES(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_MORE_BYTES save_fram_more_bytes;
    unsigned int len = sizeof(DATA_FRAM_MORE_BYTES);
    uint16_t address = save_fram_more_bytes.data.address;
    for(unsigned int n=0; n<len; n++)
    {
        fram->write_data_8(static_cast<uint16_t>(address+n),
                           question->body.data.secret_data[n]);
    }

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_more_bytes.buf[n];
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_SAVE_DATE_AND_SERNO(void)
{
    B590_CMD_6D_QUESTION *question = reinterpret_cast<B590_CMD_6D_QUESTION *>(ba_input.data());
    B590_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::secret_CMD_COPY_USER_TO_FACTORY(void)
{
    B590_CMD_6D_ANSWER answer;

    //---
    fram->copy_user_to_factory_calibration();
    //---

    answer.body.header.address = static_cast<uint8_t>(address);
    answer.body.header.cmd = B590_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                                               sizeof(B590_CMD_6D_ANSWER) - 2);

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf), sizeof(B590_CMD_6D_ANSWER));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::clear_profile(int number_profile)
{
    if(number_profile > MAX_PROFILE_B590) return false;

    for(int n=MIN_POINT_B590; n<MAX_POINT_B590; n++)
    {
        s_point point;
        point.voltage = 0;
        point.current = 0;
        point.time = 0;

        bool ok = fram->add_profile_point(static_cast<uint16_t>(number_profile),
                                     point);
        if(!ok) return ok;
    }
    return true;
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::regenerate_dac_U_values(int noise_value)
{
#if 1
    Q_UNUSED(noise_value)
    dac_U[0]=65;
    dac_U[1]=1057;
    dac_U[2]=2031;
    dac_U[3]=3006;
    dac_U[4]=3980;
    dac_U[5]=4955;
    dac_U[6]=5930;
    dac_U[7]=6905;
    dac_U[8]=7880;
    dac_U[9]=8854;
    dac_U[10]=9830;
    dac_U[11]=10805;
    dac_U[12]=11782;
    dac_U[13]=12757;
    dac_U[14]=13730;
    dac_U[15]=14705;
    dac_U[16]=15680;
    dac_U[17]=16654;
    dac_U[18]=17629;
    dac_U[19]=18605;
    dac_U[20]=19580;
    dac_U[21]=20554;
    dac_U[22]=21529;
    dac_U[23]=22504;
    dac_U[24]=23479;
    dac_U[25]=24454;
    dac_U[26]=25430;
    dac_U[27]=26406;
    dac_U[28]=27381;
    dac_U[29]=28358;
    dac_U[30]=29333;
    dac_U[31]=30307;
    dac_U[32]=31282;
    dac_U[33]=32260;
    dac_U[34]=33235;
    dac_U[35]=34210;
    dac_U[36]=35185;
    dac_U[37]=36160;
    dac_U[38]=37134;
    dac_U[39]=38109;
    dac_U[40]=39084;
    dac_U[41]=40060;
    dac_U[42]=41034;
    dac_U[43]=42010;
    dac_U[44]=42985;
    dac_U[45]=43960;
    dac_U[46]=44934;
    dac_U[47]=45908;
    dac_U[48]=46883;
    dac_U[49]=47858;
    dac_U[50]=48832;
    dac_U[51]=49808;
    dac_U[52]=50782;
    dac_U[53]=51757;
    dac_U[54]=52731;
    dac_U[55]=53706;
    dac_U[56]=54681;
    dac_U[57]=55656;
    dac_U[58]=56631;
    dac_U[59]=57606;
    dac_U[60]=58581;
#else
    QRandomGenerator generator;
    for(uint16_t n=0; n<0xFFFF; n++)
    {
        if(noise_value == 0)
        {
            dac_U[n] = n;
        }
        else
        {
            int noise = generator.generate() % noise_value;
            int new_value = n + noise;
            if(new_value > 0xFFFF) new_value = 0xFFFF;
            dac_U[n] = (uint16_t)new_value;
        }
    }
#endif
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::regenerate_dac_I_values(int noise_value)
{
#if 1
    Q_UNUSED(noise_value)
    dac_I[0]=235;
    dac_I[1]=1039;
    dac_I[2]=1845;
    dac_I[3]=2651;
    dac_I[4]=3457;
    dac_I[5]=4263;
    dac_I[6]=5069;
    dac_I[7]=5877;
    dac_I[8]=6687;
    dac_I[9]=7495;
    dac_I[10]=8303;
    dac_I[11]=9112;
    dac_I[12]=9923;
    dac_I[13]=10733;
    dac_I[14]=11540;
    dac_I[15]=12350;
    dac_I[16]=13160;
    dac_I[17]=13969;
    dac_I[18]=14777;
    dac_I[19]=15585;
    dac_I[20]=16393;
    dac_I[21]=17201;
    dac_I[22]=18009;
    dac_I[23]=18817;
    dac_I[24]=19625;
    dac_I[25]=20435;
    dac_I[26]=21244;
    dac_I[27]=22052;
    dac_I[28]=22860;
    dac_I[29]=23668;
    dac_I[30]=24476;
    dac_I[31]=25284;
    dac_I[32]=26092;
    dac_I[33]=26900;
    dac_I[34]=27710;
    dac_I[35]=28520;
    dac_I[36]=29328;
    dac_I[37]=30136;
    dac_I[38]=30945;
    dac_I[39]=31753;
    dac_I[40]=32561;
    dac_I[41]=33369;
    dac_I[42]=34178;
    dac_I[43]=34985;
    dac_I[44]=35794;
    dac_I[45]=36602;
    dac_I[46]=37410;
    dac_I[47]=38218;
    dac_I[48]=39024;
    dac_I[49]=39832;
    dac_I[50]=40640;
    dac_I[51]=41448;
    dac_I[52]=42255;
    dac_I[53]=43062;
    dac_I[54]=43870;
    dac_I[55]=44676;
    dac_I[56]=45484;
    dac_I[57]=46292;
    dac_I[58]=47098;
    dac_I[59]=47906;
    dac_I[60]=48714;
#else
    QRandomGenerator generator;
    for(uint16_t n=0; n<0xFFFF; n++)
    {
        if(noise_value == 0)
        {
            dac_I[n] = n;
        }
        else
        {
            int noise = generator.generate() % noise_value;
            int new_value = n + noise;
            if(new_value > 0xFFFF) new_value = 0xFFFF;
            dac_I[n] = (uint16_t)new_value;
        }
    }
#endif
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::regenerate_all(void)
{
    int noise = ui->sb_noise->value();
    regenerate_dac_U_values(noise);
    regenerate_dac_I_values(noise);
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::calculate(int32_t  new_setting_U,
                                 uint16_t new_setting_I)
{
    // все значения в mV и mA
//    ui->sb_voltage->setValue(new_setting_U);
//    ui->sb_current->setValue(new_setting_I);

    if(new_setting_U < 0)
    {
        new_setting_U *= (-1);
    }

    double R = ui->sb_R->value();
    double U = new_setting_U;
    double I = static_cast<double>(new_setting_U / R);
    if(I > static_cast<double>(new_setting_I))
    {
        U = static_cast<double>(new_setting_I * R);
        I = new_setting_I;
    }

    ui->sb_voltage->setValue(static_cast<int>(U / 1000.0));
    ui->sb_current->setValue(static_cast<int>(I / 1000.0));
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::port_open(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_b590::port_close(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_b590::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Worker_fake_b590::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Worker_fake_b590::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_b590::save_setting(void)
{

}
//--------------------------------------------------------------------------------
