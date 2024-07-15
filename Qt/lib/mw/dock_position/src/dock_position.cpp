/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include "mainwindow.hpp"
#include "dock_position.hpp"
#include "ui_dock_position.h"
//--------------------------------------------------------------------------------
Dock_position::Dock_position(const QString &doc_name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dock_position)
{
    ui->setupUi(this);

    this->dock_name = doc_name;

#ifndef NORMAL_BUTTONS
    int size = 24;
    ui->btn_show->setFixedSize(size, size);

    ui->buttons_layout->setContentsMargins(0, 0, 0, 0);
    ui->buttons_layout->setSpacing(0);
#endif

    ui->btn_show->setToolTip(QObject::tr("Show buttons"));

    connect(ui->btn_show,   &QToolButton::clicked,
            this,           &Dock_position::show_dlg);
}
//--------------------------------------------------------------------------------
Dock_position::~Dock_position()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Dock_position::show_dlg(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(QApplication::activeWindow());
    if(mw)
    {
        QDockWidget *dw = mw->findChild<QDockWidget *>(dock_name);
        if(dw)
        {
            Docker_mover *dm = new Docker_mover();
            Q_ASSERT(dm);
            dm->setWindowFlags(Qt::WindowCloseButtonHint);
            int btn = dm->exec();
            if(btn == QDialog::Accepted)
            {
                DOCKER_STATES state = dm->get_state();
                switch(state)
                {
                case UP:
                    mw->addDockWidget(Qt::TopDockWidgetArea, dw);
                    break;

                case DOWN:
                    mw->addDockWidget(Qt::BottomDockWidgetArea, dw);
                    break;

                case LEFT:
                    mw->addDockWidget(Qt::LeftDockWidgetArea, dw);
                    break;

                case RIGHT:
                    mw->addDockWidget(Qt::RightDockWidgetArea, dw);
                    break;

                default:
                    break;
                }
            }
        }
    }
    else
    {
#ifdef Q_DEBUG
        qDebug() << "mw is null!";
#endif
    }
}
//--------------------------------------------------------------------------------
