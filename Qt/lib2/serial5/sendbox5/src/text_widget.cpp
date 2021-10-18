/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "text_widget.hpp"
#include "ui_text_widget.h"
//--------------------------------------------------------------------------------
Text_widget::Text_widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Text_widget)
{
    init();
}
//--------------------------------------------------------------------------------
Text_widget::~Text_widget()
{
    save_setting();
    delete ui;
}
//--------------------------------------------------------------------------------
void Text_widget::init(void)
{
    ui->setupUi(this);

    ui->btn_add->setIcon(QIcon(":/plus_minus/plus.png"));
    ui->btn_rem->setIcon(QIcon(":/plus_minus/minus.png"));
    ui->btn_edt->setIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_up->setIcon(QIcon(":/arrows/up.png"));
    ui->btn_down->setIcon(QIcon(":/arrows/down.png"));

    connect(ui->btn_add,    &QToolButton::clicked,  this,   &Text_widget::append);
    connect(ui->btn_rem,    &QToolButton::clicked,  this,   &Text_widget::remove);
    connect(ui->btn_edt,    &QToolButton::clicked,  this,   &Text_widget::edit);
    connect(ui->btn_run,    &QToolButton::clicked,  this,   &Text_widget::run);
    connect(ui->btn_up,     &QToolButton::clicked,  this,   &Text_widget::up);
    connect(ui->btn_down,   &QToolButton::clicked,  this,   &Text_widget::down);

    connect(ui->listWidget, &QListWidget::doubleClicked,    this,   &Text_widget::edit);

#ifndef QT_DEBUG
    //FIXME надо позже сделать нормально
    ui->btn_up->setVisible(false);
    ui->btn_down->setVisible(false);
#endif

    setAttribute(Qt::WA_DeleteOnClose);

    load_setting();
}
//--------------------------------------------------------------------------------
void Text_widget::append(void)
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "Добавление",
                                         "Команда:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if (ok && !text.isEmpty())
    {
        ui->listWidget->addItem(text);
    }
}
//--------------------------------------------------------------------------------
void Text_widget::remove(void)
{
    int res = messagebox_question("Удаление команды", "Вы уверены, что хотите удалить вбранную команду?");
    if(res == QMessageBox::Yes)
    {
        ui->listWidget->takeItem(ui->listWidget->currentIndex().row());
    }
}
//--------------------------------------------------------------------------------
void Text_widget::edit(void)
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "Добавление",
                                         "Команда:",
                                         QLineEdit::Normal,
                                         ui->listWidget->currentItem()->text(),
                                         &ok);
    if (ok && !text.isEmpty())
    {
        ui->listWidget->currentItem()->setText(text);
    }
}
//--------------------------------------------------------------------------------
void Text_widget::run(void)
{
    emit send_command(ui->listWidget->currentItem()->text().toLatin1());
}
//--------------------------------------------------------------------------------
void Text_widget::up(void)
{
    int curr_row = ui->listWidget->currentRow();
    if(curr_row > 0)
    {
        QListWidgetItem *item = ui->listWidget->takeItem(curr_row);
        ui->listWidget->insertItem(curr_row - 1, item);
        ui->listWidget->setCurrentRow(curr_row - 1);
    }
}
//--------------------------------------------------------------------------------
void Text_widget::down(void)
{
    int curr_row = ui->listWidget->currentRow();
    if(curr_row < ui->listWidget->count() - 1)
    {
        QListWidgetItem *item = ui->listWidget->takeItem(curr_row);
        ui->listWidget->insertItem(curr_row + 1, item);
        ui->listWidget->setCurrentRow(curr_row + 1);
    }
}
//--------------------------------------------------------------------------------
void Text_widget::load_setting(void)
{
    QStringList sl = load_stringlist(P_TEXT_WIDGET);
    foreach (QString text, sl)
    {
        ui->listWidget->addItem(text);
    }
    restoreGeometry(load_value(P_TEXT_WIDGET_GEOMETRY).toByteArray());
}
//--------------------------------------------------------------------------------
void Text_widget::save_setting(void)
{
    QStringList sl;
    for(int i=0; i<ui->listWidget->count(); i++)
    {
        sl.append(ui->listWidget->item(i)->text());
    }
    save_stringlist(P_TEXT_WIDGET, sl);
    save_value(P_TEXT_WIDGET_GEOMETRY, saveGeometry());
}
//--------------------------------------------------------------------------------
