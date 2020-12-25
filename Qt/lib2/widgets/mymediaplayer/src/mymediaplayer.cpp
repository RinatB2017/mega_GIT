/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "mymediaplayer.hpp"
#include "ui_mymediaplayer.h"
//--------------------------------------------------------------------------------
MyMediaPlayer::MyMediaPlayer(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MyMediaPlayer)
{
    init();
}
//--------------------------------------------------------------------------------
MyMediaPlayer::~MyMediaPlayer()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::init(void)
{
    ui->setupUi(this);

    player = new QMediaPlayer;

    probe = new QVideoProbe(this);
    connect(probe,  &QVideoProbe::videoFrameProbed,
            this,   &MyMediaPlayer::processFrame);
    probe->setSource(player); // Returns true, hopefully.

    ui->btn_load->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_pause->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPause));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    ui->btn_first->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->btn_prev->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->btn_next->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->btn_last->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(ui->btn_first,  &QToolButton::clicked,  this,   &MyMediaPlayer::first);
    connect(ui->btn_prev,   &QToolButton::clicked,  this,   &MyMediaPlayer::prev);
    connect(ui->btn_next,   &QToolButton::clicked,  this,   &MyMediaPlayer::next);
    connect(ui->btn_last,   &QToolButton::clicked,  this,   &MyMediaPlayer::last);

    connect(ui->btn_load,   &QToolButton::clicked,  this,   &MyMediaPlayer::load);
    connect(ui->btn_start,  &QToolButton::clicked,  this,   &MyMediaPlayer::start);
    connect(ui->btn_pause,  &QToolButton::clicked,  this,   &MyMediaPlayer::pause);
    connect(ui->btn_stop,   &QToolButton::clicked,  this,   &MyMediaPlayer::stop);

    connect(player,         &QMediaPlayer::durationChanged, this,           &MyMediaPlayer::durationChanged);
    connect(player,         &QMediaPlayer::positionChanged, this,           &MyMediaPlayer::positionChanged);
    connect(player,         &QMediaPlayer::positionChanged, ui->sl_movie,   &QSlider::setValue);
    connect(ui->sl_movie,   &QSlider::sliderMoved,          this,           &MyMediaPlayer::set_position);

    connect(ui->te_position,    &QTimeEdit::timeChanged,    this,           &MyMediaPlayer::timeChanged);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::processFrame(const QVideoFrame &frame)
{
    current_frame = frame;
    //emit debug(QString("%1 %2").arg(current_frame.width()).arg(current_frame.height()));
    emit get_frame(frame);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::durationChanged(qint64 pos)
{
    QTime time;
    time.setHMS(0, 0, 0);
    time = time.addMSecs(pos);
    emit debug(QString(QString("duration: %1").arg(time.toString())));

    ui->te_position->setMaximumTime(time);
    ui->sl_movie->setMaximum(pos);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::positionChanged(qint64 pos)
{
    emit debug(QString("pos: %1").arg(pos));

    ui->sl_movie->setMaximum(player->duration());
    ui->sl_movie->setValue(pos);

    QTime time;
    time.setHMS(0, 0, 0);
    time = time.addMSecs(pos);

    ui->te_position->setTime(time);
    ui->sl_movie->setToolTip(time.toString());
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::timeChanged(const QTime &time)
{
    int t_time = (time.hour() * 3600 +
                  time.minute() * 60 +
                  time.second()) * 1000;
    emit debug(time.toString());
    qDebug() << time.toString() << t_time;
//    player->setPosition(t_time);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::load(void)
{
    const QStringList filters({"Movie files (*.avi *.mkv *.mp4)",
                               "Any files (*)"
                              });

    MyFileDialog *dlg = new MyFileDialog("mediaplayer_box", "mediaplayer_box");
    dlg->setNameFilters(filters);
    dlg->setDefaultSuffix("mp4");
    dlg->setDirectory(".");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        if(filename.isEmpty() == false)
        {
            QUrl url = QUrl::fromLocalFile(QFileInfo(filename).absoluteFilePath());
            emit info(url.toString());
            player->setMedia(url);

            QTime time;
            time.setHMS(0, 0, 0);
            time = time.addMSecs(player->duration());
            emit debug(QString(QString("duration: %1").arg(time.toString())));

            ui->te_position->setMaximumTime(time);
            ui->sl_movie->setMaximum(player->duration());
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::start(void)
{
    player->play();
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::pause(void)
{
    player->pause();
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::stop(void)
{
    player->stop();
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::first(void)
{
    player->setPosition(0);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::prev(void)
{
    player->pause();

    qint64 pos = player->position();
    pos -= 40;
    player->setPosition(pos);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::next(void)
{
    player->pause();

    qint64 pos = player->position();
    pos += 40;
    player->setPosition(pos);
    player->pause();
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::set_position(int pos)
{
    player->setPosition(pos);
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::last(void)
{
    player->setPosition(player->duration());
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MyMediaPlayer::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MyMediaPlayer::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MyMediaPlayer::save_setting(void)
{

}
//--------------------------------------------------------------------------------
