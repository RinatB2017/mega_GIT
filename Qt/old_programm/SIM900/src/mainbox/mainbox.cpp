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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox(0)
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

    ui->cb_command->addItem(SIM900_GMM_TEXT, QVariant(Qt::UserRole + SIM900_GMM));
    ui->cb_command->addItem(SIM900_GOI_TEXT, QVariant(Qt::UserRole + SIM900_GOI));
    ui->cb_command->addItem(SIM900_GMR_TEXT, QVariant(Qt::UserRole + SIM900_GMR));
    ui->cb_command->addItem(SIM900_GSV_TEXT, QVariant(Qt::UserRole + SIM900_GSV));
    ui->cb_command->addItem(SIM900_GSN_TEXT, QVariant(Qt::UserRole + SIM900_GSN));
    ui->cb_command->insertSeparator(ui->cb_command->count());

    ui->cb_command->addItem(SIM900_CSQ_TEXT, QVariant(Qt::UserRole + SIM900_CSQ));
    ui->cb_command->addItem(SIM900_CREG_A_TEXT, QVariant(Qt::UserRole + SIM900_CREG_A));
    ui->cb_command->addItem(SIM900_CREG_Q_TEXT, QVariant(Qt::UserRole + SIM900_CREG_Q));
    ui->cb_command->addItem(SIM900_CSPN_TEXT, QVariant(Qt::UserRole + SIM900_CSPN));
    ui->cb_command->addItem(SIM900_COPS_TEXT, QVariant(Qt::UserRole + SIM900_COPS));
    ui->cb_command->addItem(SIM900_CPAS_TEXT, QVariant(Qt::UserRole + SIM900_CPAS));
    ui->cb_command->addItem(SIM900_CCLK_A_TEXT, QVariant(Qt::UserRole + SIM900_CCLK_A));
    ui->cb_command->addItem(SIM900_CCLK_Q_TEXT, QVariant(Qt::UserRole + SIM900_CCLK_Q));
    ui->cb_command->insertSeparator(ui->cb_command->count());

    ui->cb_command->addItem(SIM900_ATE0_TEXT, QVariant(Qt::UserRole + SIM900_ATE0));
    ui->cb_command->addItem(SIM900_ATV0_TEXT, QVariant(Qt::UserRole + SIM900_ATV0));
    ui->cb_command->addItem(SIM900_CMEE_TEXT, QVariant(Qt::UserRole + SIM900_CMEE));
    ui->cb_command->insertSeparator(ui->cb_command->count());

    ui->cb_command->addItem(SIM900_ATD_TEXT, QVariant(Qt::UserRole + SIM900_ATD));
    ui->cb_command->addItem(SIM900_ATD1_TEXT, QVariant(Qt::UserRole + SIM900_ATD1));
    ui->cb_command->addItem(SIM900_ATDL_TEXT, QVariant(Qt::UserRole + SIM900_ATDL));
    ui->cb_command->addItem(SIM900_CLIP_TEXT, QVariant(Qt::UserRole + SIM900_CLIP));
    ui->cb_command->addItem(SIM900_ATA_TEXT, QVariant(Qt::UserRole + SIM900_ATA));
    ui->cb_command->addItem(SIM900_ATH0_TEXT, QVariant(Qt::UserRole + SIM900_ATH0));
    ui->cb_command->addItem(SIM900_ATS0_TEXT, QVariant(Qt::UserRole + SIM900_ATS0));
    ui->cb_command->addItem(SIM900_GSMBUSY_TEXT, QVariant(Qt::UserRole + SIM900_GSMBUSY));
    ui->cb_command->insertSeparator(ui->cb_command->count());

    ui->cb_command->addItem(SIM900_CSCA_TEXT, QVariant(Qt::UserRole + SIM900_CSCA));
    ui->cb_command->addItem(SIM900_CMGF_TEXT, QVariant(Qt::UserRole + SIM900_CMGF));
    ui->cb_command->addItem(SIM900_CSCB_TEXT, QVariant(Qt::UserRole + SIM900_CSCB));
    ui->cb_command->addItem(SIM900_CMGL_TEXT, QVariant(Qt::UserRole + SIM900_CMGL));
    ui->cb_command->addItem(SIM900_CMGDA_TEXT, QVariant(Qt::UserRole + SIM900_CMGDA));
    ui->cb_command->addItem(SIM900_CMGD_TEXT, QVariant(Qt::UserRole + SIM900_CMGD));
    ui->cb_command->addItem(SIM900_CMGS_TEXT, QVariant(Qt::UserRole + SIM900_CMGS));
    ui->cb_command->insertSeparator(ui->cb_command->count());

    ui->cb_command->addItem(SIM900_CUSD_TEXT);

    serialBox = new SerialBox5(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
    connect(ui->btn_run_command, SIGNAL(clicked()), this, SLOT(run_command()));
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QTime time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        if(is_ready)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug("read_data");
    data_rs232.append(ba);
    is_ready = true;
}
//--------------------------------------------------------------------------------
void MainBox::send_command(const QString &command)
{
    QByteArray ba;
    ba.append(command);
    ba.append(0x0D);
    ba.append(0x0A);
    emit trace(ba);
    emit send(ba);
}
//--------------------------------------------------------------------------------
void MainBox::run_command(void)
{
    int cmd = ui->cb_command->itemData(ui->cb_command->currentIndex(), Qt::UserRole).toInt() - Qt::UserRole;
    switch(cmd)
    {
    case SIM900_AT:
        emit info(SIM900_AT_TEXT);
        send_command("AT");
        break;

    case SIM900_GMM:
        emit info(SIM900_GMM_TEXT);
        send_command("AT+GMM");
        break;

    case SIM900_GOI:
        emit info(SIM900_GOI_TEXT);
        send_command("AT+GOI");
        break;

    case SIM900_GMR:
        emit info(SIM900_GMR_TEXT);
        send_command("AT+GMR");
        break;

    case SIM900_GSV:
        emit info(SIM900_GSV_TEXT);
        send_command("AT+GSV");
        break;

    case SIM900_GSN:
        emit info(SIM900_GSN_TEXT);
        send_command("AT+GSN");
        break;

    case SIM900_CSQ:
        emit info(SIM900_CSQ_TEXT);
        send_command("AT+CSQ");
        break;

    case SIM900_CREG_Q:
        emit info(SIM900_CREG_Q_TEXT);
        send_command("AT+CREG=1");
        break;

    case SIM900_CREG_A:
        emit info(SIM900_CREG_A_TEXT);
        send_command("AT+CREG?");
        break;

    case SIM900_CSPN:
        emit info(SIM900_CSPN_TEXT);
        send_command("AT+CSPN?");
        break;

    case SIM900_COPS:
        emit info(SIM900_COPS_TEXT);
        send_command("AT+COPS?");
        break;

    case SIM900_CPAS:
        emit info(SIM900_CPAS_TEXT);
        send_command("AT+CPAS");
        break;

    case SIM900_CCLK_A:
        emit info(SIM900_CCLK_A_TEXT);
        send_command("AT+CCLK?"); //Вернет текущее время в виде: +CCLK: «11/04/11,01:46:33+00»
        break;

    case SIM900_CCLK_Q:
        emit info(SIM900_CCLK_Q_TEXT);
        send_command("AT+CCLK=15/01/01,00:00:00+00");
        break;

    case SIM900_ATE0:
        emit info(SIM900_ATE0_TEXT);
        send_command("AT+ATE0");
        break;

    case SIM900_ATV0:
        emit info(SIM900_ATV0_TEXT);
        send_command("AT+ATV0");
        break;

    case SIM900_CMEE:
        emit info(SIM900_CMEE_TEXT);
        send_command("AT+CMEE=2");
        break;

    case SIM900_CLIP:
        emit info(SIM900_CLIP_TEXT);
        send_command("AT+CLIP=1");
        break;

    case SIM900_ATA:
        emit info(SIM900_ATA_TEXT);
        send_command("AT+ATA");
        break;

    case SIM900_ATH0:
        emit info(SIM900_ATH0_TEXT);
        send_command("AT+ATH0");
        break;

    case SIM900_ATS0:
        emit info(SIM900_ATS0_TEXT);
        send_command("AT+ATS0=0");
        break;

    case SIM900_ATD:
        emit info(SIM900_ATD_TEXT);
        send_command("AT+ATD");
        break;

    case SIM900_ATD1:
        emit info(SIM900_ATD1_TEXT);
        send_command("AT+ATD1");
        break;

    case SIM900_ATDL:
        emit info(SIM900_ATDL_TEXT);
        send_command("AT+ATDL");
        break;

    case SIM900_GSMBUSY:
        emit info(SIM900_GSMBUSY_TEXT);
        send_command("AT+GSMBUSY=1");
        break;

    case SIM900_CSCA:
        emit info(SIM900_CSCA_TEXT);
        send_command("AT+CSCA?");
        break;

    case SIM900_CMGF:
        emit info(SIM900_CMGF_TEXT);
        send_command("AT+CMGF=1");
        break;

    case SIM900_CSCB:
        emit info(SIM900_CSCB_TEXT);
        send_command("AT+CSCB=1");
        break;

    case SIM900_CMGL:
        emit info(SIM900_CMGL_TEXT);
        send_command("AT+CMGL=0");
        break;

    case SIM900_CMGDA:
        emit info(SIM900_CMGDA_TEXT);
        send_command("AT+CMGDA=\"DEL ALL\"");
        break;

    case SIM900_CMGD:
        emit info(SIM900_CMGD_TEXT);
        send_command("AT+CMGD=0");
        break;

    case SIM900_CMGS:
        emit info(SIM900_CMGS_TEXT);
        send_command("AT+CMGS=\"XXX\"");
        break;

    case SIM900_CUSD:
        emit info(SIM900_CUSD_TEXT);
        send_command("AT+CUSD=1,\"#102#\"");
        break;

    default:
        emit error(QString("unknown cmd: %1").arg(cmd));
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
//AT+CUSD=1,"*105#"
