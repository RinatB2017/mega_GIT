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
#include "tester.hpp"
//--------------------------------------------------------------------------------
Tester::Tester(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
Tester::~Tester()
{

}
//--------------------------------------------------------------------------------
void Tester::pushFrame(uint8_t* data)
{
    int err;
    if (!videoFrame)
    {
        videoFrame = av_frame_alloc();
        videoFrame->format = AV_PIX_FMT_YUV420P;
        videoFrame->width = cctx->width;
        videoFrame->height = cctx->height;
        if ((err = av_frame_get_buffer(videoFrame, 32)) < 0)
        {
            emit error(QString("Failed to allocate picture: %1").arg(err));
            return;
        }
    }
    if (!swsCtx)
    {
        swsCtx = sws_getContext(cctx->width,
                                cctx->height,
                                AV_PIX_FMT_RGB24, cctx->width,
                                cctx->height,
                                AV_PIX_FMT_YUV420P,
                                SWS_BICUBIC,
                                0,
                                0,
                                0);
    }
    int inLinesize[1] = { 3 * cctx->width };
    // From RGB to YUV
    sws_scale(swsCtx,
              (const uint8_t* const*)&data,
              inLinesize,
              0,
              cctx->height,
              videoFrame->data,
              videoFrame->linesize);
    videoFrame->pts = (1.0 / 30.0) * 90000 * (frameCounter++);
    emit info(QString("%1 %2 %3 %4")
              .arg(videoFrame->pts)
              .arg(cctx->time_base.num)
              .arg(cctx->time_base.den)
              .arg(frameCounter));
    if ((err = avcodec_send_frame(cctx, videoFrame)) < 0)
    {
        emit error(QString("Failed to send frame: %1").arg(err));
        return;
    }
    AV_TIME_BASE;
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;
    pkt.flags |= AV_PKT_FLAG_KEY;
    if (avcodec_receive_packet(cctx, &pkt) == 0)
    {
        static int counter = 0;
        if (counter == 0)
        {
            FILE* fp = fopen("dump_first_frame1.dat", "wb");
            fwrite(pkt.data, pkt.size, 1, fp);
            fclose(fp);
        }
        emit info(QString("pkt key: %1 %2 %3")
                  .arg(pkt.flags & AV_PKT_FLAG_KEY)
                  .arg(pkt.size)
                  .arg(counter++));
        uint8_t* size = ((uint8_t*)pkt.data);
        emit info(QString("first: %1 %2 %3 %4 %5 %6 %7 %8")
                  .arg((int)size[0])
                  .arg((int)size[1])
                  .arg((int)size[2])
                  .arg((int)size[3])
                  .arg((int)size[4])
                  .arg((int)size[5])
                  .arg((int)size[6])
                  .arg((int)size[7]));
        av_interleaved_write_frame(ofctx, &pkt);
        av_packet_unref(&pkt);
    }
}
//--------------------------------------------------------------------------------
void Tester::finish()
{
    //DELAYED FRAMES
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    for (;;)
    {
        avcodec_send_frame(cctx, NULL);
        if (avcodec_receive_packet(cctx, &pkt) == 0)
        {
            av_interleaved_write_frame(ofctx, &pkt);
            av_packet_unref(&pkt);
        }
        else
        {
            break;
        }
    }

    av_write_trailer(ofctx);
    if (!(oformat->flags & AVFMT_NOFILE))
    {
        int err = avio_close(ofctx->pb);
        if (err < 0)
        {
            emit error(QString("Failed to close file: %1").arg(err));
        }
    }
}
//--------------------------------------------------------------------------------
void Tester::free()
{
    if (videoFrame)
    {
        av_frame_free(&videoFrame);
    }
    if (cctx)
    {
        avcodec_free_context(&cctx);
    }
    if (ofctx)
    {
        avformat_free_context(ofctx);
    }
    if (swsCtx)
    {
        sws_freeContext(swsCtx);
    }
}
//--------------------------------------------------------------------------------
bool Tester::test(void)
{
#if 0
    av_register_all();
    avcodec_register_all();

    oformat = av_guess_format(nullptr, "test.mp4", nullptr);
    if (!oformat)
    {
        emit error("can't create output format");
        return false;
    }
    //oformat->video_codec = AV_CODEC_ID_H265;

    int err = avformat_alloc_output_context2(&ofctx, oformat, nullptr, "test.mp4");

    if (err)
    {
        emit error("can't create output context");
        return false;
    }

    AVCodec* codec = nullptr;

    codec = avcodec_find_encoder(oformat->video_codec);
    if (!codec)
    {
        emit error("can't create codec");
        return false;
    }

    AVStream* stream = avformat_new_stream(ofctx, codec);

    if (!stream)
    {
        emit error("can't find format");
        return false;
    }

    cctx = avcodec_alloc_context3(codec);

    if (!cctx)
    {
        emit error("can't create codec context");
        return false;
    }

    stream->codecpar->codec_id = oformat->video_codec;
    stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    stream->codecpar->width = width;
    stream->codecpar->height = height;
    stream->codecpar->format = AV_PIX_FMT_YUV420P;
    stream->codecpar->bit_rate = bitrate * 1000;
    avcodec_parameters_to_context(cctx, stream->codecpar);
    cctx->time_base = (AVRational){ 1, 1 };
    cctx->max_b_frames = 2;
    cctx->gop_size = 12;
    cctx->framerate = (AVRational){ fps, 1 };
    //must remove the following
    //cctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    if (stream->codecpar->codec_id == AV_CODEC_ID_H264)
    {
        av_opt_set(cctx, "preset", "ultrafast", 0);
    }
    else if (stream->codecpar->codec_id == AV_CODEC_ID_H265)
    {
        av_opt_set(cctx, "preset", "ultrafast", 0);
    }

    avcodec_parameters_from_context(stream->codecpar, cctx);

    if ((err = avcodec_open2(cctx, codec, NULL)) < 0)
    {
        emit error(QString("Failed to open codec: %1").arg(err));
        return false;
    }

    if (!(oformat->flags & AVFMT_NOFILE))
    {
        if ((err = avio_open(&ofctx->pb, "test.mp4", AVIO_FLAG_WRITE)) < 0)
        {
            emit error(QString("Failed to open file: %1").arg(err));
            return false;
        }
    }

    if ((err = avformat_write_header(ofctx, NULL)) < 0)
    {
        emit error(QString("Failed to write header: %1").arg(err));
        return false;
    }

    av_dump_format(ofctx, 0, "test.mp4", 1);

    uint8_t* frameraw = new uint8_t[1920 * 1080 * 4];
    memset(frameraw, 222, 1920 * 1080 * 4);
    for (int i=0; i<60; ++i)
    {
        pushFrame(frameraw);
    }

    delete[] frameraw;
    finish();
    free();
#endif
}
//--------------------------------------------------------------------------------
