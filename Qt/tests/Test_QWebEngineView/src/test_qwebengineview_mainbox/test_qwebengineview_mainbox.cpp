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

    connect(ui->webEngineView->page(),  SIGNAL(loadFinished(bool)), this,               SLOT(test_JS(bool)));
    connect(ui->webEngineView->page(),  SIGNAL(loadProgress(int)),  ui->progressBar,    SLOT(setValue(int)));

    //ui->webEngineView->setUrl(QUrl("http://localhost/mso/"));
    ui->webEngineView->setUrl(QUrl("http://localhost/mso/home/next/12"));

    ui->webEngineView->show();
}
//--------------------------------------------------------------------------------
void MainBox::test_JS(bool)
{
#if 0
    ui->webEngineView->page()->runJavaScript("document.title", [=](const QVariant &v)
    {
        emit info(QString("result [%1]").arg(v.toString()));
    }
    );
#else
    ui->webEngineView->page()->runJavaScript("function myFunction() {"
                                             //"var elements = document.getElementsByTagName('div');"
                                             //"var input = elements[0];"
                                             //"return input.innerHTML;} myFunction();",
                                             "var table = document.getElementById('reklama_table');"
                                             "var Cells = table.getElementsByTagName('td');"
                                             "var Rows = table.getElementsByTagName('tr');"
                                             "var Cells2 = Rows[1].getElementsByTagName('td');"
                                             "return Cells2[4].innerText;} myFunction();",
                                             [=] (const QVariant &result)
    {
        //emit error("------------------------------------");
        emit error(QString("result [%1]").arg(result.toString()));
    }
    );
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *testbar = new QToolBar("testbar", this);
    testbar->setObjectName("testbar");

    testbar->addWidget(btnTest);

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
