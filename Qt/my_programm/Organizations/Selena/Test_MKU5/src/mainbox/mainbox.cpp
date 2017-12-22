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
#include <QMessageBox>
#include <QToolButton>
#include <QSettings>
#include <QWidget>
#include <QTimer>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "logbox.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef DEBUG_FRAME
    #include "debug_frame/debugframe.hpp"
#endif
//--------------------------------------------------------------------------------
#include "test_rs232_1.hpp"
#include "test_rs232_2.hpp"
#include "test_rs232_3.hpp"
//--------------------------------------------------------------------------------
#include "test_flbi_1.hpp"
#include "test_flbi_2.hpp"
#include "test_flbi_3.hpp"
//--------------------------------------------------------------------------------
#include "tcp_client.hpp"
//--------------------------------------------------------------------------------
#define COLOR_BUTTON "background:rgb(80,200,120)"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
#ifdef DEBUG_FRAME
    delete debug_frame;
#endif
    delete client;
#ifdef RS232
    delete serialBox;
#endif
    delete logBox;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init_log(void)
{
#ifdef RS232
    serialBox = new SerialBox5(this, tr("RS-232"));
    ui->layout_serial->addWidget(serialBox);
#endif

    logBox = new LogBox("Test_MKU5", this);
    ui->layout_log->addWidget(logBox);
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    init_log();
    connects();
    load_setting();
    init_tcp_client();
#ifdef DEBUG_FRAME
    debug_frame = new DebugFrame(client, this);
    ui->layout_DEBUG->addWidget(debug_frame);
#endif
    QTimer::singleShot(100, this, SLOT(clicked_btn_main_01()));
}
//--------------------------------------------------------------------------------
void MainBox::init_tcp_client(void)
{
    client = new TCP_Client(this);
    client->setAddress(QHostAddress(ui->lnAddress->text()));
    client->setPort(ui->spbPort->value());
}
//--------------------------------------------------------------------------------
void MainBox::connects(void)
{
    connect(ui->btn_main_1,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_01()));
    connect(ui->btn_main_2,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_02()));
    connect(ui->btn_main_3,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_03()));
    connect(ui->btn_main_4,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_04()));
    connect(ui->btn_main_5,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_05()));
    connect(ui->btn_main_6,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_06()));
    connect(ui->btn_main_7,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_07()));
    connect(ui->btn_main_8,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_08()));
    connect(ui->btn_main_9,  SIGNAL(clicked()), this, SLOT(clicked_btn_main_09()));
    connect(ui->btn_main_10, SIGNAL(clicked()), this, SLOT(clicked_btn_main_10()));
    connect(ui->btn_main_11, SIGNAL(clicked()), this, SLOT(clicked_btn_main_11()));
    connect(ui->btn_main_12, SIGNAL(clicked()), this, SLOT(clicked_btn_main_12()));
    connect(ui->btn_main_13, SIGNAL(clicked()), this, SLOT(clicked_btn_main_13()));
    connect(ui->btn_main_14, SIGNAL(clicked()), this, SLOT(clicked_btn_main_14()));

    connect(ui->btn_main_clear, SIGNAL(clicked()), this, SLOT(clicked_btn_clear()));

    connect(ui->btn_out_1, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_01(bool)));
    connect(ui->btn_out_2, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_02(bool)));
    connect(ui->btn_out_3, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_03(bool)));
    connect(ui->btn_out_4, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_04(bool)));
    connect(ui->btn_out_5, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_05(bool)));
    connect(ui->btn_out_6, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_06(bool)));
    connect(ui->btn_out_7, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_07(bool)));
    connect(ui->btn_out_8, SIGNAL(toggled(bool)), this, SLOT(clicked_btn_out_08(bool)));

    connect(ui->btn_in_1, SIGNAL(clicked()), this, SLOT(clicked_btn_in_01()));
    connect(ui->btn_in_2, SIGNAL(clicked()), this, SLOT(clicked_btn_in_02()));
    connect(ui->btn_in_3, SIGNAL(clicked()), this, SLOT(clicked_btn_in_03()));
    connect(ui->btn_in_4, SIGNAL(clicked()), this, SLOT(clicked_btn_in_04()));
    connect(ui->btn_in_5, SIGNAL(clicked()), this, SLOT(clicked_btn_in_05()));
    connect(ui->btn_in_6, SIGNAL(clicked()), this, SLOT(clicked_btn_in_06()));
    connect(ui->btn_in_7, SIGNAL(clicked()), this, SLOT(clicked_btn_in_07()));
    connect(ui->btn_in_8, SIGNAL(clicked()), this, SLOT(clicked_btn_in_08()));

    connect(ui->btn_takt_1, SIGNAL(clicked()), this, SLOT(clicked_takt_1()));
    connect(ui->btn_takt_2, SIGNAL(clicked()), this, SLOT(clicked_takt_2()));
    connect(ui->btn_takt_3, SIGNAL(clicked()), this, SLOT(clicked_takt_3()));
    connect(ui->btn_takt_4, SIGNAL(clicked()), this, SLOT(clicked_takt_4()));
    connect(ui->btn_takt_5, SIGNAL(clicked()), this, SLOT(clicked_takt_5()));
    connect(ui->btn_takt_6, SIGNAL(clicked()), this, SLOT(clicked_takt_6()));
    connect(ui->btn_takt_7, SIGNAL(clicked()), this, SLOT(clicked_takt_7()));
    connect(ui->btn_takt_8, SIGNAL(clicked()), this, SLOT(clicked_takt_8()));
    connect(ui->btn_takt_9, SIGNAL(clicked()), this, SLOT(clicked_takt_9()));

    connect(ui->lnAddress, SIGNAL(editingFinished()), this, SLOT(updateClient()));
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    QSettings *settings = new QSettings("MKU5.ini", QSettings::IniFormat);
    ui->lnAddress->setText(settings->value("IP", "127.0.0.1").toString());
    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    QSettings *settings = new QSettings("MKU5.ini", QSettings::IniFormat);
    settings->setValue("IP", QVariant(ui->lnAddress->text()));
    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::updateClient(void)
{
    client->setAddress(QHostAddress(ui->lnAddress->text()));
    client->setPort(ui->spbPort->value());
    save_setting();
}
//--------------------------------------------------------------------------------
void MainBox::message_error(const QString &text)
{
    QMessageBox::warning(this,
                         tr("Error"),
                         text,
                         QMessageBox::Ok);
}
//--------------------------------------------------------------------------------
#if 0
typedef struct
{
    uint16_t marker;
    uint16_t length;
    unsigned char address;
    unsigned char usecase_data;
    unsigned char null_byte;
    unsigned char code_command;
    unsigned char *data;
} Packet_header;
#endif
//--------------------------------------------------------------------------------
void MainBox::build_data(unsigned char cmd)
{
    int n;

    emit debug(QString("build_data: CMD_0x%1").arg(cmd, 0, 16));
    data.clear();
    data.append((unsigned char)0x55);   // marker
    data.append((unsigned char)0xA9);

    data.append((char)1);               // length
    data.append((char)0);

    data.append((unsigned char)0x01);   // address
    data.append((char)0x02);            // usecase_data
    data.append((char)0x00);            // null_byte

    data.append((unsigned char)cmd);    // code_command

    for(n=0; n<256; n++)                // *data
        data.append((char)0x00);
}
//--------------------------------------------------------------------------------
void MainBox::build_data_ext(unsigned char cmd, unsigned char cmd_ext)
{
    int n;

    emit debug(QString("build_data: CMD_0x%1 [%2]")
               .arg(cmd, 0, 16)
               .arg(cmd_ext, 0, 16));

    data.clear();
    data.append((unsigned char)0x55);   // marker
    data.append((unsigned char)0xA9);

    data.append((char)1);               // length
    data.append((char)0);

    data.append((unsigned char)0x01);   // address
    data.append((char)0x02);            // usecase_data
    data.append((char)0x00);            // null_byte

    data.append((unsigned char)cmd);    // code_command
    data.append((unsigned char)cmd_ext); // code_command (ex)

    for(n=0; n<255; n++)                // *data
        data.append((char)0x00);
}
//--------------------------------------------------------------------------------
bool MainBox::check(uchar test_byte,
                    uchar true_byte,
                    uchar number_table,
                    uchar number_takt)
{
    // emit debug(QString("test_byte = 0x%1").arg(test_byte, 0, 16));
    if(test_byte == true_byte)
        return true;
    emit error(QString("Ошибка: таблица %1 пункт %2. Ожидалось 0x%3. Вернулось 0x%4")
               .arg(number_table)
               .arg(number_takt)
               .arg(true_byte, 0, 16)
               .arg(test_byte, 0, 16));
    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::check_xF(uchar test_byte,
                       uchar number_table,
                       uchar number_takt)
{
    union U_BYTE temp;

    // emit debug(QString("test_byte = 0x%1").arg(test_byte, 0, 16));
    temp.value = test_byte;
    if((temp.bits.bit0 == 1) &&
            (temp.bits.bit1 == 1) &&
            (temp.bits.bit2 == 1) &&
            (temp.bits.bit3 == 1))
    {
        return true;
    }
    emit error(QString("Ошибка таблица %1 пункт %2. Вернулось 0x%3")
               .arg(number_table)
               .arg(number_takt)
               .arg(test_byte, 0, 16));
    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::check_x0(uchar test_byte,
                       uchar number_table,
                       uchar number_takt)
{
    union U_BYTE temp;

    // emit debug(QString("test_byte = 0x%1").arg(test_byte, 0, 16));
    temp.value = test_byte;
    if((temp.bits.bit0 == 0) &&
            (temp.bits.bit1 == 0) &&
            (temp.bits.bit2 == 0) &&
            (temp.bits.bit3 == 0))
    {
        return true;
    }
    emit error(QString("Ошибка таблица %1 пункт %2. Вернулось 0x%3")
               .arg(number_table)
               .arg(number_takt)
               .arg(test_byte, 0, 16));
    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::check_bits(uchar test_byte,
                         uchar first_bit_pos,
                         uchar first_bit_value,
                         uchar second_bit_pos,
                         uchar second_bit_value,
                         uchar number_table,
                         uchar number_takt)
{
    // emit debug(QString("test_byte = 0b%1").arg(test_byte, 0, 2));
    if(first_bit_pos > 7)
        return false;
    if(first_bit_value > 1)
        return false;

    if(second_bit_pos > 7)
        return false;
    if(second_bit_value > 1)
        return false;

    uchar mask = ((first_bit_value << first_bit_pos)) | (second_bit_value << second_bit_pos);

    if(test_byte & mask)
    {
        return true;
    }
    emit error(QString("Ошибка таблица %1 пункт %2. Вернулось 0b%3")
               .arg(number_table)
               .arg(number_takt)
               .arg(test_byte, 0, 2));
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::clear_main_checkable(void)
{
    ui->btn_main_1->setCheckable(false);
    ui->btn_main_2->setCheckable(false);
    ui->btn_main_3->setCheckable(false);
    ui->btn_main_4->setCheckable(false);
    ui->btn_main_5->setCheckable(false);
    ui->btn_main_6->setCheckable(false);
    ui->btn_main_7->setCheckable(false);
    ui->btn_main_8->setCheckable(false);
    ui->btn_main_9->setCheckable(false);
    ui->btn_main_10->setCheckable(false);
    ui->btn_main_11->setCheckable(false);
    ui->btn_main_12->setCheckable(false);
    ui->btn_main_13->setCheckable(false);
    ui->btn_main_14->setCheckable(false);

    ui->btn_main_1->setStyleSheet("");
    ui->btn_main_2->setStyleSheet("");
    ui->btn_main_3->setStyleSheet("");
    ui->btn_main_4->setStyleSheet("");
    ui->btn_main_5->setStyleSheet("");
    ui->btn_main_6->setStyleSheet("");
    ui->btn_main_7->setStyleSheet("");
    ui->btn_main_8->setStyleSheet("");
    ui->btn_main_9->setStyleSheet("");
    ui->btn_main_10->setStyleSheet("");
    ui->btn_main_11->setStyleSheet("");
    ui->btn_main_12->setStyleSheet("");
    ui->btn_main_13->setStyleSheet("");
    ui->btn_main_14->setStyleSheet("");
}
//--------------------------------------------------------------------------------
int MainBox::get_main_checkable(void)
{
    if(ui->btn_main_1->isChecked())  return 1;
    if(ui->btn_main_2->isChecked())  return 2;
    if(ui->btn_main_3->isChecked())  return 3;
    if(ui->btn_main_4->isChecked())  return 4;
    if(ui->btn_main_5->isChecked())  return 5;
    if(ui->btn_main_6->isChecked())  return 6;
    if(ui->btn_main_7->isChecked())  return 7;
    if(ui->btn_main_8->isChecked())  return 8;
    if(ui->btn_main_9->isChecked())  return 9;
    if(ui->btn_main_10->isChecked()) return 10;
    if(ui->btn_main_11->isChecked()) return 11;
    if(ui->btn_main_12->isChecked()) return 12;
    if(ui->btn_main_13->isChecked()) return 13;
    if(ui->btn_main_14->isChecked()) return 14;
    return -1;
}
//--------------------------------------------------------------------------------
void MainBox::clear_lbl_value(void)
{
    ui->lbl_out_value_1->setText("1");
    ui->lbl_out_value_2->setText("1");
    ui->lbl_out_value_3->setText("1");
    ui->lbl_out_value_4->setText("1");
    ui->lbl_out_value_5->setText("1");
    ui->lbl_out_value_6->setText("1");
    ui->lbl_out_value_7->setText("1");
    ui->lbl_out_value_8->setText("1");

    ui->lbl_in_value_1->setText("1");
    ui->lbl_in_value_2->setText("1");
    ui->lbl_in_value_3->setText("1");
    ui->lbl_in_value_4->setText("1");
    ui->lbl_in_value_5->setText("1");
    ui->lbl_in_value_6->setText("1");
    ui->lbl_in_value_7->setText("1");
    ui->lbl_in_value_8->setText("1");
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_01(void)
{
    clear_main_checkable();
    ui->btn_main_1->setCheckable(true);
    ui->btn_main_1->setChecked(true);
    ui->btn_main_1->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(0);

    ui->lbl_out_text_1->setText("КМД. 1,2/2,4");
    ui->lbl_out_text_2->setText("КМД. БИ/ТЧ,КВ");
    ui->lbl_out_text_3->setText("КМД. ТЧ/КВ");
    ui->lbl_out_text_4->setText("КМД. К/Д");
    ui->lbl_out_text_5->setText("КМД. общ. стир.");
    ui->lbl_out_text_6->setText("КМД. Адр. 2/1");
    ui->lbl_out_text_7->setText("ВКЛ.ПУР");
    ui->lbl_out_text_8->setText("ВКЛ.АТ3017");

    ui->lbl_in_text_1->setText("Авария АПП");
    ui->lbl_in_text_2->setText("");
    ui->lbl_in_text_3->setText("");
    ui->lbl_in_text_4->setText("");
    ui->lbl_in_text_5->setText("");
    ui->lbl_in_text_6->setText("");
    ui->lbl_in_text_7->setText("Работа");
    ui->lbl_in_text_8->setText("Дуплекс");

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_02(void)
{
    clear_main_checkable();
    ui->btn_main_2->setCheckable(true);
    ui->btn_main_2->setChecked(true);
    ui->btn_main_2->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(0);

    ui->lbl_out_text_1->setText("Посылка вызова (ПВ)");
    ui->lbl_out_text_2->setText("УПР ПРД/ПРМ");
    ui->lbl_out_text_3->setText("Обход РПУ");
    ui->lbl_out_text_4->setText("");
    ui->lbl_out_text_5->setText("");
    ui->lbl_out_text_6->setText("");
    ui->lbl_out_text_7->setText("");
    ui->lbl_out_text_8->setText("");

    ui->lbl_in_text_1->setText("");
    ui->lbl_in_text_2->setText("");
    ui->lbl_in_text_3->setText("");
    ui->lbl_in_text_4->setText("");
    ui->lbl_in_text_5->setText("");
    ui->lbl_in_text_6->setText("");
    ui->lbl_in_text_7->setText("Прием вызова");
    ui->lbl_in_text_8->setText("Авария");

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_03(void)
{
    clear_main_checkable();
    ui->btn_main_3->setCheckable(true);
    ui->btn_main_3->setChecked(true);
    ui->btn_main_3->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(0);

    ui->lbl_out_text_1->setText("КМД. 1,2/2,4");
    ui->lbl_out_text_2->setText("КМД. БИ/ТЧ,КВ");
    ui->lbl_out_text_3->setText("КМД. ТЧ/КВ");
    ui->lbl_out_text_4->setText("КМД. К/Д");
    ui->lbl_out_text_5->setText("КМД. общ. стир.");
    ui->lbl_out_text_6->setText("КМД. Адр. 2/1");
    ui->lbl_out_text_7->setText("ВКЛ. ПУР");
    ui->lbl_out_text_8->setText("ВКЛ. АТ3017");

    ui->lbl_in_text_1->setText("Авария АПП");
    ui->lbl_in_text_2->setText("");
    ui->lbl_in_text_3->setText("");
    ui->lbl_in_text_4->setText("");
    ui->lbl_in_text_5->setText("");
    ui->lbl_in_text_6->setText("");
    ui->lbl_in_text_7->setText("Работа");
    ui->lbl_in_text_8->setText("Дуплекс");

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_04(void)
{
    clear_main_checkable();
    ui->btn_main_4->setCheckable(true);
    ui->btn_main_4->setChecked(true);
    ui->btn_main_4->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(0);

    ui->lbl_out_text_1->setText("Посылка вызова (ПВ)");
    ui->lbl_out_text_2->setText("УПР ПРД/ПРМ");
    ui->lbl_out_text_3->setText("Обход РПУ");
    ui->lbl_out_text_4->setText("");
    ui->lbl_out_text_5->setText("");
    ui->lbl_out_text_6->setText("");
    ui->lbl_out_text_7->setText("");
    ui->lbl_out_text_8->setText("");

    ui->lbl_in_text_1->setText("");
    ui->lbl_in_text_2->setText("");
    ui->lbl_in_text_3->setText("");
    ui->lbl_in_text_4->setText("");
    ui->lbl_in_text_5->setText("");
    ui->lbl_in_text_6->setText("");
    ui->lbl_in_text_7->setText("Прием вызова");
    ui->lbl_in_text_8->setText("Авария");

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_05(void)
{
    clear_main_checkable();
    ui->btn_main_5->setCheckable(true);
    ui->btn_main_5->setChecked(true);
    ui->btn_main_5->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(0);

    ui->lbl_out_text_1->setText("КМД. 1,2/2,4");
    ui->lbl_out_text_2->setText("КМД. БИ/ТЧ,КВ");
    ui->lbl_out_text_3->setText("КМД. ТЧ/КВ");
    ui->lbl_out_text_4->setText("КМД. К/Д");
    ui->lbl_out_text_5->setText("КМД. общ.стир.");
    ui->lbl_out_text_6->setText("КМД. Адр.2/1");
    ui->lbl_out_text_7->setText("ВКЛ.ПУР");
    ui->lbl_out_text_8->setText("ВКЛ.АТ3017");

    ui->lbl_in_text_1->setText("Авария АПП");
    ui->lbl_in_text_2->setText("");
    ui->lbl_in_text_3->setText("");
    ui->lbl_in_text_4->setText("");
    ui->lbl_in_text_5->setText("");
    ui->lbl_in_text_6->setText("");
    ui->lbl_in_text_7->setText("Работа");
    ui->lbl_in_text_8->setText("Дуплекс");

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_06(void)
{
    clear_main_checkable();
    ui->btn_main_6->setCheckable(true);
    ui->btn_main_6->setChecked(true);
    ui->btn_main_6->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(0);

    ui->lbl_out_text_1->setText("Посылка вызова (ПВ)");
    ui->lbl_out_text_2->setText("УПР ПРД/ПРМ");
    ui->lbl_out_text_3->setText("Обход РПУ");
    ui->lbl_out_text_4->setText("");
    ui->lbl_out_text_5->setText("");
    ui->lbl_out_text_6->setText("");
    ui->lbl_out_text_7->setText("");
    ui->lbl_out_text_8->setText("");

    ui->lbl_in_text_1->setText("");
    ui->lbl_in_text_2->setText("");
    ui->lbl_in_text_3->setText("");
    ui->lbl_in_text_4->setText("");
    ui->lbl_in_text_5->setText("");
    ui->lbl_in_text_6->setText("");
    ui->lbl_in_text_7->setText("Прием вызова");
    ui->lbl_in_text_8->setText("Авария");

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_07(void)
{
    clear_main_checkable();
    ui->btn_main_7->setCheckable(true);
    ui->btn_main_7->setChecked(true);
    ui->btn_main_7->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(2);

    ui->lbl_takt_1->setText(tr("..."));
    ui->lbl_takt_2->setText(tr("..."));
    ui->lbl_takt_3->setText(tr("..."));
    ui->lbl_takt_4->setText(tr("..."));
    ui->lbl_takt_5->setText(tr("..."));
    ui->lbl_takt_6->setText(tr("..."));
    ui->lbl_takt_7->setText(tr("..."));
    ui->lbl_takt_8->setText(tr("..."));
    ui->lbl_takt_9->setText(tr("..."));
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_08(void)
{
    clear_main_checkable();
    ui->btn_main_8->setCheckable(true);
    ui->btn_main_8->setChecked(true);
    ui->btn_main_8->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(2);

    ui->lbl_takt_1->setText(tr("..."));
    ui->lbl_takt_2->setText(tr("..."));
    ui->lbl_takt_3->setText(tr("..."));
    ui->lbl_takt_4->setText(tr("..."));
    ui->lbl_takt_5->setText(tr("..."));
    ui->lbl_takt_6->setText(tr("..."));
    ui->lbl_takt_7->setText(tr("..."));
    ui->lbl_takt_8->setText(tr("..."));
    ui->lbl_takt_9->setText(tr("..."));
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_09(void)
{
    clear_main_checkable();
    ui->btn_main_9->setCheckable(true);
    ui->btn_main_9->setChecked(true);
    ui->btn_main_9->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(1);
    ui->te_Result->clear();
    test_rs232_1 = new Test_RS232_1(this, ui->lnAddress->text(), ui->spbPort->value());
    test_rs232_1->start();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_10(void)
{
    clear_main_checkable();
    ui->btn_main_10->setCheckable(true);
    ui->btn_main_10->setChecked(true);
    ui->btn_main_10->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(1);
    ui->te_Result->clear();
    test_rs232_2 = new Test_RS232_2(this, ui->lnAddress->text(), ui->spbPort->value());
    test_rs232_2->start();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_11(void)
{
    clear_main_checkable();
    ui->btn_main_11->setCheckable(true);
    ui->btn_main_11->setChecked(true);
    ui->btn_main_11->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(1);
    ui->te_Result->clear();
    test_rs232_3 = new Test_RS232_3(this, ui->lnAddress->text(), ui->spbPort->value());
    test_rs232_3->start();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_12(void)
{
    clear_main_checkable();
    ui->btn_main_12->setCheckable(true);
    ui->btn_main_12->setChecked(true);
    ui->btn_main_12->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(1);
    ui->te_Result->clear();
    test_flbi_1 = new Test_FLBI_1(this, ui->lnAddress->text(), ui->spbPort->value());
    test_flbi_1->start();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_13(void)
{
    clear_main_checkable();
    ui->btn_main_13->setCheckable(true);
    ui->btn_main_13->setChecked(true);
    ui->btn_main_13->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(1);
    ui->te_Result->clear();
    test_flbi_2 = new Test_FLBI_2(this, ui->lnAddress->text(), ui->spbPort->value());
    test_flbi_2->start();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_main_14(void)
{
    clear_main_checkable();
    ui->btn_main_14->setCheckable(true);
    ui->btn_main_14->setChecked(true);
    ui->btn_main_14->setStyleSheet(COLOR_BUTTON);
    ui->stackedWidget->setCurrentIndex(1);
    ui->te_Result->clear();
    test_flbi_3 = new Test_FLBI_3(this, ui->lnAddress->text(), ui->spbPort->value());
    test_flbi_3->start();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_clear(void)
{
    QByteArray ba;
    QString temp;

    emit debug("table16");
    build_data(CMD_0x52);
    ba = client->input(data);
    temp = ba.mid(8).toHex().toUpper();
    emit debug(QString("%1 %2 %3 %4 %5 %6")
               .arg(temp.mid(0, 2))
               .arg(temp.mid(2, 2))
               .arg(temp.mid(4, 2))
               .arg(temp.mid(6, 2))
               .arg(temp.mid(8, 2))
               .arg(temp.mid(10, 2)));

    emit debug("table17");
    build_data(CMD_0x53);
    ba = client->input(data);
    temp = ba.mid(8).toHex().toUpper();
    emit debug(QString("%1 %2")
               .arg(temp.mid(0, 2))
               .arg(temp.mid(2, 2)));

    emit debug("table18");
    build_data(CMD_0x54);
    client->input(data);

    clear_lbl_value();
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_01(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-1");
//        build_data(CMD_0x10);
        build_data_ext(CMD_0x10, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_1->setText(text);
        emit debug(ba.toHex());
        break;
    case 2:
        emit debug("table7-2");
//        build_data(CMD_0x32);
        build_data_ext(CMD_0x32, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_1->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-1");
//        build_data(CMD_0x18);
        build_data_ext(CMD_0x18, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_1->setText(text);
        emit debug(ba.toHex());
        break;
    case 4:
        emit debug("table9-2");
//        build_data(CMD_0x37);
        build_data_ext(CMD_0x37, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_1->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-1");
//        build_data(CMD_0x20);
        build_data_ext(CMD_0x20, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_1->setText(text);
        emit debug(ba.toHex());
        break;
    case 6:
        emit debug("table11-2");
//        build_data(CMD_0x3C);
        build_data_ext(CMD_0x3C, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_1->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_02(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-2");
//        build_data(CMD_0x11);
        build_data_ext(CMD_0x11, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_2->setText(text);
        emit debug(ba.toHex());
        break;
    case 2:
        emit debug("table7-1");
//        build_data(CMD_0x31);
        build_data_ext(CMD_0x31, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_2->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-2");
//        build_data(CMD_0x19);
        build_data_ext(CMD_0x19, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_2->setText(text);
        emit debug(ba.toHex());
        break;
    case 4:
        emit debug("table9-1");
//        build_data(CMD_0x36);
        build_data_ext(CMD_0x36, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_2->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-2");
//        build_data(CMD_0x21);
        build_data_ext(CMD_0x21, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_2->setText(text);
        emit debug(ba.toHex());
        break;
    case 6:
        emit debug("table11-1");
//        build_data(CMD_0x3B);
        build_data_ext(CMD_0x3B, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_2->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_03(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-3");
//        build_data(CMD_0x12);
        build_data_ext(CMD_0x12, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_3->setText(text);
        emit debug(ba.toHex());
        break;
    case 2:
        emit debug("table7-3");
//        build_data(CMD_0x33);
        build_data_ext(CMD_0x33, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_3->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-3");
//        build_data(CMD_0x1A);
        build_data_ext(CMD_0x1A, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_3->setText(text);
        emit debug(ba.toHex());
        break;
    case 4:
        emit debug("table9-3");
//        build_data(CMD_0x38);
        build_data_ext(CMD_0x38, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_3->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-3");
//        build_data(CMD_0x22);
        build_data_ext(CMD_0x22, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_3->setText(text);
        emit debug(ba.toHex());
        break;
    case 6:
        emit debug("table11-3");
//        build_data(CMD_0x3D);
        build_data_ext(CMD_0x3D, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_3->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_04(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-4");
//        build_data(CMD_0x13);
        build_data_ext(CMD_0x13, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_4->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-4");
//        build_data(CMD_0x1B);
        build_data_ext(CMD_0x1B, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_4->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-4");
//        build_data(CMD_0x23);
        build_data_ext(CMD_0x23, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_4->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_05(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-5");
//        build_data(CMD_0x14);
        build_data_ext(CMD_0x14, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_5->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-5");
//        build_data(CMD_0x1C);
        build_data_ext(CMD_0x1C, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_5->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-5");
//        build_data(CMD_0x24);
        build_data_ext(CMD_0x24, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_5->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_06(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-6");
//        build_data(CMD_0x15);
        build_data_ext(CMD_0x15, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_6->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-6");
//        build_data(CMD_0x1D);
        build_data_ext(CMD_0x1D, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_6->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-6");
//        build_data(CMD_0x25);
        build_data_ext(CMD_0x25, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_6->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_07(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-7");
//        build_data(CMD_0x16);
        build_data_ext(CMD_0x16, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_7->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-7");
//        build_data(CMD_0x1E);
        build_data_ext(CMD_0x1E, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_7->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-7");
//        build_data(CMD_0x26);
        build_data_ext(CMD_0x26, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_7->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_out_08(bool state)
{
    QByteArray ba;
    int btn;
    unsigned char cmd_ext;
    QString text;

    if(state)
    {
        cmd_ext = 0xF0;
        text = "0";
    }
    else
    {
        cmd_ext = 0xAA;
        text = "1";
    }

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table1-8");
//        build_data(CMD_0x17);
        build_data_ext(CMD_0x17, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_8->setText(text);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table2-8");
//        build_data(CMD_0x1F);
        build_data_ext(CMD_0x1F, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_8->setText(text);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table3-8");
//        build_data(CMD_0x27);
        build_data_ext(CMD_0x27, cmd_ext);
        ba = client->input(data);
//        clear_lbl_value();
        ui->lbl_out_value_8->setText(text);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::check_in(uchar test_byte,
                       uchar true_byte,
                       QLabel *label)
{
    if(test_byte == true_byte)
    {
        label->setText("0");
        emit info("Выполнено");
    }
    else
    {
        label->setText("1");
        emit error(QString("Ошибка: ожидаем 0x%1, принято 0x%2")
                   .arg(true_byte, 0, 16)
                   .arg(test_byte, 0, 16));
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_01(void)
{
    QByteArray ba;
    int btn;
    unsigned char temp;

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table4-3");
        build_data(CMD_0x2A);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xFE, ui->lbl_in_value_1);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table5-3");
        build_data(CMD_0x2D);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xFE, ui->lbl_in_value_1);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table6-3");
        build_data(CMD_0x30);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xFE, ui->lbl_in_value_1);
        emit debug(ba.toHex());
        break;
    case 7:
        emit debug("table4-2");
        build_data(CMD_0x29);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_1);
        emit debug(ba.toHex());
        break;
    case 8:
        emit debug("table4-1");
        build_data(CMD_0x28);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_1);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_02(void)
{
    int btn;

    btn = get_main_checkable();
    switch(btn)
    {
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_03(void)
{
    int btn;

    btn = get_main_checkable();
    switch(btn)
    {
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_04(void)
{
    int btn;

    btn = get_main_checkable();
    switch(btn)
    {
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_05(void)
{
    int btn;

    btn = get_main_checkable();
    switch(btn)
    {
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_06(void)
{
    int btn;

    btn = get_main_checkable();
    switch(btn)
    {
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_07(void)
{
    QByteArray ba;
    int btn;
    unsigned char temp;

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table4-2");
        build_data(CMD_0x29);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_7);
        emit debug(ba.toHex());
        break;
    case 2:
        emit debug("table8-2");
        build_data(CMD_0x35);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_7);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table5-2");
        build_data(CMD_0x2C);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_7);
        emit debug(ba.toHex());
        break;
    case 4:
        emit debug("table10-2");
        build_data(CMD_0x3A);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_7);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table6-2");
        build_data(CMD_0x2F);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_7);
        emit debug(ba.toHex());
        break;
    case 6:
        emit debug("table12-2");
        build_data(CMD_0x3F);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0xBF, ui->lbl_in_value_7);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_btn_in_08(void)
{
    QByteArray ba;
    unsigned char temp;
    int btn;

    btn = get_main_checkable();
    switch(btn)
    {
    case 1:
        emit debug("table4-1");
        build_data(CMD_0x28);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_8);
        emit debug(ba.toHex());
        break;
    case 2:
        emit debug("table8-1");
        build_data(CMD_0x34);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_8);
        emit debug(ba.toHex());
        break;
    case 3:
        emit debug("table5-1");
        build_data(CMD_0x2B);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_8);
        emit debug(ba.toHex());
        break;
    case 4:
        emit debug("table10-1");
        build_data(CMD_0x39);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_8);
        emit debug(ba.toHex());
        break;
    case 5:
        emit debug("table6-1");
        build_data(CMD_0x2E);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_8);
        emit debug(ba.toHex());
        break;
    case 6:
        emit debug("table12-1");
        build_data(CMD_0x3E);
        ba = client->input(data);
        temp = ba.at(8);
        check_in(temp, 0x7F, ui->lbl_in_value_8);
        emit debug(ba.toHex());
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_1(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-1");
        build_data(CMD_0x40);
        ba = client->input(data);
        //        emit debug(ba.toHex());
        r1 = check_xF(ba.at(8), 13, 1);
        r2 = check(ba.at(9), 0xF0, 13, 1);
        r3 = check_bits(ba.at(10), 0, 1, 4, 1, 13, 1);
        r4 = check_bits(ba.at(11), 0, 1, 4, 1, 13, 1);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_1->setText(tr("Выполнено"));
        else
            ui->lbl_takt_1->setText(tr("Ошибка"));
        break;

    case 8:
        emit debug("table14-1");
        build_data(CMD_0x49);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 14, 1);
        r2 = check(ba.at(9), 0xF0, 14, 1);
        r3 = check_bits(ba.at(10), 0, 0, 4, 1, 14, 1);
        r4 = check_bits(ba.at(11), 0, 0, 4, 1, 14, 1);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_1->setText(tr("Выполнено"));
        else
            ui->lbl_takt_1->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_2(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-2");
        build_data(CMD_0x41);
        ba = client->input(data);
        emit debug(ba.mid(8).toHex().toUpper());
        r1 = check_xF(ba.at(8), 13, 2);
        r2 = check(ba.at(9), 0x0F, 13, 2);
        r3 = check_bits(ba.at(10), 1, 1, 5, 1, 13, 2);
        r4 = check_bits(ba.at(11), 1, 1, 5, 1, 13, 2);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_2->setText(tr("Выполнено"));
        else
            ui->lbl_takt_2->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-2");
        build_data(CMD_0x4A);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 14, 2);
        r2 = check(ba.at(9), 0x0F, 14, 2);
        r3 = check_bits(ba.at(10), 1, 0, 5, 1, 14, 2);
        r4 = check_bits(ba.at(11), 1, 0, 5, 1, 14, 2);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_2->setText(tr("Выполнено"));
        else
            ui->lbl_takt_2->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_3(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-3");
        build_data(CMD_0x42);
        ba = client->input(data);
        r1 = check_x0(ba.at(8), 13, 3);
        r2 = check(ba.at(9), 0xFF, 13, 3);
        r3 = check_bits(ba.at(10), 2, 1, 6, 1, 13, 3);
        r4 = check_bits(ba.at(11), 2, 1, 6, 1, 13, 3);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_3->setText(tr("Выполнено"));
        else
            ui->lbl_takt_3->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-3");
        build_data(CMD_0x4B);
        ba = client->input(data);
        r1 = check_x0(ba.at(8), 14, 3);
        r2 = check(ba.at(9), 0xFF, 14, 3);
        r3 = check_bits(ba.at(10), 2, 0, 6, 1, 14, 3);
        r4 = check_bits(ba.at(11), 2, 0, 6, 1, 14, 3);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_3->setText(tr("Выполнено"));
        else
            ui->lbl_takt_3->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_4(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-4");
        build_data(CMD_0x43);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 13, 4);
        r2 = check(ba.at(9), 0xF0, 13, 4);
        r3 = check_bits(ba.at(10), 0, 1, 4, 1, 13, 4);
        r4 = check_bits(ba.at(11), 1, 1, 5, 1, 13, 4);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_4->setText(tr("Выполнено"));
        else
            ui->lbl_takt_4->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-4");
        build_data(CMD_0x4C);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 14, 4);
        r2 = check(ba.at(9), 0xF0, 14, 4);
        r3 = check_bits(ba.at(10), 0, 0, 4, 1, 14, 4);
        r4 = check_bits(ba.at(11), 0, 0, 4, 1, 14, 4);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_4->setText(tr("Выполнено"));
        else
            ui->lbl_takt_4->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_5(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-5");
        build_data(CMD_0x44);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 13, 5);
        r2 = check(ba.at(9), 0x0F, 13, 5);
        r3 = check_bits(ba.at(10), 2, 1, 6, 1, 13, 5);
        r4 = check_bits(ba.at(11), 2, 1, 6, 1, 13, 5);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_5->setText(tr("Выполнено"));
        else
            ui->lbl_takt_5->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-5");
        build_data(CMD_0x4D);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 14, 5);
        r2 = check(ba.at(9), 0x0F, 14, 5);
        r3 = check_bits(ba.at(10), 1, 0, 5, 1, 14, 5);
        r4 = check_bits(ba.at(11), 1, 0, 5, 1, 14, 5);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_5->setText(tr("Выполнено"));
        else
            ui->lbl_takt_5->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_6(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-6");
        build_data(CMD_0x45);
        ba = client->input(data);
        r1 = check_x0(ba.at(8), 13, 6);
        r2 = check(ba.at(9), 0xFF, 13, 6);
        r3 = check_bits(ba.at(10), 2, 1, 6, 1, 13, 6);
        r4 = check_bits(ba.at(11), 2, 1, 6, 1, 13, 6);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_6->setText(tr("Выполнено"));
        else
            ui->lbl_takt_6->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-6");
        build_data(CMD_0x4E);
        ba = client->input(data);
        r1 = check_x0(ba.at(8), 14, 6);
        r2 = check(ba.at(9), 0xFF, 14, 6);
        r3 = check_bits(ba.at(10), 2, 0, 6, 1, 14, 6);
        r4 = check_bits(ba.at(11), 2, 0, 6, 1, 14, 6);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_6->setText(tr("Выполнено"));
        else
            ui->lbl_takt_6->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_7(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-7");
        build_data(CMD_0x46);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 13, 7);
        r2 = check(ba.at(9), 0xF0, 13, 7);
        r3 = check_bits(ba.at(10), 0, 1, 4, 1, 13, 7);
        r4 = check_bits(ba.at(11), 0, 1, 4, 1, 13, 7);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_7->setText(tr("Выполнено"));
        else
            ui->lbl_takt_7->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-7");
        build_data(CMD_0x4F);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 14, 7);
        r2 = check(ba.at(9), 0xF0, 14, 7);
        r3 = check_bits(ba.at(10), 0, 0, 4, 1, 14, 7);
        r4 = check_bits(ba.at(11), 0, 0, 4, 1, 14, 7);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_7->setText(tr("Выполнено"));
        else
            ui->lbl_takt_7->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_8(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-8");
        build_data(CMD_0x47);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 13, 8);
        r2 = check(ba.at(9), 0x0F, 13, 8);
        r3 = check_bits(ba.at(10), 1, 1, 5, 1, 13, 8);
        r4 = check_bits(ba.at(11), 1, 1, 5, 1, 13, 8);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_8->setText(tr("Выполнено"));
        else
            ui->lbl_takt_8->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-8");
        build_data(CMD_0x50);
        ba = client->input(data);
        r1 = check_xF(ba.at(8), 14, 8);
        r2 = check(ba.at(9), 0x0F, 14, 8);
        r3 = check_bits(ba.at(10), 1, 0, 5, 1, 14, 8);
        r4 = check_bits(ba.at(11), 1, 0, 5, 1, 14, 8);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_8->setText(tr("Выполнено"));
        else
            ui->lbl_takt_8->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::clicked_takt_9(void)
{
    int btn;
    QByteArray ba;
    bool r1, r2, r3, r4;

    btn = get_main_checkable();
    switch(btn)
    {
    case 7:
        emit debug("table13-9");
        build_data(CMD_0x48);
        ba = client->input(data);
        r1 = check_x0(ba.at(8), 13, 9);
        r2 = check(ba.at(9), 0xFF, 13, 9);
        r3 = check_bits(ba.at(10), 2, 1, 6, 1, 13, 9);
        r4 = check_bits(ba.at(11), 2, 1, 6, 1, 13, 9);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_9->setText(tr("Выполнено"));
        else
            ui->lbl_takt_9->setText(tr("Ошибка"));
        break;
    case 8:
        emit debug("table14-9");
        build_data(CMD_0x51);
        ba = client->input(data);
        r1 = check_x0(ba.at(8), 14, 9);
        r2 = check(ba.at(9), 0xFF, 14, 9);
        r3 = check_bits(ba.at(10), 2, 0, 6, 1, 14, 9);
        r4 = check_bits(ba.at(11), 2, 0, 6, 1, 14, 9);
        if(r1 && r2 && r3 && r4)
            ui->lbl_takt_9->setText(tr("Выполнено"));
        else
            ui->lbl_takt_9->setText(tr("Ошибка"));
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::customEvent(QEvent* event)
{
    InfoEvent* info_event = static_cast<InfoEvent*>(event);
    if (!info_event) return;
    print(info_event->text, info_event->type, info_event->bold);
}
//--------------------------------------------------------------------------------
void MainBox::print(const QString& text, int type, bool bold)
{
    QString str = text;
    if (type == 1) str = QString("<font color=green>%1</font>").arg(str);
    if (type == -1) str = QString("<font color=red>%1</font>").arg(str);
    if (bold) str = QString("<strong>%1</strong>").arg(str);
    ui->te_Result->append(str);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        // updateText();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
