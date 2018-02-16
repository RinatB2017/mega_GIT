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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "test_qdockwidget.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Test_QDockWidget::Test_QDockWidget(const QString &title,
                                   QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(title);
    setObjectName(title);

    grid_map = new QGridLayout;

    int index = 1;
    for(int n=0; n<9; n++)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setProperty("value", index);
        btn->setObjectName(QString("btn_dock_%1").arg(index));
        btn->setText(QString("%1").arg(index));
        connect(btn,    SIGNAL(clicked(bool)),  this,   SLOT(click_btn()));
        widgets.append(btn);

        index++;
    }

    QWidget *w = new QWidget;

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addLayout(grid_map);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addStretch(1);

    w->setLayout(vbox);
    setWidget(w);

    //connect(this,   SIGNAL(allowedAreasChanged( Qt::DockWidgetAreas)),          this,   SLOT(allowedAreasChanged(Qt::DockWidgetAreas)));
    connect(this,   SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),            this,   SLOT(dockLocationChanged(Qt::DockWidgetArea)));;
    //connect(this,   SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)),   this,   SLOT(featuresChanged(QDockWidget::DockWidgetFeatures)));
    //connect(this,   SIGNAL(topLevelChanged(bool)),                              this,   SLOT(topLevelChanged(bool)));
    //connect(this,   SIGNAL(visibilityChanged(bool)),                            this,   SLOT(visibilityChanged(bool)));
}
//--------------------------------------------------------------------------------
void Test_QDockWidget::click_btn(void)
{
    QToolButton *btn = dynamic_cast<QToolButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    emit info(QString("%1").arg(btn->property("value").toInt()));

}
//--------------------------------------------------------------------------------
void Test_QDockWidget::create_left_dock(void)
{
    emit trace(Q_FUNC_INFO);

    // удаляем все объекты
    QLayoutItem *child = 0;
    while ((child = grid_map->takeAt(0)) != 0)
    {
        grid_map->removeItem(child);
        if (child->layout())
            delete child->layout();
        delete child;
    }
    //---

    int x = 0;
    int y = 0;
    foreach (QToolButton *btn, widgets)
    {
        grid_map->addWidget(btn, y, x);
        x++;
        if(x>2)
        {
            y++;
            x=0;
        }
    }
    widget()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
//--------------------------------------------------------------------------------
void Test_QDockWidget::create_top_dock(void)
{
    emit trace(Q_FUNC_INFO);

    // удаляем все объекты
    QLayoutItem *child = 0;
    while ((child = grid_map->takeAt(0)) != 0)
    {
        grid_map->removeItem(child);
        if (child->layout())
            delete child->layout();
        delete child;
    }
    //---

    int x = 0;
    foreach (QToolButton *btn, widgets)
    {
        grid_map->addWidget(btn, 0, x);
        x++;
    }
    widget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
#if 0
void Test_QDockWidget::allowedAreasChanged ( Qt::DockWidgetAreas allowedAreas )
{
    emit trace(Q_FUNC_INFO);
    switch (allowedAreas) {
    case Qt::LeftToolBarArea:
        emit debug("LeftToolBarArea");
        //create_left_dock();
        break;

    case Qt::RightToolBarArea:
        emit debug("RightToolBarArea");
        //create_left_dock();
        break;

    case Qt::TopToolBarArea:
        emit debug("TopToolBarArea");
        //create_top_dock();
        break;

    case Qt::BottomToolBarArea:
        emit debug("BottomToolBarArea");
        //create_top_dock();
        break;

    default:
        break;
    }
}
#endif
//--------------------------------------------------------------------------------
void Test_QDockWidget::dockLocationChanged ( Qt::DockWidgetArea area )
{
    emit trace(Q_FUNC_INFO);
    switch (area) {
    case Qt::LeftDockWidgetArea:
        emit debug("LeftDockWidgetArea");
        create_left_dock();
        break;

    case Qt::RightDockWidgetArea:
        emit debug("RightDockWidgetArea");
        create_left_dock();
        break;

    case Qt::TopDockWidgetArea:
        emit debug("TopDockWidgetArea");
        create_top_dock();
        break;

    case Qt::BottomDockWidgetArea:
        emit debug("BottomDockWidgetArea");
        create_top_dock();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
#if 0
void Test_QDockWidget::featuresChanged ( QDockWidget::DockWidgetFeatures features )
{
    emit trace(Q_FUNC_INFO);
    switch (features) {
    case DockWidgetClosable:
        emit debug("DockWidgetClosable");
        break;

    case DockWidgetMovable:
        emit debug("DockWidgetMovable");
        break;

    case DockWidgetFloatable:
        emit debug("DockWidgetFloatable");
        break;

    default:
        break;
    }
}
#endif
//--------------------------------------------------------------------------------
#if 0
void Test_QDockWidget::topLevelChanged ( bool topLevel )
{
    emit trace(Q_FUNC_INFO);
    emit debug(QString("topLevel is %1").arg(topLevel ? "true" : "false"));
}
#endif
//--------------------------------------------------------------------------------
#if 0
void Test_QDockWidget::visibilityChanged ( bool visible )
{
    emit trace(Q_FUNC_INFO);
    emit debug(QString("visible is %1").arg(visible ? "true" : "false"));
}
#endif
//--------------------------------------------------------------------------------
