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
#include "myfiledialog.hpp"
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

    ui->sb_width->setRange(100, 10000);
    ui->sb_width->setObjectName("sb_width");
    connect(ui->sb_width,   SIGNAL(valueChanged(int)),  this,   SLOT(set_pic_width(int)));
    set_pic_width(ui->sb_width->value());

    ui->sb_height->setRange(100, 10000);
    ui->sb_height->setObjectName("sb_height");
    connect(ui->sb_height,  SIGNAL(valueChanged(int)),  this,   SLOT(set_pic_height(int)));
    set_pic_height(ui->sb_height->value());

    connect(ui->btn_create_orig_image,  SIGNAL(clicked(bool)),  this,   SLOT(s_create_orig_image()));
    connect(ui->btn_load_orig_image,    SIGNAL(clicked(bool)),  this,   SLOT(s_load_orig_image()));
    connect(ui->btn_show_orig_image,    SIGNAL(clicked(bool)),  this,   SLOT(s_show_orig_image()));

    connect(ui->btn_create_new_image,   SIGNAL(clicked(bool)),  this,   SLOT(s_create_new_image()));
    connect(ui->btn_show_new_image,     SIGNAL(clicked(bool)),  this,   SLOT(s_show_new_image()));

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(f_test()));

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
    emit info("Test_0()");

#if 0
    if(orig_image == nullptr)
    {
        emit error("orig_image not created");
        return false;
    }

    qreal BIG_R = pic_width / 2.0;
    qreal SMALL_R = (qreal)(pic_width / 8.0);

    qreal len_big_circle   = M_PI * (qreal)pic_width;
    qreal len_small_circle = M_PI * 2.0 * SMALL_R;

    emit info(QString("Длина большой окружности %1 pix").arg(len_big_circle));
    emit info(QString("Длина малой окружности %1 pix").arg(len_small_circle));
    emit info(QString("Отношения длин %1").arg(len_big_circle / len_small_circle));
    emit info(QString("pict_point %1").arg(pic_width * pic_height, 0, 'f', 0));
    emit info(QString("cnt_sin  %1").arg(cnt_sin));
    emit info(QString("cnt_cos  %1").arg(cnt_cos));
    emit info(QString("cnt_point  %1").arg(cnt_sin + cnt_cos));

    qreal big_square   = M_PI * BIG_R * BIG_R;
    qreal small_square = M_PI * SMALL_R * SMALL_R;
    emit info(QString("Эффективная площадь %1").arg(big_square - small_square, 0, 'f', 2));

    int stride = orig_image->bytesPerLine();
    emit info(QString("stride %1").arg(stride));
#endif

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
void MainBox::set_pic_width(int value)
{
    pic_width = value;
}
//--------------------------------------------------------------------------------
void MainBox::set_pic_height(int value)
{
    pic_height = value;
}
//--------------------------------------------------------------------------------
void MainBox::f_test(void)
{
#if 1
    int pic_width  = 800;
    int pic_height = 600;
#else
    int pic_width  = 3464;
    int pic_height = 3464;
#endif

    //---
    QImage *orig_image = new QImage(pic_width + 1,
                                    pic_height + 1,
                                    QImage::Format_RGB32);
    QPainter p(orig_image);
    QPointF center;
    center.setX(210);
    center.setY(pic_height / 2);

    QPointF center2;
    center2.setX(pic_width - 110);
    center2.setY(pic_height / 2);

    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    p.drawEllipse(center,  200, 200);
    p.drawEllipse(center2, 100, 100);
    //---
    QImage *new_image = new QImage(pic_height + 1,
                                   pic_width + 1,
                                   QImage::Format_RGB32);
    //---
    QTime timer;
    //---
#if 0
    timer.start();
    for(int y=0; y<pic_height; y++)
    {
        for(int x=0; x<pic_width; x++)
        {
            QRgb rgb = orig_image->pixel(x, y);
            new_image->setPixel(y, x, rgb);
        }
    }
    emit info(QString("time elapsed (soft) %1").arg(timer.elapsed()));
#endif
    //---
#if 0
    qreal t_x = 0;
    qreal t_y = 0;

    timer.start();
    for(int y=0; y<pic_height; y++)
    {
        for(int x=0; x<pic_width; x++)
        {
            qreal angle = x;
            t_x = qCos(qDegreesToRadians(angle));
            t_y = qSin(qDegreesToRadians(angle));

            QRgb rgb = orig_image->pixel(x, y);
            new_image->setPixel(y, x, rgb);
        }
    }
    emit info(QString("time elapsed (cos/sin) %1").arg(timer.elapsed()));
    emit debug(QString("%1 %2")
               .arg(t_x)
               .arg(t_y));
    new_image->fill(Qt::red);
#endif
    //---
#if 1
    //FAST
    quint8 const* orig_line = orig_image->constScanLine(0);
    int orig_stride = orig_image->bytesPerLine();

    quint8 * new_line = new_image->scanLine(0);
    int new_stride = new_image->bytesPerLine();

    timer.start();
    for(int y=0; y<pic_height; y++)
    {
        for(int x=0; x<pic_width; x++)
        {
            new_line[x*new_stride + y*4]     = orig_line[y*orig_stride + x*4];
            new_line[x*new_stride + y*4 + 1] = orig_line[y*orig_stride + x*4 + 1];
            new_line[x*new_stride + y*4 + 2] = orig_line[y*orig_stride + x*4 + 2];
            new_line[x*new_stride + y*4 + 3] = orig_line[y*orig_stride + x*4 + 3];
        }
    }
    emit info(QString("time elapsed (fast) %1").arg(timer.elapsed()));
#endif
    //---
    emit debug(QString("orig_stride %1").arg(orig_stride));
    emit debug(QString("new_stride  %1").arg(new_stride));
    //---

#if 1
    QLabel *label = new QLabel;
    label->setFixedSize(new_image->width(),
                        new_image->height());
    label->setPixmap(QPixmap::fromImage(*new_image));
    label->show();

    QScrollArea *area = new QScrollArea;
    area->setWidget(label);

    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(area);
    widget->setLayout(vbox);
    widget->show();
#endif
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

    QScrollArea *area = new QScrollArea;
    area->setWidget(label);

    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(area);
    widget->setLayout(vbox);
    widget->show();
}
//--------------------------------------------------------------------------------
bool MainBox::s_load_orig_image(void)
{
    emit trace(Q_FUNC_INFO);
    if(orig_image)
    {
        delete orig_image;
        orig_image = 0;
    }

    QString filename;
    MyFileDialog *dlg = 0;

    dlg = new MyFileDialog("Image", "image_dialog");
    dlg->setNameFilter(tr("Image Files (*.png *.jpg *.bmp)"));
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }

    if(filename.isEmpty())
    {
        return false;
    }
#ifndef Q_OS_LINUX
    filename.replace('/', '\\');
#endif

    emit debug(filename);

    orig_image = new QImage();
    bool ok = orig_image->load(filename);
    if(ok == false)
    {
        emit error(QString("cannot load [%1]").arg(filename));
        return false;
    }
    //---
    emit info(QString("width  %1").arg(orig_image->width()));
    emit info(QString("height %1").arg(orig_image->height()));
    emit info("OK");

    //---
    if(ui->cb_auto_show_orig_image->isChecked())
    {
        s_show_orig_image();
    }
    //---
    return true;
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
    orig_image = new QImage(pic_width + 1, pic_height + 1, QImage::Format_RGB32);
    if(orig_image == nullptr)
    {
        emit error("cannot create orig_image");
        return false;
    }
    block_this_button(true);

    qreal kx = pic_width / pic_height;

    qreal BIG_R_X = pic_width / 2;
    qreal BIG_R_Y = pic_height / 2;
    qreal SMALL_R_X = pic_width / 8.0;
    qreal SMALL_R_Y = pic_height / 8.0;

    QPointF center;
    center.setX(pic_width / 2);
    center.setY(pic_height / 2);

    QPainter p(orig_image);

    p.fillRect(0,
               0,
               orig_image->width(),
               orig_image->height(),
               QBrush(Qt::black));

    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    p.drawEllipse(center, BIG_R_X, BIG_R_Y);

    p.drawEllipse(center, SMALL_R_X, SMALL_R_Y);

    //---
    qreal radius_w = (BIG_R_X - SMALL_R_X) / 2  + SMALL_R_X;
    qreal radius_h = (BIG_R_Y - SMALL_R_Y) / 2  + SMALL_R_Y;
    p.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    p.drawEllipse(center, radius_w, radius_h);
    p.setPen(QPen(Qt::red, 1, Qt::SolidLine));

    emit info(QString("kx %1").arg(kx));
    emit info(QString("BIG_R_X %1").arg(BIG_R_X));
    emit info(QString("BIG_R_Y %1").arg(BIG_R_Y));
    emit info(QString("SMALL_R_X %1").arg(SMALL_R_X));
    emit info(QString("SMALL_R_Y %1").arg(SMALL_R_Y));
    emit info(QString("pic_width  %1").arg(pic_width));
    emit info(QString("pic_height %1").arg(pic_height));
    emit info(QString("radius_w   %1").arg(radius_w));
    emit info(QString("radius_h   %1").arg(radius_h));
    //---

#if 1
    qreal x1 = (pic_width - radius_w * 2.0) / 2;
    qreal y1 = (pic_height - radius_h * 2.0) / 2;
    qreal w = radius_w * 2.0;
    qreal h = radius_h * 2.0;
    p.drawRect(x1, y1, w, h);

    for(qreal a=0.0; a<360.0; a+=10.0)
    {
        qreal new_x = BIG_R_X * qCos(qDegreesToRadians(a));
        qreal new_y = BIG_R_Y * qSin(qDegreesToRadians(a));
        p.drawLine(center.x(),
                   center.y(),
                   center.x() + new_x,
                   center.y() + new_y);
    }
#endif

#if 1
    QPointF center_circle_1;
    QPointF center_circle_2;
    QPointF center_circle_3;
    QPointF center_circle_4;

    qreal angle = 45.0;
    center_circle_1.setX(center.x() + radius_w * qCos(qDegreesToRadians(angle)));
    center_circle_1.setY(center.y() + radius_h * qSin(qDegreesToRadians(angle)));
    angle += 90.0;
    center_circle_2.setX(center.x() + radius_w * qCos(qDegreesToRadians(angle)));
    center_circle_2.setY(center.y() - radius_h * qCos(qDegreesToRadians(angle)));
    angle += 90.0;
    center_circle_3.setX(center.x() + radius_w * qCos(qDegreesToRadians(angle)));
    center_circle_3.setY(center.y() + radius_h * qCos(qDegreesToRadians(angle)));
    angle = -45.0;
    center_circle_4.setX(center.x() + radius_w * qCos(qDegreesToRadians(angle)));
    center_circle_4.setY(center.y() - radius_h * qCos(qDegreesToRadians(angle)));

    qreal max_r = 0;
    if(pic_width > pic_height)
    {
        max_r = radius_h - SMALL_R_Y;
    }
    else
    {
        max_r = radius_w - SMALL_R_X;
    }
    for(qreal r=50; r<max_r; r+=((max_r - 50.0) / 10.0))   //FIXME
    {
        qreal rx = r;
        qreal ry = r / kx;
        p.drawEllipse(center_circle_1, rx, ry);
        p.drawEllipse(center_circle_2, rx, ry);
        p.drawEllipse(center_circle_3, rx, ry);
        p.drawEllipse(center_circle_4, rx, ry);
    }
#endif
    //---
    emit info("OK");
    //---
    if(ui->cb_auto_show_orig_image->isChecked())
    {
        s_show_orig_image();
    }
    //---
    block_this_button(false);
    return true;
}
//--------------------------------------------------------------------------------
#if 0
// QImage::Format_RGB32:
// QImage::Format_ARGB32:
// QImage::Format_ARGB32_Premultiplied:

// do smth within area [left;right) x [top; bottom)

quint8 const* line = img.constScanLine(r_top) + r_left *4;
int stride = img.bytesPerLine();

quint32 red = 0, green = 0, blue = 0;
for ( int y = top; y < bottom; ++y, line += stride )
{
    quint8 const* pix = line;
    for ( int x = left; x < right; ++x, pix += 4 )
    {
        blue  = pix[0];
        green = pix[1];
        red   = pix[2];
    }
}
#endif
//--------------------------------------------------------------------------------
//#define FAST_PIC
bool MainBox::s_create_new_image(void)
{
    emit trace(Q_FUNC_INFO);

    if(orig_image == nullptr)
    {
        emit error("orig_image is not created");
        return false;
    }

    block_this_button(true);

    qreal BIG_R_X = pic_width / 2;
    qreal BIG_R_Y = pic_height / 2;
    qreal SMALL_R_X = pic_width / 8.0;
    qreal SMALL_R_Y = pic_height / 8.0;

    qreal width = 0;
    qreal height = 0;
    qreal min_r = 0;
    qreal max_r = 0;
    if(pic_width > pic_height)
    {
        width = M_PI * pic_width;
        height = BIG_R_X - SMALL_R_X;
        min_r = SMALL_R_X;
        max_r = BIG_R_X;
    }
    else
    {
        width = M_PI * pic_height;
        height = BIG_R_Y - SMALL_R_Y;
        min_r = SMALL_R_Y;
        max_r = BIG_R_Y;
    }

    new_image = new QImage(width  + 1.0,
                           height + 1.0,
                           QImage::Format_RGB32);

    QPointF center;
    center.setX(pic_width  / 2.0);
    center.setY(pic_height / 2.0);

    QTime timer;
    timer.start();
    cnt_sin = 0;
    cnt_cos = 0;
    //---
#ifdef FAST_PIC
    quint8 const* orig_line = orig_image->constScanLine(0);
    int orig_stride = orig_image->bytesPerLine();
    quint8 const* new_line = new_image->constScanLine(0);
    int new_stride = new_image->bytesPerLine();

    quint32 red = 0, green = 0, blue = 0;
#endif
    //---
    for(qreal y=min_r; y<max_r; y++)
    {
        qreal small_width = M_PI * 2.0 * (qreal)y;
        qreal k = width / small_width;
        for(qreal x=0; x<small_width; x++)
        {
            qreal angle = 0;
            qreal kx = 0;
            qreal ky = 0;
            if(width > height)
            {
                angle = (k * x) * 360.0 / width;
                kx = pic_height / pic_width;
                ky = 1.0;
            }
            else
            {
                angle = (k * x) * 360.0 / height;
                kx = 1.0;
                ky = pic_width / pic_height;
            }
            //qreal kx = pic_width / pic_height;
            qreal res_x = qreal(y) * qCos(qDegreesToRadians(angle)) * kx;
            qreal res_y = qreal(y) * qSin(qDegreesToRadians(angle)) * ky;
            cnt_cos++;
            cnt_sin++;

            qreal t_x = center.x() + res_x;
            qreal t_y = center.y() + res_y;
            if((t_x < 0) || (t_x > pic_width))
            {
                emit error(QString("incorrect x %1").arg(t_x));
                emit error(QString("angle %1").arg(angle));
                block_this_button(false);
                return false;
            }
            if((t_y < 0) || (t_y > pic_height))
            {
                emit error(QString("incorrect y %1").arg(t_y));
                emit error(QString("angle %1").arg(angle));
                block_this_button(false);
                return false;
            }
#ifdef FAST_PIC
            // быстрая отрисовка
            quint8 const* orig_pix = orig_line;
            blue  = orig_pix[0];
            green = orig_pix[1];
            red   = orig_pix[2];
#else
            QRgb rgb = orig_image->pixel(t_x, t_y);
            new_image->setPixel(x * k,
                                y - min_r,
                                rgb);
#endif
        }
    }
    emit info(QString("time elapsed %1").arg(timer.elapsed()));
    emit info("OK");

    //---
    if(ui->cb_auto_show_new_image->isChecked())
    {
        s_show_new_image();
    }
    //---
    block_this_button(false);
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
