/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef KALEIDOSCOPE_SCENE_HPP
#define KALEIDOSCOPE_SCENE_HPP
//--------------------------------------------------------------------------------
#include "kaleidoscope_widget.hpp"
#include "creator_movie.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class KaleidoscopeScene;
}
//--------------------------------------------------------------------------------
class KaleidoscopeScene : public MyWidget, Creator_movie
{
    Q_OBJECT

public:
    explicit KaleidoscopeScene(QWidget *parent = nullptr);
    virtual ~KaleidoscopeScene();

    bool load_image(const QImage &new_image);

    void f_start(void);
    void f_stop(void);
    void f_step(void);
    void f_test(void);

    void f_move_lu(void);
    void f_move_l(void);
    void f_move_ld(void);
    void f_move_u(void);
    void f_move_d(void);
    void f_move_ru(void);
    void f_move_r(void);
    void f_move_rd(void);

    void f_set(int x,  int y);
    void f_get(int *x, int *y);

    QImage f_screenshot(void);
    void f_update(void);
    void f_update_one_widget(void);
    void f_update_two_widget(void);

    void set_image(QImage image);
    void set_background_image(QImage image);

private:
    Ui::KaleidoscopeScene *ui;

    QTimer *timer = nullptr;

    int counter_time = 0;

    QImage background_image;
    QImage result_image;
    QPainter p_result_image;

    void init(void);
    void init_timer(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
#ifdef Q_OS_LINUX
    //QString filename_movie = "/home/boss/HDD/YouTube/_MyYoutube/wallpaper.avi";
    QString filename_movie = "/dev/shm/wallpaper.avi";
#else
    QString filename_movie = "wallpaper.avi";
#endif

    qreal frames_per_second = 60.0f;

#ifdef RES_NORMAL
    int screen_w = 1920;
    int screen_h = 1080;
#endif

#ifdef RES_4K
    int screen_w = 3840;
    int screen_h = 2160;
#endif

#ifdef RES_8K
    int screen_w = 7680;
    int screen_h = 4320;
#endif

    int offset_x = 0;
    int offset_y = 0;

    bool flag_show = true;
    bool flag_save = true;

    KaleidoscopeWidget *k_widget = nullptr;
    int k_widget_w = 600;
    int k_widget_h = 600;

    QImage image;
    QImage t_image;

    void set_flag_show(bool state);
    void set_flag_video(bool state);

    //---
    bool record_movie_start(int screen_w,
                            int screen_h,
                            double frames_per_second,
                            const QString filename_movie);
    void record_movie_stop(void);
    void add_frame_movie(const QImage &image, bool need_convert = true);
    QImage convert_rgb_to_bgr(QImage image);
};
//--------------------------------------------------------------------------------
#endif // KALEIDOSCOPE_SCENE_HPP
