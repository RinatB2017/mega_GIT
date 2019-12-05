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
#include <QHostInfo>
//--------------------------------------------------------------------------------
#include "ui_simple_http_reader_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "simple_http_reader_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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
    save_widgets(APPNAME);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    //createTestBar();
    create_tcp_socket();

    ui->host_widget->set_url(QUrl("127.0.0.1"));

    ui->sb_port->setRange(0, 0xFFFF);
    ui->sb_port->setValue(80);

    connect(ui->btn_run,        SIGNAL(clicked(bool)),  this,   SLOT(f_run()));
    connect(ui->btn_update,     SIGNAL(clicked(bool)),  this,   SLOT(f_update()));
    connect(ui->btn_host_to_ip, SIGNAL(clicked(bool)),  this,   SLOT(f_host_to_ip()));

    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

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

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::create_tcp_socket(void)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,  SIGNAL(readyRead()),    this,   SLOT(read_data()));
    connect(tcpSocket,  SIGNAL(error(QAbstractSocket::SocketError)), this, (SLOT(s_error(QAbstractSocket::SocketError))));
    connect(tcpSocket,  SIGNAL(connected()),    this,   SLOT(f_connected()));
    connect(tcpSocket,  SIGNAL(disconnected()), this,   SLOT(f_disconnected()));
}
//--------------------------------------------------------------------------------
void MainBox::read_data(void)
{
    QByteArray ba = tcpSocket->readAll();

    emit debug("read_data");
    emit debug("---------------------------------------------------------------");
    emit info(ba);
    emit debug("---------------------------------------------------------------");
}
//--------------------------------------------------------------------------------
void MainBox::s_error(QAbstractSocket::SocketError err)
{
    switch (err)
    {
    case QAbstractSocket::ConnectionRefusedError:           emit error("ConnectionRefusedError");           break;
    case QAbstractSocket::RemoteHostClosedError:            emit error("RemoteHostClosedError");            break;
    case QAbstractSocket::HostNotFoundError:                emit error("HostNotFoundError");                break;
    case QAbstractSocket::SocketAccessError:                emit error("SocketAccessError");                break;
    case QAbstractSocket::SocketResourceError:              emit error("SocketResourceError");              break;
    case QAbstractSocket::SocketTimeoutError:               emit error("SocketTimeoutError");               break;
    case QAbstractSocket::DatagramTooLargeError:            emit error("DatagramTooLargeError");            break;
    case QAbstractSocket::NetworkError:                     emit error("NetworkError");                     break;
    case QAbstractSocket::AddressInUseError:                emit error("AddressInUseError");                break;
    case QAbstractSocket::SocketAddressNotAvailableError:   emit error("SocketAddressNotAvailableError");   break;
    case QAbstractSocket::UnsupportedSocketOperationError:  emit error("UnsupportedSocketOperationError");  break;
    case QAbstractSocket::UnfinishedSocketOperationError:   emit error("UnfinishedSocketOperationError");   break;
    case QAbstractSocket::ProxyAuthenticationRequiredError: emit error("ProxyAuthenticationRequiredError"); break;
    case QAbstractSocket::SslHandshakeFailedError:          emit error("SslHandshakeFailedError");          break;
    case QAbstractSocket::ProxyConnectionRefusedError:      emit error("ProxyConnectionRefusedError");      break;
    case QAbstractSocket::ProxyConnectionClosedError:       emit error("ProxyConnectionClosedError");       break;
    case QAbstractSocket::ProxyConnectionTimeoutError:      emit error("ProxyConnectionTimeoutError");      break;
    case QAbstractSocket::ProxyNotFoundError:               emit error("ProxyNotFoundError");               break;
    case QAbstractSocket::ProxyProtocolError:               emit error("ProxyProtocolError");               break;
    case QAbstractSocket::OperationError:                   emit error("OperationError");                   break;
    case QAbstractSocket::SslInternalError:                 emit error("SslInternalError");                 break;
    case QAbstractSocket::SslInvalidUserDataError:          emit error("SslInvalidUserDataError");          break;
    case QAbstractSocket::TemporaryError:                   emit error("TemporaryError");                   break;

    case QAbstractSocket::UnknownSocketError:               emit error("UnknownSocketError");               break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_connected(void)
{
    emit debug("Connected");
}
//--------------------------------------------------------------------------------
void MainBox::f_disconnected(void)
{
    emit debug("Disconnected");
    f_disconnect();
}
//--------------------------------------------------------------------------------
bool MainBox::f_connect(void)
{
    QString ip = get_url().host();
    if(ip.isEmpty())
    {
        emit error("ip is empty!");
        return false;
    }
    int port = get_url().port();

    emit debug(QString("%1:%2")
               .arg(ip)
               .arg(port));

    tcpSocket->connectToHost(ip, static_cast<quint16>(port));
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::f_disconnect(void)
{
    if(tcpSocket->isOpen())
    {
        tcpSocket->close();
    }
    tcpSocket->disconnectFromHost();
}
//--------------------------------------------------------------------------------
QUrl MainBox::get_url(void)
{
    QUrl url = ui->host_widget->get_url();
    url.setPort(ui->sb_port->value());

    return url;
}
//--------------------------------------------------------------------------------
void MainBox::f_run(void)
{
    // https://unixforum.org/index.php?act=rssout&id=1
    //---
    bool ok = false;
    ok = tcpSocket->isOpen();
    if(ok == false)
    {
        emit debug("isOpen() return false");
        ok = f_connect();
        if(ok == false)
        {
            return;
        }
    }
    //---

    QByteArray reqStr;
    QString str=ui->le_question->toPlainText();
    QStringList strList=str.split('\n');
    foreach (QString line, strList)
    {
        reqStr.append(QString("%1\r\n").arg(line));
    }
    reqStr.append("\r\n");

    qint64 bytes = tcpSocket->write(reqStr);
    if(bytes < 0)
    {
        emit error(QString("write bytes %1").arg(bytes));
        f_disconnect();
        return;
    }
    //---
}
//--------------------------------------------------------------------------------
void MainBox::f_update(void)
{
    ui->le_question->clear();
    ui->le_question->append("GET / HTTP/1.1");
    ui->le_question->append(QString("Host: %1").arg(get_url().host()));
    ui->le_question->append("User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5");
    ui->le_question->append("Accept: text/html");
    ui->le_question->append("Connection: close");
}
//--------------------------------------------------------------------------------
void MainBox::f_host_to_ip(void)
{
    QString h_name = ui->le_host->text();
    if(h_name.isEmpty())
    {
        emit error("host is empty!");
        return;
    }

    QUrl url;
    url.setUrl(h_name);

    QHostInfo h_info =  QHostInfo::fromName(url.host());
    QList<QHostAddress> list = h_info.addresses();
    if(list.isEmpty() == false)
    {
        ui->host_widget->set_url(QUrl(list.first().toString()));
    }
    else
    {
        emit error("list is empty!");
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
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
