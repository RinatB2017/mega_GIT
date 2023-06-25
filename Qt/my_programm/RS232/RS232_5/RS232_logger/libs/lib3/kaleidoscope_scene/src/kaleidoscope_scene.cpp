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
#include "kaleidoscope_scene.hpp"
#include "ui_kaleidoscope_scene.h"
//--------------------------------------------------------------------------------
KaleidoscopeScene::KaleidoscopeScene(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::KaleidoscopeScene)
{
    init();
}
//--------------------------------------------------------------------------------
KaleidoscopeScene::~KaleidoscopeScene()
{
    if(timer)
    {
        timer->stop();
        delete timer;
    }
    if(k_widget)
    {
        delete k_widget;
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::init(void)
{
    ui->setupUi(this);

    k_widget = new KaleidoscopeWidget(this);
    k_widget->get_size(&k_widget_w, &k_widget_h);

    //TODO установка фона
    //set_background_image(QImage(":/images/background.png"));

    result_image = QImage(screen_w,
                          screen_h,
                          QImage::Format_ARGB32);
    result_image.fill(QColor(Qt::white));
    p_result_image.begin(&result_image);

    image = QImage(screen_w+k_widget_w,
                   screen_h+k_widget_h,
                   QImage::Format_ARGB32);


    init_timer();
    //setFixedSize(screen_w, screen_h);
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::set_background_image(QImage image)
{
    background_image = image;
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::init_timer(void)
{
    timer = new QTimer(this);
#if 1
    connect(timer,  &QTimer::timeout,
            this,   &KaleidoscopeScene::f_step);
#else
    connect(timer,  &QTimer::timeout,
            this,   &KaleidoscopeScene::f_update);
#endif
    //timer->start();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_start(void)
{
    if(timer)
    {
        if(record_movie_start(screen_w,
                              screen_h,
                              frames_per_second,
                              filename_movie))
        {
            timer->start();
        }
    }
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_stop(void)
{
    if(timer)
    {
        record_movie_stop();
        timer->stop();
    }
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_step(void)
{
    k_widget->f_move_ru();

    counter_time++;
    if(counter_time > frames_per_second*60)
    {
        record_movie_stop();
        timer->stop();
        qInfo() << "Запись завершена";
        //qApp->quit();
    }
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_test(void)
{
    //TODO f_test
    Q_ASSERT(k_widget);

    //---
    QImage image = QImage(":/images/ковер.jpg");
    Q_ASSERT(!image.isNull());
    int w = image.width();
    int h = image.height();

    QImage white_image = QImage(w, h, QImage::Format_ARGB32);
    white_image.fill(QColor(Qt::white));

    QImage full_image = QImage(w, h*2, QImage::Format_ARGB32);
    QPainter p(&full_image);
    p.drawImage(0, 0, image);
    p.drawImage(0, h, white_image);
    //---

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(full_image));
    label->show();

    bool ok = k_widget->load_image(full_image);
    if(!ok)
    {
        qInfo() << "The image is not loaded";
        return;
    }

    set_flag_show(true);
    set_flag_video(true);

    if(flag_save)
    {
        record_movie_start(screen_w,
                           screen_h,
                           frames_per_second,
                           filename_movie);
    }
    QElapsedTimer timer;
    timer.start();
    k_widget->get_size(&k_widget_w, &k_widget_h);
    //k_widget->f_move_to(k_widget_w / 2.0, k_widget_h / 2.0);
    k_widget->f_move_to(0, h);

    int pos_x, pos_y;
    k_widget->get_pos(&pos_x, &pos_y);
    qInfo() << "pos_x:" << pos_x;
    qInfo() << "pos_y:" << pos_y;

    for(int sec=0; sec<10; sec++)
    {
        for(int n=0; n<30; n++)
        {
            k_widget->f_move_d(30);
            for(int n=0; n<30; n++)
            {
                f_update();
            }
        }
        for(int n=0; n<30; n++)
        {
            k_widget->f_move_u(30);
            for(int n=0; n<30; n++)
            {
                f_update();
            }
        }
    }
    qInfo() << "Elapsed" << timer.elapsed() / 1000.0 << "second";
    if(flag_save)
    {
        record_movie_stop();
    }
    qInfo() << "The end!";
}
//--------------------------------------------------------------------------------
QImage KaleidoscopeScene::f_screenshot(void)
{
#if 0
    QImage image(screen_w+k_widget_w,
                 screen_h+k_widget_h,
                 QImage::Format_ARGB32);
    QPainter p(&image);

    Q_ASSERT(k_widget);
    QImage image_w = k_widget->get_image();
    for(int y=0; y<(screen_h+k_widget_h); y+=k_widget_h)
    {
        for(int x=0; x<(screen_w+k_widget_w); x+=k_widget_w)
        {
            p.drawImage(x,
                        y,
                        image_w);
        }
    }

    QImage t_image = image.copy(offset_x,
                                offset_y,
                                screen_w,
                                screen_h);
    return t_image;
#else
    //return ui->lbl_image->pixmap(Qt::ReturnByValue).toImage();
    return ui->lbl_image->pixmap()->toImage();
#endif
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_update(void)
{
    Q_ASSERT(k_widget);

    QCoreApplication::processEvents();
    QPainter p(&image);

    QImage image_w = k_widget->get_image();
    for(int y=0; y<(screen_h+k_widget_h); y+=k_widget_h)
    {
        for(int x=0; x<(screen_w+k_widget_w); x+=k_widget_w)
        {
            p.drawImage(x,
                        y,
                        image_w);
        }
    }

    t_image = image.copy(offset_x,
                         offset_y,
                         screen_w,
                         screen_h);

    if(flag_show)
    {
        ui->lbl_image->setPixmap(QPixmap::fromImage(t_image));
    }

    if(flag_save)
    {
        t_image = convert_rgb_to_bgr(t_image);
        add_frame_movie(t_image, false);
    }
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_update_one_widget(void)
{
    QCoreApplication::processEvents();

    Q_ASSERT(k_widget);
    QImage image_w = k_widget->get_image();
    int widget_w = 0;
    int widget_h = 0;
    k_widget->get_size(&widget_w, &widget_h);

    //TODO
    p_result_image.drawImage(0, 0, background_image);

    p_result_image.drawImage(screen_w / 2 - widget_w / 2,
                             screen_h / 2 - widget_h / 2,
                             image_w);

    if(flag_show)
    {
        ui->lbl_image->setPixmap(QPixmap::fromImage(result_image));
    }

    if(flag_save)
    {
        t_image = convert_rgb_to_bgr(result_image);
        add_frame_movie(t_image, false);
    }
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_update_two_widget(void)
{
    QCoreApplication::processEvents();

    Q_ASSERT(k_widget);
    QImage image_w = k_widget->get_image();
    int widget_w = 0;
    int widget_h = 0;
    k_widget->get_size(&widget_w, &widget_h);
    p_result_image.drawImage(screen_w / 4 - widget_w / 2,
                             screen_h / 2 - widget_h / 2,
                             image_w);
    p_result_image.drawImage(3 * screen_w / 4 - widget_w / 2,
                             screen_h / 2 - widget_h / 2,
                             image_w);

    if(flag_show)
    {
        ui->lbl_image->setPixmap(QPixmap::fromImage(result_image));
    }

    if(flag_save)
    {
        t_image = convert_rgb_to_bgr(result_image);
        add_frame_movie(t_image, false);
    }
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::set_image(QImage image)
{
    ui->lbl_image->setPixmap(QPixmap::fromImage(image));
}
//--------------------------------------------------------------------------------
bool KaleidoscopeScene::load_image(const QImage &new_image)
{
    Q_ASSERT(!new_image.isNull());
    Q_ASSERT(k_widget);

    bool ok = k_widget->load_image(new_image);
    f_update();
    return ok;
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_lu(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_lu();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_l(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_l();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_ld(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_ld();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_u(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_u();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_d(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_d();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_ru(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_ru();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_r(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_r();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_move_rd(void)
{
    Q_ASSERT(k_widget);

    k_widget->f_move_rd();
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_set(int x, int y)
{
    Q_ASSERT(k_widget);
    k_widget->set_pos(x, y);
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::f_get(int *x, int *y)
{
    Q_ASSERT(k_widget);

    k_widget->get_pos(x, y);
    f_update();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::set_flag_show(bool state)
{
    flag_show = state;
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::set_flag_video(bool state)
{
    flag_save = state;
}
//--------------------------------------------------------------------------------
bool KaleidoscopeScene::record_movie_start(int screen_w,
                                           int screen_h,
                                           double frames_per_second,
                                           const QString filename_movie)
{
    return Creator_movie::record_movie_start(screen_w,
                                             screen_h,
                                             frames_per_second,
                                             filename_movie);
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::record_movie_stop(void)
{
    Creator_movie::record_movie_stop();
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::add_frame_movie(const QImage &image, bool need_convert)
{
    Creator_movie::add_frame_movie(image, need_convert);
}
//--------------------------------------------------------------------------------
QImage KaleidoscopeScene::convert_rgb_to_bgr(QImage image)
{
    return Creator_movie::convert_rgb_to_bgr(image);
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool KaleidoscopeScene::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void KaleidoscopeScene::save_setting(void)
{

}
//--------------------------------------------------------------------------------
