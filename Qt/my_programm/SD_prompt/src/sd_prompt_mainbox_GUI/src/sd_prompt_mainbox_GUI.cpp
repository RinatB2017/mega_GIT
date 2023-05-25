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
#include "sd_prompt_mainbox_GUI.hpp"
//--------------------------------------------------------------------------------
MainBox_GUI::MainBox_GUI(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox_GUI)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox_GUI::~MainBox_GUI()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox_GUI::init(void)
{
    ui->setupUi(this);
    connect(ui->btn_create_positive_prompt, &QPushButton::clicked,
            this,                           &MainBox_GUI::s_create_positive_prompt);
    connect(ui->btn_create_negative_prompt, &QPushButton::clicked,
            this,                           &MainBox_GUI::s_create_negative_prompt);
    load_widgets();
}
//--------------------------------------------------------------------------------
QString MainBox_GUI::get_positive_prompt(void)
{
    QString prompt;

    QString p0 = ui->le_0->text();
    QString p1 = ui->le_1->text();
    QString p2 = ui->le_2->text();
    QString p3 = ui->le_3->text();
    QString p4 = ui->le_4->text();
    QString p5 = ui->le_5->text();
    QString p6 = ui->le_6->text();
    QString p7 = ui->le_7->text();
    QString p8 = ui->le_8->text();

    if(p0.isEmpty() == false)
    {
        prompt.append(p0);
        prompt.append(",");
    }
    if(p1.isEmpty() == false)
    {
        prompt.append(p1);
        prompt.append(",");
    }
    if(p2.isEmpty() == false)
    {
        prompt.append(p2);
        prompt.append(",");
    }
    if(p3.isEmpty() == false)
    {
        prompt.append(p3);
        prompt.append(",");
    }
    if(p4.isEmpty() == false)
    {
        prompt.append(p4);
        prompt.append(",");
    }
    if(p5.isEmpty() == false)
    {
        prompt.append(p5);
        prompt.append(",");
    }
    if(p6.isEmpty() == false)
    {
        prompt.append(p6);
        prompt.append(",");
    }
    if(p7.isEmpty() == false)
    {
        prompt.append(p7);
        prompt.append(",");
    }
    if(p8.isEmpty() == false)
    {
        prompt.append(p8);
        prompt.append(",");
    }

    return prompt;
}
//--------------------------------------------------------------------------------
QString MainBox_GUI::get_negative_prompt(void)
{
    QString prompt;

    prompt.append(ui->te_negative_prompt->toPlainText());
    return prompt;
}
//--------------------------------------------------------------------------------
void MainBox_GUI::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
