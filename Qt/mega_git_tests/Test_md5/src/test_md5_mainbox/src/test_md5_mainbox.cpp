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
#include "ui_test_md5_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_md5_mainbox.hpp"
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
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar", this);
    testbar->setObjectName("testbar");

    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    testbar->addWidget(btnTest);

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QElapsedTimer time;
    int max_n = 1000;

    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);

    time.start();
    for(int n=0; n<max_n; n++)
    {
        md5->reset();
        md5->addData("abcde");
        QByteArray ba = md5->result();
        emit debug(ba.toHex());
    }
    emit info(QString(tr("посчитали %1 за %2 мсек, т.е. %3 в мсек"))
              .arg(max_n)
              .arg(time.elapsed())
              .arg(static_cast<float>(max_n) / static_cast<float>(time.elapsed())));
}
//--------------------------------------------------------------------------------
QString MainBox::get_md5(const QString &filename)
{
    QString md5_string = "";
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);
        md5->addData(file.readAll());
        emit debug(QString("%1 %2")
                   .arg(file.fileName())
                   .arg(md5->result().toHex().data()));
        md5_string = QString("%1").arg(md5->result().toHex().data());
        delete md5;
    }
    return md5_string;
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
