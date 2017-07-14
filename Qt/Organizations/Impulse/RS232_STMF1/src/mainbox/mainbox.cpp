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
#include <QProgressDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStyle>
#include <QTime>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "controlbox.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "sleeper.h"

#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    controlBox(0)
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

    controlBox = new ControlBox(this);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(controlBox);

    setLayout(vbox);

    createTestBar();
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                       "clear delta",
                                       "clear delta");

    //connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_test, SIGNAL(clicked()), controlBox, SLOT(reset()));
}
//--------------------------------------------------------------------------------
void MainBox::draw_progress(void)
{
    QProgressDialog *progress = new QProgressDialog(this);
    progress->setLabelText(tr("калибровка"));
    progress->setAutoClose(true);
    progress->setMaximum(99);
    for(int n=0; n<100; n++)
    {
        QCoreApplication::processEvents();
        progress->setLabelText(QString(tr("шаг %1")).arg(n));
        progress->setValue(n);
        if(progress->wasCanceled())
        {
            progress->close();
            break;
        }
        Sleeper::msleep(100);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    // emit info("start test");
    QMessageBox::information(this, tr("info"), tr("test"));
    // emit info("end test");
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
