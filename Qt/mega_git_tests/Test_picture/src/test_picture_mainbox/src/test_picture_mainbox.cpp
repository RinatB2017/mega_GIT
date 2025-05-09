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
#include "ui_test_picture_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_picture_mainbox.hpp"
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

    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_all_ON(false);
    ui->grapher_widget->set_visible_btn_all_OFF(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);
    ui->grapher_widget->set_visible_btn_Options(false);

    ui->sb_R_min->setRange(0, 359);
    ui->sb_R_max->setRange(0, 359);
    ui->sb_G_min->setRange(0, 359);
    ui->sb_G_max->setRange(0, 359);
    ui->sb_B_min->setRange(0, 359);
    ui->sb_B_max->setRange(0, 359);

    //TODO находим и НЕ показываем лишнее
    Color_widget *widget = findChild<Color_widget *>();
    if(widget)
    {
        widget->setVisible(false);
    }
    //---

    grapher_curve = ui->grapher_widget->add_curve("data");
    emit debug(QString("grapher_curve %1").arg(grapher_curve));

    connect(ui->btn_load_picture,   &QPushButton::clicked,  this,   static_cast<void (MainBox::*)(void)>(&MainBox::load_picture));
    connect(ui->btn_find_max_color, &QPushButton::clicked,  this,   &MainBox::find_max_color);
    connect(ui->btn_redraw_picture, &QPushButton::clicked,  this,   &MainBox::redraw_picture);

    connect(ui->btn_show_only_R,    &QPushButton::clicked,  this,   &MainBox::show_only_R);
    connect(ui->btn_show_only_G,    &QPushButton::clicked,  this,   &MainBox::show_only_G);
    connect(ui->btn_show_only_B,    &QPushButton::clicked,  this,   &MainBox::show_only_B);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test", &MainBox::test });

        QToolBar *testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_block = new QCheckBox("block", this);
        testbar->addWidget(cb_block);

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

        connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
        connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
    }
    else
    {
        emit error("mw not found!");
    }
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
bool MainBox::load_picture(QString fileName)
{
    if(fileName.isEmpty())
    {
        emit error("fileName is empty");
        return false;
    }

    emit info(QString("load %1").arg(fileName));
    bool ok = picture.load(fileName);
    if(!ok)
    {
        emit error("can't load picture");
        return false;
    }

    ui->orig_picture->setPixmap(picture);

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_picture(void)
{
    QFileDialog *dlg;
    QString filename;

    dlg = new QFileDialog;
    dlg->setNameFilter("Image Files (*.png *.jpg *.jpeg *.bmp)");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    delete dlg;

    if(filename.isEmpty())
    {
        return;
    }
    load_picture(filename);
}
//--------------------------------------------------------------------------------
void MainBox::find_max_color(void)
{
    if(picture.isNull())
    {
        emit error("picture is null!");
        return;
    }

    int max_x = picture.width();
    int max_y = picture.height();
    int max_cnt = max_x * max_y;
    int cnt = 0;
    int max_green = 0;
    max_color = 0;

    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, max_cnt);
    pd->setWindowModality(Qt::WindowModal);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            pd->setValue(cnt++);
            if(pd->wasCanceled())
            {
                break;
            }

            QRgb color = picture.toImage().pixel(x, y);
            if(qGreen(color) > max_green)
            {
                max_color = color;
            }

            int index = qGreen(color);
            if(index <= 0xFF)
            {
                grapher_data[index]++;
            }
        }
    }
    pd->close();
    delete pd;

    for(int n=0; n<0xFF; n++)
    {
        ui->grapher_widget->add_curve_data(grapher_curve, grapher_data[n]);
    }

    emit info(QString("R %1").arg(qRed(max_color)));
    emit info(QString("G %1").arg(qGreen(max_color)));
    emit info(QString("B %1").arg(qBlue(max_color)));
}
//--------------------------------------------------------------------------------
void MainBox::redraw_picture(void)
{
    if(picture.isNull())
    {
        emit error("picture is null!");
        return;
    }

    int max_x = picture.width();
    int max_y = picture.height();
    int max_cnt = max_x * max_y;
    int cnt = 0;

    QImage tmp = picture.toImage();

    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, max_cnt);
    pd->setWindowModality(Qt::WindowModal);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            pd->setValue(cnt++);
            if(pd->wasCanceled())
            {
                break;
            }

            QRgb color = picture.toImage().pixel(x, y);
            if(color != max_color)
            {
                tmp.setPixelColor(x, y, Qt::white);
            }
            else
            {
                tmp.setPixel(x, y, color);
            }
        }
    }
    pd->close();
    delete pd;

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
void MainBox::show_only_color(QSpinBox *min_value, QSpinBox *max_value)
{
    if(picture.isNull())
    {
        emit error("picture is null!");
        return;
    }

    int max_x = picture.width();
    int max_y = picture.height();
    int max_cnt = max_x * max_y;
    int cnt = 0;

    QImage tmp = picture.toImage();

    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, max_cnt);
    pd->setWindowModality(Qt::WindowModal);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            pd->setValue(cnt++);
            if(pd->wasCanceled())
            {
                break;
            }

            QRgb color = picture.toImage().pixel(x, y);

#if 1
            int h;
            int s;
            int v;
            QColor t_color = color;
            t_color.getHsv(&h, &s, &v);
            if((h>=min_value->value()) && (h<=max_value->value()))
            {
                v = 0;
            }
            tmp.setPixelColor(x, y, QColor::fromHsv(h, s, v));
#else
            int color_R = qRed(color);
            int color_G = qGreen(color);
            int color_B = qBlue(color);
            if((color_R > color_G) && (color_R > color_B))
            {
                tmp.setPixel(x, y, color);
            }
            else
            {
                tmp.setPixelColor(x, y, Qt::white);
            }
#endif
        }
    }
    pd->close();
    delete pd;

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
void MainBox::show_only_R(void)
{
    //show_only_color(ui->sb_R_min, ui->sb_R_max);

    if(picture.isNull())
    {
        emit error("picture is null!");
        return;
    }

    int max_x = picture.width();
    int max_y = picture.height();
    int max_cnt = max_x * max_y;
    int cnt = 0;

    QImage tmp = picture.toImage();

    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, max_cnt);
    pd->setWindowModality(Qt::WindowModal);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            pd->setValue(cnt++);
            if(pd->wasCanceled())
            {
                break;
            }

            QRgb color = picture.toImage().pixel(x, y);

#if 1
            int h;
            int s;
            int v;
            QColor t_color = color;
            t_color.getHsv(&h, &s, &v);
            if((h>=ui->sb_R_min->value()) && (h<=ui->sb_R_max->value()))
            {
                v = 0;
            }
            tmp.setPixelColor(x, y, QColor::fromHsv(h, s, v));
#else
            int color_R = qRed(color);
            int color_G = qGreen(color);
            int color_B = qBlue(color);
            if((color_R > color_G) && (color_R > color_B))
            {
                tmp.setPixel(x, y, color);
            }
            else
            {
                tmp.setPixelColor(x, y, Qt::white);
            }
#endif
        }
    }
    pd->close();
    delete pd;

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
void MainBox::show_only_G(void)
{
    if(picture.isNull())
    {
        emit error("picture is null!");
        return;
    }

    int max_x = picture.width();
    int max_y = picture.height();
    int max_cnt = max_x * max_y;
    int cnt = 0;

    QImage tmp = picture.toImage();

    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, max_cnt);
    pd->setWindowModality(Qt::WindowModal);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            pd->setValue(cnt++);
            if(pd->wasCanceled())
            {
                break;
            }

            QRgb color = picture.toImage().pixel(x, y);

#if 1
            int h;
            int s;
            int v;
            QColor t_color = color;
            t_color.getHsv(&h, &s, &v);
            if((h>=ui->sb_G_min->value()) && (h<=ui->sb_G_max->value()))
            {
                v = 0;
            }
            tmp.setPixelColor(x, y, QColor::fromHsv(h, s, v));
#else
            int color_R = qRed(color);
            int color_G = qGreen(color);
            int color_B = qBlue(color);
            if((color_G > color_R) && (color_G > color_B))
            {
                tmp.setPixel(x, y, color);
            }
            else
            {
                tmp.setPixelColor(x, y, Qt::white);
            }
#endif
        }
    }
    pd->close();
    delete pd;

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
void MainBox::show_only_B(void)
{
    if(picture.isNull())
    {
        emit error("picture is null!");
        return;
    }

    int max_x = picture.width();
    int max_y = picture.height();
    int max_cnt = max_x * max_y;
    int cnt = 0;

    QImage tmp = picture.toImage();

    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, max_cnt);
    pd->setWindowModality(Qt::WindowModal);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            pd->setValue(cnt++);
            if(pd->wasCanceled())
            {
                break;
            }

            QRgb color = picture.toImage().pixel(x, y);

#if 1
            int h;
            int s;
            int v;
            QColor t_color = color;
            t_color.getHsv(&h, &s, &v);
            if((h>=ui->sb_B_min->value()) && (h<=ui->sb_B_max->value()))
            {
                v = 0;
            }
            tmp.setPixelColor(x, y, QColor::fromHsv(h, s, v));
#else
            int color_R = qRed(color);
            int color_G = qGreen(color);
            int color_B = qBlue(color);
            if((color_B > color_R) && (color_B > color_G))
            {
                tmp.setPixel(x, y, color);
            }
            else
            {
                tmp.setPixelColor(x, y, Qt::white);
            }
#endif
        }
    }
    pd->close();
    delete pd;

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
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
