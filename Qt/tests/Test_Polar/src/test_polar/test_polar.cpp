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
#include <QtMath>
//--------------------------------------------------------------------------------
#include "ui_test_polar.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_polar.hpp"
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
    save_widgets("Polar");
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    ui->sb_len_side->setRange(100, 10000);
    ui->sb_len_side->setObjectName("sb_len_side");
    connect(ui->sb_len_side,    SIGNAL(valueChanged(int)), this,    SLOT(set_len_side(int)));
    set_len_side(ui->sb_len_side->value());

    connect(ui->btn_create_orig_image,  SIGNAL(clicked(bool)),  this,   SLOT(s_create_orig_image()));
    connect(ui->btn_create_new_image,   SIGNAL(clicked(bool)),  this,   SLOT(s_create_new_image()));
    connect(ui->btn_show_orig_image,    SIGNAL(clicked(bool)),  this,   SLOT(s_show_orig_image()));
    connect(ui->btn_show_new_image,     SIGNAL(clicked(bool)),  this,   SLOT(s_show_new_image()));

    load_widgets("Polar");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
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
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
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
bool MainBox::test_0(void)
{
    emit info("Test_1()");

    qreal len_big_circle   = M_PI * (double)LEN_SIDE;
    qreal len_small_circle = M_PI * 2.0 * SMALL_R;

    emit info(QString("Длина большой окружности %1 pix").arg(len_big_circle));
    emit info(QString("Длина малой окружности %1 pix").arg(len_small_circle));
    emit info(QString("Отношения длин %1").arg(len_big_circle / len_small_circle));
    emit info(QString("pict_point %1").arg(LEN_SIDE * LEN_SIDE, 0, 'f', 0));
    emit info(QString("cnt_point  %1").arg(cnt_point));

    qreal big_square   = M_PI * (LEN_SIDE / 2) * (LEN_SIDE / 2);
    qreal small_square = M_PI * SMALL_R * SMALL_R;
    emit info(QString("Эффективная площадь %1").arg(big_square - small_square));

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_0()");

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
void MainBox::set_len_side(int value)
{
    LEN_SIDE = value;
}
//--------------------------------------------------------------------------------
void MainBox::s_show_orig_image(void)
{
    if(orig_image == nullptr)
    {
        emit error("cannot create orig_image");
        return;
    }
    QLabel *label = new QLabel;
    label->setFixedSize(orig_image->width(),
                        orig_image->height());
    label->setPixmap(QPixmap::fromImage(*orig_image));
    label->show();
}
//--------------------------------------------------------------------------------
void MainBox::s_show_new_image(void)
{
    if(new_image == nullptr)
    {
        emit error("cannot create new_image");
        return;
    }
    QLabel *label = new QLabel;
    label->setFixedSize(new_image->width(),
                        new_image->height());
    label->setPixmap(QPixmap::fromImage(*new_image));
    label->show();
}
//--------------------------------------------------------------------------------
bool MainBox::s_create_orig_image(void)
{
    emit trace(Q_FUNC_INFO);
    if(orig_image)
    {
        delete orig_image;
        orig_image = 0;
    }
    //---
    orig_image = new QImage(LEN_SIDE + 1, LEN_SIDE + 1, QImage::Format_RGB32);
    if(orig_image == nullptr)
    {
        emit error("cannot create orig_image");
        return false;
    }
    QPointF center;
    center.setX(LEN_SIDE / 2);
    center.setY(LEN_SIDE / 2);

    QPainter p(orig_image);

    p.fillRect(0,
               0,
               orig_image->width(),
               orig_image->height(),
               QBrush(Qt::black));

    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    p.drawEllipse(center, LEN_SIDE / 2, LEN_SIDE / 2);

    p.drawEllipse(center, SMALL_R, SMALL_R);

    //---
    qreal radius = ((LEN_SIDE / 2) - SMALL_R) / 2 + SMALL_R;
    p.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    p.drawEllipse(center, radius, radius);
    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    emit info(QString("radius %1").arg(radius));
    //---

    qreal x1 = (LEN_SIDE - radius * 2.0) / 2;// LEN_SIDE / 4;
    qreal y1 = (LEN_SIDE - radius * 2.0) / 2;// LEN_SIDE / 4;
    qreal w = radius * 2.0;
    qreal h = radius * 2.0;
    p.drawRect(x1, y1, w, h);
    p.drawRect(x1+10, y1+10, w-20, h-20);

    for(qreal a=0.0; a<360.0; a+=10.0)
    {
        qreal new_x = center.x() + (LEN_SIDE / 2) * qCos(qDegreesToRadians(a));
        qreal new_y = center.x() + (LEN_SIDE / 2) * qSin(qDegreesToRadians(a));
        p.drawLine(center.x(),
                   center.y(),
                   new_x,
                   new_y);
    }

    QPointF center_circle_1;
    QPointF center_circle_2;
    QPointF center_circle_3;
    QPointF center_circle_4;

    qreal angle = 45.0;
    center_circle_1.setX(center.x() + radius * qCos(qDegreesToRadians(angle)));
    center_circle_1.setY(center.y() + radius * qSin(qDegreesToRadians(angle)));
    angle += 90.0;
    center_circle_2.setX(center.x() + radius * qCos(qDegreesToRadians(angle)));
    center_circle_2.setY(center.y() - radius * qCos(qDegreesToRadians(angle)));
    angle += 90.0;
    center_circle_3.setX(center.x() + radius * qCos(qDegreesToRadians(angle)));
    center_circle_3.setY(center.y() + radius * qCos(qDegreesToRadians(angle)));
    angle = -45.0;
    center_circle_4.setX(center.x() + radius * qCos(qDegreesToRadians(angle)));
    center_circle_4.setY(center.y() - radius * qCos(qDegreesToRadians(angle)));

    for(int r=50; r<(radius - SMALL_R); r+=10)
    {
        p.drawEllipse(center_circle_1, r, r);
        p.drawEllipse(center_circle_2, r, r);
        p.drawEllipse(center_circle_3, r, r);
        p.drawEllipse(center_circle_4, r, r);
    }
    //---

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::s_create_new_image(void)
{
    emit trace(Q_FUNC_INFO);

    if(orig_image == nullptr)
    {
        emit error("orig_image is not created");
        return false;
    }

    qreal min_r = SMALL_R;
    qreal max_r = (LEN_SIDE / 2);

    qreal height = max_r - min_r;
    qreal width  = M_PI * (double)LEN_SIDE;

    emit debug(QString("min_r %1").arg(min_r));
    emit debug(QString("max_r %1").arg(max_r));
    emit debug(QString("width %1").arg(width));
    emit debug(QString("height %1").arg(height));

    new_image = new QImage(width + 1.0, height + 1.0, QImage::Format_RGB32);
    //res_image->fill(Qt::white);

    QPointF center;
    center.setX(LEN_SIDE / 2);
    center.setY(LEN_SIDE / 2);

    cnt_point = 0;
    for(qreal y=min_r; y<max_r; y++)
    {
        qreal small_width = M_PI * 2.0 * (double)y;
        qreal k = width / small_width;
        for(qreal x=0; x<small_width; x++)
        {
            qreal angle = qreal(k * x) * 360.0 / qreal(width);
            qreal res_x = qreal(y) * qCos(qDegreesToRadians(angle));
            qreal res_y = qreal(y) * qSin(qDegreesToRadians(angle));
            QRgb rgb = orig_image->pixel(center.x() + res_x,
                                         center.y() + res_y);
            new_image->setPixel(x * k,
                                y - min_r,
                                rgb);
            cnt_point++;
        }
    }

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
