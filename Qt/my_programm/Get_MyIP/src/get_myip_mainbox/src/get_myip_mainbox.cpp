/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "ui_get_myip_mainbox.h"
//--------------------------------------------------------------------------------
#include "get_myip_mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    connect(ui->btn_get_my_ip,  &QPushButton::clicked,  this,   &MainBox::show_my_IP);

    ui->lcd_0->display("---");
    ui->lcd_1->display("---");
    ui->lcd_2->display("---");
    ui->lcd_3->display("---");

    show_my_IP();
}
//--------------------------------------------------------------------------------
void MainBox::show_my_IP(void)
{
    block_this_button(true);

    request.setUrl(QUrl("https://api.ipify.org/"));
    QNetworkReply *reply = networkManager.get(QNetworkRequest(request));
    while (!reply->isFinished())
    {
        QCoreApplication::processEvents();
    }
    QByteArray ba;
    do
    {
        ba = reply->readLine();
        if(ba.isEmpty() == false)
        {
            emit debug(ba);
            QString temp = ba.data();
            QStringList sl = temp.split('.');
            if(sl.count() == 4)
            {
                ui->lcd_0->display(sl.at(0));
                ui->lcd_1->display(sl.at(1));
                ui->lcd_2->display(sl.at(2));
                ui->lcd_3->display(sl.at(3));
            }
        }
    } while(ba.isEmpty() == false);

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
