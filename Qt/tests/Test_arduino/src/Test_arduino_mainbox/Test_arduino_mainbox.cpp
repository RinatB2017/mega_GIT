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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_Test_arduino_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "Test_arduino_mainbox.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    serial = new SerialBox5(this, "Arduino");

    connect(this, SIGNAL(send(QByteArray)), serial, SLOT(input(QByteArray)));
    connect(serial, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));

    ui->serial_layout->addWidget(serial, 1);
    ui->serial_layout->addStretch(100);

    ui->Slider_R->setMaximum(LED_MAX_VALUE);
    ui->Slider_G->setMaximum(LED_MAX_VALUE);
    ui->Slider_B->setMaximum(LED_MAX_VALUE);
    ui->Slider_W->setMaximum(LED_MAX_VALUE);

    ui->Slider_motor_1->setMaximum(MOTOR_MAX_ANGLE);
    ui->Slider_motor_2->setMaximum(MOTOR_MAX_ANGLE);

    ui->sb_R->setReadOnly(true);
    ui->sb_G->setReadOnly(true);
    ui->sb_B->setReadOnly(true);
    ui->sb_W->setReadOnly(true);

    ui->sb_R->setMaximum(LED_MAX_VALUE);
    ui->sb_G->setMaximum(LED_MAX_VALUE);
    ui->sb_B->setMaximum(LED_MAX_VALUE);
    ui->sb_W->setMaximum(LED_MAX_VALUE);

    ui->sb_Motor_1->setReadOnly(true);
    ui->sb_Motor_2->setReadOnly(true);

    ui->sb_Motor_1->setMaximum(MOTOR_MAX_ANGLE);
    ui->sb_Motor_2->setMaximum(MOTOR_MAX_ANGLE);

    clear_data();

    set_color_frame(0, 0, 0);

    connect(ui->Slider_R,       SIGNAL(valueChanged(int)), ui->sb_R, SLOT(setValue(int)));
    connect(ui->Slider_G,       SIGNAL(valueChanged(int)), ui->sb_G, SLOT(setValue(int)));
    connect(ui->Slider_B,       SIGNAL(valueChanged(int)), ui->sb_B, SLOT(setValue(int)));
    connect(ui->Slider_W,       SIGNAL(valueChanged(int)), ui->sb_W, SLOT(setValue(int)));

    connect(ui->Slider_motor_1, SIGNAL(valueChanged(int)), ui->sb_Motor_1, SLOT(setValue(int)));
    connect(ui->Slider_motor_2, SIGNAL(valueChanged(int)), ui->sb_Motor_2, SLOT(setValue(int)));

    connect(ui->btn_R, SIGNAL(clicked()), this, SLOT(command_set_color_R()));
    connect(ui->btn_G, SIGNAL(clicked()), this, SLOT(command_set_color_G()));
    connect(ui->btn_B, SIGNAL(clicked()), this, SLOT(command_set_color_B()));
    connect(ui->btn_W, SIGNAL(clicked()), this, SLOT(command_set_color_W()));

    connect(ui->btnSetColor,    SIGNAL(clicked()), this, SLOT(command_set_color_all()));

    connect(ui->btn_motor_1, SIGNAL(clicked()), this, SLOT(command_run_motor_1()));
    connect(ui->btn_motor_2, SIGNAL(clicked()), this, SLOT(command_run_motor_2()));

    connect(ui->btnRunMotors,   SIGNAL(clicked()), this, SLOT(command_run_motors()));

#if 1
    connect(ui->Slider_motor_1, SIGNAL(sliderReleased()), this, SLOT(command_run_motor_1()));
    connect(ui->Slider_motor_2, SIGNAL(sliderReleased()), this, SLOT(command_run_motor_2()));

    connect(ui->Slider_R,       SIGNAL(sliderReleased()), this, SLOT(command_set_color_R()));
    connect(ui->Slider_G,       SIGNAL(sliderReleased()), this, SLOT(command_set_color_G()));
    connect(ui->Slider_B,       SIGNAL(sliderReleased()), this, SLOT(command_set_color_B()));
    connect(ui->Slider_W,       SIGNAL(sliderReleased()), this, SLOT(command_set_color_W()));
#endif

    connect(ui->Slider_R,       SIGNAL(sliderMoved(int)), this, SLOT(color_changed(int)));
    connect(ui->Slider_G,       SIGNAL(sliderMoved(int)), this, SLOT(color_changed(int)));
    connect(ui->Slider_B,       SIGNAL(sliderMoved(int)), this, SLOT(color_changed(int)));
    connect(ui->Slider_W,       SIGNAL(sliderMoved(int)), this, SLOT(color_changed(int)));

    connect(ui->btnChoice,      SIGNAL(clicked()), this, SLOT(set_color()));

    QTimer::singleShot(500, this, SLOT(update_frame_color()));
}
//--------------------------------------------------------------------------------
void MainBox::update_frame_color(void)
{
    //qDebug() << ui->Slider_R->value() << ui->Slider_G->value() << ui->Slider_B->value();
    set_color_frame(ui->Slider_R->value(),
                    ui->Slider_G->value(),
                    ui->Slider_B->value());
}
//--------------------------------------------------------------------------------
void MainBox::color_changed(int)
{
    if(ui->cb_Tie->isChecked())
    {
        QSlider *slider = (QSlider *)sender();
        if(slider->objectName() == "Slider_R")
        {
            ui->Slider_G->setValue(ui->Slider_R->value());
            ui->Slider_B->setValue(ui->Slider_R->value());
        }
        if(slider->objectName() == "Slider_G")
        {
            ui->Slider_R->setValue(ui->Slider_G->value());
            ui->Slider_B->setValue(ui->Slider_G->value());
        }
        if(slider->objectName() == "Slider_B")
        {
            ui->Slider_R->setValue(ui->Slider_B->value());
            ui->Slider_G->setValue(ui->Slider_B->value());
        }
    }

    set_color_frame(ui->Slider_R->value(),
                    ui->Slider_G->value(),
                    ui->Slider_B->value());
}
//--------------------------------------------------------------------------------
void MainBox::read_data(const QByteArray &data)
{
    received_array.append(data);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::clear_data(void)
{
    for(int n=0; n<MAX_EEPROM_BYTES; n++)
    {
        eeprom_buffer[n] = 0xFF;
    }
}
//--------------------------------------------------------------------------------
void MainBox::append_data(char *buf, int len)
{
    for(int n=0; n<len; n++)
    {
        sending_array.append(buf[n]);
    }
}
//--------------------------------------------------------------------------------
void MainBox::append_end_byte(void)
{
    sending_array.append(END_BYTE);
}
//--------------------------------------------------------------------------------
void MainBox::clear_sending_array(void)
{
    sending_array.clear();
}
//--------------------------------------------------------------------------------
void MainBox::clear_receive_array(void)
{
    received_array.clear();
}
//--------------------------------------------------------------------------------
void MainBox::wait_msec(int timeout_msec)
{
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::send_data(void)
{
    if(serial->isOpen() == false)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Порт не открыт!"));
        return;
    }

    //emit debug(sending_array.toHex());

    clear_receive_array();
    emit send(sending_array);
    wait_msec(500);

    int len = received_array.length();
    emit info(QString("received %1 bytes").arg(len));
    emit info(received_array.toHex());
}
//--------------------------------------------------------------------------------
void MainBox::set_color_frame(int color_R,
                              int color_G,
                              int color_B)
{
    ui->Color_Frame->setStyleSheet(QString("background-color:rgb(%1,%2,%3)")
                                   .arg(color_R)
                                   .arg(color_G)
                                   .arg(color_B));
}
//--------------------------------------------------------------------------------
void MainBox::set_color(void)
{
    QColorDialog *dlg = new QColorDialog(this);
    qDebug() << ui->Slider_R->value() << ui->Slider_G->value() << ui->Slider_B->value();

    QColor color;
    color.setRed(ui->Slider_R->value() - 1);
    color.setGreen(ui->Slider_G->value() - 1);
    color.setBlue(ui->Slider_B->value() - 1);
    qDebug() << color;

    dlg->setCurrentColor(color);
//    dlg->setCurrentColor(QColor(ui->Slider_R->value(),
//                                ui->Slider_G->value(),
//                                ui->Slider_B->value()));

    int button = dlg->exec();
    if(button == QColorDialog::Accepted)
    {
        QColor color = dlg->selectedColor();
        int color_R = color.red();
        int color_G = color.green();
        int color_B = color.blue();
        emit info(QString("R %1 G %2 B %3")
                  .arg(color_R)
                  .arg(color_G)
                  .arg(color_B));
        ui->Slider_R->setValue(color_R);
        ui->Slider_G->setValue(color_G);
        ui->Slider_B->setValue(color_B);
        set_color_frame(color_R, color_G, color_B);
    }
}
//--------------------------------------------------------------------------------
unsigned char MainBox::calculateCRC_8(char *pcBlock, unsigned int len)
{
    unsigned char crc = 0xFF;
    unsigned int i;

    while (len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}
//--------------------------------------------------------------------------------
unsigned int MainBox::calculateCRC_16(unsigned char *frame, unsigned char bufferSize)
{
    unsigned int temp, temp2, flag;
    temp = 0xFFFF;
    for (unsigned char i = 0; i < bufferSize; i++)
    {
        temp = temp ^ frame[i];
        for (unsigned char j = 1; j <= 8; j++)
        {
            flag = temp & 0x0001;
            temp >>= 1;
            if (flag)
                temp ^= 0xA001;
        }
    }
    // Reverse byte order.
    temp2 = temp >> 8;
    temp = (temp << 8) | temp2;
    temp &= 0xFFFF;
    // the returned value is already swapped
    // crcLo byte is first & crcHi byte is last
    return temp;
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_command(unsigned short cmd)
{
    MODBUS_FULL full;
    full.data.address_slave = 1;
    full.data.modbus_cmd = 16;
    full.data.address = htons(0);
    full.data.count = htons(7);
    full.data.count_bytes = 14;
    full.data.command = htons(cmd);
    full.data.value_R = htons(ui->Slider_R->value());
    full.data.value_G = htons(ui->Slider_G->value());
    full.data.value_B = htons(ui->Slider_B->value());
    full.data.value_W = htons(ui->Slider_W->value());
    full.data.motor_1 = htons(ui->Slider_motor_1->value());
    full.data.motor_2 = htons(ui->Slider_motor_2->value());

    for(unsigned int n=0; n<sizeof(MODBUS_FULL); n++)
    {
        sending_array.append(full.buf[n]);
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_color_all(void)
{
    prepare_modbus_command(CMD_SET_ALL_COLOR);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_color_R(void)
{
    prepare_modbus_command(CMD_SET_COLOR_R);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_color_G(void)
{
    prepare_modbus_command(CMD_SET_COLOR_G);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_color_B(void)
{
    prepare_modbus_command(CMD_SET_COLOR_B);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_color_W(void)
{
    prepare_modbus_command(CMD_SET_COLOR_W);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_motor_1(void)
{
    prepare_modbus_command(CMD_RUN_MOTOR_1);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_motor_2(void)
{
    prepare_modbus_command(CMD_RUN_MOTOR_2);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_motor_all(void)
{
    prepare_modbus_command(CMD_RUN_ALL_MOTOR);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_modbus_set_all(void)
{
    prepare_modbus_command(CMD_SET_ALL);
}
//--------------------------------------------------------------------------------
void MainBox::append_crc(void)
{
    unsigned int crc16 = calculateCRC_16((unsigned char *)sending_array.data(), sending_array.size());
    sending_array.append((char)((crc16 >> 8) & 0xFF));
    sending_array.append((char)(crc16 & 0xFF));
    //emit debug(QString("CRC %1").arg(crc16, 0, 16));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");

#if 0
    while(ui->Slider_R->value() < ui->Slider_R->maximum())
    {
        QCoreApplication::processEvents();
        ui->Slider_R->setValue(ui->Slider_R->value() + 10);
        command_set_color_R();
        Sleeper::msleep(500);
    }
    while(ui->Slider_R->value() > ui->Slider_R->minimum())
    {
        QCoreApplication::processEvents();
        ui->Slider_R->setValue(ui->Slider_R->value() - 10);
        command_set_color_R();
        Sleeper::msleep(500);
    }
#endif

#if 1
    clear_sending_array();
    prepare_modbus_set_all();
    append_crc();
    send_data();
#endif

    emit info("the end");
}
//--------------------------------------------------------------------------------
void MainBox::command_set_color_R(void)
{
    emit info("command_set_color_R");

    clear_sending_array();
    prepare_modbus_color_R();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_set_color_G(void)
{
    emit info("command_set_color_G");

    clear_sending_array();
    prepare_modbus_color_G();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_set_color_B(void)
{
    emit info("command_set_color_B");

    clear_sending_array();
    prepare_modbus_color_B();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_set_color_W(void)
{
    emit info("command_set_color_W");

    clear_sending_array();
    prepare_modbus_color_W();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_set_color_all(void)
{
    emit info("command_set_color_all");

    clear_sending_array();
    prepare_modbus_color_all();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_run_motor_1(void)
{
    emit info("command_run_motor_1");

    clear_sending_array();
    prepare_modbus_motor_1();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_run_motor_2(void)
{
    emit info("command_run_motor_2");

    clear_sending_array();
    prepare_modbus_motor_2();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_run_motors(void)
{
    emit info("command_run_motors");

    clear_sending_array();
    prepare_modbus_motor_all();
    append_crc();

    send_data();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
