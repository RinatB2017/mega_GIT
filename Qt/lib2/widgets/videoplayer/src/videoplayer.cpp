/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include "videoplayer.hpp"
//--------------------------------------------------------------------------------
VideoPlayer::VideoPlayer(QWidget *parent) :
    QVideoWidget(parent)
{
    player = new QMediaPlayer(this);
    player->setVideoOutput(this);

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(s_error(QMediaPlayer::Error)));
}
//--------------------------------------------------------------------------------
VideoPlayer::~VideoPlayer()
{
    if(player)
    {
        delete player;
    }
}
//--------------------------------------------------------------------------------
void VideoPlayer::set_url(QUrl new_url)
{
    const QNetworkRequest requestRtsp(new_url);
    player->setMedia(requestRtsp);
    player->play();
}
//--------------------------------------------------------------------------------
void VideoPlayer::play(void)
{
    if(player->isAvailable())
    {
        player->play();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void VideoPlayer::pause(void)
{
    if(player->isAvailable())
    {
        player->pause();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void VideoPlayer::stop(void)
{
    if(player->isAvailable())
    {
        player->stop();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void VideoPlayer::s_error(QMediaPlayer::Error err)
{
    switch(err)
    {
    case QMediaPlayer::NoError:             emit error("NoError");              break;
    case QMediaPlayer::ResourceError:       emit error("ResourceError");        break;
    case QMediaPlayer::FormatError:         emit error("FormatError");          break;
    case QMediaPlayer::NetworkError:        emit error("NetworkError");         break;
    case QMediaPlayer::AccessDeniedError:   emit error("AccessDeniedError");    break;
    case QMediaPlayer::ServiceMissingError: emit error("ServiceMissingError");  break;
    case QMediaPlayer::MediaIsPlaylist:     emit error("MediaIsPlaylist");      break;
    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }
    emit error(player->errorString());
}
//--------------------------------------------------------------------------------
