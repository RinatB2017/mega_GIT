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
#ifndef CREATOR_MOVIE_HPP
#define CREATOR_MOVIE_HPP
//--------------------------------------------------------------------------------
#include <QImage>
#include <QtDebug>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <opencv2/imgproc/imgproc.hpp>
#   include <opencv2/videoio.hpp>

#   include <opencv2/imgcodecs.hpp>
#else
#   include <imgproc/imgproc.hpp>
#   include <videoio.hpp>

#   include <imgcodecs.hpp>
#endif

using namespace cv;
//--------------------------------------------------------------------------------
class Creator_movie
{
public:
    explicit Creator_movie(void);
    ~Creator_movie();

    bool record_movie_start(int screen_w,
                            int screen_h,
                            double frames_per_second,
                            const QString &filename_movie);
    void record_movie_stop(void);
    void add_frame_movie(const QImage &image,
                         bool need_convert = true);
    QImage convert_rgb_to_bgr(QImage image);
    void set_max_frames(int max_f);
    int  get_max_frames(void);
    int  get_frames(void);

private:
    VideoWriter out;
    int m_frames = 0;
    int max_frames = INT_MAX;
};
//--------------------------------------------------------------------------------
#endif // CREATOR_MOVIE_HPP
