/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QToolButton>
#include <QFileInfo>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
    load_config();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(md)
    {
        md->deleteLater();
    }

    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    //---
    md = new MetaData(this);
    connect(md, SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(error(QString)),     this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(message(QString)),   this,   SIGNAL(info(QString)));
    //---

    createTestBar();
    prepare_tv_main();
    update_cb_main();

    ui->tv_main->setContextMenuPolicy(Qt::CustomContextMenu);
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
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QCheckBox *cb_block = new QCheckBox("block");
    toolBar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MainBox::prepare_tv_main(void)
{
    ui->tv_main->header()->hide();

    root = new QTreeWidgetItem;
    root->setText(0,        "Метаданные");
    root->setToolTip(0,     "Метаданные");
    root->setStatusTip(0,   "Метаданные");
    root->setIcon(0,        QIcon(NAME_RADIATION_ICON));

    QTreeWidgetItem *other = new QTreeWidgetItem;
    other->setText(0,       "other");
    other->setToolTip(0,    "other");
    other->setStatusTip(0,  "other");
    other->setIcon(0,       QIcon(NAME_RADIATION_ICON));

    Q_CHECK_PTR(md);
    if(md)
    {
        bool ok = md->load_data(root);
        if(ok)
        {
            emit info("OK");
        }
        md->set_root_item(root);
    }

    ui->tv_main->addTopLevelItem(root);
    root->setExpanded(true);

    ui->tv_main->addTopLevelItem(other);

    connect(ui->tv_main,    SIGNAL(customContextMenuRequested(QPoint)),
            this,           SLOT(popup(QPoint)));
    connect(ui->tv_main,    SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,           SLOT(tv_main_clicked(QTreeWidgetItem*,int)));
    connect(ui->btn_view_table, SIGNAL(clicked(bool)),
            md,             SLOT(view_database()));
    connect(this,   SIGNAL(update_metadata()),  this,   SLOT(update_cb_main()));
}

//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    QSqlDatabase db;
    QStringList sl = db.drivers();
    emit info("-----------------------");
    foreach (QString name, sl)
    {
        emit info(name);
    }
    emit info("-----------------------");
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::f_delete_metadata(QString name)
{
    QStringList sl;
    bool ok = false;
    QFileInfo fi;
    sl.clear();
    fi.setFile(name);
    ok = md->find_data(fi.baseName(), &sl);
    if(ok)
    {
        emit debug("-----");
        foreach (QString name, sl)
        {
            emit message(name);
        }
        emit debug("-----");
    }
    //---
    int btn = MyWidget::messagebox_question("Удаление записей",
                                            QString("Удалить %1 из метаданных").arg(name));
    if(btn == QMessageBox::Yes)
    {
        if(sl.count())
        {
            ok = md->delete_data(fi.baseName(), name);
            if(ok)
            {
                MyWidget::messagebox_info("удаление записей", "Удаление прошло успешно");
                bool ok = md->load_data(root);
                if(ok)
                {
                    if(root)
                    {
                        while(root->childCount() > 1)
                        {
                            if(root->child(0)->text(0) != "...")
                            {
                                root->removeChild(root->child(0));
                            }
                        }
                    }
                    foreach (QString name, sl)
                    {
                        QTreeWidgetItem *item = new QTreeWidgetItem();
                        item->setText(0, name);
                        item->setData(0, Qt::UserRole, ID_METADATA_DELETE);
                        root->insertChild(0, item);
                    }
                }
            }
            else
            {
                MyWidget::messagebox_critical("Ошибка", QString("Удаление %1 пока не сделано!")
                                              .arg(sl.at(0)));
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::tv_main_clicked(QTreeWidgetItem *item, int)
{
    int id = item->data(0, Qt::UserRole).toInt();
    switch(id)
    {
    case ID_METADATA_APPEND:
        if(md)
        {
            md->f_add_metadata();
            emit update_metadata();
        }
        break;

    case ID_METADATA_DELETE:
        break;

    default:
        if(id)
        {
            emit error(QString("unknown ID %1").arg(id));
        }
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::popup(QPoint point)
{
    //FIXME
    int id = ui->tv_main->itemAt(point)->data(0, Qt::UserRole).toInt();
    if((id != ID_METADATA_APPEND) && (id != ID_METADATA_DELETE) && (id != ID_METADATA))
    {
        return;
    }

    QMenu *popup_menu = new QMenu(this);

    QAction *add_file_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                           QObject::tr("Добавить файл"),
                                           this);
    QAction *del_file_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                           QObject::tr("Удалить файл"),
                                           this);

    add_file_action->setProperty("name", ui->tv_main->itemAt(point)->text(0));
    add_file_action->setProperty("fullname", ui->tv_main->itemAt(point)->toolTip(0));

    del_file_action->setProperty("name", ui->tv_main->itemAt(point)->text(0));
    del_file_action->setProperty("fullname", ui->tv_main->itemAt(point)->toolTip(0));

    switch(id)
    {
    case ID_METADATA:
        add_file_action->setEnabled(true);
        del_file_action->setEnabled(false);
        break;

    case ID_METADATA_APPEND:
        add_file_action->setEnabled(true);
        del_file_action->setEnabled(false);
        break;

    case ID_METADATA_DELETE:
        add_file_action->setEnabled(false);
        del_file_action->setEnabled(true);
        break;

    default:
        break;
    }

    popup_menu->addAction(add_file_action);
    popup_menu->addSeparator();
    popup_menu->addAction(del_file_action);

    connect(add_file_action,  SIGNAL(triggered()), this, SLOT(f_add_metadata()));
    connect(del_file_action,  SIGNAL(triggered()), this, SLOT(f_del_metadata()));

    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void MainBox::update_cb_main(void)
{
    ui->cb_main->clear();

    Q_CHECK_PTR(md);
    if(md)
    {
        bool ok = md->load_data(ui->cb_main);
        if(ok)
        {
            emit info("OK");
        }
        md->set_root_item(root);
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_add_metadata(void)
{
    if(md)
    {
        bool ok = md->f_add_metadata(); //f_add_metadata
        if(ok)
        {
            emit update_metadata();
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_del_metadata(void)
{
    QAction *s = dynamic_cast<QAction *>(sender());
    if(s == nullptr)
    {
        return;
    }
    if(md)
    {
        bool ok = md->f_del_metadata(s->property("name").toString(),
                                     s->property("fullname").toString());
        if(ok)
        {
            emit update_metadata();
        }
    }
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
