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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define SIM900_AT   0
#define SIM900_AT_TEXT      "AT"

#define SIM900_GMM  1
#define SIM900_GMM_TEXT     tr("GMM: идентификатор модуля")

#define SIM900_GOI  2
#define SIM900_GOI_TEXT     tr("GOI: более короткий идентификатор модуля")

#define SIM900_GMR  3
#define SIM900_GMR_TEXT     tr("GMR: версия прошивки")

#define SIM900_GSV  4
#define SIM900_GSV_TEXT     tr("GSV: производитель модуля, модель, версия прошивки")

#define SIM900_GSN  5
#define SIM900_GSN_TEXT     tr("GSN: IMEI модуля")

#define SIM900_CSQ  6
#define SIM900_CSQ_TEXT     tr("CSQ: информация о качестве сигнала")

#define SIM900_CREG_Q 7
#define SIM900_CREG_Q_TEXT  tr("CREG: разрешить выдачу сообщения о смене регистрации в сети")

#define SIM900_CREG_A 8
#define SIM900_CREG_A_TEXT  tr("CREG: получить тип регистрации в сети")

#define SIM900_CSPN 9
#define SIM900_CSPN_TEXT    tr("CSPN: информация об операторе из сим-карты")

#define SIM900_COPS 10
#define SIM900_COPS_TEXT    tr("COPS: информация об операторе, в сети которого зарегистрирован модуль")

#define SIM900_CPAS 11
#define SIM900_CPAS_TEXT    tr("CPAS: информация о текущем состоянии телефона")

#define SIM900_CCLK_A 12
#define SIM900_CCLK_A_TEXT  tr("CCLK: получить текущее время")

#define SIM900_CCLK_Q 13
#define SIM900_CCLK_Q_TEXT  tr("CCLK: установить время")

#define SIM900_ATE0 14
#define SIM900_ATE0_TEXT    tr("ATE0: режим эха")

#define SIM900_ATV0 15
#define SIM900_ATV0_TEXT    tr("ATV0: формат ответа модуля")

#define SIM900_CMEE 16
#define SIM900_CMEE_TEXT    tr("CMEE: уровень информации об ошибке")

#define SIM900_CLIP 17
#define SIM900_CLIP_TEXT    tr("CLIP: включает режим CLI (Calling Line Identification)")

#define SIM900_ATA  18
#define SIM900_ATA_TEXT     tr("ATA: ответить на входной звонок")

#define SIM900_ATH0 19
#define SIM900_ATH0_TEXT    tr("ATH0: разорвать все соединения")

#define SIM900_ATS0 20
#define SIM900_ATS0_TEXT    tr("ATS0: количество звонков до авто поднятия трубки")

#define SIM900_ATD  21
#define SIM900_ATD_TEXT     tr("ATD: позвонить по номеру")

#define SIM900_ATD1 22
#define SIM900_ATD1_TEXT    tr("ATD1: позвонить по 1 номеру из сим карты")

#define SIM900_ATDL 23
#define SIM900_ATDL_TEXT    tr("ATDL: позвонить по последнему набранному номеру")

#define SIM900_GSMBUSY  24
#define SIM900_GSMBUSY_TEXT tr("GSMBUSY: запрет всех входящих звонков")

#define SIM900_CSCA 25
#define SIM900_CSCA_TEXT    tr("CSCA: номер сервис центра отправки сообщений")

#define SIM900_CMGF 26
#define SIM900_CMGF_TEXT    tr("CMGF: формат сообщений")

#define SIM900_CSCB 27
#define SIM900_CSCB_TEXT    tr("CSCB: прием широковещательных сообщений")

#define SIM900_CMGL 28
#define SIM900_CMGL_TEXT    tr("CMGL: чтение смс")

#define SIM900_CMGDA    29
#define SIM900_CMGDA_TEXT   tr("CMGDA: удаление всех смс определенного типа")

#define SIM900_CMGD 30
#define SIM900_CMGD_TEXT    tr("CMGD: удаление смс")

#define SIM900_CMGS 31
#define SIM900_CMGS_TEXT    tr("CMGS: отправка смс")

#define SIM900_CUSD 32
#define SIM900_CUSD_TEXT    tr("CUSD: USSD запрос")
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5;
class QToolButton;
class QToolBar;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void read_data(QByteArray ba);
    void run_command(void);
    void send_command(const QString &command);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    SerialBox5 *serialBox = 0;
    QByteArray data_rs232;
    bool is_ready = false;

    void init(void);
    void wait(int max_time_ms);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
