/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QtEndian>
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
enum CURVE {
    DOTS = 0,
    LINES,
    SPLINE_LINES
};

// Wav Header
struct wav_header_t
{
    char chunkId[4];        // "RIFF" = 0x46464952
    quint32 chunkSize;      // 28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
    char format[4];         // "WAVE" = 0x45564157
    char subchunk1ID[4];    // "fmt " = 0x20746D66
    quint32 subchunk1Size;  // 16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
    quint16 audioFormat;
    quint16 numChannels;
    quint32 sampleRate;
    quint32 byteRate;
    quint16 blockAlign;
    quint16 bitsPerSample;
};
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class PlotPicker;
class GrapherBox;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void choice_test(void);
    void test(void);
    void read_wav(void);
    void test_load(void);
    void test_save(void);
    void test0(void);
    void test1(void);
    void test2(void);
    void test3(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        void (MainBox::*func)(void);
    } CMD_t;
    QList<CMD> commands;
    QPointer<QComboBox> cb_test;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;
    bool all_break = false;

#ifdef ONE_CURVE
    int curve_0 = 0;
#else
    int curves[MAX_CHANNELS];
#endif

    wav_header_t wavHeader;

    void init(void);
    void createTestBar(void);

    void readWAV(const QString &wavFile);
    void ReadWav(const QString &fileName);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

    void test_data(void);
    void test_data2(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
