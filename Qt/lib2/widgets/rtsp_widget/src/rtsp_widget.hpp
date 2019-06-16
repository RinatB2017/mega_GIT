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
#ifndef RTSP_WIDGET_HPP
#define RTSP_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QMediaPlayer>
#include <QWidget>
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class RTSP_widget;
}
//--------------------------------------------------------------------------------
class RTSP_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit RTSP_widget(QWidget *parent = nullptr);
    ~RTSP_widget();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void play(void);
    void pause(void);
    void stop(void);

private slots:
    void choice(void);
    void f_error(QMediaPlayer::Error err);

private:
    Ui::RTSP_widget *ui;
    QMediaPlayer *player;

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // RTSP_WIDGET_HPP
