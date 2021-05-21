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
#include "ui_test_PropertyBrowser_mainbox.h"
//--------------------------------------------------------------------------------
#include "test_PropertyBrowser_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "qtgroupboxpropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
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
    comboBoxFactory = new QtEnumEditorFactory(this);

    QtGroupPropertyManager *groupManager = new QtGroupPropertyManager(this);
    enumManager = new QtEnumPropertyManager(this);

    item0 = groupManager->addProperty("Serial port");

    device   = enumManager->addProperty(tr("Устройство"));
    baudrate = enumManager->addProperty(tr("Скорость"));
    databits = enumManager->addProperty(tr("databits"));
    parity   = enumManager->addProperty(tr("parity"));
    stopbits = enumManager->addProperty(tr("stopbits"));
    flow     = enumManager->addProperty(tr("flow"));

    item0->addSubProperty(device);
    item0->addSubProperty(baudrate);
    item0->addSubProperty(databits);
    item0->addSubProperty(parity);
    item0->addSubProperty(stopbits);
    item0->addSubProperty(flow);

    QStringList enumNames;

    enumNames.clear();
    enumNames << "x" << "y";
    enumManager->setEnumNames(device, enumNames);

    enumNames.clear();
    enumNames << "300" << "1200" << "4800" << "9600";
    enumManager->setEnumNames(baudrate, enumNames);

    enumNames.clear();
    enumNames << "1" << "2" << "4" << "8";
    enumManager->setEnumNames(databits, enumNames);

    enumNames.clear();
    enumNames << "0" << "1";
    enumManager->setEnumNames(parity, enumNames);

    enumNames.clear();
    enumNames << "0" << "1";
    enumManager->setEnumNames(stopbits, enumNames);

    enumNames.clear();
    enumNames << "0" << "1";
    enumManager->setEnumNames(flow, enumNames);

    editor = new QtTreePropertyBrowser();

    editor->setFactoryForManager(enumManager, comboBoxFactory);
    editor->addProperty(item0);

    QGridLayout *layout = new QGridLayout(this);
    QLabel *label = new QLabel("Test");
    label->setWordWrap(true);
    label->setFrameShadow(QFrame::Sunken);
    label->setFrameShape(QFrame::Panel);
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label, 0, 0);
    layout->addWidget(editor, 1, 0);

    QPushButton *pb = new QPushButton("test");
    connect(pb, SIGNAL(clicked()), this, SLOT(test()));
    layout->addWidget(pb);
    layout->setColumnMinimumWidth(0, 250);

    setLayout(layout);

    setMinimumHeight(400);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QWidget *w = new QWidget();

    QtBoolPropertyManager *boolManager = new QtBoolPropertyManager(w);
    QtIntPropertyManager *intManager = new QtIntPropertyManager(w);
    QtStringPropertyManager *stringManager = new QtStringPropertyManager(w);
    QtSizePropertyManager *sizeManager = new QtSizePropertyManager(w);
    QtRectPropertyManager *rectManager = new QtRectPropertyManager(w);
    QtSizePolicyPropertyManager *sizePolicyManager = new QtSizePolicyPropertyManager(w);
    QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(w);
    QtGroupPropertyManager *groupManager = new QtGroupPropertyManager(w);

    QtProperty *item0 = groupManager->addProperty("QObject");

    QtProperty *item1 = stringManager->addProperty("objectName");
    item0->addSubProperty(item1);

    QtProperty *item2 = boolManager->addProperty("enabled");
    item0->addSubProperty(item2);

    QtProperty *item3 = rectManager->addProperty("geometry");
    item0->addSubProperty(item3);

    QtProperty *item4 = sizePolicyManager->addProperty("sizePolicy");
    item0->addSubProperty(item4);

    QtProperty *item5 = sizeManager->addProperty("sizeIncrement");
    item0->addSubProperty(item5);

    QtProperty *item7 = boolManager->addProperty("mouseTracking");
    item0->addSubProperty(item7);

    QtProperty *item8 = enumManager->addProperty("direction");
    QStringList enumNames;
    enumNames << "Up" << "Right" << "Down" << "Left";
    enumManager->setEnumNames(item8, enumNames);
    QMap<int, QIcon> enumIcons;
    enumIcons[0] = QIcon(":/demo/images/up.png");
    enumIcons[1] = QIcon(":/demo/images/right.png");
    enumIcons[2] = QIcon(":/demo/images/down.png");
    enumIcons[3] = QIcon(":/demo/images/left.png");
    enumManager->setEnumIcons(item8, enumIcons);
    item0->addSubProperty(item8);

    QtProperty *item9 = intManager->addProperty("value");
    intManager->setRange(item9, -100, 100);
    item0->addSubProperty(item9);

    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(w);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(w);
    QtSliderFactory *sliderFactory = new QtSliderFactory(w);
    QtScrollBarFactory *scrollBarFactory = new QtScrollBarFactory(w);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(w);
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(w);

    QtAbstractPropertyBrowser *editor1 = new QtTreePropertyBrowser();
    editor1->setFactoryForManager(boolManager, checkBoxFactory);
    editor1->setFactoryForManager(intManager, spinBoxFactory);
    editor1->setFactoryForManager(stringManager, lineEditFactory);
    editor1->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
    editor1->setFactoryForManager(rectManager->subIntPropertyManager(), spinBoxFactory);
    editor1->setFactoryForManager(sizePolicyManager->subIntPropertyManager(), spinBoxFactory);
    editor1->setFactoryForManager(sizePolicyManager->subEnumPropertyManager(), comboBoxFactory);
    editor1->setFactoryForManager(enumManager, comboBoxFactory);

    editor1->addProperty(item0);

    QtAbstractPropertyBrowser *editor2 = new QtTreePropertyBrowser();
    editor2->addProperty(item0);

    QtAbstractPropertyBrowser *editor3 = new QtGroupBoxPropertyBrowser();
    editor3->setFactoryForManager(boolManager, checkBoxFactory);
    editor3->setFactoryForManager(intManager, spinBoxFactory);
    editor3->setFactoryForManager(stringManager, lineEditFactory);
    editor3->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
    editor3->setFactoryForManager(rectManager->subIntPropertyManager(), spinBoxFactory);
    editor3->setFactoryForManager(sizePolicyManager->subIntPropertyManager(), spinBoxFactory);
    editor3->setFactoryForManager(sizePolicyManager->subEnumPropertyManager(), comboBoxFactory);
    editor3->setFactoryForManager(enumManager, comboBoxFactory);

    editor3->addProperty(item0);

    QScrollArea *scroll3 = new QScrollArea();
    scroll3->setWidgetResizable(true);
    scroll3->setWidget(editor3);

    QtAbstractPropertyBrowser *editor4 = new QtGroupBoxPropertyBrowser();
    editor4->setFactoryForManager(boolManager, checkBoxFactory);
    editor4->setFactoryForManager(intManager, scrollBarFactory);
    editor4->setFactoryForManager(stringManager, lineEditFactory);
    editor4->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
    editor4->setFactoryForManager(rectManager->subIntPropertyManager(), spinBoxFactory);
    editor4->setFactoryForManager(sizePolicyManager->subIntPropertyManager(), sliderFactory);
    editor4->setFactoryForManager(sizePolicyManager->subEnumPropertyManager(), comboBoxFactory);
    editor4->setFactoryForManager(enumManager, comboBoxFactory);

    editor4->addProperty(item0);

    QScrollArea *scroll4 = new QScrollArea();
    scroll4->setWidgetResizable(true);
    scroll4->setWidget(editor4);

    QtAbstractPropertyBrowser *editor5 = new QtButtonPropertyBrowser();
    editor5->setFactoryForManager(boolManager, checkBoxFactory);
    editor5->setFactoryForManager(intManager, scrollBarFactory);
    editor5->setFactoryForManager(stringManager, lineEditFactory);
    editor5->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
    editor5->setFactoryForManager(rectManager->subIntPropertyManager(), spinBoxFactory);
    editor5->setFactoryForManager(sizePolicyManager->subIntPropertyManager(), sliderFactory);
    editor5->setFactoryForManager(sizePolicyManager->subEnumPropertyManager(), comboBoxFactory);
    editor5->setFactoryForManager(enumManager, comboBoxFactory);

    editor5->addProperty(item0);

    QScrollArea *scroll5 = new QScrollArea();
    scroll5->setWidgetResizable(true);
    scroll5->setWidget(editor5);

    QGridLayout *layout = new QGridLayout(w);
    QLabel *label1 = new QLabel("Editable Tree Property Browser");
    QLabel *label2 = new QLabel("Read Only Tree Property Browser, editor factories are not set");
    QLabel *label3 = new QLabel("Group Box Property Browser");
    QLabel *label4 = new QLabel("Group Box Property Browser with different editor factories");
    QLabel *label5 = new QLabel("Button Property Browser");
    label1->setWordWrap(true);
    label2->setWordWrap(true);
    label3->setWordWrap(true);
    label4->setWordWrap(true);
    label5->setWordWrap(true);
    label1->setFrameShadow(QFrame::Sunken);
    label2->setFrameShadow(QFrame::Sunken);
    label3->setFrameShadow(QFrame::Sunken);
    label4->setFrameShadow(QFrame::Sunken);
    label5->setFrameShadow(QFrame::Sunken);
    label1->setFrameShape(QFrame::Panel);
    label2->setFrameShape(QFrame::Panel);
    label3->setFrameShape(QFrame::Panel);
    label4->setFrameShape(QFrame::Panel);
    label5->setFrameShape(QFrame::Panel);
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);
    label3->setAlignment(Qt::AlignCenter);
    label4->setAlignment(Qt::AlignCenter);
    label5->setAlignment(Qt::AlignCenter);

    layout->addWidget(label1, 0, 0);
    layout->addWidget(label2, 0, 1);
    layout->addWidget(label3, 0, 2);
    layout->addWidget(label4, 0, 3);
    layout->addWidget(label5, 0, 4);
    layout->addWidget(editor1, 1, 0);
    layout->addWidget(editor2, 1, 1);
    layout->addWidget(scroll3, 1, 2);
    layout->addWidget(scroll4, 1, 3);
    layout->addWidget(scroll5, 1, 4);
    w->show();

#if 0
    QList<QtProperty *> pl = editor->properties();
    foreach (QtProperty *property, pl)
    {
        emit debug(QString("property [%1]").arg(property->valueText()));
    }

    emit debug(QString("[%1]").arg(editor->property("databits").toString()));
    bool r = editor->setProperty("databits", "2");
    emit info(r ? "true" : "false");
    emit debug(editor->property("databits").toString());
#endif
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
