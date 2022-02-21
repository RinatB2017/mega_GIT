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
#ifndef CREATOR_HPP
#define CREATOR_HPP
//--------------------------------------------------------------------------------
#include <QWidget>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
2
/home/boss/Programming/_GitHub/Private/Qt/_private_tests/test_ffmpeg/test_ffmpeg
film.mpg
*/

extern "C" {
    //#include <libavutil/avassert.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/opt.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/timestamp.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    AVPacket *tmp_pkt;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;
//--------------------------------------------------------------------------------
class Creator : public QWidget
{
public:
    explicit Creator(QWidget *parent = nullptr);
    virtual ~Creator();

    int test(int argc, char **argv);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    void log_packet(const AVFormatContext *fmt_ctx,
                    const AVPacket *pkt);
    int write_frame(AVFormatContext *fmt_ctx,
                    AVCodecContext *c,
                    AVStream *st,
                    AVFrame *frame,
                    AVPacket *pkt);
    void add_stream(OutputStream *ost,
                    AVFormatContext *oc,
                    const AVCodec **codec,
                    enum AVCodecID codec_id);
    AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                               uint64_t channel_layout,
                               int sample_rate,
                               int nb_samples);
    void open_audio(AVFormatContext *oc,
                    const AVCodec *codec,
                    OutputStream *ost,
                    AVDictionary *opt_arg);
    AVFrame *get_audio_frame(OutputStream *ost);
    int write_audio_frame(AVFormatContext *oc,
                          OutputStream *ost);
    AVFrame *alloc_picture(enum AVPixelFormat pix_fmt,
                           int width,
                           int height);
    void open_video(AVFormatContext *oc,
                    const AVCodec *codec,
                    OutputStream *ost,
                    AVDictionary *opt_arg);
    void fill_yuv_image(AVFrame *pict,
                        int frame_index,
                        int width,
                        int height);
    AVFrame *get_video_frame(OutputStream *ost);
    int write_video_frame(AVFormatContext *oc,
                          OutputStream *ost);
    void close_stream(AVFormatContext *oc,
                      OutputStream *ost);
};
//--------------------------------------------------------------------------------
#endif // CREATOR_HPP
