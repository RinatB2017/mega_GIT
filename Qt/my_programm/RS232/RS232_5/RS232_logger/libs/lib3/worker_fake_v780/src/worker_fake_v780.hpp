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
#ifndef WORKER_FAKE_V780_HPP
#define WORKER_FAKE_V780_HPP
//--------------------------------------------------------------------------------
#include <QTimer>

#include "connection.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Worker_fake_v780;
}
//--------------------------------------------------------------------------------
class Worker_fake_v780 : public QWidget
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
    explicit Worker_fake_v780(QWidget *parent = nullptr);
    virtual ~Worker_fake_v780();

    void input(QByteArray data);
    void port_open(void);
    void port_close(void);

private:
    Ui::Worker_fake_v780 *ui;

    QByteArray ba_input;
    QByteArray ba_output;

    QTimer *timer = nullptr;

    //---
    int address = 0;
    //---

    template<typename T>
    bool check_question(void);

    bool check_question_41(void);
    bool check_question_42(void);
    bool check_question_45(void);
    bool check_question_46(void);
    bool check_question_47(void);
    bool check_question_48(void);
    bool check_question_53(void);

    void prepare_answer_41(void);
    void prepare_answer_42(void);
    void prepare_answer_45(void);
    void prepare_answer_46(void);
    void prepare_answer_47(void);
    void prepare_answer_48(void);
    void prepare_answer_53(void);

    void run_timer(void);
    void send_answer(void);

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // Worker_fake_v780_HPP
