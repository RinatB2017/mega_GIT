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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_QTreeWidget_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QTreeWidget_mainbox.hpp"
#include "defines.hpp"
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
    prepare_treeWidget();

    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(click(QModelIndex)));

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::prepare_treeWidget(void)
{
    QStringList sl;
    sl << "col 0";
    sl << "col 1";
    sl << "col 2";
    sl << "col 3";
    sl << "col 4";

    ui->treeWidget->setHeaderLabels(sl);

    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0, "root");
    ui->treeWidget->addTopLevelItem(root);

    for(int y=0; y<7; y++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        for(int x=0; x<5; x++)
        {
            item->setText(x, QString("%1_%2").arg(x).arg(y));
            item->setData(x, Qt::UserRole, QString("%1_%2").arg(x).arg(y));
            if(x==3)
            {
                QTreeWidgetItem *item2 = new QTreeWidgetItem();
                item2->setText(0, "item2_0");
                item2->setText(1, "item2_1");
                item2->setText(2, "item2_2");

                item2->setData(0, Qt::UserRole, "item2_0");
                item2->setData(1, Qt::UserRole, "item2_1");
                item2->setData(2, Qt::UserRole, "item2_2");

                item->addChild(item2);

                QTreeWidgetItem *item3 = new QTreeWidgetItem();
                item3->setText(0, "item3_0");
                item3->setText(1, "item3_1");
                item3->setText(2, "item3_2");

                item2->addChild(item3);
            }
        }
        root->addChild(item);
    }

    expanded_item(root);

    //---
    QTreeWidgetItem *root2 = new QTreeWidgetItem();
    root2->setText(0, "root2");
    ui->treeWidget->addTopLevelItem(root2);

    QTreeWidgetItem *root2_item = new QTreeWidgetItem();
    root2_item->setText(0, "root2_item_0");
    root2_item->setText(1, "root2_item_1");
    root2_item->setText(2, "root2_item_2");

    root2->addChild(root2_item);

    expanded_item(root2);
    //---

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}
//--------------------------------------------------------------------------------
void MainBox::click(QModelIndex index)
{
    emit info(QString("click row %1 col %2")
              .arg(index.row())
              .arg(index.column()));

    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(index.row());
    if(item)
    {
        emit error(QString("%1 %2 %3 %4 %5")
                   .arg(item->text(0))
                   .arg(item->text(1))
                   .arg(item->text(2))
                   .arg(item->text(3))
                   .arg(item->text(4)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *frm_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    frm_choice_test->setObjectName("frm_choice_test");

    connect(frm_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), frm_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef void (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::find_item(QTreeWidgetItem *root,
                        QVariant data,
                        QString text)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        if(root->data(n, Qt::UserRole) == data)
        {
            root->setText(n, text);
            return;
        }
    }

    for(int n=0; n<root->childCount(); n++)
    {
        find_item(root->child(n), data, text);
    }
}
//--------------------------------------------------------------------------------
void MainBox::show_itemtext(QTreeWidgetItem *root)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        emit info(root->text(n));
    }

    for(int n=0; n<root->childCount(); n++)
    {
        show_itemtext(root->child(n));
    }
}
//--------------------------------------------------------------------------------
void MainBox::replace_itemtext(QTreeWidgetItem *root, QString text)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        root->setText(n, text);
    }

    for(int n=0; n<root->childCount(); n++)
    {
        replace_itemtext(root->child(n), text);
    }
}
//--------------------------------------------------------------------------------
void MainBox::expanded_item(QTreeWidgetItem *root)
{
    if(!root) return;

    for(int n=0; n<root->columnCount(); n++)
    {
        root->setExpanded(true);
    }

    for(int n=0; n<root->childCount(); n++)
    {
        expanded_item(root->child(n));
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    emit info("Test_0()");

    QTreeWidgetItem *root = ui->treeWidget->topLevelItem(0);
    if(!root)
    {
        return;
    }
    emit error(QString("[%1]").arg(root->text(-100)));

    //show_itemtext(root);
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");

    QTreeWidgetItem *root = ui->treeWidget->topLevelItem(0);
    if(!root)
    {
        return;
    }

    QVariant data = "item2_2";
    find_item(root, data, "YES");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");

    for(int n=0; n<ui->treeWidget->topLevelItemCount(); n++)
    {
        QTreeWidgetItem *root = ui->treeWidget->topLevelItem(n);
        if(root)
        {
            replace_itemtext(root, "---");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");
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
