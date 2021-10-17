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
#include <QSvgRenderer>
#include <QSvgGenerator>
//--------------------------------------------------------------------------------
#include "ui_cards_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "cards_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "card.hpp"
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
    prepare_and_show_cards();

    setMinimumSize(640, 480);
}
//--------------------------------------------------------------------------------
void MainBox::prepare_and_show_cards(void)
{
    QSvgRenderer *re =  new QSvgRenderer(QString(":/cards/Ancient_Egyptians.svgz"));
    if(re == nullptr)
    {
        return;
    }

    splash->set_progress(20);

    int w_card = 170;
    int h_card = 210;
    int y = 10;
    Card *card_0_T  = new Card(this); card_0_T->create_card("0_T", 5,  5, w_card, h_card, re);          card_0_T->move(0,y);
    Card *card_0_2  = new Card(this); card_0_2->create_card("0_2", 170, 5, w_card, h_card, re);         card_0_2->move(50,y);
    Card *card_0_3  = new Card(this); card_0_3->create_card("0_3", 348, 5, w_card, h_card, re);         card_0_3->move(100,y);
    Card *card_0_4  = new Card(this); card_0_4->create_card("0_4", 522, 5, w_card, h_card, re);         card_0_4->move(150,y);
    Card *card_0_5  = new Card(this); card_0_5->create_card("0_5", 698, 5, w_card, h_card, re);         card_0_5->move(200,y);
    Card *card_0_6  = new Card(this); card_0_6->create_card("0_6", 875, 5, w_card, h_card, re);         card_0_6->move(250,y);
    Card *card_0_7  = new Card(this); card_0_7->create_card("0_7", 1055, 5, w_card, h_card, re);        card_0_7->move(300,y);
    Card *card_0_8  = new Card(this); card_0_8->create_card("0_8", 1225, 5, w_card, h_card, re);        card_0_8->move(350,y);
    Card *card_0_9  = new Card(this); card_0_9->create_card("0_9", 1400, 5, w_card, h_card, re);        card_0_9->move(400,y);
    Card *card_0_10 = new Card(this); card_0_10->create_card("0_10", 1402, 932, w_card, h_card, re);    card_0_10->move(450,y);
    Card *card_0_V  = new Card(this); card_0_V->create_card("0_V", 1592, 5, w_card, h_card, re);        card_0_V->move(500,y);
    Card *card_0_D  = new Card(this); card_0_D->create_card("0_D", 1774, 5, w_card, h_card, re);        card_0_D->move(550,y);
    Card *card_0_K  = new Card(this); card_0_K->create_card("0_K", 1930, 5, w_card, h_card, re);        card_0_K->move(600,y);

    splash->set_progress(40);

    y+=50;
    Card *card_1_T  = new Card(this); card_1_T->create_card("1_T", 5,  235, w_card, h_card, re);        card_1_T->move(0,y);
    Card *card_1_2  = new Card(this); card_1_2->create_card("1_2", 180, 235, w_card, h_card, re);       card_1_2->move(50,y);
    Card *card_1_3  = new Card(this); card_1_3->create_card("1_3", 358, 235, w_card, h_card, re);       card_1_3->move(100,y);
    Card *card_1_4  = new Card(this); card_1_4->create_card("1_4", 532, 235, w_card, h_card, re);       card_1_4->move(150,y);
    Card *card_1_5  = new Card(this); card_1_5->create_card("1_5", 708, 235, w_card, h_card, re);       card_1_5->move(200,y);
    Card *card_1_6  = new Card(this); card_1_6->create_card("1_6", 885, 235, w_card, h_card, re);       card_1_6->move(250,y);
    Card *card_1_7  = new Card(this); card_1_7->create_card("1_7", 1065, 235, w_card, h_card, re);      card_1_7->move(300,y);
    Card *card_1_8  = new Card(this); card_1_8->create_card("1_8", 1235, 235, w_card, h_card, re);      card_1_8->move(350,y);
    Card *card_1_9  = new Card(this); card_1_9->create_card("1_9", 1410, 235, w_card, h_card, re);      card_1_9->move(400,y);
    Card *card_1_10 = new Card(this); card_1_10->create_card("1_10", 1580, 235, w_card, h_card, re);    card_1_10->move(450,y);
    Card *card_1_V  = new Card(this); card_1_V->create_card("1_V", 1592, 235, w_card, h_card, re);      card_1_V->move(500,y);
    Card *card_1_D  = new Card(this); card_1_D->create_card("1_D", 1764, 235, w_card, h_card, re);      card_1_D->move(550,y);
    Card *card_1_K  = new Card(this); card_1_K->create_card("1_K", 1926, 235, w_card, h_card, re);      card_1_K->move(600,y);

    splash->set_progress(60);

    y+=50;
    Card *card_2_T  = new Card(this); card_2_T->create_card("2_T", 12,  461, w_card, h_card, re);       card_2_T->move(0,y);
    Card *card_2_2  = new Card(this); card_2_2->create_card("2_2", 180, 461, w_card, h_card, re);       card_2_2->move(50,y);
    Card *card_2_3  = new Card(this); card_2_3->create_card("2_3", 348, 461, w_card, h_card, re);       card_2_3->move(100,y);
    Card *card_2_4  = new Card(this); card_2_4->create_card("2_4", 522, 461, w_card, h_card, re);       card_2_4->move(150,y);
    Card *card_2_5  = new Card(this); card_2_5->create_card("2_5", 700, 461, w_card, h_card, re);       card_2_5->move(200,y);
    Card *card_2_6  = new Card(this); card_2_6->create_card("2_6", 875, 461, w_card, h_card, re);       card_2_6->move(250,y);
    Card *card_2_7  = new Card(this); card_2_7->create_card("2_7", 1055, 461, w_card, h_card, re);      card_2_7->move(300,y);
    Card *card_2_8  = new Card(this); card_2_8->create_card("2_8", 1225, 461, w_card, h_card, re);      card_2_8->move(350,y);
    Card *card_2_9  = new Card(this); card_2_9->create_card("2_9", 1400, 461, w_card, h_card, re);      card_2_9->move(400,y);
    Card *card_2_10 = new Card(this); card_2_10->create_card("2_10", 1752, 932, w_card, h_card, re);    card_2_10->move(450,y);
    Card *card_2_V  = new Card(this); card_2_V->create_card("2_V", 1582, 461, w_card, h_card, re);      card_2_V->move(500,y);
    Card *card_2_D  = new Card(this); card_2_D->create_card("2_D", 1764, 461, w_card, h_card, re);      card_2_D->move(550,y);
    Card *card_2_K  = new Card(this); card_2_K->create_card("2_K", 1935, 461, w_card, h_card, re);      card_2_K->move(600,y);

    splash->set_progress(80);

    y+=50;
    Card *card_3_T  = new Card(this); card_3_T->create_card("3_T", 12,  692, w_card, h_card, re);       card_3_T->move(0,y);
    Card *card_3_2  = new Card(this); card_3_2->create_card("3_2", 180, 692, w_card, h_card, re);       card_3_2->move(50,y);
    Card *card_3_3  = new Card(this); card_3_3->create_card("3_3", 348, 692, w_card, h_card, re);       card_3_3->move(100,y);
    Card *card_3_4  = new Card(this); card_3_4->create_card("3_4", 522, 692, w_card, h_card, re);       card_3_4->move(150,y);
    Card *card_3_5  = new Card(this); card_3_5->create_card("3_5", 690, 692, w_card, h_card, re);       card_3_5->move(200,y);
    Card *card_3_6  = new Card(this); card_3_6->create_card("3_6", 870, 692, w_card, h_card, re);       card_3_6->move(250,y);
    Card *card_3_7  = new Card(this); card_3_7->create_card("3_7", 1055, 692, w_card, h_card, re);      card_3_7->move(300,y);
    Card *card_3_8  = new Card(this); card_3_8->create_card("3_8", 1225, 692, w_card, h_card, re);      card_3_8->move(350,y);
    Card *card_3_9  = new Card(this); card_3_9->create_card("3_9", 1400, 692, w_card, h_card, re);      card_3_9->move(400,y);
    Card *card_3_10 = new Card(this); card_3_10->create_card("3_10", 1928, 932, w_card, h_card, re);    card_3_10->move(450,y);
    Card *card_3_V  = new Card(this); card_3_V->create_card("3_V", 1592, 692, w_card, h_card, re);      card_3_V->move(500,y);
    Card *card_3_D  = new Card(this); card_3_D->create_card("3_D", 1764, 692, w_card, h_card, re);      card_3_D->move(550,y);
    Card *card_3_K  = new Card(this); card_3_K->create_card("3_K", 1946, 692, w_card, h_card, re);      card_3_K->move(600,y);

}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->addItem(TEST_0_TEXT, QVariant(Qt::UserRole + TEST_0));
    cb_test->addItem(TEST_1_TEXT, QVariant(Qt::UserRole + TEST_1));
    cb_test->addItem(TEST_2_TEXT, QVariant(Qt::UserRole + TEST_2));
    cb_test->addItem(TEST_3_TEXT, QVariant(Qt::UserRole + TEST_3));

    testbar->addWidget(cb_test);
    QToolButton *btn_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    switch (cmd)
    {
    case TEST_0: test_0(); break;
    case TEST_1: test_1(); break;
    case TEST_2: test_2(); break;
    case TEST_3: test_3(); break;
    default: break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::show_node(QDomNode node, QString space)
{
    if(node.isNull())   return;

    QDomElement e = node.toElement();
    if(!e.isNull())
    {
        if(e.tagName() == "g")
        {
            cnt++;
        }
        emit info(QString("%1%2").arg(space).arg(e.tagName()));
        space.append("\t");

        QDomNode child = e.firstChild();
        while(!child.isNull())
        {
            show_node(child, space);
            child = child.nextSibling();
        }
    }
}

void MainBox::test_0(void)
{
    emit info("Test_0()");

    QProcess proc(this);
    QStringList args;
    args << "-c" << "Ancient_Egyptians.svgz";
    proc.start("gunzip",args);
    proc.waitForFinished();

    QDomDocument dom;
    bool ok = dom.setContent(&proc);
    if(ok == false)
    {
        emit error("FAIL");
        return;
    }

    QDomElement docElem = dom.documentElement();

    QDomNode n = docElem.firstChild();
    show_node(n, "");

    emit info(QString("Found %1").arg(cnt));

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");

    QSvgRenderer *re =  new QSvgRenderer(QString(":/cards/Ancient_Egyptians.svgz"));
    if(re == nullptr)
    {
        return;
    }

    QRectF r;

    for(int n=0; n<10; n++)
    {
        r = re->boundsOnElement("rect");
        emit info(QString("%1").arg(r.x()));
        emit info(QString("%1").arg(r.y()));
        emit info(QString("%1").arg(r.width()));
        emit info(QString("%1").arg(r.height()));

        emit info("---");
    }
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");

    QProcess proc(this);
    QStringList args;
    args << "-c" << "Ancient_Egyptians.svgz";
    proc.start("gunzip",args);
    proc.waitForFinished();

    QDomDocument doc;
    bool ok = doc.setContent(&proc);
    if(ok == false)
    {
        emit error("FAIL");
        return;
    }

    QDomNodeList gList = doc.elementsByTagName("g");
    // Начинаем их перебирать
    int cnt = 0;
    for (int i = 0; i < gList.size(); i++)
    {
        QDomNode gNode = gList.item(i);     // Выделяем из списка ноду
        QDomElement rectangle = gNode.firstChildElement("rect");    // И ищем в ней элемент c тегом rect
        if (rectangle.isNull() == false)
        {
            cnt++;
        }
    }
    emit info(QString("gList.size() %1").arg(gList.size()));
    emit info(QString("cnt %1").arg(cnt));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");
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
void MainBox::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
#ifdef Q_DEBUG
    qDebug() << "MainBox::closeEvent";
#endif
}
//--------------------------------------------------------------------------------
