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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QNetworkRequest>
#include <QNetworkReply>
//--------------------------------------------------------------------------------
#include "ui_get_myip_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "get_myip_mainbox.hpp"
#include "defines.hpp"
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
    connect(ui->btn_get_my_ip,  SIGNAL(clicked(bool)),  this,   SLOT(show_my_IP()));

    ui->lcd_0->display("---");
    ui->lcd_1->display("---");
    ui->lcd_2->display("---");
    ui->lcd_3->display("---");
}
//--------------------------------------------------------------------------------
void MainBox::show_my_IP(void)
{
    block_this_button(true);

    request.setUrl(QUrl("https://2ip.ru/"));
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
            if(ba.contains("Ваш IP адрес"))
            {
                QString begin_str = "<big id=\"d_clip_button\">";
                int begin = ba.lastIndexOf(begin_str);
                int end = ba.lastIndexOf("</big>");
                //emit debug(QString("begin %1").arg(begin));
                //emit debug(QString("end %1").arg(end));
                if((end - begin) > 6)
                {
                    QString str = ba.mid(begin+begin_str.length(), end-begin-begin_str.length());
                    emit debug(QString("%1").arg(str));
                    quint32 ip = QHostAddress(str).toIPv4Address();
                    ui->lcd_0->display(static_cast<int>((ip >> 24) & 0xFF ));
                    ui->lcd_1->display(static_cast<int>((ip >> 16) & 0xFF ));
                    ui->lcd_2->display(static_cast<int>((ip >> 8) & 0xFF ));
                    ui->lcd_3->display(static_cast<int>(ip & 0xFF ));

                    block_this_button(false);
                    return;
                }
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
