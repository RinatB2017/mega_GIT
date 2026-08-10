/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "secretbox.hpp"
#include "ui_secretbox.h"
//--------------------------------------------------------------------------------
SecretBox::SecretBox(const QString &secret,
                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecretBox),
    secret_word(secret)
{
    init();
}
//--------------------------------------------------------------------------------
void SecretBox::init(void)
{
    ui->setupUi(this);

    connect(ui->btn_0,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_1,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_2,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_3,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_4,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_5,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_6,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_7,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_8,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));
    connect(ui->btn_9,  SIGNAL(clicked(bool)),  this,   SLOT(click_number()));

    connect(ui->btn_bs, SIGNAL(clicked(bool)),  this,   SLOT(click_button()));
    connect(ui->btn_clr,SIGNAL(clicked(bool)),  this,   SLOT(click_button()));
    connect(ui->btn_OK, SIGNAL(clicked(bool)),  this,   SLOT(click_button()));

    ui->btn_0->setShortcut(Qt::Key_0);
    ui->btn_1->setShortcut(Qt::Key_1);
    ui->btn_2->setShortcut(Qt::Key_2);
    ui->btn_3->setShortcut(Qt::Key_3);
    ui->btn_4->setShortcut(Qt::Key_4);
    ui->btn_5->setShortcut(Qt::Key_5);
    ui->btn_6->setShortcut(Qt::Key_6);
    ui->btn_7->setShortcut(Qt::Key_7);
    ui->btn_8->setShortcut(Qt::Key_8);
    ui->btn_9->setShortcut(Qt::Key_9);

    ui->btn_0->setProperty("number", 0);
    ui->btn_1->setProperty("number", 1);
    ui->btn_2->setProperty("number", 2);
    ui->btn_3->setProperty("number", 3);
    ui->btn_4->setProperty("number", 4);
    ui->btn_5->setProperty("number", 5);
    ui->btn_6->setProperty("number", 6);
    ui->btn_7->setProperty("number", 7);
    ui->btn_8->setProperty("number", 8);
    ui->btn_9->setProperty("number", 9);

    ui->btn_bs->setShortcut(Qt::Key_Backspace);
    ui->btn_clr->setShortcut(Qt::Key_Clear);
    ui->btn_OK->setShortcut(Qt::Key_Enter);

    ui->btn_bs->setProperty("name",  "btn_bs");
    ui->btn_clr->setProperty("name", "btn_clr");
    ui->btn_OK->setProperty("name",  "btn_OK");

#if 0
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowMinMaxButtonsHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
#endif
   setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void SecretBox::click_number(void)
{
    QToolButton *btn = static_cast<QToolButton *>(sender());
    if(btn)
    {
        bool ok = false;
        int num = btn->property("number").toInt(&ok);
        if(ok)
        {
            emit debug(QString("num %1").arg(num));
            if(input_word.length() < 3)
            {
                input_word.append(QString::number(num));
            }
            ui->display->display(input_word);
        }
    }
}
//--------------------------------------------------------------------------------
void SecretBox::click_button(void)
{
    QToolButton *btn = static_cast<QToolButton *>(sender());
    if(btn)
    {
        QString name = btn->property("name").toString();
        if(name == "btn_bs")
        {
            emit debug("BS");
            input_word = input_word.left(input_word.length() - 1);
        }
        if(name == "btn_clr")
        {
            emit debug("CLEAR");
            input_word = "";
        }
        if(name == "btn_OK")
        {
            emit debug("OK");
            if(secret_word == input_word)
            {
                accept();
            }
            else
            {
                reject();
            }
        }
        show_word();
    }
}
//--------------------------------------------------------------------------------
void SecretBox::show_word(void)
{
    if(input_word.length() > 3)
    {
        input_word = input_word.left(3);
    }
    ui->display->display(input_word);
}
//--------------------------------------------------------------------------------
SecretBox::~SecretBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
