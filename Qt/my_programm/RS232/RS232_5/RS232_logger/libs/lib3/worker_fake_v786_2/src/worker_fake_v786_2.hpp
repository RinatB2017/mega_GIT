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
#ifndef WORKER_FAKE_V786_2_HPP
#define WORKER_FAKE_V786_2_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QWidget>
#include <QTimer>
//--------------------------------------------------------------------------------
#ifdef Q_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
#define FAKE_DELAY_MS   100
//--------------------------------------------------------------------------------
namespace Ui {
    class Worker_fake_v786_2;
}
//--------------------------------------------------------------------------------
class Worker_fake_v786_2 : public QWidget
{
    Q_OBJECT

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void output(QByteArray text);
    void readyRead(void);

public:
    explicit Worker_fake_v786_2(QWidget *parent = nullptr);
    virtual ~Worker_fake_v786_2();

    void input(QByteArray data);
    void port_open(void);
    void port_close(void);

    //---
    bool isOpen(void);
    QByteArray readAll();
    QStringList get_port_names(void);
    void setPortName(const QString &name);
    void setAddress(uint8_t addr);
    void setChannel(uint8_t addr);
    void setSerNo(uint16_t addr);
    void setYear(uint16_t addr);
    bool serial_open(void);
    void serial_close(void);
    bool setBaudRate(int speed);
    void write(QByteArray data);
    void receive(const QVariant &data);
    //---

private:
    Ui::Worker_fake_v786_2 *ui;

    QByteArray ba_input;
    QByteArray ba_output;

    QTimer *timer = nullptr;

    //---
    int address = 0;
    //---

    template<typename T>
    bool check_question(void);

    bool check_question_01(void);
    bool check_question_02(void);
    bool check_question_03(void);
    bool check_question_04(void);
    bool check_question_05(void);
    bool check_question_06(void);
    bool check_question_07(void);
    bool check_question_08(void);
    bool check_question_09(void);
    bool check_question_0A(void);
    bool check_question_0B(void);
    bool check_question_0C(void);
    bool check_question_0D(void);
    bool check_question_0E(void);
    bool check_question_0F(void);
    bool check_question_10(void);
    bool check_question_11(void);
    bool check_question_12(void);
    bool check_question_13(void);
    bool check_question_14(void);
    bool check_question_15(void);
    bool check_question_16(void);
    bool check_question_17(void);
    bool check_question_18(void);
    bool check_question_19(void);
    bool check_question_1A(void);
    bool check_question_1B(void);
    bool check_question_1C(void);

    void prepare_answer_01(void);
    void prepare_answer_02(void);
    void prepare_answer_03(void);
    void prepare_answer_04(void);
    void prepare_answer_05(void);
    void prepare_answer_06(void);
    void prepare_answer_07(void);
    void prepare_answer_08(void);
    void prepare_answer_09(void);
    void prepare_answer_0A(void);
    void prepare_answer_0B(void);
    void prepare_answer_0C(void);
    void prepare_answer_0D(void);
    void prepare_answer_0E(void);
    void prepare_answer_0F(void);
    void prepare_answer_10(void);
    void prepare_answer_11(void);
    void prepare_answer_12(void);
    void prepare_answer_13(void);
    void prepare_answer_14(void);
    void prepare_answer_15(void);
    void prepare_answer_16(void);
    void prepare_answer_17(void);
    void prepare_answer_18(void);
    void prepare_answer_19(void);
    void prepare_answer_1A(void);
    void prepare_answer_1B(void);
    void prepare_answer_1C(void);

    void run_timer(void);
    void send_answer(void);

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // WORKER_FAKE_V786_2_HPP
