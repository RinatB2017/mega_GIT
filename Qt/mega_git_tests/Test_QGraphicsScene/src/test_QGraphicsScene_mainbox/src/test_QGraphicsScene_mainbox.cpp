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
#include "ui_test_QGraphicsScene_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QGraphicsScene_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    is_blocked(false)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(rectangle_R) delete rectangle_R;
    if(rectangle_G) delete rectangle_G;
    if(rectangle_B) delete rectangle_B;
    if(text)        delete text;
    if(scene)       delete scene;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 320, 320);

    QBrush redBrush(Qt::red);
    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    rectangle_R = new Item(0,   0,   100, 100, outlinePen, redBrush);
    rectangle_G = new Item(110, 110, 100, 100, outlinePen, greenBrush);
    rectangle_B = new Item(220, 220, 100, 100, outlinePen, blueBrush);

    rectangle_R->set_name("R");
    rectangle_G->set_name("G");
    rectangle_B->set_name("B");

    rectangle_R->setFlag(QGraphicsItem::ItemIsMovable);
    rectangle_G->setFlag(QGraphicsItem::ItemIsMovable);
    rectangle_B->setFlag(QGraphicsItem::ItemIsMovable);

    scene->addItem(rectangle_R);
    scene->addItem(rectangle_G);
    scene->addItem(rectangle_B);

    text = scene->addText("text", QFont("Arial", 20) );
    text->setPos(250, 50);
    text->setFlag(QGraphicsItem::ItemIsMovable);

    ui->graphicsView->setFixedSize(322, 322);
    ui->graphicsView->setScene(scene);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
    //rectangle_R->moveBy(rectangle_R->x()-10, rectangle_B->y()-10);
}
//--------------------------------------------------------------------------------
void MainBox::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
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
