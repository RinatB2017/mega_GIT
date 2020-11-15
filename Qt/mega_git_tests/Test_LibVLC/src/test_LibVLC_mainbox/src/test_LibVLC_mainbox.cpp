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
#include <vlc/vlc.h>
//--------------------------------------------------------------------------------
#include "ui_test_LibVLC_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_LibVLC_mainbox.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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

#ifdef QT_DEBUG
    createTestBar();
#endif
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw != nullptr);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    QToolButton *btn_test_rtsp = add_button(testbar,
                                            new QToolButton(this),
                                            qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                            "rtsp",
                                            "rtsp");

    connect(btn_test,       SIGNAL(clicked(bool)),  this,   SLOT(test()));
    connect(btn_test_rtsp,  SIGNAL(clicked(bool)),  this,   SLOT(test_rtsp()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),
                                                    ".",
                                                    tr("Video Files (*.avi *.mpg *.mov)"));

    if(fileName.isEmpty())
    {
        emit error("no file");
        return;
    }

    libvlc_instance_t *inst = nullptr;
    libvlc_media_player_t *mp = nullptr;
    libvlc_media_t *m = nullptr;

    /* Load the VLC engine */
    inst = libvlc_new (0, nullptr);

    /* Create a new item */
    m = libvlc_media_new_path (inst, fileName.toLocal8Bit());

    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);

    /* No need to keep the media now */
    libvlc_media_release (m);

    qDebug() << "time" << libvlc_media_player_get_time(mp);
    qDebug() << "title" << libvlc_media_player_get_title(mp);

    /* play the media_player */
    libvlc_media_player_play (mp);
}
//--------------------------------------------------------------------------------
void MainBox::test_rtsp(void)
{
    libvlc_instance_t *inst = nullptr;
    libvlc_media_player_t *mp = nullptr;
    libvlc_media_t *m = nullptr;

    inst = libvlc_new (0, nullptr);
    mp = libvlc_media_player_new(inst);
    m = libvlc_media_new_location(inst, "rtsp://192.168.0.66:554/av0_0");
    Q_CHECK_PTR(m);

    libvlc_media_player_set_media (mp, m);

    libvlc_media_player_set_xwindow (mp, static_cast<uint32_t>(ui->widget->winId()));
    libvlc_media_player_play (mp);
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
