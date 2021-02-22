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
#include "controls_widget.hpp"
#include "ui_controls_widget.h"
//--------------------------------------------------------------------------------
Controls_widget::Controls_widget(QWidget *parent) :
    Default_widget(parent),
    ui(new Ui::Controls_widget)
{
    init();
}
//--------------------------------------------------------------------------------
Controls_widget::~Controls_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Controls_widget::init(void)
{
    ui->setupUi(this);

    ui->horizontalSlider->setRange(0, 1000);

    ui->toolButton->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon)));
    connect(ui->toolButton, &QToolButton::clicked,  this,   &Controls_widget::delete_string);

    //---
    connect(ui->btn_click,  &QPushButton::clicked,  [this]() {
        emit info("click");
    });
    connect(ui->btn_set_dt,  &QPushButton::clicked,  [this]() {
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    });
    connect(ui->btn_clear_strings,  &QPushButton::clicked,  [this]() {
        ui->comboBox_2->clear();
    });
    connect(ui->btn_add_strings,  &QPushButton::clicked,  [this]() {
        ui->comboBox_2->addItem("a0");
        ui->comboBox_2->addItem("a1");
        ui->comboBox_2->addItem("a2");

        ui->listWidget->addItem("b0");
        ui->listWidget->addItem("b1");
        ui->listWidget->addItem("b2");
    });

    connect(ui->btn_set_current_time,  &QPushButton::clicked,  [this]() {
        ui->te_current_time->setTime(QTime::currentTime());
    });
    connect(ui->btn_add_time,  &QPushButton::clicked,  [this]() {
        QTime time = ui->te_current_time->time();
        ui->te_current_time->setTime(time.addSecs(60));
    });


    //---
    connect(ui->de_begin,   &QDateEdit::editingFinished,  [this]() {
        emit info("correct min");
        ui->de_test->setMinimumDate(ui->de_begin->date());
    });
    connect(ui->de_end,   &QDateEdit::editingFinished,  [this]() {
        emit info("correct max");
        ui->de_test->setMaximumDate(ui->de_end->date());
    });

    QTimer::singleShot(100, [this]{
        if(ui->de_test->minimumDate() != ui->de_begin->date())
        {
            ui->de_test->setMinimumDate(ui->de_begin->date());
        }
        if(ui->de_test->maximumDate() != ui->de_end->date())
        {
            ui->de_test->setMaximumDate(ui->de_end->date());
        }
        emit info("CORRECT");
    });
    //---

    ui->hex_widget->setMaximum(std::numeric_limits<qlonglong>::max());
    ui->hex_widget->setMinimum(std::numeric_limits<qlonglong>::min());
}
//--------------------------------------------------------------------------------
void Controls_widget::delete_string(void)
{
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}
//--------------------------------------------------------------------------------
bool Controls_widget::programm_is_exit(void)
{
    bool is_exit = ui->cb_fag_exit->isChecked();
    if(is_exit == false)
    {
        messagebox_info("Info", "Низзя");
    }
    else
    {
//        save_widgets();
    }
    return is_exit;
}
//--------------------------------------------------------------------------------
