/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "passwordbox.hpp"
#include "ui_passwordbox.h"
//--------------------------------------------------------------------------------
PasswordBox::PasswordBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::PasswordBox)
{
    init();
}
//--------------------------------------------------------------------------------
PasswordBox::~PasswordBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void PasswordBox::init(void)
{
    ui->setupUi(this);

    setFixedSize(sizeHint());

    l_buttons.append(ui->btn_0);
    l_buttons.append(ui->btn_1);
    l_buttons.append(ui->btn_2);
    l_buttons.append(ui->btn_3);
    l_buttons.append(ui->btn_4);
    l_buttons.append(ui->btn_5);
    l_buttons.append(ui->btn_6);
    l_buttons.append(ui->btn_7);
    l_buttons.append(ui->btn_8);
    l_buttons.append(ui->btn_9);

    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    int id = 0;
    foreach (QToolButton *btn, l_buttons)
    {
        btn->setProperty(P_ID, QString("%1").arg(id++));
        connect(btn,    &QToolButton::clicked,  this,   &PasswordBox::click);
    }
    connect(ui->btn_bs,     &QToolButton::clicked,  this,   &PasswordBox::bs);
    connect(ui->btn_run,    &QToolButton::clicked,  this,   &PasswordBox::run);

    installEventFilter(this);
}
//--------------------------------------------------------------------------------
void PasswordBox::click(void)
{
    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if(btn)
    {
        QString val = btn->property(P_ID).toString();
        if(val.isEmpty() == false)
        {
            append_char(val);
        }
    }
}
//--------------------------------------------------------------------------------
void PasswordBox::append_char(const QString &key)
{
    if(pass.length()>4)
    {
        return;
    }
    if(key.length() != 1)
    {
        return;
    }
    pass.append(key);
    ui->display->display(pass);
}
//--------------------------------------------------------------------------------
void PasswordBox::bs(void)
{
    if(pass.isEmpty() == false)
    {
        pass = pass.remove(pass.length() - 1, 1);
        ui->display->display(pass);
    }
}
//--------------------------------------------------------------------------------
void PasswordBox::run(void)
{
    if(pass == true_pass)
    {
        emit debug("Pass is true");
        emit pass_is_true();
    }
}
//--------------------------------------------------------------------------------
bool PasswordBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch(keyEvent->key())
        {
        case Qt::Key_0: append_char("0");   break;
        case Qt::Key_1: append_char("1");   break;
        case Qt::Key_2: append_char("2");   break;
        case Qt::Key_3: append_char("3");   break;
        case Qt::Key_4: append_char("4");   break;
        case Qt::Key_5: append_char("5");   break;
        case Qt::Key_6: append_char("6");   break;
        case Qt::Key_7: append_char("7");   break;
        case Qt::Key_8: append_char("8");   break;
        case Qt::Key_9: append_char("9");   break;

        case Qt::Key_Backspace:
            bs();
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            run();
            break;

        default:
            break;
        }
        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
//--------------------------------------------------------------------------------
void PasswordBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool PasswordBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void PasswordBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void PasswordBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
