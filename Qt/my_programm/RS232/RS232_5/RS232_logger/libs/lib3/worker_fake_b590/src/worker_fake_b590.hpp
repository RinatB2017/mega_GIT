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
#ifndef WORKER_FAKE_B590_HPP
#define WORKER_FAKE_B590_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
#include "b590_defines.hpp"
#include "B590_fram.hpp"

#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Worker_fake_b590;
}
//--------------------------------------------------------------------------------
class Worker_fake_b590 : public MyWidget
{
    Q_OBJECT

signals:
    void output(QByteArray text);
    void readyRead(void);

public:
    explicit Worker_fake_b590(QWidget *parent = nullptr);
    virtual ~Worker_fake_b590();

    void input(QByteArray data);
    void port_open(void);
    void port_close(void);

private:
    Ui::Worker_fake_b590 *ui;

    QByteArray ba_input;
    QByteArray ba_output;

    QPointer<QTimer> timer;

    //---
    QPointer<B590_Fram> fram;
    int address = 0;

    uint16_t dac_U[0xFFFF]; //[MAX_CALIBRATION_POINTS_B590_U];
    uint16_t dac_I[0xFFFF]; //[MAX_CALIBRATION_POINTS_B590_I];

    double factor_k_DAC = 0;
    double factor_k_ADC = 0;

    s_zero_b590_U zero_U;
    s_zero_b590_I zero_I;

    int dac_point = 0;
    //---

    template<typename T>
    bool check_question(void);

    bool check_question_11(void);
    bool check_question_12(void);
    bool check_question_13(void);
    bool check_question_14(void);
    bool check_question_15(void);
    bool check_question_16(void);
    bool check_question_17(void);

    bool check_question_41(void);
    bool check_question_42(void);
    bool check_question_46(void);
    bool check_question_47(void);
    bool check_question_48(void);
    bool check_question_49(void);
    bool check_question_4A(void);

    bool check_question_54(void);
    bool check_question_55(void);
    bool check_question_56(void);
    bool check_question_57(void);
    bool check_question_5A(void);
    bool check_question_5B(void);
    bool check_question_5C(void);
    bool check_question_5D(void);
    bool check_question_5E(void);
    bool check_question_5F(void);

    bool check_question_61(void);
    bool check_question_62(void);
    bool check_question_63(void);
    bool check_question_64(void);
    bool check_question_65(void);
    bool check_question_66(void);
    bool check_question_67(void);
    bool check_question_68(void);
    bool check_question_69(void);
    bool check_question_6A(void);
    bool check_question_6B(void);
    bool check_question_6C(void);
    bool check_question_6D(void);
    bool check_question_6E(void);
    bool check_question_6F(void);

    bool check_question_71(void);
    bool check_question_72(void);
    bool check_question_73(void);
    bool check_question_75(void);

    void prepare_answer_11(void);
    void prepare_answer_12(void);
    void prepare_answer_15(void);
    void prepare_answer_16(void);
    void prepare_answer_17(void);

    void prepare_answer_41(void);
    void prepare_answer_42(void);
    void prepare_answer_46(void);
    void prepare_answer_47(void);
    void prepare_answer_48(void);
    void prepare_answer_49(void);
    void prepare_answer_4A(void);

    void prepare_answer_54(void);
    void prepare_answer_55(void);
    void prepare_answer_56(void);
    void prepare_answer_57(void);
    void prepare_answer_5A(void);
    void prepare_answer_5B(void);
    void prepare_answer_5C(void);
    void prepare_answer_5D(void);
    void prepare_answer_5E(void);
    void prepare_answer_5F(void);

    void prepare_answer_61(void);
    void prepare_answer_62(void);
    void prepare_answer_63(void);
    void prepare_answer_64(void);
    void prepare_answer_65(void);
    void prepare_answer_66(void);
    void prepare_answer_67(void);
    void prepare_answer_68(void);
    void prepare_answer_69(void);
    void prepare_answer_6A(void);
    void prepare_answer_6B(void);
    void prepare_answer_6C(void);

    void prepare_answer_6D(void);
    void prepare_answer_6E(void);
    void prepare_answer_6F(void);

    void prepare_answer_71(void);
    void prepare_answer_72(void);
    void prepare_answer_73(void);
    void prepare_answer_75(void);

    void secret_CMD_GET_ID(void);
    void secret_CMD_RESET(void);
    void secret_CMD_CLEAR_MOTOHOUR(void);
    void secret_CMD_SAVE_DATE_AND_SERNO(void);
    void secret_CMD_COPY_USER_TO_FACTORY(void);

    void secret_CMD_CLEAR_FRAM(void);
    void secret_CMD_READ_FRAM_8BIT(void);
    void secret_CMD_READ_FRAM_16BIT(void);
    void secret_CMD_READ_FRAM_32BIT(void);
    void secret_CMD_READ_FRAM_MORE_BYTES(void);
    void secret_CMD_SAVE_FRAM_8BIT(void);
    void secret_CMD_SAVE_FRAM_16BIT(void);
    void secret_CMD_SAVE_FRAM_32BIT(void);
    void secret_CMD_SAVE_FRAM_MORE_BYTES(void);

    bool clear_profile(int number_profile);

    void regenerate_dac_U_values(int noise_value);
    void regenerate_dac_I_values(int noise_value);
    void regenerate_all(void);

    void calculate(int32_t  new_setting_U,
                   uint16_t new_setting_I);
    void run_timer(void);
    void send_answer(void);

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // WORKER_FAKE_B590_HPP
