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
#include "creator_movie.hpp"
//--------------------------------------------------------------------------------
Creator_movie::Creator_movie(void)
{

}
//--------------------------------------------------------------------------------
Creator_movie::~Creator_movie()
{
    out.release();
}
//--------------------------------------------------------------------------------
bool Creator_movie::record_movie_start(int screen_w,
                                       int screen_h,
                                       double frames_per_second,
                                       const QString &filename_movie)
{
    bool ok = out.open(filename_movie.toStdString().c_str(),
                       // cv::VideoWriter::fourcc('X','V','I','D'),
                       // VideoWriter::fourcc('h','2','6','4'),
                       cv::VideoWriter::fourcc('V','P','8','0'),    // объём гораздо меньше получается, чем XVID
                       // cv::VideoWriter::fourcc('V','P','9','0'),    // плохой вариант по времени создания
                       frames_per_second,
                       cv::Size(screen_w, screen_h),
                       true);
    if(!ok)
    {
        qDebug() << QString("%1 not created")
                    .arg(filename_movie);
    }
    return ok;
}
//--------------------------------------------------------------------------------
void Creator_movie::record_movie_stop(void)
{
    out.release();
}
//--------------------------------------------------------------------------------
void Creator_movie::add_frame_movie(const QImage &image,
                                    bool need_convert)
{
    m_frames++;
    if(m_frames > max_frames)
    {
        return;
    }
    if(out.isOpened())
    {
        QImage img;
        //TODO преобразование в формат BGR необходимо, так как OCV работает в BGR
        if(need_convert)
            img = image.convertToFormat(QImage::Format_BGR30);
        else
            img = image;

        Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
        cvtColor( mat, mat, COLOR_BGR2RGB );
        out.write(mat);
    }
    else
    {
        qDebug() << "Out not opened";
    }
}
//--------------------------------------------------------------------------------
QImage Creator_movie::convert_rgb_to_bgr(QImage image)
{
    //TODO странно, что image.convertToFormat(QImage::Format_BGR30) не работает
    QImage new_image = image;
    int w = image.width();
    int h = image.height();
#if 1
    uchar *src_ptr = image.bits();
    uchar *dst_ptr = new_image.bits();

    int max_n = w * h; //не забывать про 4 байта
    for(int n=0; n<max_n; n++)
    {
        //RGBA
        *dst_ptr     = *(src_ptr+2);
        *(dst_ptr+1) = *(src_ptr+1);
        *(dst_ptr+2) = *src_ptr;
        *(dst_ptr+3) = *(src_ptr+3);

        src_ptr+=4;
        dst_ptr+=4;
    }
#else
    QRgb rgb;
    QColor old_color;
    QColor new_color;
    for(int y=0; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            rgb = image.pixel(x, y);

            old_color.setRgb(rgb);
            new_color.setRgb(old_color.blue(),
                             old_color.green(),
                             old_color.red());

            new_image.setPixel(x, y, new_color.rgb());
        }
    }
#endif
    return new_image;
}
//--------------------------------------------------------------------------------
void Creator_movie::set_max_frames(int max_f)
{
    max_frames = max_f;
}
//--------------------------------------------------------------------------------
int Creator_movie::get_max_frames(void)
{
    return max_frames;
}
//--------------------------------------------------------------------------------
int Creator_movie::get_frames(void)
{
    return m_frames;
}
//--------------------------------------------------------------------------------
