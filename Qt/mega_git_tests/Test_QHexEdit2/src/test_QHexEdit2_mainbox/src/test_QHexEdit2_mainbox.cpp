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
#include "ui_test_QHexEdit2_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QHexEdit2_mainbox.hpp"
#include "defines.hpp"

#include "qhexedit2.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    QFont font("Courier", 10);
    ui->hexedit_widget->setFont(font);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->addItem(TEST_0_TEXT, TEST_0);
    cb_test->addItem(TEST_1_TEXT, TEST_1);
    cb_test->addItem(TEST_2_TEXT, TEST_2);
    cb_test->addItem(TEST_3_TEXT, TEST_3);

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    switch (cmd)
    {
    case TEST_0: test_0(); break;
    case TEST_1: test_1(); break;
    case TEST_2: test_2(); break;
    case TEST_3: test_3(); break;
    default: break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    QByteArray ba;
    ba.clear();
    for(int n=0; n<=0xFF; n++) ba.append(static_cast<char>(n));

    ui->hexedit_widget->setData(ba);
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    QByteArray data;

    QFile file("Test_QHexEdit2");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit error("file not open");
        return;
    }
    data = file.readAll();
    file.close();

    ui->hexedit_widget->setData(data);
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2: info");
    emit debug("Test_2: debug");
    emit error("Test_2: error");
    emit trace("Test_2: trace");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3: info");
    emit debug("Test_3: debug");
    emit error("Test_3: error");
    emit trace("Test_3: trace");
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
void MainBox::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    qDebug() << "MainBox::closeEvent";
}
//--------------------------------------------------------------------------------
