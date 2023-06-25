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
#ifndef WORKER_FAKE_B588_HPP
#define WORKER_FAKE_B588_HPP
//--------------------------------------------------------------------------------
#include <QPointer>
#include <QWidget>

#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Worker_fake_b588;
}
//--------------------------------------------------------------------------------
class B588_Fram;
//--------------------------------------------------------------------------------
class Worker_fake_b588 : public MyWidget
{
    Q_OBJECT

signals:
    void output(QByteArray text);
    void readyRead(void);

public:
    explicit Worker_fake_b588(QWidget *parent = nullptr);
    virtual ~Worker_fake_b588();

    void input(QByteArray data);
    void port_open(void);
    void port_close(void);

private:
    Ui::Worker_fake_b588 *ui;

    QByteArray ba_input;
    QByteArray ba_output;

    QPointer<QTimer> timer;

    //---
    QPointer<B588_Fram> fram;

    double factor_K = 0;

    int dac_point = 0;

    int32_t Value_Zero_ADC_U  = 0;
    int32_t Value_delta_ADC_U = 0;
    uint16_t Value_DAC_MIN_U  = 0;
    uint16_t Value_DAC_MAX_U  = 0;

    int32_t Value_Zero_ADC_I  = 0;
    int32_t Value_delta_ADC_I = 0;
    uint16_t Value_DAC_MIN_I  = 0;
    uint16_t Value_DAC_MAX_I  = 0;
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

    bool check_question_11(void);
    bool check_question_12(void);
    bool check_question_13(void);
    bool check_question_14(void);
    bool check_question_15(void);
    bool check_question_16(void);
    bool check_question_17(void);

    bool check_question_20(void);
    bool check_question_21(void);
    bool check_question_22(void);
    bool check_question_23(void);
    bool check_question_24(void);
    bool check_question_25(void);
    bool check_question_26(void);
    bool check_question_27(void);
    bool check_question_28(void);
    bool check_question_29(void);
    bool check_question_2A(void);
    bool check_question_2B(void);
    bool check_question_2C(void);
    bool check_question_2D(void);
    bool check_question_2E(void);
    bool check_question_2F(void);

    bool check_question_31(void);
    bool check_question_32(void);
    bool check_question_33(void);
    bool check_question_34(void);
    bool check_question_35(void);
    bool check_question_36(void);
    bool check_question_37(void);
    bool check_question_38(void);
    bool check_question_39(void);
    bool check_question_3A(void);
    bool check_question_3B(void);
    bool check_question_3C(void);
    bool check_question_3D(void);
    bool check_question_3E(void);
    bool check_question_3F(void);

    bool check_question_41(void);
    bool check_question_42(void);
    bool check_question_43(void);
    bool check_question_44(void);
    bool check_question_45(void);
    bool check_question_46(void);
    bool check_question_47(void);
    bool check_question_48(void);
    bool check_question_49(void);
    bool check_question_4A(void);
    bool check_question_4B(void);

    bool check_question_54(void);
    bool check_question_55(void);
    bool check_question_56(void);
    bool check_question_57(void);
    bool check_question_58(void);
    bool check_question_59(void);
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

    void prepare_answer_11(void);
    void prepare_answer_12(void);
    void prepare_answer_13(void);
    void prepare_answer_14(void);
    void prepare_answer_15(void);
    void prepare_answer_16(void);
    void prepare_answer_17(void);

    void prepare_answer_28(void);
    void prepare_answer_29(void);
    void prepare_answer_2A(void);
    void prepare_answer_2B(void);
    void prepare_answer_2C(void);
    void prepare_answer_2D(void);
    void prepare_answer_2E(void);
    void prepare_answer_2F(void);

    void prepare_answer_31(void);
    void prepare_answer_32(void);
    void prepare_answer_33(void);
    void prepare_answer_34(void);
    void prepare_answer_35(void);
    void prepare_answer_36(void);
    void prepare_answer_37(void);
    void prepare_answer_38(void);
    void prepare_answer_39(void);
    void prepare_answer_3A(void);
    void prepare_answer_3B(void);
    void prepare_answer_3C(void);
    void prepare_answer_3D(void);
    void prepare_answer_3E(void);
    void prepare_answer_3F(void);

    void prepare_answer_41(void);
    void prepare_answer_42(void);
    void prepare_answer_43(void);
    void prepare_answer_44(void);
    void prepare_answer_46(void);
    void prepare_answer_47(void);
    void prepare_answer_48(void);
    void prepare_answer_49(void);
    void prepare_answer_4A(void);

    void prepare_answer_54(void);
    void prepare_answer_55(void);
    void prepare_answer_56(void);
    void prepare_answer_57(void);
    void prepare_answer_58(void);
    void prepare_answer_59(void);
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
    void prepare_answer_6D(void);

    void prepare_answer_6E(void);
    void prepare_answer_6F(void);

    void prepare_answer_71(void);
    void prepare_answer_72(void);
    void prepare_answer_73(void);
    void prepare_answer_75(void);

    void calculate(uint16_t new_setting_U,
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
#endif // WORKER_FAKE_B588
