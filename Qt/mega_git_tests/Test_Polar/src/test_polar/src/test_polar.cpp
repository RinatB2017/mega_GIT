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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

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
    connect(ui->btn_create_new_image_2, SIGNAL(clicked(bool)),  this,   SLOT(s_create_new_image_2()));
    connect(ui->btn_show_new_image,     SIGNAL(clicked(bool)),  this,   SLOT(s_show_new_image()));

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(f_test()));
    connect(ui->btn_test_2, SIGNAL(clicked(bool)),  this,   SLOT(f_test_2()));

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

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
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
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
bool MainBox::test(void)
{
    emit info("Test");
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
#if 0
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
    QElapsedTimer timer;
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
    show_image(new_image, 800, 800);
#endif
}
//--------------------------------------------------------------------------------
void MainBox::f_test_2(void)
{
    emit trace(Q_FUNC_INFO);

#if 0
    //width  2513.27
    //height 300
#endif

    qreal src_width  = 2513;
    qreal src_height = 300;

    qreal dst_width  = 800;
    qreal dst_height = 800;

    //---
    QImage *src_image = new QImage(src_width + 1,
                                   src_height + 1,
                                   QImage::Format_RGB32);
    //src_image->fill(Qt::red);

    QPainter p(src_image);
    int pos_x = 57 + 150;
    p.setPen(QPen(Qt::white, 1, Qt::SolidLine));
    for(int n=0; n<8; n++)
    {
        QPoint center;
        center.setX(pos_x);
        center.setY(150);

        for(int i=10; i<150; i+=10)
        {
            p.drawEllipse(center, i, i);
        }
        pos_x += 300;
    }

    //show_image(src_image, 2500, 350);
    //---

    //---
    QImage *dst_image = new QImage(dst_width + 1,
                                   dst_height + 1,
                                   QImage::Format_RGB32);

    qreal begin_r = (dst_width / 2.0) - src_height;
    qreal end_r   = (dst_width / 2.0);

    qreal t_x = 0;
    qreal t_y = 0;

    QPointF center;
    center.setX(dst_width  / 2.0);
    center.setY(dst_height / 2.0);

    for(qreal radius = begin_r; radius < end_r; radius++)
    {
        qreal angle = 0.0;
        qreal temp_w = M_PI * 2.0 * radius;
        qreal k_angle = 360.0 / temp_w;
        qreal k_tx = src_width / temp_w;

        t_x = 0;
        int cnt = 0;
        while(angle < 360.0)
        {
            qreal x = radius * qCos(qDegreesToRadians(angle));
            qreal y = radius * qSin(qDegreesToRadians(angle));

            QRgb rgb = src_image->pixel(t_x, t_y);
            dst_image->setPixel(center.x() + x,
                                center.y() + y,
                                rgb);

            t_x += k_tx;
            angle += k_angle;
            cnt++;
        }
        t_y++;
    }

    show_image(dst_image, 850, 850);
    //---
}
//--------------------------------------------------------------------------------
void MainBox::show_image(QImage *image,
                         int min_x,
                         int min_y)
{
    QLabel *label = new QLabel;
    label->setFixedSize(image->width(),
                        image->height());
    label->setPixmap(QPixmap::fromImage(*image));
    label->show();

    QScrollArea *area = new QScrollArea;
    area->setWidget(label);

    QWidget *widget = new QWidget;
    widget->setMinimumSize(min_x, min_y);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(area);
    widget->setLayout(vbox);
    widget->show();
}
//--------------------------------------------------------------------------------
void MainBox::s_show_orig_image(void)
{
    show_image(orig_image, 800, 800);
}
//--------------------------------------------------------------------------------
void MainBox::s_show_new_image(void)
{
    show_image(new_image, 800, 800);
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

    emit info(QString("width  %1").arg(width));
    emit info(QString("height %1").arg(height));

    new_image = new QImage(width  + 1.0,
                           height + 1.0,
                           QImage::Format_RGB32);

    QPointF center;
    center.setX(pic_width  / 2.0);
    center.setY(pic_height / 2.0);

    QElapsedTimer timer;
    timer.start();
    cnt_sin = 0;
    cnt_cos = 0;
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
            QRgb rgb = orig_image->pixel(t_x, t_y);
            new_image->setPixel(x * k,
                                y - min_r,
                                rgb);
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
bool MainBox::s_create_new_image_2(void)
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

    emit info(QString("width  %1").arg(width));
    emit info(QString("height %1").arg(height));

    new_image = new QImage(width  + 1.0,
                           height + 1.0,
                           QImage::Format_RGB32);

    QPointF center;
    center.setX(pic_width  / 2.0);
    center.setY(pic_height / 2.0);

    QElapsedTimer timer;
    timer.start();
    cnt_sin = 0;
    cnt_cos = 0;
    //---
#ifdef FAST_PIC
    quint8 const* orig_line = orig_image->constScanLine(0);
    //int orig_stride = orig_image->bytesPerLine();
    quint8 * new_line = new_image->scanLine(0);
    //int new_stride = new_image->bytesPerLine();

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
//            quint8 const* orig_pix = orig_line;
//            blue  = orig_pix[0];
//            green = orig_pix[1];
//            red   = orig_pix[2];

//            quint8 * new_pix = new_line;
//            new_pix[0] = blue;
//            new_pix[1] = green;
//            new_pix[2] = red;

            quint8 const* orig_line = orig_image->constScanLine(0);
            int orig_stride = orig_image->bytesPerLine();

            quint8 * new_line = new_image->scanLine(0);
            int new_stride = new_image->bytesPerLine();

//            for(int y=0; y<pic_height; y++)
//            {
//                for(int x=0; x<pic_width; x++)
//                {
//                    new_line[x*new_stride + y*4]     = orig_line[y*orig_stride + x*4];
//                    new_line[x*new_stride + y*4 + 1] = orig_line[y*orig_stride + x*4 + 1];
//                    new_line[x*new_stride + y*4 + 2] = orig_line[y*orig_stride + x*4 + 2];
//                    new_line[x*new_stride + y*4 + 3] = orig_line[y*orig_stride + x*4 + 3];
//                }
//            }

            new_line[(int)x*new_stride + (int)y*4]     = orig_line[(int)y*orig_stride + (int)x*4];
            new_line[(int)x*new_stride + (int)y*4 + 1] = orig_line[(int)y*orig_stride + (int)x*4 + 1];
            new_line[(int)x*new_stride + (int)y*4 + 2] = orig_line[(int)y*orig_stride + (int)x*4 + 2];
            new_line[(int)x*new_stride + (int)y*4 + 3] = orig_line[(int)y*orig_stride + (int)x*4 + 3];

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
