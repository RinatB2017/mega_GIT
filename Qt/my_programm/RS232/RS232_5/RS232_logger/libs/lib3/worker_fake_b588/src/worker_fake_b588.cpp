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
#include "worker_fake_b588.hpp"
#include "b588_powersupply.hpp"
#include "ui_worker_fake_b588.h"
//--------------------------------------------------------------------------------
#include "connection.hpp"
#include "b588_packets.hpp"
#include "B588_fram.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Worker_fake_b588::Worker_fake_b588(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Worker_fake_b588)
{
    init();
}
//--------------------------------------------------------------------------------
Worker_fake_b588::~Worker_fake_b588()
{
    if(timer)
    {
        timer->stop();
        disconnect(timer, &QTimer::timeout, this, &Worker_fake_b588::send_answer);

        delete timer;
    }
    if(fram)
    {
        delete fram;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::init(void)
{
    ui->setupUi(this);

    connect(this,   &Worker_fake_b588::info,     ui->log_widget, &LogBox::infoLog);
    connect(this,   &Worker_fake_b588::debug,    ui->log_widget, &LogBox::debugLog);
    connect(this,   &Worker_fake_b588::error,    ui->log_widget, &LogBox::errorLog);
    connect(this,   &Worker_fake_b588::trace,    ui->log_widget, &LogBox::traceLog);

    ui->sb_address->setRange(MIN_ADDRESS_B588, MAX_ADDRESS_B588);

    QWidgetList wl = qApp->topLevelWidgets();
    foreach (QWidget *w, wl)
    {
        if(strcmp(w->metaObject()->className(), "MainWindow") == 0)
        {
            static_cast<MainWindow *>(w)->add_dock_widget("FAKE log", "fake_log_dock",  Qt::BottomDockWidgetArea, this);

            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, &Worker_fake_b588::send_answer);

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::send_answer(void)
{
    emit output(ba_output);
    emit readyRead();
    if(timer)
    {
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::input(QByteArray data)
{
    if(data.isEmpty()) return;

    ba_input.clear();
    ba_input.append(data);

    if(data.length() < 2) return;
    switch(data.at(1))
    {
    case B588_CMD_0x01: prepare_answer_01(); break;
    case B588_CMD_0x02: prepare_answer_02(); break;
    case B588_CMD_0x03: prepare_answer_03(); break;
    case B588_CMD_0x04: prepare_answer_04(); break;
    case B588_CMD_0x05: prepare_answer_05(); break;
    case B588_CMD_0x06: prepare_answer_06(); break;
    case B588_CMD_0x07: prepare_answer_07(); break;
    case B588_CMD_0x08: prepare_answer_08(); break;
    case B588_CMD_0x09: prepare_answer_09(); break;
    case B588_CMD_0x0A: prepare_answer_0A(); break;
    case B588_CMD_0x0B: prepare_answer_0B(); break;
    case B588_CMD_0x0C: prepare_answer_0C(); break;
    case B588_CMD_0x0D: prepare_answer_0D(); break;
    case B588_CMD_0x0E: prepare_answer_0E(); break;
    case B588_CMD_0x0F: prepare_answer_0F(); break;

    case B588_CMD_0x11: prepare_answer_11(); break;
    case B588_CMD_0x12: prepare_answer_12(); break;
    case B588_CMD_0x13: prepare_answer_13(); break;
    case B588_CMD_0x14: prepare_answer_14(); break;
    case B588_CMD_0x15: prepare_answer_15(); break;
    case B588_CMD_0x16: prepare_answer_16(); break;
    case B588_CMD_0x17: prepare_answer_17(); break;

    case B588_CMD_0x28: prepare_answer_28(); break;
    case B588_CMD_0x29: prepare_answer_29(); break;
    case B588_CMD_0x2A: prepare_answer_2A(); break;
    case B588_CMD_0x2B: prepare_answer_2B(); break;
    case B588_CMD_0x2C: prepare_answer_2C(); break;
    case B588_CMD_0x2D: prepare_answer_2D(); break;
    case B588_CMD_0x2E: prepare_answer_2E(); break;
    case B588_CMD_0x2F: prepare_answer_2F(); break;

    case B588_CMD_0x31: prepare_answer_31(); break;
    case B588_CMD_0x32: prepare_answer_32(); break;
    case B588_CMD_0x33: prepare_answer_33(); break;
    case B588_CMD_0x34: prepare_answer_34(); break;
    case B588_CMD_0x35: prepare_answer_35(); break;
    case B588_CMD_0x36: prepare_answer_36(); break;
    case B588_CMD_0x37: prepare_answer_37(); break;
    case B588_CMD_0x38: prepare_answer_38(); break;
    case B588_CMD_0x39: prepare_answer_39(); break;
    case B588_CMD_0x3A: prepare_answer_3A(); break;
    case B588_CMD_0x3B: prepare_answer_3B(); break;
    case B588_CMD_0x3C: prepare_answer_3C(); break;
    case B588_CMD_0x3D: prepare_answer_3D(); break;
    case B588_CMD_0x3E: prepare_answer_3E(); break;
    case B588_CMD_0x3F: prepare_answer_3F(); break;

    case B588_CMD_0x41: prepare_answer_41(); break;
    case B588_CMD_0x42: prepare_answer_42(); break;
    case B588_CMD_0x43: prepare_answer_43(); break;
    case B588_CMD_0x44: prepare_answer_44(); break;
    case B588_CMD_0x46: prepare_answer_46(); break;
    case B588_CMD_0x47: prepare_answer_47(); break;
    case B588_CMD_0x48: prepare_answer_48(); break;
    case B588_CMD_0x49: prepare_answer_49(); break;
    case B588_CMD_0x4A: prepare_answer_4A(); break;

    case B588_CMD_0x54: prepare_answer_54(); break;
    case B588_CMD_0x55: prepare_answer_55(); break;
    case B588_CMD_0x56: prepare_answer_56(); break;
    case B588_CMD_0x57: prepare_answer_57(); break;
    case B588_CMD_0x58: prepare_answer_58(); break;
    case B588_CMD_0x59: prepare_answer_59(); break;
    case B588_CMD_0x5A: prepare_answer_5A(); break;
    case B588_CMD_0x5B: prepare_answer_5B(); break;
    case B588_CMD_0x5C: prepare_answer_5C(); break;
    case B588_CMD_0x5D: prepare_answer_5D(); break;
    case B588_CMD_0x5E: prepare_answer_5E(); break;
    case B588_CMD_0x5F: prepare_answer_5F(); break;

    case B588_CMD_0x61: prepare_answer_61(); break;
    case B588_CMD_0x62: prepare_answer_62(); break;
    case B588_CMD_0x63: prepare_answer_63(); break;
    case B588_CMD_0x64: prepare_answer_64(); break;
    case B588_CMD_0x65: prepare_answer_65(); break;
    case B588_CMD_0x66: prepare_answer_66(); break;
    case B588_CMD_0x67: prepare_answer_67(); break;
    case B588_CMD_0x68: prepare_answer_68(); break;
    case B588_CMD_0x69: prepare_answer_69(); break;
    case B588_CMD_0x6A: prepare_answer_6A(); break;
    case B588_CMD_0x6B: prepare_answer_6B(); break;
    case B588_CMD_0x6C: prepare_answer_6C(); break;
    case B588_CMD_0x6D: prepare_answer_6D(); break;
    case B588_CMD_0x6E: prepare_answer_6E(); break;
    case B588_CMD_0x6F: prepare_answer_6F(); break;

    case B588_CMD_0x71: prepare_answer_71(); break;
    case B588_CMD_0x72: prepare_answer_72(); break;
    case B588_CMD_0x73: prepare_answer_73(); break;
    case B588_CMD_0x75: prepare_answer_75(); break;
    }
}
//--------------------------------------------------------------------------------
template<typename T>
bool Worker_fake_b588::check_question(void)
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
bool Worker_fake_b588::check_question_01(void)
{
    return check_question<B588_CMD_01_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_02(void)
{
    return check_question<B588_CMD_02_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_03(void)
{
    return check_question<B588_CMD_03_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_04(void)
{
    return check_question<B588_CMD_04_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_05(void)
{
    return check_question<B588_CMD_05_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_06(void)
{
    return check_question<B588_CMD_06_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_07(void)
{
    return check_question<B588_CMD_07_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_08(void)
{
    return check_question<B588_CMD_08_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_09(void)
{
    return check_question<B588_CMD_09_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_0A(void)
{
    return check_question<B588_CMD_0A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_0B(void)
{
    return check_question<B588_CMD_0B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_0C(void)
{
    return check_question<B588_CMD_0C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_0D(void)
{
    return check_question<B588_CMD_0D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_0E(void)
{
    return check_question<B588_CMD_0E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_0F(void)
{
    return check_question<B588_CMD_0F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_11(void)
{
    return check_question<B588_CMD_11_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_12(void)
{
    return check_question<B588_CMD_12_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_13(void)
{
    return check_question<B588_CMD_13_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_14(void)
{
    return check_question<B588_CMD_14_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_15(void)
{
    return check_question<B588_CMD_15_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_16(void)
{
    return check_question<B588_CMD_16_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_17(void)
{
    return check_question<B588_CMD_17_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_20(void)
{
    return check_question<B588_CMD_20_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_21(void)
{
    return check_question<B588_CMD_21_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_22(void)
{
    return check_question<B588_CMD_22_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_23(void)
{
    return check_question<B588_CMD_23_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_24(void)
{
    return check_question<B588_CMD_24_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_25(void)
{
    return check_question<B588_CMD_25_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_26(void)
{
    return check_question<B588_CMD_26_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_27(void)
{
    return check_question<B588_CMD_27_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_28(void)
{
    return check_question<B588_CMD_28_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_29(void)
{
    return check_question<B588_CMD_29_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_2A(void)
{
    return check_question<B588_CMD_2A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_2B(void)
{
    return check_question<B588_CMD_2B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_2C(void)
{
    return check_question<B588_CMD_2C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_2D(void)
{
    return check_question<B588_CMD_2D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_2E(void)
{
    return check_question<B588_CMD_2E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_2F(void)
{
    return check_question<B588_CMD_2F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_31(void)
{
    return check_question<B588_CMD_31_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_32(void)
{
    return check_question<B588_CMD_32_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_33(void)
{
    return check_question<B588_CMD_33_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_34(void)
{
    return check_question<B588_CMD_34_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_35(void)
{
    return check_question<B588_CMD_35_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_36(void)
{
    return check_question<B588_CMD_36_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_37(void)
{
    return check_question<B588_CMD_37_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_38(void)
{
    return check_question<B588_CMD_38_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_39(void)
{
    return check_question<B588_CMD_39_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_3A(void)
{
    return check_question<B588_CMD_3A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_3B(void)
{
    return check_question<B588_CMD_3B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_3C(void)
{
    return check_question<B588_CMD_3C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_3D(void)
{
    return check_question<B588_CMD_3D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_3E(void)
{
    return check_question<B588_CMD_3E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_3F(void)
{
    return check_question<B588_CMD_3F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_41(void)
{
    return check_question<B588_CMD_41_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_42(void)
{
    return check_question<B588_CMD_42_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_43(void)
{
    return check_question<B588_CMD_43_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_44(void)
{
    return check_question<B588_CMD_44_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_45(void)
{
    return check_question<B588_CMD_45_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_46(void)
{
    return check_question<B588_CMD_46_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_47(void)
{
    return check_question<B588_CMD_47_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_48(void)
{
    return check_question<B588_CMD_48_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_49(void)
{
    return check_question<B588_CMD_49_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_4A(void)
{
    return check_question<B588_CMD_4A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_4B(void)
{
    return check_question<B588_CMD_4B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_54(void)
{
    return check_question<B588_CMD_54_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_55(void)
{
    return check_question<B588_CMD_55_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_56(void)
{
    return check_question<B588_CMD_56_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_57(void)
{
    return check_question<B588_CMD_57_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_58(void)
{
    return check_question<B588_CMD_58_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_59(void)
{
    return check_question<B588_CMD_59_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_5A(void)
{
    return check_question<B588_CMD_5A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_5B(void)
{
    return check_question<B588_CMD_5B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_5C(void)
{
    return check_question<B588_CMD_5C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_5D(void)
{
    return check_question<B588_CMD_5D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_5E(void)
{
    return check_question<B588_CMD_5E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_5F(void)
{
    return check_question<B588_CMD_5F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_61(void)
{
    return check_question<B588_CMD_61_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_62(void)
{
    return check_question<B588_CMD_62_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_63(void)
{
    return check_question<B588_CMD_63_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_64(void)
{
    return check_question<B588_CMD_64_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_65(void)
{
    return check_question<B588_CMD_65_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_66(void)
{
    return check_question<B588_CMD_66_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_67(void)
{
    return check_question<B588_CMD_67_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_68(void)
{
    return check_question<B588_CMD_68_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_69(void)
{
    return check_question<B588_CMD_69_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_6A(void)
{
    return check_question<B588_CMD_6A_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_6B(void)
{
    return check_question<B588_CMD_6B_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_6C(void)
{
    return check_question<B588_CMD_6C_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_6D(void)
{
    return check_question<B588_CMD_6D_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_6E(void)
{
    return check_question<B588_CMD_6E_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_6F(void)
{
    return check_question<B588_CMD_6F_QUESTION>();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_71(void)
{
    return check_question<B588_CMD_71_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_72(void)
{
    return check_question<B588_CMD_72_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_73(void)
{
    return check_question<B588_CMD_73_QUESTION>();
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::check_question_75(void)
{
    return check_question<B588_CMD_75_QUESTION>();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_01(void)
{
    if(!check_question_01()) return;

    B588_CMD_01_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x01;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_code = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
                        sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
                                            sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_02(void)
{
    if(!check_question_02()) return;

    B588_CMD_02_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x02;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_U = static_cast<uint16_t>(ui->sb_voltage->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
     sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_03(void)
{
    if(!check_question_03()) return;

    B588_CMD_03_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x03;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.setting_U = static_cast<uint16_t>(ui->sb_voltage->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_04(void)
{
    if(!check_question_04()) return;

    B588_CMD_04_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x04;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_code = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_05(void)
{
    if(!check_question_05()) return;

    B588_CMD_05_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x05;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_I = static_cast<int16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_06(void)
{
    if(!check_question_06()) return;

    B588_CMD_06_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x06;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.setting_I = static_cast<int16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_07(void)
{
    if(!check_question_07()) return;

    B588_CMD_07_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x07;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.temperature = static_cast<uint8_t>(ui->dsb_temperature->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_08(void)
{
    if(!check_question_08()) return;

    B588_CMD_08_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x08;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.version = 4;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_09(void)
{
    if(!check_question_09()) return;

    B588_CMD_09_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x09;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<11; n++)
    {
        answer.body.data.data_compile[n] = 0;
    }

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_0A(void)
{
    if(!check_question_0A()) return;

    B588_CMD_0A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0A;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<8; n++)
    {
        answer.body.data.time_compile[n] = 0;
    }

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_0B(void)
{
    if(!check_question_0B()) return;

    B588_CMD_0B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.serno = 1234;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_0C(void)
{
    if(!check_question_0C()) return;

    B588_CMD_0C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.month = 1;
    answer.body.data.year  = 2010;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_0D(void)
{
    if(!check_question_0D()) return;

    B588_CMD_0D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0D;
    answer.body.header.count_data = sizeof(answer.body.data);

    for(int n=0; n<9; n++)
    {
        answer.body.data.profiles[n] = 0;
    }

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_0E(void)
{
    if(!check_question_0E()) return;

    B588_CMD_0E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.active_point_profil = 0;
    answer.body.data.active_profil = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_0F(void)
{
    if(!check_question_0F()) return;

    B588_CMD_0F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x0F;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_11(void)
{
    if(!check_question_11()) return;

    B588_CMD_11_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x11;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_12(void)
{
    if(!check_question_12()) return;

    B588_CMD_12_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x12;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_13(void)
{
    if(!check_question_13()) return;

    B588_CMD_13_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x13;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_14(void)
{
    if(!check_question_14()) return;

    B588_CMD_14_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x14;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_15(void)
{
    if(!check_question_15()) return;

    B588_CMD_15_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x15;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_16(void)
{
    if(!check_question_16()) return;

    B588_CMD_16_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x16;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_17(void)
{
    if(!check_question_17()) return;

    B588_CMD_17_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x17;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_28(void)
{
    if(!check_question_28()) return;

    B588_CMD_28_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x28;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_29(void)
{
    if(!check_question_29()) return;

    B588_CMD_29_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x29;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_2A(void)
{
    if(!check_question_2A()) return;

    B588_CMD_2A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2A;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_2B(void)
{
    if(!check_question_2B()) return;

    B588_CMD_2B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2B;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_2C(void)
{
    if(!check_question_2C()) return;

    B588_CMD_2C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2C;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_2D(void)
{
    if(!check_question_2D()) return;

    B588_CMD_2D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2D;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_2E(void)
{
    if(!check_question_2E()) return;

    B588_CMD_2E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2E;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_2F(void)
{
    if(!check_question_2F()) return;

    B588_CMD_2F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x2F;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_31(void)
{
    if(!check_question_31()) return;

    B588_CMD_31_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x31;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_32(void)
{
    if(!check_question_32()) return;

    B588_CMD_32_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x32;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_33(void)
{
    if(!check_question_33()) return;

    B588_CMD_33_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x33;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_34(void)
{
    if(!check_question_34()) return;

    B588_CMD_34_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x34;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_35(void)
{
    if(!check_question_35()) return;

    B588_CMD_35_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x35;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_36(void)
{
    if(!check_question_36()) return;

    B588_CMD_36_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x36;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_37(void)
{
    if(!check_question_37()) return;

    B588_CMD_37_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x37;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_38(void)
{
    if(!check_question_38()) return;

    B588_CMD_38_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x46;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_39(void)
{
    if(!check_question_39()) return;

    B588_CMD_39_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x39;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_3A(void)
{
    if(!check_question_3A()) return;

    B588_CMD_3A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3A;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_3B(void)
{
    if(!check_question_3B()) return;

    B588_CMD_3B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3B;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_3C(void)
{
    if(!check_question_3C()) return;

    B588_CMD_3C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3C;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_3D(void)
{
    if(!check_question_3D()) return;

    B588_CMD_3D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3D;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_3E(void)
{
    if(!check_question_3E()) return;

    B588_CMD_3E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3E;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_3F(void)
{
    if(!check_question_3F()) return;

    B588_CMD_3F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x3F;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_41(void)
{
    if(!check_question_41()) return;

    B588_CMD_41_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x41;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_42(void)
{
    if(!check_question_42()) return;

    B588_CMD_42_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x42;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_43(void)
{
    if(!check_question_43()) return;

    B588_CMD_43_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x43;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_44(void)
{
    if(!check_question_44()) return;

    B588_CMD_44_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x44;
    answer.body.header.count_data = 0;
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_46(void)
{
    if(!check_question_46()) return;

    B588_CMD_46_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x46;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.type = 3;
    answer.body.data.year = 1;
    answer.body.data.month = 1;
    answer.body.data.serno = 123;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_47(void)
{
    if(!check_question_47()) return;

    B588_CMD_47_ANSWER answer;

    calculate(static_cast<uint16_t>(ui->sb_voltage->value()),
              static_cast<uint16_t>(ui->sb_current->value()));
    Connection::set_value(static_cast<int16_t>(ui->sb_current->value()));

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x47;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.in_voltage  = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.in_current  = static_cast<uint16_t>(ui->sb_current->value());
    answer.body.data.out_voltage = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.out_current = static_cast<uint16_t>(ui->sb_current->value());
    answer.body.data.temperature = static_cast<uint8_t>(ui->dsb_temperature->value());
    answer.body.data.current_point = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_48(void)
{
    if(!check_question_48()) return;

    B588_CMD_48_QUESTION *question = reinterpret_cast<B588_CMD_48_QUESTION *>(ba_input.data());
    B588_CMD_48_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x48;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.new_address_modbus = question->body.data.new_address_modbus;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_49(void)
{
    if(!check_question_49()) return;

    B588_CMD_49_QUESTION *question = reinterpret_cast<B588_CMD_49_QUESTION *>(ba_input.data());
    B588_CMD_49_ANSWER answer;

    ui->sb_voltage->setValue(question->body.data.voltage);
    ui->sb_current->setValue(question->body.data.current);

    calculate(question->body.data.voltage,
              question->body.data.current);
    Connection::set_value(static_cast<uint16_t>(ui->sb_voltage->value()));

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x49;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.voltage = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.current = static_cast<uint16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_4A(void)
{
    if(!check_question_4A()) return;

    B588_CMD_4A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x4A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error_data = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_54(void)
{
    if(!check_question_54()) return;

    B588_CMD_54_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x54;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.count_point = 0;
    answer.body.data.count_repeat_profile = 0;
    answer.body.data.error = 0;
    answer.body.data.profile = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_55(void)
{
    if(!check_question_55()) return;

    B588_CMD_55_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x55;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.mototime_min = 1;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_56(void)
{
    if(!check_question_56()) return;

    B588_CMD_56_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x56;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current = 0;
    answer.body.data.error = 0;
    answer.body.data.point = 0;
    answer.body.data.profile = 0;
    answer.body.data.time = 0;
    answer.body.data.voltage = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_57(void)
{
    if(!check_question_57()) return;

    B588_CMD_57_QUESTION *question = reinterpret_cast<B588_CMD_57_QUESTION *>(ba_input.data());
    B588_CMD_57_ANSWER answer;

    calculate(question->body.data.U,
              question->body.data.I);
    Connection::set_value(static_cast<uint16_t>(ui->sb_voltage->value()));

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x57;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.I = static_cast<uint16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_58(void)
{
    if(!check_question_58()) return;

    B588_CMD_58_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x58;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.I = static_cast<uint16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_59(void)
{
    if(!check_question_59()) return;

    B588_CMD_59_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x59;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.U = static_cast<uint16_t>(ui->sb_voltage->value());
    answer.body.data.I = static_cast<uint16_t>(ui->sb_current->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_5A(void)
{
    if(!check_question_5A()) return;

    B588_CMD_5A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.active_point = 0;
    answer.body.data.active_profile = 0;
    answer.body.data.current_ADC_I = 0;
    answer.body.data.current_ADC_U = 0;
    answer.body.data.setting_DAC_I = 0;
    answer.body.data.setting_DAC_U = 0;
    answer.body.data.temperature = static_cast<uint8_t>(ui->dsb_temperature->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_5B(void)
{
    if(!check_question_5B()) return;

    B588_CMD_5B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.ADC_I = 0;
    answer.body.data.ADC_U = 0;
    answer.body.data.ADC_U_input = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_5C(void)
{
    if(!check_question_5C()) return;

    B588_CMD_5C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_Zero_ADC_U  = Value_Zero_ADC_U;
    answer.body.data.Value_delta_ADC_U = Value_delta_ADC_U;
    answer.body.data.Value_DAC_MIN     = Value_DAC_MIN_U;
    answer.body.data.Value_DAC_MAX     = Value_DAC_MAX_U;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_5D(void)
{
    if(!check_question_5D()) return;

    B588_CMD_5D_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5D;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_Zero_ADC_I  = Value_Zero_ADC_I;
    answer.body.data.Value_delta_ADC_I = Value_delta_ADC_I;
    answer.body.data.Value_DAC_MIN     = Value_DAC_MIN_I;
    answer.body.data.Value_DAC_MAX     = Value_DAC_MAX_I;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_5E(void)
{
    if(!check_question_5E()) return;

    B588_CMD_5E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current = 0;
    answer.body.data.error = 0;
    answer.body.data.point = 0;
    answer.body.data.profile = 0;
    answer.body.data.time = 0;
    answer.body.data.voltage = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_5F(void)
{
    if(!check_question_5F()) return;

    B588_CMD_5F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x5F;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.number_profile = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_61(void)
{
    if(!check_question_61()) return;

    B588_CMD_61_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x61;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_ADC_Zero_U = Value_Zero_ADC_U;
    answer.body.data.factor_K = factor_K;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_62(void)
{
    if(!check_question_62()) return;

    B588_CMD_62_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x62;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.Value_ADC_Zero_I  = Value_Zero_ADC_I;
    answer.body.data.factor_K = factor_K;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_63(void)
{
    if(!check_question_63()) return;

    B588_CMD_63_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x63;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_point = 0;
    answer.body.data.number_point = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

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
void Worker_fake_b588::prepare_answer_64(void)
{
    if(!check_question_64()) return;

    B588_CMD_64_QUESTION *question = reinterpret_cast<B588_CMD_64_QUESTION *>(ba_input.data());
    B588_CMD_64_ANSWER answer;

    dac_point = 0;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x64;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.code = question->body.data.code;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_65(void)
{
    if(!check_question_65()) return;

    B588_CMD_65_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x65;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.number_current_point = static_cast<int8_t>(dac_point);

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    dac_point++;

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_66(void)
{
    if(!check_question_66()) return;

    B588_CMD_66_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x66;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.points_crc16 = 0;
    answer.body.data.point_value  = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_67(void)
{
    if(!check_question_67()) return;

    B588_CMD_67_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x67;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.speed_cooler = 1600;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_68(void)
{
    if(!check_question_68()) return;

    B588_CMD_68_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x68;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_PWM = 1600;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_69(void)
{
    if(!check_question_69()) return;

    B588_CMD_69_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x69;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.OCR3A = 0;
    answer.body.data.OCR3B = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_6A(void)
{
    if(!check_question_6A()) return;

    B588_CMD_6A_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6A;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_6B(void)
{
    if(!check_question_6B()) return;

    B588_CMD_6B_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6B;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.current_driver = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_6C(void)
{
    if(!check_question_6C()) return;

    B588_CMD_6C_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6C;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.data_compile_day = 0;
    answer.body.data.data_compile_month = 0;
    answer.body.data.data_compile_year = 0;
    answer.body.data.time_compile_hour = 0;
    answer.body.data.time_compile_min = 0;
    answer.body.data.time_compile_sec = 0;
    answer.body.data.version = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_6D(void)
{
    if(!check_question_6D()) return;

    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());

    //---
    switch(question->body.data.secret_cmd)
    {
    case SECRET_CMD_GET_ID:                 secret_CMD_GET_ID();                break;
    case SECRET_CMD_RESET:                  secret_CMD_RESET();                 break;
    case SECRET_CMD_CLEAR_MOTOHOUR:         secret_CMD_CLEAR_MOTOHOUR();        break;
    case SECRET_CMD_CLEAR_FRAM:             secret_CMD_CLEAR_FRAM();            break;
    case SECRET_CMD_READ_FRAM_8BIT:         secret_CMD_READ_FRAM_8BIT();        break;
    case SECRET_CMD_READ_FRAM_16BIT:        secret_CMD_READ_FRAM_16BIT();       break;
    case SECRET_CMD_READ_FRAM_32BIT:        secret_CMD_READ_FRAM_32BIT();       break;
    case SECRET_CMD_READ_FRAM_MORE_BYTES:   secret_CMD_READ_FRAM_MORE_BYTES();  break;
    case SECRET_CMD_SAVE_FRAM_8BIT:         secret_CMD_SAVE_FRAM_8BIT();        break;
    case SECRET_CMD_SAVE_FRAM_16BIT:        secret_CMD_SAVE_FRAM_16BIT();       break;
    case SECRET_CMD_SAVE_FRAM_32BIT:        secret_CMD_SAVE_FRAM_32BIT();       break;
    case SECRET_CMD_SAVE_FRAM_MORE_BYTES:   secret_CMD_SAVE_FRAM_MORE_BYTES();  break;
    case SECRET_CMD_SAVE_DATE_AND_SERNO:    secret_CMD_SAVE_DATE_AND_SERNO();   break;

    default:    break;
    }
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_6E(void)
{
    if(!check_question_6E()) return;

    B588_CMD_6E_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6E;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.profile = 0;
    answer.body.data.repeat = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_6F(void)
{
    if(!check_question_6F()) return;

    B588_CMD_6F_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6F;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.error = 0;
    answer.body.data.profile = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
     sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_71(void)
{
    if(!check_question_71()) return;

    B588_CMD_71_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x71;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.state_ADC_I = 0;
    answer.body.data.state_ADC_U = 0;
    answer.body.data.state_ADC_Ui = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_72(void)
{
    if(!check_question_72()) return;

    B588_CMD_72_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x72;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.conf_ADC = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_73(void)
{
    if(!check_question_73()) return;

    B588_CMD_73_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x73;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.real_DAC_I = 0;
    answer.body.data.real_DAC_U = 0;
    answer.body.data.revers = 0;
    answer.body.data.temperature = static_cast<uint8_t>(ui->dsb_temperature->value());

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::prepare_answer_75(void)
{
    if(!check_question_75()) return;

    B588_CMD_75_ANSWER answer;

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x75;
    answer.body.header.count_data = sizeof(answer.body.data);

    answer.body.data.count_Error_ADC = 0;
    answer.body.data.count_Error_Answer_ADC = 0;
    answer.body.data.count_ReStart_ADC_0 = 0;
    answer.body.data.count_ReStart_ADC_1 = 0;
    answer.body.data.count_ReStart_ADC_2 = 0;
    answer.body.data.MCUCSR = 0;

    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_GET_ID(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_RESET(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_CLEAR_MOTOHOUR(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_CLEAR_FRAM(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_READ_FRAM_8BIT(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_READ_FRAM_16BIT(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_READ_FRAM_32BIT(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_READ_FRAM_MORE_BYTES(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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
        bool ok = fram->read_data_8(static_cast<uint16_t>(address+n), &value);
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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_SAVE_FRAM_8BIT(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_SAVE_FRAM_16BIT(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_SAVE_FRAM_32BIT(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

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

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_SAVE_FRAM_MORE_BYTES(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

    //---
    DATA_FRAM_MORE_BYTES save_fram_more_bytes;
    unsigned int len = sizeof(DATA_FRAM_MORE_BYTES);
    uint16_t address = save_fram_more_bytes.data.address;
    for(unsigned int n=0; n<len; n++)
    {
        fram->write_data_8(static_cast<uint16_t>(address+n), question->body.data.secret_data[n]);
    }

    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<len; n++)
    {
        answer.body.data.secret_data[n] = save_fram_more_bytes.buf[n];
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_SAVE_DATE_AND_SERNO(void)
{
    B588_CMD_6D_QUESTION *question = reinterpret_cast<B588_CMD_6D_QUESTION *>(ba_input.data());
    B588_CMD_6D_ANSWER answer;

    //---
    answer.body.data.secret_cmd = question->body.data.secret_cmd;
    answer.body.data.result = 0;
    for(unsigned int n=0; n<sizeof(answer.body.data.secret_data); n++)
    {
        answer.body.data.secret_data[n] = 0;
    }
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::secret_CMD_COPY_USER_TO_FACTORY(void)
{
    B588_CMD_6D_ANSWER answer;

    //---
    fram->copy_user_to_factory_calibration();
    //---

    answer.body.header.address = 0;
    answer.body.header.cmd = B588_CMD_0x6D;
    answer.body.header.count_data = sizeof(answer.body.data);
    answer.body.crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(&answer.buf),
    sizeof(answer) - sizeof(answer.crc16));

    ba_output.clear();
    ba_output.append(reinterpret_cast<const char *>(answer.buf),
    sizeof(answer));

    emit info(QString("send answer: %1").arg(ba_output.toHex().data()));

    run_timer();
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::calculate(uint16_t new_setting_U,
                                 uint16_t new_setting_I)
{
    // все значения в mV и mA
//    ui->sb_voltage->setValue(new_setting_U);
//    ui->sb_current->setValue(new_setting_I);

    double R = ui->sb_R->value();
    double U = new_setting_U;
    double I = static_cast<double>(new_setting_U) / R;
    if(I > new_setting_I)
    {
        U = new_setting_I * R;
        I = new_setting_I;
    }

    ui->sb_voltage->setValue(U / 1000.0);
    ui->sb_current->setValue(I / 1000.0);
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::run_timer(void)
{
    Q_ASSERT(timer);
    timer->start(FAKE_DELAY_MS);
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::port_open(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_b588::port_close(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_b588::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Worker_fake_b588::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Worker_fake_b588::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Worker_fake_b588::save_setting(void)
{

}
//--------------------------------------------------------------------------------
