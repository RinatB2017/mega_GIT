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
#   include <QProgressDialog>
#   include <QVBoxLayout>
#   include <QMessageBox>
#   include <QStyle>
#   include <QTime>

#   include <QToolButton>
#   include <QToolBar>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_b590_imitator.h"
#include "sleeper.h"
#include "crc.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "upacket.hpp"
#include "b590_imitator.hpp"
//--------------------------------------------------------------------------------
#include "b590_packets.hpp"
//--------------------------------------------------------------------------------
B590_imitator::B590_imitator(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::B590_imitator)
{
    init();
}
//--------------------------------------------------------------------------------
B590_imitator::~B590_imitator()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void B590_imitator::init(void)
{
    ui->setupUi(this);

    up = new UPacket;
    connect(up, SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(up, SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(up, SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(up, SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    up->set_address(ui->sb_addr->value());

//    commands.append({ B590_CMD_0x11, B590_CMD_0x11_TEXT });
//    commands.append({ B590_CMD_0x12, B590_CMD_0x12_TEXT });
//    commands.append({ B590_CMD_0x15, B590_CMD_0x15_TEXT });
//    commands.append({ B590_CMD_0x16, B590_CMD_0x16_TEXT });
//    commands.append({ B590_CMD_0x17, B590_CMD_0x17_TEXT });

//    commands.append({ B590_CMD_0x41, B590_CMD_0x41_TEXT });
//    commands.append({ B590_CMD_0x42, B590_CMD_0x42_TEXT });
    commands.append({ B590_CMD_0x46, B590_CMD_0x46_TEXT });
//    commands.append({ B590_CMD_0x47, B590_CMD_0x47_TEXT });
//    commands.append({ B590_CMD_0x48, B590_CMD_0x48_TEXT });
//    commands.append({ B590_CMD_0x49, B590_CMD_0x49_TEXT });
//    commands.append({ B590_CMD_0x4A, B590_CMD_0x4A_TEXT });

//    commands.append({ B590_CMD_0x54, B590_CMD_0x54_TEXT });
//    commands.append({ B590_CMD_0x55, B590_CMD_0x55_TEXT });
//    commands.append({ B590_CMD_0x56, B590_CMD_0x56_TEXT });
//    commands.append({ B590_CMD_0x57, B590_CMD_0x57_TEXT });
//    commands.append({ B590_CMD_0x5A, B590_CMD_0x5A_TEXT });
//    commands.append({ B590_CMD_0x5B, B590_CMD_0x5B_TEXT });
//    commands.append({ B590_CMD_0x5C, B590_CMD_0x5C_TEXT });
//    commands.append({ B590_CMD_0x5D, B590_CMD_0x5D_TEXT });
//    commands.append({ B590_CMD_0x5E, B590_CMD_0x5E_TEXT });
//    commands.append({ B590_CMD_0x5F, B590_CMD_0x5F_TEXT });

//    commands.append({ B590_CMD_0x60, B590_CMD_0x60_TEXT });
//    commands.append({ B590_CMD_0x61, B590_CMD_0x61_TEXT });
//    commands.append({ B590_CMD_0x62, B590_CMD_0x62_TEXT });
//    commands.append({ B590_CMD_0x63, B590_CMD_0x63_TEXT });
//    commands.append({ B590_CMD_0x64, B590_CMD_0x64_TEXT });
//    commands.append({ B590_CMD_0x65, B590_CMD_0x65_TEXT });
//    commands.append({ B590_CMD_0x66, B590_CMD_0x66_TEXT });
//    commands.append({ B590_CMD_0x67, B590_CMD_0x67_TEXT });
//    commands.append({ B590_CMD_0x68, B590_CMD_0x68_TEXT });
//    commands.append({ B590_CMD_0x69, B590_CMD_0x69_TEXT });
//    commands.append({ B590_CMD_0x6A, B590_CMD_0x6A_TEXT });
//    commands.append({ B590_CMD_0x6B, B590_CMD_0x6B_TEXT });
//    commands.append({ B590_CMD_0x6C, B590_CMD_0x6C_TEXT });
//    commands.append({ B590_CMD_0x6D, B590_CMD_0x6D_TEXT });
//    commands.append({ B590_CMD_0x6E, B590_CMD_0x6E_TEXT });
//    commands.append({ B590_CMD_0x6F, B590_CMD_0x6F_TEXT });

//    commands.append({ B590_CMD_0x70, B590_CMD_0x70_TEXT });
//    commands.append({ B590_CMD_0x71, B590_CMD_0x71_TEXT });
//    commands.append({ B590_CMD_0x72, B590_CMD_0x72_TEXT });
//    commands.append({ B590_CMD_0x73, B590_CMD_0x73_TEXT });
//    commands.append({ B590_CMD_0x75, B590_CMD_0x75_TEXT });

    ui->sb_addr->setRange(MIN_ADDRESS_B590, MAX_ADDRESS_B590);

    ui->serial_widget->set_caption("RS232");

    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,   SLOT(output(QByteArray)));
    connect(this,               SIGNAL(send(QByteArray)),   this,   SLOT(input(QByteArray)));

    connect(ui->sb_addr,    SIGNAL(valueChanged(int)),  up, SLOT(set_address(int)));

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));
}
//--------------------------------------------------------------------------------
void B590_imitator::test(void)
{
    Q_CHECK_PTR(up);

    //up->set_address(ui->sb_addr->value());
    emit info(QString("addr %1").arg(up->get_address()));
    emit info(QString("speed %1").arg(ui->serial_widget->get_baudRate()));
}
//--------------------------------------------------------------------------------
void B590_imitator::input(QByteArray data)
{
    int err = ui->serial_widget->input(data);
    if(err != SerialBox5::E_NO_ERROR)
    {
        emit error(QString("err: %1").arg(err));
        return;
    }
    emit info("Data sending...");
}
//--------------------------------------------------------------------------------
void B590_imitator::output(QByteArray data)
{
    emit debug(QString("output: [%1]").arg(data.toHex().data()));

    int cmd = 0;
    QByteArray r_packet;
    up->set_address(ui->sb_addr->value());  //FIXME надо убрать в дальнейшем
    bool ok = up->check_packet(data, &cmd, &r_packet);
    if(!ok)
    {
        emit error(up->get_err_str());
        return;
    }
    foreach (CMD c, commands)
    {
        if(cmd == c.cmd)
        {
            emit info("FOUND");

            B590_CMD_46_ANSWER answer;

            answer.body.header.address = ui->sb_addr->value();
            answer.body.header.cmd = B590_CMD_0x46;
            answer.body.header.count_data = sizeof(answer.body.data);

            answer.body.data.type  = 4;
            answer.body.data.year  = 1;
            answer.body.data.month = 1;
            answer.body.data.serno = 123;

            answer.body.crc16 = CRC::powersupply_crc16((unsigned char *)&answer.buf, sizeof(B590_CMD_46_ANSWER) - 2);

            QByteArray output;
            output.clear();
            output.append((const char *)answer.buf, sizeof(B590_CMD_46_ANSWER));

            Sleeper::msleep(100);   //TODO

            emit send(output);
            emit debug(QString("emit send(%1)").arg(output.toHex().data()));
            return;
        }
    }
    emit info("NOT FOUND");
}
//--------------------------------------------------------------------------------
void B590_imitator::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
