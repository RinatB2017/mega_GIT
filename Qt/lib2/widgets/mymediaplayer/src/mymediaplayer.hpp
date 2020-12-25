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
#ifndef MYMEDIAPLAYER_HPP
#define MYMEDIAPLAYER_HPP
//--------------------------------------------------------------------------------
#include <QMediaPlayer>
#include <QVideoProbe>
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MyMediaPlayer;
}
//--------------------------------------------------------------------------------
class MyMediaPlayer : public MyWidget
{
    Q_OBJECT

signals:
    void get_frame(QVideoFrame);

public:
    explicit MyMediaPlayer(QWidget *parent = nullptr);
    ~MyMediaPlayer();

private:
    Ui::MyMediaPlayer *ui;

    QMediaPlayer *player = nullptr;
    QVideoProbe *probe = nullptr;
    QVideoFrame current_frame;

    void init(void);

    void load(void);
    void start(void);
    void pause(void);
    void stop(void);

    void first(void);
    void prev(void);
    void next(void);
    void last(void);

    void set_position(int pos);
    void durationChanged(qint64 pos);
    void positionChanged(qint64 pos);
    void timeChanged(const QTime &time);

    void processFrame(QVideoFrame const &frame);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MYMEDIAPLAYER_HPP
