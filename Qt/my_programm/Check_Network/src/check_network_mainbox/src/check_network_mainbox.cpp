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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "ui_check_network_mainbox.h"
//--------------------------------------------------------------------------------
#include "rtsp_widget.hpp"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "check_network_mainbox.hpp"
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
    save_widgets();
    if(m_pTcpSocket)
    {
        m_pTcpSocket->deleteLater();
    }
#ifdef USE_RTSP
    if(rtsp_widget)
    {
        rtsp_widget->deleteLater();
    }
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef USE_RTSP
    rtsp_widget = new RTSP_widget(this);
    ui->layout_rtsp->addWidget(rtsp_widget);

    connect(ui->tv_scan,    &QTreeView::doubleClicked,  this,   &MainBox::f_set_url);
#endif

#ifdef QT_DEBUG
    createTestBar();
#else
    ui->gb_test->setVisible(false);
#endif

    ui->sb_port->setRange(0, 0xFFFF);

    ui->sb_max_wait->setRange(100, 30000);

    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("scan log"));
    model->setHorizontalHeaderLabels(QStringList() << "IP" << "Port");

    // выделение строки целиком и запрет редактирования
    ui->tv_scan->setSelectionBehavior( QAbstractItemView::SelectRows );
    ui->tv_scan->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv_scan->setModel(model);
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

    //TODO layout = 0
    layout()->setMargin(0);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",             &MainBox::f_test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::choice_test);

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    // QList<MainBox::CMD>::iterator cmd_it = std::find_if(
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::scan(void)
{
    //554

    emit trace(Q_FUNC_INFO);
    Q_CHECK_PTR(m_pTcpSocket);

    block_this_button(true);

    emit info("Scanning ...");
    QHostAddress src = QHostAddress(QString("%1")
                                    .arg(ui->ip_widget_begin->get_url().host()));
    QHostAddress dst = QHostAddress(QString("%1")
                                    .arg(ui->ip_widget_end->get_url().host()));

    quint32 min_address = src.toIPv4Address();
    quint32 max_address = dst.toIPv4Address();

    if(min_address >= max_address)
    {
        emit error("Адреса не корректны");
        block_this_button(false);
        return;
    }

    int max_wait = ui->sb_max_wait->value();

    nPort = ui->sb_port->value();

    QProgressDialog *dlg = new QProgressDialog(this);
    dlg->setWindowTitle("Сканирование");
    dlg->setLabelText("Пожалуйста, ждите!");
    dlg->setMinimumSize(500, 100);
    dlg->setMinimum(static_cast<int>(min_address));
    dlg->setMaximum(static_cast<int>(max_address));
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
        dlg->setValue(static_cast<int>(address));

        if(dlg->wasCanceled())
        {
            break;
        }
        QApplication::processEvents();
        strHost = QHostAddress(address).toString();
        m_pTcpSocket->connectToHost(QHostAddress(address), static_cast<quint16>(nPort));
        bool ok = m_pTcpSocket->waitForConnected(max_wait);
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
    m_pTcpSocket->connectToHost(strHost, static_cast<quint16>(nPort));
    bool ok = m_pTcpSocket->waitForConnected(max_wait);
    if(ok)
    {
        QString address = QString("http://%1:%2")
                .arg(strHost)
                .arg(nPort);
        emit info(address);
        m_pTcpSocket->close();

        //ui->webEngineView->load(QUrl(address));
    }

    block_this_button(false);
}
//--------------------------------------------------------------------------------
void MainBox::f_disconnect(void)
{
    emit trace(Q_FUNC_INFO);
    Q_CHECK_PTR(m_pTcpSocket);

    auto index = ui->tv_scan->currentIndex();
    if(index.row() == -1)
    {
        emit error("Ничего не выбрано");
        return;
    }

    emit info("Disconnect");
    if(m_pTcpSocket->isOpen())
    {
        m_pTcpSocket->close();
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_set_url(const QModelIndex &index)
{
    auto url  = model->data(model->index(index.row(), 0, index.parent()), Qt::DisplayRole).toString();
    auto port = model->data(model->index(index.row(), 1, index.parent()), Qt::DisplayRole).toString();
    emit debug(QString("%1:%2")
               .arg(url)
               .arg(port));

#ifdef USE_RTSP
    rtsp_widget->set_address("admin",
                             "admin",
                             url,
                             port);
    rtsp_widget->play();
#endif
}
//--------------------------------------------------------------------------------
bool MainBox::f_test(void)
{
    for(int n=1; n<5; n++)
    {
        model->insertRow(0);
        model->setData(model->index(0, 0, QModelIndex()), QString("192.168.1.%1").arg(n));
        model->setData(model->index(0, 1, QModelIndex()), QString("%1").arg(n));
    }

    return true;
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
