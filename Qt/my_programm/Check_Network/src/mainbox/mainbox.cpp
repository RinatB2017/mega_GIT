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
#   include <QProgressDialog>
#   include <QApplication>
#   include <QPushButton>
#   include <QToolButton>
#   include <QCheckBox>
#   include <QToolBar>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
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
    if(m_pTcpSocket)
    {
        m_pTcpSocket->deleteLater();
    }
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    ui->sb_src_a->setRange(0, 0xFF);
    ui->sb_src_b->setRange(0, 0xFF);
    ui->sb_src_c->setRange(0, 0xFF);
    ui->sb_src_d->setRange(0, 0xFF);

    ui->sb_dst_a->setRange(0, 0xFF);
    ui->sb_dst_b->setRange(0, 0xFF);
    ui->sb_dst_c->setRange(0, 0xFF);
    ui->sb_dst_d->setRange(0, 0xFF);

    ui->sb_port->setRange(0, 0xFFFF);

    ui->sb_max_wait->setRange(100, 30000);

    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("scan log"));
    model->setHorizontalHeaderLabels(QStringList() << "IP" << "Port");
    ui->tv_scan->setModel(model);

    // выделение строки целиком и запрет редактирования
    ui->tv_scan->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->tv_scan->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //---
    ui->tv_scan->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    //ui->tv_scan->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    m_pTcpSocket = new QTcpSocket(this);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    connect(ui->btn_scan,       SIGNAL(clicked(bool)),  this,   SLOT(scan()));
    connect(ui->btn_connect,    SIGNAL(clicked(bool)),  this,   SLOT(f_connect()));
    connect(ui->btn_disconnect, SIGNAL(clicked(bool)),  this,   SLOT(f_disconnect()));

#if 1
    connect(ui->widget_camera,  SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(ui->widget_camera,  SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(ui->widget_camera,  SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(ui->widget_camera,  SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));
#endif

    //setFixedSize(sizeHint());

    //ui->webEngineView->load(QUrl("https://www.google.ru/"));
    //ui->webEngineView->load(QUrl("https://www.youtube.com/"));

    load_config();
}
//--------------------------------------------------------------------------------
void MainBox::scan(void)
{
    //554

    emit trace(Q_FUNC_INFO);
    Q_CHECK_PTR(m_pTcpSocket);

    block_this_button(true);

    emit info("Scanning ...");
    QHostAddress src = QHostAddress(QString("%1.%2.%3.%4")
                                    .arg(ui->sb_src_a->value())
                                    .arg(ui->sb_src_b->value())
                                    .arg(ui->sb_src_c->value())
                                    .arg(ui->sb_src_d->value()));
    QHostAddress dst = QHostAddress(QString("%1.%2.%3.%4")
                                    .arg(ui->sb_dst_a->value())
                                    .arg(ui->sb_dst_b->value())
                                    .arg(ui->sb_dst_c->value())
                                    .arg(ui->sb_dst_d->value()));

    quint32 min_address = src.toIPv4Address();
    quint32 max_address = dst.toIPv4Address();

    if(min_address >= max_address)
    {
        emit error("Адреса не корректны");
        block_this_button(false);
        return;
    }

    bool ok = false;
    int max_wait = ui->sb_max_wait->value();

    nPort = ui->sb_port->value();

    QProgressDialog *dlg = new QProgressDialog(this);
    dlg->setWindowTitle("Сканирование");
    dlg->setLabelText("Пожалуйста, ждите!");
    dlg->setMinimumSize(500, 100);
    dlg->setMinimum(min_address);
    dlg->setMaximum(max_address);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();

    //---
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "IP" << "Port");
    ui->tv_scan->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    //ui->tv_scan->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //---

    for(quint32 address = min_address; address<max_address; address++)
    {
        dlg->setLabelText(QString("Пожалуйста, ждите! (осталось %1 хостов)").arg(max_address - address));
        dlg->setValue(address);

        if(dlg->wasCanceled())
        {
            break;
        }
        QApplication::processEvents();
        strHost = QHostAddress(address).toString();
        m_pTcpSocket->connectToHost(QHostAddress(address), nPort);
        ok = m_pTcpSocket->waitForConnected(max_wait);
        if(ok)
        {
            emit info(QString("%1:%2 OK")
                      .arg(strHost)
                      .arg(nPort));

            model->insertRow(0);
            model->setData(model->index(0, 0, QModelIndex()), strHost);
            model->setData(model->index(0, 1, QModelIndex()), nPort);

            m_pTcpSocket->close();
        }
    }

    dlg->close();
    dlg->deleteLater();

    block_this_button(false);
    emit info("The end!");
}
//--------------------------------------------------------------------------------
void MainBox::f_connect(void)
{
    emit trace(Q_FUNC_INFO);
    Q_CHECK_PTR(m_pTcpSocket);

    auto index = ui->tv_scan->currentIndex();
    if(index.row() == -1)
    {
        emit error("Ничего не выбрано");
        return;
    }
    emit info(QString("%1:%2")
              .arg(model->item(index.row(), 0)->text())
              .arg(model->item(index.row(), 1)->text()));

    block_this_button(true);
    int max_wait = ui->sb_max_wait->value();

    emit info("Connect");
    strHost = model->item(index.row(), 0)->text();
    nPort = model->item(index.row(), 1)->text().toInt();
    m_pTcpSocket->connectToHost(strHost, nPort);
    bool ok = m_pTcpSocket->waitForConnected(max_wait);
    if(ok)
    {
        QString address = QString("http://%1:%2")
                .arg(strHost)
                .arg(nPort);
        emit info(address);
        m_pTcpSocket->close();

        ui->webEngineView->load(QUrl(address));
    }

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void MainBox::f_disconnect(void)
{
    emit trace(Q_FUNC_INFO);
    Q_CHECK_PTR(m_pTcpSocket);

    emit info("Disconnect");
    if(m_pTcpSocket->isOpen())
    {
        m_pTcpSocket->close();
    }
}
//--------------------------------------------------------------------------------
void MainBox::slotReadyRead (void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::slotError (QAbstractSocket::SocketError)
{
    emit trace(Q_FUNC_INFO);

    emit error(QString("%1:%2 %3")
               .arg(strHost)
               .arg(nPort)
               .arg(m_pTcpSocket->errorString()));
}
//--------------------------------------------------------------------------------
void MainBox::slotSendToServer(void)
{
    emit trace(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void MainBox::slotConnected (void)
{
    emit trace(Q_FUNC_INFO);
    //emit info("Connected!");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
