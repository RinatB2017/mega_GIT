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
#include "ui_test_pixmap_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "qxmlputget.h"
#include "test_pixmap_mainbox.hpp"
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

    createTestBar();

    load_images();
    load_labirint();

    setFocusPolicy(Qt::StrongFocus);
    //setFixedSize(640, 480);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "choice_test",
                                       "choice_test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    //block_this_button(true);
    emit info("Test()");
}
//--------------------------------------------------------------------------------
bool MainBox::get_int(QXmlGet *xml, QString name, int *data)
{
    bool ok = xml->find(name);
    if(!ok)
    {
        emit error(QString("get_int(%1):find").arg(name));
        return false;
    }

    int temp = xml->getInt(-1);
    if(temp<0)
    {
        emit error(QString("get_int(%1):getInt < 0").arg(name));
        return false;
    }
    *data = temp;
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::load_wall(QPixmap *pixmap)
{
    QPixmap temp;
    bool ok = temp.load(":/data/wall.png");
    if(!ok)
    {
        emit error("wall not loaded!");
        return ok;
    }
    temp  = temp.scaled(48, 48);
    *pixmap = temp;
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::load_field(QPixmap *pixmap)
{
    QPixmap temp;
    bool ok = temp.load(":/data/field.png");
    if(!ok)
    {
        emit error("field not loaded!");
        return ok;
    }
    temp  = temp.scaled(48, 48);
    *pixmap = temp;
    return ok;
}
//--------------------------------------------------------------------------------
void MainBox::load_labirint(void)
{
    QXmlGet *xmlGet = new QXmlGet();
    bool ok = xmlGet->load(":/data/labirint.dat");
    if(!ok)
    {
        emit error("xmlGet->load");
        return;
    }

    int w = 0;
    int h = 0;
    QPixmap wall;
    QPixmap field;

    ok = get_int(xmlGet, "width", &w);
    if(!ok) return;
    ok = get_int(xmlGet, "height", &h);
    if(!ok) return;

    ok = load_wall(&wall);
    if(!ok) return;
    ok = load_field(&field);
    if(!ok) return;

    int map_w = w*wall.width();
    int map_h = h*wall.height();

    QPixmap temp(map_w, map_h);
    temp.fill(Qt::gray);
    QPainter t(&temp);

    int y = 0;
    ok = xmlGet->find("items");
    while(ok)
    {
        QString temp = xmlGet->getString("");
        QByteArray ba;

        QByteArray ta;
        ta.clear();
        ta.append(temp);

        ba = QByteArray::fromHex(ta);
        for(int x=0; x<ba.length(); x++)
        {
            if(ba.at(x) == 0xd)
            {
                t.drawPixmap(x*wall.width(), y, wall);
            }
            else
            {
                t.drawPixmap(x*wall.width(), y, field);
            }
        }

        y+=wall.height();
        ok = xmlGet->findNext("items");
    }

    p = temp.copy(0, 0, map_w, map_h);

    emit info(QString("width %1").arg(w));
    emit info(QString("height %1").arg(h));
    emit info(QString("map_w %1").arg(map_w));
    emit info(QString("map_h %1").arg(map_h));

    x = 0;
    y = 0;
    picture_width = p.width();
    picture_height = p.height();
    step = 10;
}
//--------------------------------------------------------------------------------
void MainBox::load_images(void)
{
    QPixmap comp;
    bool ok = comp.load(ICON_PROGRAMM);
    if(!ok)
    {
        emit error(QString("%1 not loaded!").arg(ICON_PROGRAMM));
        return;
    }

    int w = 2000;
    int h = 1000;

    QPixmap temp(w, h);
    temp.fill(Qt::gray);

    QPainter t(&temp);

    t.drawLine(0, 0, w, h);
    t.drawLine(0, h, w, 0);

    t.drawPixmap(0, 0, comp);
    t.drawPixmap(w-comp.width(), 0, comp);
    t.drawPixmap(0, h-comp.height(), comp);
    t.drawPixmap(w-comp.width(), h-comp.height(), comp);

    p = temp.copy(0, 0, w, h);

    x = 0;
    y = 0;
    picture_width = p.width();
    picture_height = p.height();
    step = 10;
}
//--------------------------------------------------------------------------------
void MainBox::move_left(void)
{
    if((x - step) < 0)
    {
        return;
    }

    x-=step;
    update();
}
//--------------------------------------------------------------------------------
void MainBox::move_right(void)
{
    if((x + step + width()) > picture_width)
    {
        return;
    }

    x+=step;
    update();
}
//--------------------------------------------------------------------------------
void MainBox::move_up(void)
{
    if((y - step) < 0)
    {
        return;
    }

    y-=step;
    update();
}
//--------------------------------------------------------------------------------
void MainBox::move_down(void)
{
    if((y + step + height()) > picture_height)
    {
        return;
    }

    y+=step;
    update();
}
//--------------------------------------------------------------------------------
#if 0
QPixmap pix(p.width()*3, p.height()*3);
pix.fill(Qt::blue);

int w = pix.width();
int h = pix.height();

QPainter t(&pix);
//t.drawLine(0, 0, p, h);
t.drawPixmap(0, 0, p);
t.drawPixmap(w-p.width(), 0, p);
t.drawPixmap(0, h-p.height(), p);
t.drawPixmap(w-p.width(), h-p.height(), p);

QPainter painter(this);
painter.drawPixmap(0, 0, p);
painter.drawPixmap(100, 0, pix);
//???
painter.drawPixmap(100, h, pix, 0, 0, w/2, h/2);
#endif
//--------------------------------------------------------------------------------
void MainBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:  move_left();    break;
    case Qt::Key_Right: move_right();   break;
    case Qt::Key_Down:  move_down();    break;
    case Qt::Key_Up:    move_up();      break;

    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //???
    painter.drawPixmap(0, 0, p, x, y, width(), height());

    Q_UNUSED(event)
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
