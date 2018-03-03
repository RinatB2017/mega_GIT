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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "ui_Test_QtPropertyBrowser_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "Test_QtPropertyBrowser_mainbox.hpp"
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
void MainBox::add_property(QtVariantPropertyManager *variantManager,
                           QList<StructProperty_t>  *l_property,
                           QString name,
                           QString object_name,
                           int propertyType)
{
    StructProperty_t d_property;
    //---
    d_property.name = name;
    d_property.object_name = object_name;
    d_property.propertyType = propertyType;
    d_property.item = variantManager->addProperty(d_property.propertyType, d_property.name);
    //---
    (*l_property).append(d_property);
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    //---
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();
    //QtGroupPropertyManager *variantManager = new QtGroupPropertyManager();
    //---
    add_property(variantManager, &l_property_0, tr("Данные 1"), "data_1", QVariant::Int);
    add_property(variantManager, &l_property_0, tr("Данные 2"), "data_2", QVariant::Int);
    add_property(variantManager, &l_property_0, tr("Данные 3"), "data_3", QVariant::Int);
    QtProperty *topItem_0 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Настройки 0"));
    foreach (StructProperty_t prop, l_property_0)
    {
        topItem_0->addSubProperty(prop.item);
    }
    //---
    add_property(variantManager, &l_property_1, tr("Данные 1"), "data_1", QVariant::Int);
    add_property(variantManager, &l_property_1, tr("Данные 2"), "data_2", QVariant::Int);
    add_property(variantManager, &l_property_1, tr("Данные 3"), "data_3", QVariant::Int);
    add_property(variantManager, &l_property_1, tr("Данные 4"), "data_4", QVariant::Int);
    QtProperty *topItem_1 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Настройки 1"));
    foreach (StructProperty_t prop, l_property_1)
    {
        topItem_1->addSubProperty(prop.item);
    }
    //---
    add_property(variantManager, &l_property_2, tr("Данные 1"), "data_1", QVariant::Int);
    add_property(variantManager, &l_property_2, tr("Данные 2"), "data_2", QVariant::Int);
    add_property(variantManager, &l_property_2, tr("Данные 3"), "data_3", QVariant::Int);
    add_property(variantManager, &l_property_2, tr("Данные 4"), "data_4", QVariant::Int);
    add_property(variantManager, &l_property_2, tr("Данные 5"), "data_5", QVariant::Int);
    QtProperty *topItem_2 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("Настройки 2"));
    foreach (StructProperty_t prop, l_property_2)
    {
        topItem_2->addSubProperty(prop.item);
    }
    //---
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser(this);
    variantEditor->setFactoryForManager(variantManager, variantFactory);

    variantEditor->addProperty(topItem_0);
    variantEditor->addProperty(topItem_1);
    variantEditor->addProperty(topItem_2);

    //variantEditor->setExpanded((QtBrowserItem *)topItem_0, false);
    //variantEditor->setExpanded((QtBrowserItem *)topItem_1, false);
    //variantEditor->setExpanded((QtBrowserItem *)topItem_2, false);

    //variantEditor->setPropertiesWithoutValueMarked(true);
    //variantEditor->setRootIsDecorated(true);
    variantEditor->setMinimumSize(variantEditor->sizeHint());
    variantEditor->setHeaderVisible(false);

    QHBoxLayout *box = new QHBoxLayout();
    box->addWidget(variantEditor);
    setLayout(box);
    //---

    createTestBar();

    setFixedWidth(sizeHint().width());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
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
            typedef void (MainBox::*function)(void);
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
void MainBox::test_0(void)
{
    emit info("Test_0()");

    bool ok = false;
    foreach (StructProperty_t prop, l_property_0)
    {
        int value = prop.item->value().toInt(&ok);
        if(ok)
        {
            emit info(QString("%1 value %2")
                      .arg(prop.object_name)
                      .arg(value));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");

    bool ok = false;
    foreach (StructProperty_t prop, l_property_1)
    {
        int value = prop.item->value().toInt(&ok);
        if(ok)
        {
            emit info(QString("%1 value %2")
                      .arg(prop.object_name)
                      .arg(value));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");

    bool ok = false;
    foreach (StructProperty_t prop, l_property_2)
    {
        int value = prop.item->value().toInt(&ok);
        if(ok)
        {
            emit info(QString("%1 value %2")
                      .arg(prop.object_name)
                      .arg(value));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");

    int n = 0;
    foreach (StructProperty_t prop, l_property_0)
    {
        prop.item->setValue(n);
        n++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");

    int n = 0;
    foreach (StructProperty_t prop, l_property_1)
    {
        prop.item->setValue(n);
        n++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");

    int n = 0;
    foreach (StructProperty_t prop, l_property_2)
    {
        prop.item->setValue(n);
        n++;
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    qDebug() << "MainBox::closeEvent";
}
//--------------------------------------------------------------------------------
