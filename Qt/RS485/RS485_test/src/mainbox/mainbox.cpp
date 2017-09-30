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
#include "defines.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox5(0)
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

    serialBox5 = new SerialBox5(this, "RS485", "RS485");
    serialBox5->add_menu(2);

    ui->serial_layout->addWidget(serialBox5);

    connect(this,       SIGNAL(send(QByteArray)),   serialBox5, SLOT(input(QByteArray)));
    connect(serialBox5, SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
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
void MainBox::read_data(QByteArray ba)
{
    emit info(ba.toHex().data());

    if(ba.isEmpty())
    {
        //emit error("ba is empty!");
        return;
    }
    if(ba.size() != 32)
    {
        emit error(QString("size packet is %1 != 32").arg(ba.size()));
        return;
    }

    NEW_PACKET *question = (NEW_PACKET *)ba.data();
    switch (question->body.header.cmd_8)
    {
    case CMD_34:    cmd_34();   break;
    case CMD_36:    cmd_36(ba); break;
    case CMD_37:    cmd_37(ba); break;

    default:
        emit error(QString("uknown cmd 0x%1").arg(question->body.header.cmd_8, 0, 16));
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::cmd_34(void)
{
    emit info("cmd_34");

    NEW_PACKET question;

    for(unsigned int n=0; n<sizeof(question); n++)
    {
        question.buf[n] = memory.buf[n];
    }
    question.body.header.cmd_8 = CMD_34;

    QByteArray output;
    output.clear();
    output.append((char *)&question.buf, sizeof(question));

    emit send(output);
}
//--------------------------------------------------------------------------------
void MainBox::cmd_36(QByteArray ba)
{
    emit info("cmd_36");

    NEW_PACKET question;

    for(unsigned int n=0; n<sizeof(question); n++)
    {
        memory.buf[n] = ba.at(n);
        question.buf[n] = ba.at(n);
    }
    question.body.header.cmd_8 = CMD_36;

    QByteArray output;
    output.clear();
    output.append((char *)&question.buf, sizeof(question));

    emit send(output);
}
//--------------------------------------------------------------------------------
void MainBox::cmd_37(QByteArray ba)
{
    emit info("cmd_37");

    NEW_PACKET question;

    for(unsigned int n=0; n<sizeof(question); n++)
    {
        memory.buf[n] = ba.at(n);
        question.buf[n] = ba.at(n);
    }
    question.body.header.cmd_8 = CMD_37;

    QByteArray output;
    output.clear();
    output.append((char *)&question.buf, sizeof(question));

    emit send(output);
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
