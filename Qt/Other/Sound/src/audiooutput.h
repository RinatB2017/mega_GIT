/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H
//---------------------------------------------------------------------------
#include <qmath.h>
//---------------------------------------------------------------------------
#include <QAudioOutput>
#include <QByteArray>
#include <QComboBox>
#include <QIODevice>
#include <QSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
//---------------------------------------------------------------------------
#include "generator.h"
//---------------------------------------------------------------------------
class AudioTest : public QMainWindow
{
    Q_OBJECT

public:
    AudioTest();
    ~AudioTest();

private:
    void initializeWindow(void);
    void initializeAudio(void);
    void createAudioOutput(void);

private:
    QTimer *m_pushTimer = 0;

    // Owned by layout
    QComboBox *m_deviceBox = 0;

    QSlider *m_left_volume = 0;
    QSlider *m_right_volume = 0;

    QAudioDeviceInfo m_device;
    Generator *m_generator = 0;
    QAudioOutput *m_audioOutput = 0;
    QIODevice *m_output = 0; // not owned
    QAudioFormat m_format;

    //---
    QDoubleSpinBox *sb_sampleRate1 = 0;
    QDoubleSpinBox *sb_sampleRate2 = 0;
    //---
    QDoubleSpinBox *sb_base_freq = 0;
    QDoubleSpinBox *sb_beats_freq = 0;
    //---
    void load_QDoubleSpinBox(QString group_name);
    void save_QDoubleSpinBox(QString group_name);

    QByteArray m_buffer;

private slots:
    void pushTimerExpired(void);
    void deviceChanged(int index);

    void calc_freq(void);

    void test(void);

};
//---------------------------------------------------------------------------
#endif // AUDIOOUTPUT_H
