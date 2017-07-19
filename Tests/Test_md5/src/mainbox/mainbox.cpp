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
#include <QCryptographicHash>
#include <QFileDialog>
#include <QToolButton>
#include <QToolBar>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QTime>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    ui->setupUi(this);

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
    createTestBar();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *toolBar = new QToolBar(this);

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        mw->addToolBar(Qt::TopToolBarArea, toolBar);
    }

    toolBar->addWidget(btnTest);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QTime time;
    int max_n = 10000000;

    QCryptographicHash *md5 = new QCryptographicHash(QCryptographicHash::Md5);

    time.start();
    for(int n=0; n<max_n; n++)
    {
        md5->reset();
        md5->addData("abcde");
        QByteArray ba = md5->result();
        //emit debug(ba.toHex());
    }
    emit info(QString(tr("посчитали %1 за %2 мсек, т.е. %3 в мсек"))
              .arg(max_n)
              .arg(time.elapsed())
              .arg((float)max_n / (float)time.elapsed()));

#if 0
    QFileDialog *dlg;
    QString temp;

    dlg = new QFileDialog;
    dlg->setNameFilter(tr("QSS files (*.*)"));
    // dlg->setDefaultSuffix(tr("qss"));
    // dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        temp = get_md5(files.at(0));
        emit debug(QString("%1=%2")
                   .arg(files.at(0))
                   .arg(temp));
    }
#endif
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
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
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
