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
#ifndef VIDEO_PLAYER
#define VIDEO_PLAYER
//--------------------------------------------------------------------------------
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QPointer>
#include <QUrl>
//--------------------------------------------------------------------------------
class VideoPlayer : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    virtual ~VideoPlayer(void);

    void set_url(QUrl new_url);

signals:
    void error(const QString &);

public slots:
    void play(void);
    void pause(void);
    void stop(void);

private slots:
    void s_error(QMediaPlayer::Error);

private:
    QPointer<QMediaPlayer> player;
};
//--------------------------------------------------------------------------------
#endif
