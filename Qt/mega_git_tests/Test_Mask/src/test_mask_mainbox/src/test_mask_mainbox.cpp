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
#include "ui_test_mask_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_mask_mainbox.hpp"
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

    load_widgets();

#if 0
    //TODO если не вызвать singleShot, то будет странно. Надо исправить позже
    QTimer::singleShot(0, [this]{
#ifdef USE_DOCK_WIDGETS
        MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
        if(mw)
        {
            mw->add_dock_widget("MainBox",
                                "mainbox",
                                Qt::TopDockWidgetArea,
                                this);
            //            setVisible(false);    //TODO не надо это разкомментировать
        }
#endif
    });
#endif

    QTimer::singleShot(0, [this]{
#if 1
    QPixmap maskPix( ":/mask.png" );
    topLevelWidget()->setMask( maskPix.scaled( size() ).mask() );
#else
        QPixmap *pixmap = new QPixmap(800, 600);

        QPainter painter(this);
        painter.begin(pixmap);
//        painter.fillRect(0, 0, 800, 600, Qt::red);
        painter.fillRect(200, 150, 400, 300, Qt::transparent);
        painter.end();

        adjustSize();

        emit info(QString("%1 %2")
                  .arg(size().width())
                  .arg(size().height()));

        topLevelWidget()->setMask( pixmap->scaled( size() ).mask() );
#endif
    });
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",     &MainBox::test });

    testbar = new QToolBar("testbar");
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

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

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

#if 1
    emit info(qApp->applicationVersion());
#endif

#if 0
    QPixmap maskPix( ":/mask.png" );
    topLevelWidget()->setMask( maskPix.scaled( size() ).mask() );
#else
    QPixmap *maskPix = new QPixmap(800, 600);

    maskPix->fill(Qt::transparent);

    QPainter painter(this);
    painter.begin(maskPix);
//    painter.fillRect(0, 0, 800, 600, Qt::red);
    painter.fillRect(0, 0, 800, 300, Qt::red);
    painter.end();

#if 0
    if(maskPix->save("/dev/shm/mask.png", "PNG"))
    {
        emit info("OK");
    }
    else
    {
        emit error("FAIL");
    }
#else
    topLevelWidget()->setMask( maskPix->scaled( topLevelWidget()->size() ).mask() );
#endif
#endif

#if 0
    QPixmap *pixmap = new QPixmap(800, 600);

    QPainter painter(this);
    painter.begin(pixmap);
    //painter.fillRect(0, 0, 800, 600, Qt::red);

    painter.fillRect(200, 150, 400, 300, Qt::transparent);

    //    painter.setCompositionMode (QPainter::CompositionMode_Source);
    //    painter.fillRect(10, 10, 100, 100, Qt::transparent);
    //    painter.setCompositionMode (QPainter::CompositionMode_SourceOver);

    //    painter.setBrush(QColor(Qt::transparent));
    //    painter.drawRect(10, 10, 100, 100);

    //    painter.fillRect(10, 10, 100, 100, QBrush(QColor(0, 0, 0, 255)));
    painter.end();

    QLabel *label = new QLabel();
    label->setPixmap(*pixmap);
    label->show();
#endif

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
    if(cb_block)
    {
        bool is_checked = load_bool("cb_block");
        cb_block->setChecked(is_checked);
        cb_block->clicked(is_checked);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    if(cb_block)
    {
        save_int("cb_block", cb_block->isChecked());
    }
}
//--------------------------------------------------------------------------------
