/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "widgetsframe.hpp"
//--------------------------------------------------------------------------------
WidgetsFrame::WidgetsFrame(QWidget *parent) :
    QFrame(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void WidgetsFrame::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void WidgetsFrame::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void WidgetsFrame::init(void)
{
    connect_log();

    stackedWidget = new QStackedWidget;

    QToolButton *btnUp = new QToolButton();
    QToolButton *btnDown = new QToolButton();
    btnUp->setText("+");
    btnDown->setText("-");
    btnUp->setFixedSize(32, 32);
    btnDown->setFixedSize(32, 32);
    connect(btnUp, SIGNAL(clicked()), this, SLOT(up()));
    connect(btnDown, SIGNAL(clicked()), this, SLOT(down()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(btnUp);
    vbox->addWidget(btnDown);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setMargin(0);
    hbox->addWidget(stackedWidget, 100);
    hbox->addLayout(vbox, 1);

    setLayout(hbox);
}
//--------------------------------------------------------------------------------
int WidgetsFrame::addWidget(QWidget *w)
{
    return stackedWidget->addWidget(w);
}
//--------------------------------------------------------------------------------
void WidgetsFrame::up(void)
{
    stackedWidget->setCurrentIndex(stackedWidget->currentIndex() - 1);
}
//--------------------------------------------------------------------------------
void WidgetsFrame::down(void)
{
    stackedWidget->setCurrentIndex(stackedWidget->currentIndex() + 1);
}
//--------------------------------------------------------------------------------
