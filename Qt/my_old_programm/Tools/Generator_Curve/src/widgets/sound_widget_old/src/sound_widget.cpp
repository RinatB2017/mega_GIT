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
#include <QVBoxLayout>
#include <QSettings>
#include <QGroupBox>
#include <QDebug>

#define PUSH_MODE_LABEL "Enable push mode"
#define PULL_MODE_LABEL "Enable pull mode"
#define SUSPEND_LABEL   "Suspend playback"
#define RESUME_LABEL    "Resume playback"
#define VOLUME_LABEL    "Volume:"

const int DurationSeconds   = 1;
const int DataSampleRateHz  = 44100;
const int BufferSize        = 32768;
//---------------------------------------------------------------------------
#define APPNAME "audiooutput"
//---------------------------------------------------------------------------
#include "generator_curve.hpp"
#include "grapherbox.hpp"
#include "sound_widget.hpp"
#include "ui_sound_widget.h"
//---------------------------------------------------------------------------
Sound_widget::Sound_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Sound_widget)
{
    ui->setupUi(this);

    m_pushTimer = new QTimer(this);
    m_device = QAudioDeviceInfo::defaultOutputDevice();

    m_buffer.resize(BufferSize);

    connect(ui->btn_test,   SIGNAL(clicked(void)),  this,   SLOT(test(void)));

    initializeWindow();
    initializeAudio();
}
//---------------------------------------------------------------------------
void Sound_widget::initializeWindow(void)
{
    QScopedPointer<QWidget> window(new QWidget);
    QScopedPointer<QVBoxLayout> layout(new QVBoxLayout);

    const QAudioDeviceInfo &defaultDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    ui->m_deviceBox->addItem(defaultDeviceInfo.deviceName(), qVariantFromValue(defaultDeviceInfo));
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        if (deviceInfo != defaultDeviceInfo)
        {
            ui->m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
        }
    }
    connect(ui->m_deviceBox,    SIGNAL(activated(int)), SLOT(deviceChanged(int)));

    ui->m_left_volume->setRange(0, 100);
    ui->m_right_volume->setRange(0, 100);

    ui->m_freq->setRange(10, 20000);

    ui->sb_freq->setRange(10, 20000);
    connect(ui->m_freq,     SIGNAL(valueChanged(int)),  ui->sb_freq,    SLOT(setValue(int)));
    connect(ui->sb_freq,    SIGNAL(valueChanged(int)),  ui->m_freq,     SLOT(setValue(int)));

    ui->m_freq->setValue(1000);

    connect(ui->m_freq,         SIGNAL(valueChanged(int)),  this,   SLOT(regenerate()));
    connect(ui->m_left_volume,  SIGNAL(valueChanged(int)),  this,   SLOT(regenerate()));
    connect(ui->m_right_volume, SIGNAL(valueChanged(int)),  this,   SLOT(regenerate()));

    connect(ui->btn_start,  SIGNAL(clicked()),  this,   SLOT(start()));
    connect(ui->btn_stop,   SIGNAL(clicked()),  this,   SLOT(stop()));

    ui->m_left_volume->setValue(100);
    ui->m_right_volume->setValue(100);

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
}
//---------------------------------------------------------------------------
void Sound_widget::regenerate(void)
{
    if(m_generator)
    {
        m_generator->generateData(m_format,
                                  DurationSeconds*1000000,
                                  ui->m_freq->value(),
                                  ui->m_freq->value(),
                                  ui->m_left_volume->value(),
                                  ui->m_right_volume->value());
    }
}
//---------------------------------------------------------------------------
void Sound_widget::calc_freq(void)
{

}
//---------------------------------------------------------------------------
void Sound_widget::load_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toDouble());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//---------------------------------------------------------------------------
void Sound_widget::save_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//---------------------------------------------------------------------------
void Sound_widget::initializeAudio(void)
{
    connect(m_pushTimer, SIGNAL(timeout()), SLOT(pushTimerExpired()));

    m_format.setSampleRate(DataSampleRateHz);
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(m_format))
    {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    load_QDoubleSpinBox("audio");
    m_generator = new Generator(m_format,
                                DurationSeconds*1000000,
                                ui->sb_freq->value(),
                                ui->sb_freq->value(),
                                ui->m_left_volume->value(),
                                ui->m_right_volume->value(),
                                this);
    createAudioOutput();
}
//--------------------------------------------------------------------------------
void Sound_widget::set_generator(Generator_Curve *gen)
{
    generator = gen;
}
//---------------------------------------------------------------------------
void Sound_widget::createAudioOutput(void)
{
    delete m_audioOutput;
    m_audioOutput = nullptr;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
}
//---------------------------------------------------------------------------
void Sound_widget::start(void)
{
    m_generator->start();
    m_audioOutput->start(m_generator);
}
//---------------------------------------------------------------------------
void Sound_widget::stop(void)
{
    m_audioOutput->stop();
    m_generator->stop();
}
//---------------------------------------------------------------------------
Sound_widget::~Sound_widget()
{
    m_generator->stop();
    m_audioOutput->stop();

    save_QDoubleSpinBox("audio");
}
//---------------------------------------------------------------------------
void Sound_widget::deviceChanged(int index)
{
    m_pushTimer->stop();
    m_generator->stop();
    m_audioOutput->stop();
    m_audioOutput->disconnect(this);
    m_device = ui->m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
    createAudioOutput();
}
//---------------------------------------------------------------------------
QByteArray Sound_widget::get_m_buffer(void)
{
    return m_buffer;
}
//---------------------------------------------------------------------------
#include "grapherbox.hpp"
void Sound_widget::test(void)
{
    GrapherBox *grapher_widget = new GrapherBox();
    int curve_0 = grapher_widget->add_curve("test");

    struct TEMP {
        uint16_t data[BufferSize / 2];
    };

#if 0
    QByteArray ba = generator->get_data();
    TEMP* temp = reinterpret_cast<TEMP *>(ba.data());

    for(int n=0; n<(ba.size() / 2); n++)
    {
        emit debug(QString("%1").arg(temp->data[n]));
        grapher_widget->add_curve_data(curve_0, temp->data[n]);
    }

#endif

#if 1
    TEMP* temp = reinterpret_cast<TEMP *>(m_buffer.data());

    for(int n=0; n<(m_buffer.count() / 2); n++)
    {
        emit debug(QString("%1").arg(temp->data[n]));
        grapher_widget->add_curve_data(curve_0, temp->data[n]);
    }
#endif
    
    grapher_widget->show();
}
//--------------------------------------------------------------------------------
void Sound_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Sound_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Sound_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Sound_widget::save_setting(void)
{

}
//---------------------------------------------------------------------------
void Sound_widget::pushTimerExpired(void)
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState)
    {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        while (chunks)
        {
            const qint64 len = m_generator->readData(m_buffer.data(), m_audioOutput->periodSize());
            if (len)
            {
                m_output->write(m_buffer.data(), len);
            }
            if (len != m_audioOutput->periodSize())
            {
                break;
            }
            --chunks;
        }
    }
}
//---------------------------------------------------------------------------
