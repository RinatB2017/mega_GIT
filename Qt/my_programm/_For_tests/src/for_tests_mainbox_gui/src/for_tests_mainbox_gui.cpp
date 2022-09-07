/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "for_tests_mainbox_gui.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox_GUI::MainBox_GUI(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox_GUI)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox_GUI::~MainBox_GUI()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox_GUI::init(void)
{
    ui->setupUi(this);

    connect_log_signals(ui->controls_widget, this);

    connect(ui->btn_show, &QPushButton::clicked, [this]() {
        emit info(ui->cw_calendar->selectedDate().toString("yyyy.MM.dd"));
    });

    connect(ui->btn_1,  &QPushButton::clicked,  this,   &MainBox_GUI::btn_1_clicked);
    connect(ui->btn_2,  &QPushButton::clicked,  this,   &MainBox_GUI::btn_2_clicked);

#ifdef USE_DOCK_WIDGETS
    QTimer::singleShot(0, [this]{
        ui->gridLayout->setContentsMargins(20, 20, 20, 20);

        MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
        if(mw)
        {
#ifndef NO_MENU
            mw->add_mdi_sorting();
#endif
#ifdef TEST_DOCK
            mw->add_dock_widget("test_dock",
                                "test_dock",
                                Qt::BottomDockWidgetArea,
                                ui->test_frame);
#endif
        }
        load_widgets();
    });
#endif
}
//--------------------------------------------------------------------------------
void MainBox_GUI::set_item_param(QTreeWidgetItem *item,
                                 Qt::ItemFlags flags,
                                 const QString &caption,
                                 Qt::CheckState state)
{
    item->setFlags(item->flags() | flags);  // Qt::ItemIsUserCheckable
    item->setText(0, caption);
    item->setCheckState(0, state);          // Qt::Checked
}
//--------------------------------------------------------------------------------
void MainBox_GUI::btn_1_clicked(void)
{
    root_item = new QTreeWidgetItem(ui->treeWidget);
    root_item->setText(0, "root");

    QTreeWidgetItem *item_1 = new QTreeWidgetItem();
    QTreeWidgetItem *item_2 = new QTreeWidgetItem();
    QTreeWidgetItem *item_3 = new QTreeWidgetItem();

    QTreeWidgetItem *item_1_1 = new QTreeWidgetItem();
    QTreeWidgetItem *item_1_2 = new QTreeWidgetItem();
    QTreeWidgetItem *item_1_3 = new QTreeWidgetItem();

    QTreeWidgetItem *item_2_1 = new QTreeWidgetItem();
    QTreeWidgetItem *item_2_2 = new QTreeWidgetItem();
    QTreeWidgetItem *item_2_3 = new QTreeWidgetItem();

    items.append({ nullptr, item_1, "item_1",  Qt::Unchecked,  &MainBox_GUI::f_not_created });
    items.append({ nullptr, item_2, "item_2",  Qt::Unchecked,  &MainBox_GUI::f_not_created });
    items.append({ nullptr, item_3, "item_3",  Qt::Unchecked,  &MainBox_GUI::f_not_created });

    items.append({ item_1, item_1_1, "item_1_1",  Qt::Unchecked,  &MainBox_GUI::f_not_created });
    items.append({ item_1, item_1_2, "item_1_2",  Qt::Unchecked,  &MainBox_GUI::f_not_created });
    items.append({ item_1, item_1_3, "item_1_3",  Qt::Unchecked,  &MainBox_GUI::f_not_created });

    items.append({ item_2, item_2_1, "item_2_1",  Qt::Unchecked,  &MainBox_GUI::f_not_created });
    items.append({ item_2, item_2_2, "item_2_2",  Qt::Unchecked,  &MainBox_GUI::f_not_created });
    items.append({ item_2, item_2_3, "item_2_3",  Qt::Unchecked,  &MainBox_GUI::f_not_created });

    foreach (ITEM item, items)
    {
        if(item.parent_item != nullptr)
        {
            item.parent_item->addChild(item.item);
        }
        else
        {
            root_item->addChild(item.item);
        }
        item.item->setText(0, item.caption);
        item.item->setCheckState(0, item.state);
    }
}
//--------------------------------------------------------------------------------
void MainBox_GUI::btn_2_clicked(void)
{
#if 1
    int cnt = ui->treeWidget->children().count();
    int cnt2 = ui->treeWidget->columnCount();
    emit info(QString("Cnt: %1").arg(cnt));
    emit info(QString("Cnt2: %1").arg(cnt2));
#endif

#if 0
    int cnt = root_item->childCount();
    emit info(QString("Cnt: %1").arg(cnt));
    for(int n=0; n<cnt; n++)
    {
        if(root_item->child(n)->isSelected())
            emit error(root_item->child(n)->text(0));
        else
            emit info(root_item->child(n)->text(0));
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox_GUI::set_children_state(QTreeWidgetItem *parent_item,
                                     Qt::CheckState state)
{
    Q_ASSERT(parent_item);
    int cnt = parent_item->childCount();
    for(int n=0; n<cnt; n++)
    {
        parent_item->child(n)->setCheckState(0, state);
    }
}
//--------------------------------------------------------------------------------
void MainBox_GUI::f_not_created(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void MainBox_GUI::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
