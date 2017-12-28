/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <vlc/vlc.h>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "sleeper.h"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

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
void MainBox::load(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::save(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),
                                                    ".",
                                                    tr("Video Files (*.avi *.mpg *.mov)"));

    if(fileName.isEmpty()) return;

    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);

    /* Create a new item */
    m = libvlc_media_new_path (inst, fileName.toLocal8Bit());

    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);

    /* No need to keep the media now */
    libvlc_media_release (m);

#if 0
    /* This is a non working code that show how to hooks into a window,
 * if we have a window around */
    libvlc_media_player_set_xwindow (mp, xid);
    /* or on windows */
    libvlc_media_player_set_hwnd (mp, hwnd);
    /* or on mac os */
    libvlc_media_player_set_nsobject (mp, view);
#endif

    qDebug() << "time" << libvlc_media_player_get_time(mp);
    qDebug() << "title" << libvlc_media_player_get_title(mp);

    /* play the media_player */
    libvlc_media_player_play (mp);

    //libvlc_media_player_set_position(mp, 100);

    return;

    Sleeper::sleep(3); /* Let it play a bit */

    /* Stop playing */
    libvlc_media_player_stop (mp);

    /* Free the media_player */
    libvlc_media_player_release (mp);

    libvlc_release (inst);
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
    qDebug() << "load_setting";
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    qDebug() << "save_setting";
}
//--------------------------------------------------------------------------------
