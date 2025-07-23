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
#include "codeeditor.hpp"
#include "multiedit.hpp"
#include "ui_multiedit.h"
//--------------------------------------------------------------------------------
MultiEdit::MultiEdit(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MultiEdit)
{
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
    ui->setupUi(this);

    connect(ui->btn_add,    &QToolButton::clicked,  this,   &MultiEdit::add_page);
    connect(ui->btn_rem,    &QToolButton::clicked,  this,   &MultiEdit::rem_page);

    while(ui->tabWidget->count())
    {
        ui->tabWidget->removeTab(0);
    }
}
//--------------------------------------------------------------------------------
CodeEditor *MultiEdit::add_page(void)
{
    CodeEditor *te = new CodeEditor(this);

    te->setObjectName(QString("te_page_%1").arg(ui->tabWidget->count()));
    ui->tabWidget->addTab(te, QString("page%1").arg(ui->tabWidget->count()));
    // return reinterpret_cast<QTextEdit *>(te);
    return te;
}
//--------------------------------------------------------------------------------
void MultiEdit::rem_page(void)
{
    if(ui->tabWidget->count() <= 1)
    {
        return;
    }
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}
//--------------------------------------------------------------------------------
void MultiEdit::setPlainText(const QString &text)
{
    QTextEdit *edit = reinterpret_cast<QTextEdit *>(ui->tabWidget->currentWidget());
    Q_ASSERT(edit);
    edit->setPlainText(text);
}
//--------------------------------------------------------------------------------
QString MultiEdit::toPlainText(void) const
{
    QTextEdit *edit = reinterpret_cast<QTextEdit *>(ui->tabWidget->currentWidget());
    Q_ASSERT(edit);
    return edit->toPlainText();
}
//--------------------------------------------------------------------------------
void MultiEdit::setTabStopDistance(qreal distance)
{
    QTextEdit *edit = reinterpret_cast<QTextEdit *>(ui->tabWidget->currentWidget());
    if(edit != nullptr)
    {
        edit->setTabStopDistance(distance);
    }
}
//--------------------------------------------------------------------------------
QTextDocument *MultiEdit::document(void) const
{
    QTextEdit *edit = reinterpret_cast<QTextEdit *>(ui->tabWidget->currentWidget());
    if(edit)
    {
        return edit->document();
    }
    return nullptr;
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
    beginGroup("MultiEdit");
    int cnt_tabs = 0;
    load_int("cnt_tabs", &cnt_tabs);
    if(cnt_tabs < 1)
    {
        cnt_tabs = 1;
    }
    for(int n=0; n<cnt_tabs; n++)
    {
        add_page()->setPlainText(load_string(QString("page%1").arg(n)));
    }
    endGroup();
}
//--------------------------------------------------------------------------------
void MultiEdit::save_setting(void)
{
    beginGroup("MultiEdit");
    int cnt_tabs = ui->tabWidget->count();
    save_int("cnt_tabs", cnt_tabs);
    for(int n=0; n<cnt_tabs; n++)
    {
        QTextEdit *edit = reinterpret_cast<QTextEdit *>(ui->tabWidget->widget(n));
        if(edit)
        {
            save_string(QString("page%1").arg(n), edit->toPlainText());
        }
    }
    endGroup();
}
//--------------------------------------------------------------------------------
