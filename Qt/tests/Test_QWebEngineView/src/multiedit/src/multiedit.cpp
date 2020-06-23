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
#include "multiedit.hpp"
#include "ui_multiedit.h"
//--------------------------------------------------------------------------------
MultiEdit::MultiEdit(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MultiEdit)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
MultiEdit::~MultiEdit()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MultiEdit::init(void)
{
    connect(ui->btn_add,    &QToolButton::clicked,  this,   &MultiEdit::add_page);
    connect(ui->btn_rem,    &QToolButton::clicked,  this,   &MultiEdit::rem_page);

    while(ui->tabWidget->count())
    {
        ui->tabWidget->removeTab(0);
    }

    add_page();
    add_page();
    add_page();
}
//--------------------------------------------------------------------------------
void MultiEdit::add_page(void)
{
    QTextEdit *te = new QTextEdit(this);
    te->setObjectName(QString("te_page_%1").arg(ui->tabWidget->count()));
    ui->tabWidget->addTab(te, QString("page%1").arg(ui->tabWidget->count()));
}
//--------------------------------------------------------------------------------
void MultiEdit::rem_page(void)
{
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}
//--------------------------------------------------------------------------------
QString MultiEdit::getCurrentText(void)
{
    return reinterpret_cast<QTextEdit *>(ui->tabWidget->currentWidget())->toPlainText();
}
//--------------------------------------------------------------------------------
void MultiEdit::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MultiEdit::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MultiEdit::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MultiEdit::save_setting(void)
{

}
//--------------------------------------------------------------------------------
