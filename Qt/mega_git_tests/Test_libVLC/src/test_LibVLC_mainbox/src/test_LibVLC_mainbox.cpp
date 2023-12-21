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
#include <QtGlobal>

#ifdef Q_OS_LINUX
#   include <vlc/vlc.h>
#endif
#ifdef Q_OS_WIN
#   include "src/vlc/src/vlc.h"
#endif
//--------------------------------------------------------------------------------
#include "test_LibVLC_mainbox.hpp"
#include "ui_test_LibVLC_mainbox.h"
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
    createTestBar();
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
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test",         &MainBox::test });
        commands.append({ id++, "test_rtsp",    &MainBox::test_rtsp });

        testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        cb_test->setProperty(NO_SAVE, true);
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
bool MainBox::test(void)
{
    QStringList filters;
    filters << "Video Files (*.avi *.mpg *.mov *.mp4 *.mkv)";
    QString filename;

    MyFileDialog *dlg = new MyFileDialog("vlc");
    dlg->setNameFilters(filters);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
    }
    delete dlg;

    libvlc_instance_t *inst = nullptr;
    libvlc_media_player_t *mp = nullptr;
    libvlc_media_t *m = nullptr;

    /* Load the VLC engine */
    inst = libvlc_new (0, nullptr);

    /* Create a new item */
    m = libvlc_media_new_path (inst, filename.toLocal8Bit());

    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);
    if(mp == NULL)
    {
        emit error("libvlc_media_player_new_from_media return NULL");
        return false;
    }

    /* No need to keep the media now */
    libvlc_media_release (m);

#ifdef QT_DEBUG
    qDebug() << "time" << libvlc_media_player_get_time(mp);
    qDebug() << "title" << libvlc_media_player_get_title(mp);
#endif

    libvlc_media_player_set_xwindow (mp, static_cast<uint32_t>(ui->vlc_widget->winId()));

    /* play the media_player */
    int err = libvlc_media_player_play (mp);
    if(err == 0)
    {
        emit info("OK");
        return true;
    }

    emit error("Error");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_rtsp(void)
{
    libvlc_instance_t *inst = nullptr;
    libvlc_media_player_t *mp = nullptr;
    libvlc_media_t *m = nullptr;

    inst = libvlc_new (0, nullptr);
    mp = libvlc_media_player_new(inst);
    m = libvlc_media_new_location(inst, "rtsp://192.168.0.66:554/av0_0");
    Q_ASSERT(m);

    libvlc_media_player_set_media (mp, m);

    libvlc_media_player_set_xwindow (mp, static_cast<uint32_t>(ui->vlc_widget->winId()));
    libvlc_media_player_play (mp);

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
