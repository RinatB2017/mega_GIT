/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
    if(timer)
    {
        timer->stop();
        delete timer;
    }

    save_setting();
    delete ui;
}
//--------------------------------------------------------------------------------
void Text_widget::init(void)
{
    ui->setupUi(this);

    ui->cb_append->setProperty(NO_SAVE, true);
    ui->cb_append->clear();
    ui->cb_append->addItems(QStringList()
                            << "no add"
                            << "0x00"
                            << "0x0D"
                            << "0x0A"
                            << "0x0D 0x0A" );

    ui->sb_auto->setRange(9, 0xFFFF);

    ui->btn_add->setIcon(QIcon(":/plus_minus/plus.png"));
    ui->btn_rem->setIcon(QIcon(":/plus_minus/minus.png"));
    ui->btn_edt->setIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_up->setIcon(QIcon(":/arrows/up.png"));
    ui->btn_down->setIcon(QIcon(":/arrows/down.png"));

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    ui->btn_stop->setEnabled(false);

    connect(ui->btn_add,    &QToolButton::clicked,  this,   &Text_widget::append);
    connect(ui->btn_rem,    &QToolButton::clicked,  this,   &Text_widget::remove);
    connect(ui->btn_edt,    &QToolButton::clicked,  this,   &Text_widget::edit);
    connect(ui->btn_run,    &QToolButton::clicked,  this,   &Text_widget::run);
    connect(ui->btn_up,     &QToolButton::clicked,  this,   &Text_widget::up);
    connect(ui->btn_down,   &QToolButton::clicked,  this,   &Text_widget::down);

    connect(ui->btn_start,  &QToolButton::clicked,  this,   &Text_widget::f_start);
    connect(ui->btn_stop,   &QToolButton::clicked,  this,   &Text_widget::f_stop);

    connect(ui->listWidget, &QListWidget::doubleClicked,    this,   &Text_widget::edit);

    setAttribute(Qt::WA_DeleteOnClose);

    init_timer();
    load_setting();
}
//--------------------------------------------------------------------------------
void Text_widget::init_timer(void)
{
    timer = new QTimer();
    connect(timer,  &QTimer::timeout,   this,   &Text_widget::f_update);
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
    int res = messagebox_question("Удаление команды", "Вы уверены, что хотите удалить выбранную команду?");
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
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item == nullptr)
    {
        return;
    }

    QByteArray ba;
    ba.clear();
    ba.append(item->text().toLatin1());
    switch(ui->cb_append->currentIndex())
    {
    case 0: // "no add"
        break;

    case 1: //  add 0x00
        ba.append(static_cast<char>(0x00));
        break;

    case 2: //  add 0x0D"
        ba.append(static_cast<char>(0x0D));
        break;

    case 3: //  add "0x0A"
        ba.append(static_cast<char>(0x0A));
        break;

    case 4: //  add 0x0D 0x0A"
        ba.append(static_cast<char>(0x0D));
        ba.append(static_cast<char>(0x0A));
        break;
    }

    emit send_command(ba);
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
void Text_widget::f_start(void)
{
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);

    ui->sb_auto->setEnabled(false);
    ui->cb_append->setEnabled(false);
    ui->btn_add->setEnabled(false);
    ui->btn_down->setEnabled(false);
    ui->btn_edt->setEnabled(false);
    ui->btn_rem->setEnabled(false);
    ui->btn_run->setEnabled(false);
    ui->btn_up->setEnabled(false);

    if(timer)
    {
       timer->start(ui->sb_auto->value());
    }
}
//--------------------------------------------------------------------------------
void Text_widget::f_stop(void)
{
    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);

    ui->sb_auto->setEnabled(true);
    ui->cb_append->setEnabled(true);
    ui->btn_add->setEnabled(true);
    ui->btn_down->setEnabled(true);
    ui->btn_edt->setEnabled(true);
    ui->btn_rem->setEnabled(true);
    ui->btn_run->setEnabled(true);
    ui->btn_up->setEnabled(true);

    if(timer)
    {
       timer->stop();
    }
}
//--------------------------------------------------------------------------------
void Text_widget::f_update(void)
{
    run();
}
//--------------------------------------------------------------------------------
#include <QKeyEvent>
void Text_widget::keyPressEvent(QKeyEvent* ev)
{
    if (ev->key() == Qt::Key_Insert)
    {
        append();
    }
    if (ev->key() == Qt::Key_Delete)
    {
        remove();
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
    int interval = 0;
    bool ok = load_int(P_INTERVAL_TEXT, &interval);
    if(ok)
    {
        ui->sb_auto->setValue(interval);
    }
    int index = 0;
    ok = load_int(P_INDEX,  &index);
    if(ok)
    {
        ui->cb_append->setCurrentIndex(index);
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
    save_int(P_INTERVAL_TEXT, ui->sb_auto->value());
    save_stringlist(P_TEXT_WIDGET, sl);
    save_value(P_TEXT_WIDGET_GEOMETRY, saveGeometry());
    save_int(P_INDEX, ui->cb_append->currentIndex());;
}
//--------------------------------------------------------------------------------
