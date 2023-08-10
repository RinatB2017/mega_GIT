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
#include "ui_splitter_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "splitter_mainbox.hpp"
#include "mysplashscreen.hpp"
#include "myfiledialog.hpp"
#include "mainwindow.hpp"
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

    ui->sb_x->setRange(1, 100);
    ui->sb_y->setRange(1, 100);

#if 0
    //FIXME позже разберусь
    ui->btn_split->setVisible(false);
#endif

    connect(ui->btn_split,          &QPushButton::clicked,  this,   &MainBox::f_split);
    connect(ui->btn_save_files,     &QPushButton::clicked,  this,   &MainBox::f_save_files);
    connect(ui->btn_load_picture,   &QPushButton::clicked,  this,   &MainBox::f_load_picture);

    updateText();
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
        commands.append({ id++, "test 0", &MainBox::test });

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
bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);
#if 0
    if(fake_label)
    {
        emit info("delete fake_label");
        delete fake_label;
    }
#endif
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::f_split(void)
{
    int max_x = ui->sb_x->value();
    int max_y = ui->sb_y->value();

#ifdef Q_OS_LINUX
    int w = ui->picture_label->pixmap(Qt::ReturnByValue).width();
    int h = ui->picture_label->pixmap(Qt::ReturnByValue).height();
#else
    int w = ui->picture_label->pixmap()->width();
    int h = ui->picture_label->pixmap()->height();
#endif
    int inc_x = w / max_x;
    int inc_y = h / max_y;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    QImage *image = new QImage(w,
                               h,
                               QImage::Format_ARGB32);
    image->fill(Qt::transparent);
    if(image)
    {
        QPainter painter(image);
        painter.setPen(Qt::red);

        for(int x=0; x<max_x; x++)
        {
            y1 = 0;
            y2 = h;
            x1 = x2 = x * inc_x;
            painter.drawLine(x1, y1, x2, y2);
        }

        for(int y=0; y<max_y; y++)
        {
            x1 = 0;
            x2 = w;
            y1 = y2 = y * inc_y;
            painter.drawLine(x1, y1, x2, y2);
        }

        fake_label = new QLabel(ui->picture_label);
        fake_label->setPixmap(QPixmap::fromImage(*image));
        fake_label->show();
    }
    else
    {
        emit error(QString("Cannot load %1").arg(filename));
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_save_files(void)
{
    MyFileDialog *dlg = new MyFileDialog("splitter_dirs", "splitter_dirs", this);
    dlg->setOption(QFileDialog::ShowDirsOnly, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString dirname = files.at(0);

        if(!dirname.isEmpty())
        {
            if(fake_label)
            {
                emit info("delete fake_label");
                delete fake_label;
            }

            int max_x = ui->sb_x->value();
            int max_y = ui->sb_y->value();

#ifdef Q_OS_LINUX
            int w = ui->picture_label->pixmap(Qt::ReturnByValue).width();
            int h = ui->picture_label->pixmap(Qt::ReturnByValue).height();
#else
            int w = ui->picture_label->pixmap()->width();
            int h = ui->picture_label->pixmap()->height();
#endif
            int inc_x = w / max_x;
            int inc_y = h / max_y;
            int x1 = 0;
            int y1 = 0;

            int index = 0;
            bool ok = false;
            for(int y=0; y<max_y; y++)
            {
                for(int x=0; x<max_y; x++)
                {
                    x1 = x * inc_x;
                    y1 = y * inc_y;
                    QPixmap pixmap = ui->picture_label->grab(QRect(x1,
                                                                   y1,
                                                                   inc_x,
                                                                   inc_y));
                    QString temp = QString("%1/%2.png")
                            .arg(dirname)
                            .arg(index++);
                    ok = pixmap.save(temp);
                    if(!ok)
                    {
                        emit error(QString("File %1 not created").arg(temp));
                    }
                }
            }
        }
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::f_load_picture(void)
{
    MyFileDialog *dlg = new MyFileDialog("splitter_mainbox", "splitter_mainbox", this);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->selectFile(filename);
    dlg->setDefaultSuffix("png");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);

        if(!filename.isEmpty())
        {
            QPixmap pixmap;
            if(pixmap.load(filename))
            {
                ui->picture_label->setPixmap(pixmap);
            }
            else
            {
                emit error(QString("Cannot read %1").arg(filename));
            }
        }
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
