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
#include "autoclicker.hpp"
#include "myfiledialog.hpp"
#include "ui_autoclicker.h"
//--------------------------------------------------------------------------------
AutoClicker::AutoClicker(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::AutoClicker)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
AutoClicker::~AutoClicker()
{
    if(timer_autoclick)
    {
        timer_autoclick->stop();
        disconnect(timer_autoclick,    &QTimer::timeout,   this,   &AutoClicker::update);

        timer_autoclick->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void AutoClicker::init(void)
{
    timer_autoclick = new QTimer(this);
    connect(timer_autoclick,    &QTimer::timeout,   this,   &AutoClicker::update);

    model = new QStringListModel(this);
    ui->lv_command->setModel(model);

    ui->btn_append->setIcon(qApp->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->btn_clear->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->btn_load->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_save->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->btn_append, &QToolButton::clicked,  this,   &AutoClicker::append);
    connect(ui->btn_clear,  &QToolButton::clicked,  this,   &AutoClicker::clear);
    connect(ui->btn_load,   &QToolButton::clicked,  this,   &AutoClicker::load);
    connect(ui->btn_save,   &QToolButton::clicked,  this,   &AutoClicker::save);

    QStringList sl;
    sl << "tap|5|5";
    sl << "pause|2";
    sl << "tap|100|100";
    sl << "pause|2";

    model->setStringList(sl);

    correct_commands.clear();
    correct_commands.append({ "tap",   2});
    correct_commands.append({ "pause", 1});
    correct_commands.append({ "swipe", 5});

    connect(ui->btn_start,  &QPushButton::clicked,  this,   &AutoClicker::timer_start);
    connect(ui->btn_stop,   &QPushButton::clicked,  this,   &AutoClicker::timer_stop);
    connect(ui->btn_test,   &QPushButton::clicked,  this,   &AutoClicker::test);
}
//--------------------------------------------------------------------------------
void AutoClicker::timer_start(void)
{
    timer_autoclick->start(1000);
    ui->lv_command->setDisabled(true);
}
//--------------------------------------------------------------------------------
void AutoClicker::timer_stop(void)
{
    timer_autoclick->stop();
    current_state = STATE_IDLE;
    ui->lv_command->setEnabled(true);
}
//--------------------------------------------------------------------------------
void AutoClicker::test(void)
{
    emit info("TEST");
    emit debug(QString("rowCount = %1").arg(ui->lv_command->model()->rowCount()));
    bool ok = test_commands();
    if(ok)
    {
        emit info("OK");
    }
    else
    {
        emit error("FAIL");
    }
}
//--------------------------------------------------------------------------------
void AutoClicker::update(void)
{
    QModelIndex cur = ui->lv_command->model()->index(current_command, 0);
    ui->lv_command->setCurrentIndex(cur);

    run_command(cur.data(Qt::DisplayRole).toString());

    if(current_command < model->rowCount() - 1)
    {
        current_command++;
    }
    else
    {
        current_command=0;
    }
}
//--------------------------------------------------------------------------------
void AutoClicker::load(void)
{
    MyFileDialog *dlg = new MyFileDialog("autoclicker_commands", "autoclicker_commands", this);
    dlg->setNameFilter("TXT files (*.txt)");
    dlg->selectFile("noname");
    dlg->setDefaultSuffix("txt");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        load_from_file(filename);
    }
}
//--------------------------------------------------------------------------------
void AutoClicker::save(void)
{
    MyFileDialog *dlg = new MyFileDialog("autoclicker_commands", "autoclicker_commands", this);
    dlg->setNameFilter("TXT files (*.txt)");
    dlg->selectFile("noname");
    dlg->setDefaultSuffix("txt");
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        save_to_file(filename);
    }
}
//--------------------------------------------------------------------------------
bool AutoClicker::load_from_file(const QString &filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        QStringList sl;
        QString line;
        do {
            line = stream.readLine();
            if(line.isEmpty() == false)
            {
                sl.append(line);
            }
        } while(!line.isNull());
        file.close();

        model->setStringList(sl);
        ui->lv_command->setModel(model);

        emit debug(QString("rowCount %1").arg(ui->lv_command->model()->rowCount()));
    }
    else
    {
        emit error(QString("error read %1").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool AutoClicker::save_to_file(const QString &filename)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        for(int n=0; n<ui->lv_command->model()->rowCount(); n++)
        {
            QModelIndex cur = ui->lv_command->model()->index(n, 0);
            stream << cur.data(Qt::DisplayRole).toString().append("\n");
        }
        file.close();
    }
    else
    {
        emit error(QString("error write %1").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void AutoClicker::append(void)
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "Append command",
                                         "Command:",
                                         QLineEdit::Normal,
                                         "cmd|param1|param2",
                                         &ok);
    if (ok && !text.isEmpty())
    {
        ok = test_command(text);
        if(ok)
        {
            emit info(text);
        }
        else
        {
            emit error("FAIL");
        }
    }
}
//--------------------------------------------------------------------------------
void AutoClicker::clear(void)
{
    int btn = messagebox_question("Question", "Clear all commands?");
    if(btn == QMessageBox::Yes)
    {
        model->setStringList( QStringList() );
    }
}
//--------------------------------------------------------------------------------
bool AutoClicker::test_command(const QString &command_str)
{
    if(command_str.isEmpty())
    {
        emit error("data is empty");
        return false;
    }

    QStringList sl = command_str.split("|");
    QString temp = sl.at(0);
    foreach (COMMAND cmd, correct_commands)
    {
        if((cmd.cmd == temp) && (cmd.cnt_param == (sl.count() - 1) ))
        {
            return true;
        }
    }

    return false;
}
//--------------------------------------------------------------------------------
bool AutoClicker::test_commands(void)
{
    if(ui->lv_command->model()->rowCount() <= 0)
    {
        emit error(QString("rowCount = %1").arg(ui->lv_command->model()->rowCount()));
        return false;
    }
    for(int n=0; n<ui->lv_command->model()->rowCount(); n++)
    {
        QModelIndex cur = ui->lv_command->model()->index(n, 0);
        bool res = test_command(cur.data(Qt::DisplayRole).toString());
        if(res == false)
        {
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
void AutoClicker::run_command(const QString &data)
{
    if(data.isEmpty())
    {
        emit error("data is empty");
        return;
    }

    emit info(data);

    QStringList sl = data.split(" ");
    if(sl.count() <= 1)
    {
        emit error("bad command");
        return;
    }
    QString cmd = sl.at(0);
    emit error(cmd);
}
//--------------------------------------------------------------------------------
void AutoClicker::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool AutoClicker::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void AutoClicker::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void AutoClicker::save_setting(void)
{

}
//--------------------------------------------------------------------------------
