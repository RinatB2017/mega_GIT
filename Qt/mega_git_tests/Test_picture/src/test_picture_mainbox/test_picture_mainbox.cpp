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

    grapher_curve = ui->grapher_widget->add_curve("data");
    emit debug(QString("grapher_curve %1").arg(grapher_curve));

    connect(ui->btn_load_picture,   SIGNAL(clicked(bool)),  this,   SLOT(load_picture()));
    connect(ui->btn_find_max_color, SIGNAL(clicked(bool)),  this,   SLOT(find_max_color()));
    connect(ui->btn_redraw_picture, SIGNAL(clicked(bool)),  this,   SLOT(redraw_picture()));

    connect(ui->btn_show_only_R,    SIGNAL(clicked(bool)),  this,   SLOT(show_only_R()));
    connect(ui->btn_show_only_G,    SIGNAL(clicked(bool)),  this,   SLOT(show_only_G()));
    connect(ui->btn_show_only_B,    SIGNAL(clicked(bool)),  this,   SLOT(show_only_B()));

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
    dlg->deleteLater();

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
    pd->deleteLater();

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
    pd->deleteLater();

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
void MainBox::show_only_R(void)
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
        }
    }
    pd->close();
    pd->deleteLater();

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
        }
    }
    pd->close();
    pd->deleteLater();

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
        }
    }
    pd->close();
    pd->deleteLater();

    ui->new_picture->setPixmap(QPixmap::fromImage(tmp));
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");
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
