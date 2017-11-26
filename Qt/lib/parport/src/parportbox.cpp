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
#include <QDebug>
#include <QTimer>
#include <QFrame>
//--------------------------------------------------------------------------------
#include "parportbox.hpp"
#include "parport.hpp"
#include "ledpanel.hpp"
#include "ui_parportbox.h"
//--------------------------------------------------------------------------------
ParportBox::ParportBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ParportBox),
    parent(parent)
{
    ui->setupUi(this);

    connect_log();

    initKeyboard();
    initWidgets();
    initTimers();

    parport = new Parport(this);
    connects();
    set_enable(false);

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
ParportBox::~ParportBox()
{
    parport->close_parport();
    delete ui;
}
//--------------------------------------------------------------------------------
void ParportBox::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void ParportBox::log(const QString &data)
{
    qDebug() << data;
}
//================================================================================
void ParportBox::initTimers()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawData()));
}
//================================================================================
void ParportBox::initWidgets()
{
    vbox = new QVBoxLayout;

#if LEDPANEL_ON
    ledPanel8 = new LedPanel(8, 32, 32);
    ledControlPanel8 = new LedPanel(8, 32, 32);

    hbox = new QHBoxLayout;
    hbox->addWidget(ledPanel8);
    hbox->addStretch();
    hbox->addWidget(ledControlPanel8);
    vbox->addLayout(hbox);
    vbox->addStretch();
#endif

    ledFrame = new QFrame;
    ledFrame->setLayout(vbox);

    ui->playout->addWidget(ledFrame);
}
//================================================================================
void ParportBox::initKeyboard()
{
    keymap.clear();
    keymap.insert(ui->btnData0, BTN_0);
    keymap.insert(ui->btnData1, BTN_1);
    keymap.insert(ui->btnData2, BTN_2);
    keymap.insert(ui->btnData3, BTN_3);
    keymap.insert(ui->btnData4, BTN_4);
    keymap.insert(ui->btnData5, BTN_5);
    keymap.insert(ui->btnData6, BTN_6);
    keymap.insert(ui->btnData7, BTN_7);

    keymap.insert(ui->btnAutoFt,   BTN_CONTROL_AUTOFT);
    keymap.insert(ui->btnInit,     BTN_CONTROL_INIT);
    keymap.insert(ui->btnSelectIn, BTN_CONTROL_SELECT_IN);
    keymap.insert(ui->btnStrobe,   BTN_CONTROL_STROBE);

    keymap.insert(ui->btnInvertData,    BTN_INVERT_DATA);
    keymap.insert(ui->btnInvertControl, BTN_INVERT_CONTROL);

    keymap.insert(ui->btnStatusError,    BTN_STATUS_ERROR);
    keymap.insert(ui->btnStatusSelect,   BTN_STATUS_SELECT);
    keymap.insert(ui->btnStatusPaperOut, BTN_STATUS_PAPER_OUT);
    keymap.insert(ui->btnStatusACK,      BTN_STATUS_ACK);
    keymap.insert(ui->btnStatusBusy,     BTN_STATUS_BUSY);
}
//================================================================================
void ParportBox::message(const QString &text)
{
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setText(text);
    msgBox->exec();
}
//--------------------------------------------------------------------------------
void ParportBox::drawData()
{
    if(!parport->is_open())
    {
        emit error("Error: port not open!");
        return;
    }

#if LEDPANEL_ON
    ledPanel8->drawValue(parport->getData());
    ledControlPanel8->drawValue(parport->get_control());
#endif

    ui->btnData0->setStyleSheet(parport->getData0() ? LED_ON : LED_OFF);
    ui->btnData1->setStyleSheet(parport->getData1() ? LED_ON : LED_OFF);
    ui->btnData2->setStyleSheet(parport->getData2() ? LED_ON : LED_OFF);
    ui->btnData3->setStyleSheet(parport->getData3() ? LED_ON : LED_OFF);
    ui->btnData4->setStyleSheet(parport->getData4() ? LED_ON : LED_OFF);
    ui->btnData5->setStyleSheet(parport->getData5() ? LED_ON : LED_OFF);
    ui->btnData6->setStyleSheet(parport->getData6() ? LED_ON : LED_OFF);
    ui->btnData7->setStyleSheet(parport->getData7() ? LED_ON : LED_OFF);

    ui->btnStatusError->setStyleSheet(parport->getStatusError() ? LED_ON : LED_OFF);
    ui->btnStatusSelect->setStyleSheet(parport->getStatusSelect() ? LED_ON : LED_OFF);
    ui->btnStatusPaperOut->setStyleSheet(parport->getStatusPaperOut() ? LED_ON : LED_OFF);
    ui->btnStatusACK->setStyleSheet(parport->getStatusACK() ? LED_ON : LED_OFF);
    ui->btnStatusBusy->setStyleSheet(parport->getStatusBusy() ? LED_ON : LED_OFF);

    ui->btnInit->setStyleSheet(parport->getInit() ? LED_ON : LED_OFF);

    ui->btnSelectIn->setStyleSheet(parport->getSelectIn() ? LED_OFF : LED_ON);
    ui->btnAutoFt->setStyleSheet(parport->getAutoFt() ? LED_OFF : LED_ON);
    ui->btnStrobe->setStyleSheet(parport->getStrobe() ? LED_OFF : LED_ON);
}
//--------------------------------------------------------------------------------
void ParportBox::set_enable(bool state)
{
    ui->btnData0->setEnabled(state);
    ui->btnData1->setEnabled(state);
    ui->btnData2->setEnabled(state);
    ui->btnData3->setEnabled(state);
    ui->btnData4->setEnabled(state);
    ui->btnData5->setEnabled(state);
    ui->btnData6->setEnabled(state);
    ui->btnData7->setEnabled(state);

    ui->btnAutoFt->setEnabled(state);
    ui->btnInit->setEnabled(state);
    ui->btnSelectIn->setEnabled(state);
    ui->btnStrobe->setEnabled(state);

    ui->btnInvertData->setEnabled(state);
    ui->btnInvertControl->setEnabled(state);

#if LEDPANEL_ON
    ledPanel8->setEnabled(state);
    ledControlPanel8->setEnabled(state);
#endif

    ui->btnData0->setEnabled(state);
    ui->btnData1->setEnabled(state);
    ui->btnData2->setEnabled(state);
    ui->btnData3->setEnabled(state);
    ui->btnData4->setEnabled(state);
    ui->btnData5->setEnabled(state);
    ui->btnData6->setEnabled(state);
    ui->btnData7->setEnabled(state);

    ui->btnInit->setEnabled(state);
    ui->btnSelectIn->setEnabled(state);
    ui->btnAutoFt->setEnabled(state);
    ui->btnStrobe->setEnabled(state);

    ui->btnStatusError->setEnabled(state);
    ui->btnStatusSelect->setEnabled(state);
    ui->btnStatusPaperOut->setEnabled(state);
    ui->btnStatusACK->setEnabled(state);
    ui->btnStatusBusy->setEnabled(state);
}
//================================================================================
int ParportBox::open_parport()
{
    return parport->open_parport();
}
//--------------------------------------------------------------------------------
void ParportBox::set_timers()
{
    if(!parport->is_open())
    {
        message("Error: port not open!");
        return;
    }
    timer->start(100);
}
//--------------------------------------------------------------------------------
void ParportBox::connects()
{
    connect(ui->btnData0, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData1, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData2, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData3, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData4, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData5, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData6, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnData7, SIGNAL(clicked()), this, SLOT(pressed()));

    connect(ui->btnAutoFt, SIGNAL(clicked()),   this, SLOT(pressed()));
    connect(ui->btnInit, SIGNAL(clicked()),     this, SLOT(pressed()));
    connect(ui->btnSelectIn, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnStrobe, SIGNAL(clicked()),   this, SLOT(pressed()));

    connect(ui->btnInvertData, SIGNAL(clicked()),    this, SLOT(pressed()));
    connect(ui->btnInvertControl, SIGNAL(clicked()), this, SLOT(pressed()));

    connect(ui->btnStatusError, SIGNAL(clicked()),    this, SLOT(pressed()));
    connect(ui->btnStatusSelect, SIGNAL(clicked()),   this, SLOT(pressed()));
    connect(ui->btnStatusPaperOut, SIGNAL(clicked()), this, SLOT(pressed()));
    connect(ui->btnStatusACK, SIGNAL(clicked()),      this, SLOT(pressed()));
    connect(ui->btnStatusBusy, SIGNAL(clicked()),     this, SLOT(pressed()));
}
//--------------------------------------------------------------------------------
void ParportBox::pressed()
{
    if(!parport->is_open())
    {
        message("Error: port not open!");
        return;
    }

    QPushButton *button = (QPushButton *) sender();
    if(!keymap.contains(button))
    {
            emit error("keymap no contains");
            return;
    }

    switch(keymap[button])
    {
    case BTN_0:
        parport->setData0(!parport->getData0());
        break;
    case BTN_1:
        parport->setData1(!parport->getData1());
        break;
    case BTN_2:
        parport->setData2(!parport->getData2());
        break;
    case BTN_3:
        parport->setData3(!parport->getData3());
        break;
    case BTN_4:
        parport->setData4(!parport->getData4());
        break;
    case BTN_5:
        parport->setData5(!parport->getData5());
        break;
    case BTN_6:
        parport->setData6(!parport->getData6());
        break;
    case BTN_7:
        parport->setData7(!parport->getData7());
        break;
    case BTN_CONTROL_AUTOFT:
        parport->setAutoFt(parport->getAutoFt());
        break;
    case BTN_CONTROL_INIT:
        parport->setInit(!parport->getInit());
        break;
    case BTN_CONTROL_SELECT_IN:
        parport->setSelectIn(parport->getSelectIn());
        break;
    case BTN_CONTROL_STROBE:
        parport->setStrobe(parport->getStrobe());
        break;
    case BTN_INVERT_DATA:
        parport->setData(~parport->getData());
        break;
    case BTN_INVERT_CONTROL:
        parport->set_control(0xF & (~parport->get_control()));
        break;
    case BTN_STATUS_ERROR:
        parport->setStatusError(!parport->getStatusError());
        break;
    case BTN_STATUS_SELECT:
        parport->setStatusSelect(!parport->getStatusSelect());
        break;
    case BTN_STATUS_PAPER_OUT:
        parport->setStatusPaperOut(!parport->getStatusPaperOut());
        break;
    case BTN_STATUS_ACK:
        parport->setStatusACK(!parport->getStatusACK());
        break;
    case BTN_STATUS_BUSY:
        parport->setStatusBusy(!parport->getStatusBusy());
        break;
    default:
        message("unknown button");
        break;
    }
}
//--------------------------------------------------------------------------------
void ParportBox::on_powerButton_toggled(bool checked)
{
    if(checked)
    {
        open_parport();
        if(parport->is_open())
        {
            set_timers();
            set_enable(true);
        }
        else
        {
            ui->powerButton->toggle();
            message("Error: port not open!");
        }
    }
    else
    {
        parport->close_parport();
        timer->stop();
        set_enable(false);
    }
}
//--------------------------------------------------------------------------------
void ParportBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
