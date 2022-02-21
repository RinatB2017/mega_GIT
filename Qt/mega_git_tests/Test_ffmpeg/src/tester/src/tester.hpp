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
#ifndef TESTER_HPP
#define TESTER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QString>
//--------------------------------------------------------------------------------
#include <iostream>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavutil/time.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
}
//--------------------------------------------------------------------------------
class Tester : public QWidget
{
    Q_OBJECT

public:
    explicit Tester(QWidget *parent = nullptr);
    virtual ~Tester();

    bool test(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    void pushFrame(uint8_t* data);
    void finish();
    void free();

    AVFrame* videoFrame = nullptr;
    AVCodecContext* cctx = nullptr;
    SwsContext* swsCtx = nullptr;
    int frameCounter = 0;
    AVFormatContext* ofctx = nullptr;
    AVOutputFormat* oformat = nullptr;

    int fps = 30;
    int width   = 1920;
    int height  = 1080;
    int bitrate = 2000;
};
//--------------------------------------------------------------------------------
#endif // TESTER_HPP
