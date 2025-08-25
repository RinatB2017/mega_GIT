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
#include "bin_widget.hpp"
#include "ui_bin_widget.h"
//--------------------------------------------------------------------------------
Bin_widget::Bin_widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bin_widget)
{
    init();
}
//--------------------------------------------------------------------------------
Bin_widget::~Bin_widget()
{
    save_setting();
    delete ui;
}
//--------------------------------------------------------------------------------
void Bin_widget::init(void)
{
    ui->setupUi(this);

    ui->sb_auto->setRange(9, 0xFFFF);

    ui->btn_add->setIcon(QIcon(":/plus_minus/plus.png"));
    ui->btn_rem->setIcon(QIcon(":/plus_minus/minus.png"));
    ui->btn_edt->setIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_up->setIcon(QIcon(":/arrows/up.png"));
    ui->btn_down->setIcon(QIcon(":/arrows/down.png"));

    ui->sb_auto->setRange(9, 0xFFFF);

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    ui->btn_stop->setEnabled(false);

    connect(ui->btn_add,    &QToolButton::clicked,  this,   &Bin_widget::append);
    connect(ui->btn_rem,    &QToolButton::clicked,  this,   &Bin_widget::remove);
    connect(ui->btn_edt,    &QToolButton::clicked,  this,   &Bin_widget::edit);
    connect(ui->btn_run,    &QToolButton::clicked,  this,   &Bin_widget::run);
    connect(ui->btn_up,     &QToolButton::clicked,  this,   &Bin_widget::up);
    connect(ui->btn_down,   &QToolButton::clicked,  this,   &Bin_widget::down);

    connect(ui->btn_start,  &QToolButton::clicked,  this,   &Bin_widget::f_start);
    connect(ui->btn_stop,   &QToolButton::clicked,  this,   &Bin_widget::f_stop);

    connect(ui->listWidget, &QListWidget::doubleClicked,    this,   &Bin_widget::edit);

    setAttribute(Qt::WA_DeleteOnClose);

    init_timer();
    load_setting();
}
//--------------------------------------------------------------------------------
void Bin_widget::init_timer(void)
{
    timer = new QTimer();
    connect(timer,  &QTimer::timeout,   this,   &Bin_widget::f_update);
}
//--------------------------------------------------------------------------------
void Bin_widget::append(void)
{
    bool ok = false;
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
void Bin_widget::remove(void)
{
    int res = messagebox_question("Удаление команды", "Вы уверены, что хотите удалить вбранную команду?");
    if(res == QMessageBox::Yes)
    {
        ui->listWidget->takeItem(ui->listWidget->currentIndex().row());
    }
}
//--------------------------------------------------------------------------------
void Bin_widget::edit(void)
{
    bool ok = false;
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
void Bin_widget::run(void)
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(!item)
    {
        return;
    }

    QByteArray input_data = item->text().toLatin1();

    QByteArray output_data;
    output_data.clear();
    output_data.append(QByteArray::fromHex(input_data));

    if(input_data.toUpper() != output_data.toHex().toUpper())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("invalid HEX-data"));
        msgBox.exec();
        return;
    }

    emit send_command(output_data);
}
//--------------------------------------------------------------------------------
void Bin_widget::up(void)
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
void Bin_widget::down(void)
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
void Bin_widget::f_start(void)
{
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);

    ui->sb_auto->setEnabled(false);
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
void Bin_widget::f_stop(void)
{
    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);

    ui->sb_auto->setEnabled(true);
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
void Bin_widget::f_update(void)
{
    run();
}
//--------------------------------------------------------------------------------
void Bin_widget::load_setting(void)
{
    QStringList sl = load_stringlist(P_BIN_WIDGET);
    foreach (QString text, sl)
    {
        ui->listWidget->addItem(text);
    }
    int interval = 0;
    bool ok = load_int(P_INTERVAL_BIN, &interval);
    if(ok)
    {
        ui->sb_auto->setValue(interval);
    }

    restoreGeometry(load_value(P_BIN_WIDGET_GEOMETRY).toByteArray());
}
//--------------------------------------------------------------------------------
void Bin_widget::save_setting(void)
{
    QStringList sl;
    for(int i=0; i<ui->listWidget->count(); i++)
    {
        sl.append(ui->listWidget->item(i)->text());
    }
    save_int(P_INTERVAL_BIN, ui->sb_auto->value());
    save_stringlist(P_BIN_WIDGET, sl);
    save_value(P_BIN_WIDGET_GEOMETRY, saveGeometry());
}
//--------------------------------------------------------------------------------
