/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include "custompage.h"
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
    save_widgets("Test_QWebEngineView");

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
    //profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; U; en) Presto/2.9.168 Version/11.50");
    profile->setHttpUserAgent("iPad: Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25");

    new_page = new CustomPage(profile);
    //new_page = new CustomPage();
    connect(new_page,   SIGNAL(err_output(QString)),  this,   SIGNAL(error(QString)));

    ui->webEngineView->setPage(new_page);

    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    connect(new_page,           SIGNAL(loadProgress(int)),
            ui->progressBar,    SLOT(setValue(int)));
    connect(new_page,           SIGNAL(loadFinished(bool)),
            this,               SLOT(test_JS(bool)));
    connect(ui->btn_run,        SIGNAL(clicked(bool)),  this,   SLOT(s_run()));
    connect(ui->btn_default,    SIGNAL(clicked(bool)),  this,   SLOT(s_default()));
    connect(ui->btn_run_js,     SIGNAL(clicked(bool)),
            this,               SLOT(test_JS(bool)));
    connect(this,               SIGNAL(send(QString)),
            this,               SLOT(analize(QString)));

    //ui->le_address->setText("https://2ip.ru/");
    //ui->le_address->setText("https://www.youtube.com/");
    //ui->le_address->setText("http://localhost/mso/");
    //ui->le_address->setText("http://localhost/mso/home/next/12");
    ui->le_address->setText("https://www.avito.ru/krasnodar");
    //ui->le_address->setText("file:///C:/Users/User/Dropbox/HTML/test.html");
    //ui->le_address->setText("file:///home/boss/HDD/Dropbox/HTML/test.html");

    //---
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setObjectName("splitter");
    splitter->setChildrenCollapsible(false);

    ui->groupBox_js->setParent(splitter);
    ui->frame_browser->setParent(splitter);
    splitter->addWidget(ui->frame_browser);
    splitter->addWidget(ui->groupBox_js);
    layout()->addWidget(splitter);
    //---

    ui->progressBar->setValue(0);

    load_widgets("Test_QWebEngineView");
}
//--------------------------------------------------------------------------------
void MainBox::load_js_default(void)
{
    QString temp;

#if 1
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("   var links = document.getElementsByTagName('a');\n");
    temp.append("   var temp = 'false';\n");
    temp.append("   for (var i = 0; i < links.length; i++)\n");
    temp.append("   {\n");

    temp.append("      if(links[i].innerHTML == 'Транспорт')\n");
    temp.append("      {\n");
    temp.append("          temp = 'true';\n");
    temp.append("          links[i].click();\n");
    temp.append("      }\n");

    temp.append("   }\n");
    temp.append("   return temp;\n");
    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

#if 0
    temp.append("function myFunction()\n");
    temp.append("{\n");
    temp.append("   document.getElementById('directions-select').value = 'Прикубанский';\n");
    //temp.append("   document.getElementById('rad2').checked = true;;\n");
    //temp.append("   document.getElementById('text1').value = 'car1';\n");
    temp.append("}\n");
    temp.append("myFunction();\n");
#endif

    ui->te_js->setPlainText(temp);
}
//--------------------------------------------------------------------------------
void MainBox::s_run(void)
{
    emit trace(Q_FUNC_INFO);

    QString address = ui->le_address->text();
    if(address.isEmpty())
    {
        emit error("address is empty!");
        return;
    }

    ui->webEngineView->setUrl(QUrl(address));
}
//--------------------------------------------------------------------------------
void MainBox::s_default(void)
{
    emit trace(Q_FUNC_INFO);

    load_js_default();
}
//--------------------------------------------------------------------------------
void MainBox::test_JS(bool)
{
    emit trace(Q_FUNC_INFO);

    const QString javascript = ui->te_js->toPlainText();
    if(javascript.isEmpty())
    {
        emit error("JS is empty!");
        return;
    }
    //emit trace(javascript);

    new_page->runJavaScript(javascript, [=](const QVariant &v)
    {
        emit info(v.toString());
        emit send(v.toString());
    });
}
//--------------------------------------------------------------------------------
void MainBox::analize(const QString data)
{
    QStringList sl = data.split(';');
    if(sl.length() > 0)
    {
        emit info(QString("len = %1").arg(sl.length()));
        foreach (QString text, sl)
        {
            emit info(QString("text [%1]").arg(text));
        }
    }
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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());    //TODO странно
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
