/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QWebEngineProfile>
#include <QWebEngineView>
//--------------------------------------------------------------------------------
#include "ui_test_qwebengineview_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_qwebengineview_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
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

    createTestBar();

    QWebEngineProfile *profile = new QWebEngineProfile();
    // изменяем необходимые http-заголовки на свои значения
    //profile->setHttpUserAgent("Mozilla/5.0 (X11; U; Linux x86_64; ru; rv:1.9.0.10) Gecko/2009042809 GranParadiso/3.0.10");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
    //profile->setHttpUserAgent("Opera/9.80 (Windows NT 6.1; U; en) Presto/2.9.168 Version/11.50");
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");
    profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
    //profile->setHttpUserAgent("iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");
    QWebEnginePage *page = new QWebEnginePage(profile);

    ui->webEngineView->setPage(page);

    connect(ui->webEngineView->page(),  SIGNAL(loadProgress(int)),
            ui->progressBar,            SLOT(setValue(int)));
    connect(ui->webEngineView->page(),  SIGNAL(loadFinished(bool)),
            this,                       SLOT(test_JS(bool)));
    connect(ui->btn_run,                SIGNAL(clicked(bool)),
            this,                       SLOT(run()));

    ui->le_address->setText("https://2ip.ru/");
    //ui->le_address->setText("http://localhost/mso/");
    //ui->le_address->setText("http://localhost/mso/home/next/12");

    ui->te_js->append("function myFunction() {");
    ui->te_js->append("var table = document.getElementById('reklama_table');");
    ui->te_js->append("var Cells = table.getElementsByTagName('td');");
    ui->te_js->append("var Rows = table.getElementsByTagName('tr');");
    ui->te_js->append("var Cells2 = Rows[1].getElementsByTagName('td');");
    ui->te_js->append("return Cells2[4].innerText;} myFunction();");

    //---
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setChildrenCollapsible(true);

    ui->te_js->setParent(splitter);
    ui->frame->setParent(splitter);
    splitter->addWidget(ui->te_js);
    splitter->addWidget(ui->frame);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    layout()->addWidget(splitter);
    //---

    ui->progressBar->setValue(0);
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    QString address = ui->le_address->text();
    if(address.isEmpty())
    {
        emit error("address is empty!");
        return;
    }

    ui->webEngineView->setUrl(QUrl(address));
}
//--------------------------------------------------------------------------------
void MainBox::test_JS(bool)
{
    //emit info("test_JS");

    const QString javascript = ui->te_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }
    //emit trace(javascript);

    ui->webEngineView->page()->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
    });
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
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    ui->te_js->clear();
    ui->te_js->append("document.title");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    ui->te_js->clear();
    ui->te_js->append("function myFunction() {");
    ui->te_js->append("var table = document.getElementById('reklama_table');");
    ui->te_js->append("var Cells = table.getElementsByTagName('td');");
    ui->te_js->append("var Rows = table.getElementsByTagName('tr');");
    ui->te_js->append("var Cells2 = Rows[1].getElementsByTagName('td');");
    ui->te_js->append("return Cells2[4].innerText;} myFunction();");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    ui->te_js->clear();
    ui->te_js->append("function myFunction() {");
    ui->te_js->append("var elements = document.getElementsByTagName('div');");
    ui->te_js->append("var input = elements[0];");
    ui->te_js->append("return input.innerHTML;} myFunction();");

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
