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
#ifndef SOUND_WIDGET_HPP
#define SOUND_WIDGET_HPP
//---------------------------------------------------------------------------
#include <qmath.h>
//---------------------------------------------------------------------------
#include <QAudioOutput>
#include <QByteArray>
#include <QComboBox>
#include <QIODevice>
#include <QSpinBox>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "generator.h"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Sound_widget;
}
//---------------------------------------------------------------------------
class Generator_Curve;
//---------------------------------------------------------------------------
class Sound_widget : public MyWidget
{
    Q_OBJECT

public:
    Sound_widget(QWidget *parent = nullptr);
    virtual ~Sound_widget();

    void set_generator(Generator_Curve *gen);
    QByteArray get_m_buffer(void);

private:
    void initializeWindow(void);
    void initializeAudio(void);
    void createAudioOutput(void);

private slots:
    void pushTimerExpired(void);
    void deviceChanged(int index);

    void calc_freq(void);
    void regenerate(void);
    void start(void);
    void stop(void);

    void test(void);

private:
    Ui::Sound_widget *ui;
    QTimer *m_pushTimer = nullptr;

    Generator_Curve *generator = nullptr;

    QAudioDeviceInfo m_device;
    Generator *m_generator = nullptr;
    QAudioOutput *m_audioOutput = nullptr;
    QIODevice *m_output = nullptr; // not owned
    QAudioFormat m_format;

    QByteArray m_buffer;

    void load_QDoubleSpinBox(QString group_name);
    void save_QDoubleSpinBox(QString group_name);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//---------------------------------------------------------------------------
#endif // SOUND_WIDGET_HPP
