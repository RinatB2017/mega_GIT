/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef OSCILLOSCOPE_GDS_840C_HPP
#define OSCILLOSCOPE_GDS_840C_HPP
//--------------------------------------------------------------------------------
#include <QSerialPort>
#include <QFrame>
//--------------------------------------------------------------------------------
#ifdef FAKE
    #include "fake_oscilloscope_GDS_840C.hpp"
#endif
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
struct OSC_HEADER {
    char octotorp;  //Содержит символ '#'
    char four;      //Содержит символ '4' (количество цифр в следующем поле, размер данных)
    char len[4];    //Содержит символы '1008' (размер данных в байтах, 1000 байт осциллограмма плюс оставшаяся часть заголовка)
    float tdiv;     //Число в формате float, содержащее масштаб по оси времени (?)
    char chan;      //Номер канала в двоичной форме, 1 - первый канал, 2 - второй канал
    char cnt[3];    //Число отсчетов в осциллограмме, всегда 500 (0x0001F4)
    short data[500];
};
union U_INT_BYTES
{
    unsigned int value;
    struct
    {
        unsigned char byte_0;
        unsigned char byte_1;
        unsigned char byte_2;
        unsigned char byte_3;
    } bytes;
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
const QString oscilloscope_answer("GW,GDS-840C,EI120422,V2.27\n");
//--------------------------------------------------------------------------------
namespace Ui {
    class Oscilloscope_GDS_840C;
}
//--------------------------------------------------------------------------------
class GrapherBox;
class QSpinBox;
//--------------------------------------------------------------------------------
class Oscilloscope_GDS_840C : public MyWidget
{
    Q_OBJECT

public:
    explicit Oscilloscope_GDS_840C(QWidget *parent = 0);

    enum Channel {
        A = 1,
        B = 2
    };

    enum CState {
        AC = 0,
        DC = 1,
        GND = 2
    };

    enum Divider {
        X1 = 0,
        X10 = 1,
        X100 = 2
    };

    enum Trigger {
        TTL = 0,
        ECL = 1,
        USR = 2
    };

    enum Average {
        a2 = 1,
        a4 = 2,
        a8  = 3,
        a16 = 4,
        a32 = 5,
        a64 = 6,
        a128 = 7,
        a256 = 8
    };

    enum Source {
        s1 = 1,
        s2 = 2,
        s3 = 3
    };

    enum Graticule {
        Full_grids = 0,
        Cross_type = 1,
        Frame = 2
    };

    enum Measurement_channel {
        Channel_1 = 1,
        Channel_2 = 2
    };

    enum Sweep {
        Main_timebase = 0,
        Window = 1,
        Window_zoom = 2,
        Roll_mode = 3,
        XY_mode = 4
    };

    enum Trigger_couple {
        t_AC = 0,
        t_DC = 1
    };

    enum Type {
        Time_setting = 0,
        Event_setting = 1
    };

    enum Mode {
        Auto_level = 0,
        Auto = 1,
        Normal = 2,
        Single = 3
    };

    enum Reject_mode {
        OFF = 0,
        LO = 1,
        HI = 2
    };

    enum Slope {
        Rising = 0,
        Falling = 1
    };

    enum Trigger_source {
        s_Channel_1 = 0,
        s_Channel_2 = 1,
        s_External_trigger = 2,
        s_AC_line_voltage = 3
    };

    enum Trigger_type {
        Edge = 0,
        Video = 1,
        Pulse = 2,
        Delay = 3
    };

    enum Field {
        Line = 0,
        Odd_frame = 1,
        Even_frame = 2
    };

    enum Polarity {
        Positive = 0,
        Negative = 1
    };

    enum Video_type {
        PAL = 0,
        NTSC = 1,
        SECAM = 2
    };

    enum Ref_memory {
        RefA = 1,
        RefB = 2
    };

    enum Data
    {
        m_Channel_1 = 1,
        m_Channel_2 = 2,
        m_Math_function = 3
    };

    enum Len {
        Len_500 = 500,
        Len_1250 = 1250,
        Len_2500 = 2500,
        Len_5000 = 5000,
        Len_12500 = 12500,
        Len_25000 = 25000,
        Len_50000 = 50000,
        Len_125000 = 125000
    };

    enum VScale {
        VScale_2mV = 0,
        VScale_5mV,
        VScale_10mV,
        VScale_20mV,
        VScale_50mV,
        VScale_100mV,
        VScale_200mV,
        VScale_500mV,
        VScale_1V,
        VScale_2V,
        VScale_5V
    };

    enum HScale {
        HScale_1ns = 0,
        HScale_2_5ns,
        HScale_5ns,
        HScale_10ns,
        HScale_25ns,
        HScale_50ns,
        HScale_100ns,
        HScale_250ns,
        HScale_500ns,

        HScale_1us,
        HScale_2_5us,
        HScale_5us,
        HScale_10us,
        HScale_25us,
        HScale_50us,
        HScale_100us,
        HScale_250us,
        HScale_500us,

        HScale_1ms,
        HScale_2_5ms,
        HScale_5ms,
        HScale_10ms,
        HScale_25ms,
        HScale_50ms,
        HScale_100ms,
        HScale_250ms,

        HScale_1s,
        HScale_2_5s,
        HScale_5s,
        HScale_10s
    };

    bool CLS(void);
    bool set_ESE(unsigned char value);
    bool get_ESE(unsigned char *value);
    bool get_ESR(int *result);
    bool get_IDN(QByteArray *output);
    bool get_LRN(QByteArray *output);
    bool get_OPC(QByteArray *output);
    bool set_OPC(int value);
    bool set_RCL(int value);
    bool set_RST(void);
    bool set_SAV(unsigned int value);
    bool get_SRE(unsigned char *value);
    bool set_SRE(unsigned char value);
    bool get_STB(int *result);
    bool set_WAI(void);
    bool set_ACQuire_AVERage(Average value);
    bool get_ACQuire_AVERage(int *result);
    bool set_ACQuire_LENGth(Len value);
    bool get_ACQuire_LENGth(int *value);
    bool set_ACQuire_MODe(unsigned char value);
    bool get_ACQuire_MODe(int *result);
    bool get_ACQuire_POINt(Channel channel, QByteArray *output);
    bool set_CHANnel_BWLimit(Channel channel, bool state);
    bool get_CHANnel_BWLimit(Channel channel, int *result);
    bool set_CHANnel_COUPling(Channel channel, CState state);
    bool get_CHANnel_COUPling(Channel channel, int *result);
    bool set_CHANnel_DISPlay(Channel channel, bool state);
    bool set_CHANnel_INVert(Channel channel, bool state);
    bool get_CHANnel_INVert(Channel channel, int *result);
    bool set_CHANnel_MATH(Channel channel, bool state);
    bool get_CHANnel_MATH(Channel channel, int *result);
    bool set_CHANnel_OFFSet(Channel channel, int voltage);
    bool get_CHANnel_OFFSet(Channel channel, int *result);
    bool set_CHANnel_PROBe(Channel channel, Divider div);
    bool get_CHANnel_PROBe(Channel channel, int *result);
    bool set_CHANnel_SCALe(Channel channel, VScale scale);
    bool get_CHANnel_SCALe(Channel channel, int *result);
    bool set_CURSor_XPosition(Channel channel, unsigned int pos);
    bool set_CURSor_YPosition(Channel channel, unsigned int pos);
    bool get_CURSor_DELta(Channel channel, int *result);
    bool set_CURSor_SOURce(Source value);
    bool get_CURSor_SOURce(int *result);
    bool set_DISPlay_ACCumulate(bool state);
    bool get_DISPlay_ACCumulate(int *result);
    bool set_DISPlay_CONTrast(int value);
    bool get_DISPlay_CONTrast(int *value);
    bool set_DISPlay_GRATicule(Graticule value);
    bool get_DISPlay_GRATicule(int *result);
    bool get_DISPlay_WAVeform(int *result);
    bool set_DISPlay_WAVeform(int value);
    bool get_MEASure_FALL(float *result);
    bool get_MEASure_FREQuency(float *result);
    bool get_MEASure_NWIDth(float *result);
    bool get_MEASure_PDUTy(float *result);
    bool get_MEASure_PERiod(float *result);
    bool get_MEASure_PWIDth(float *result);
    bool get_MEASure_RISe(float *result);
    bool set_MEASure_SOURce(Measurement_channel value);
    bool get_MEASure_VAMPlitude(float *result);
    bool get_MEASure_VAVerage(float *result);
    bool get_MEASure_VHI(float *result);
    bool get_MEASure_VLO(float *result);
    bool get_MEASure_VMAX(float *result);
    bool get_MEASure_VMIN(float *result);
    bool get_MEASure_VPP(float *result);
    bool get_MEASure_VRMS(float *result);
    bool set_SYSTem_UNLock(void);
    bool set_TIMebase_DELay(float delay);
    bool get_TIMebase_DELay(int *result);
    bool set_TIMebase_SCALe(HScale scale);
    bool get_TIMebase_SCALe(int *result);
    bool set_TIMebase_SWEep(Sweep value);
    bool get_TIMebase_SWEep(int *result);
    bool set_TIMebase_WINDow_DELay(int value);
    bool get_TIMebase_WINDow_DELay(int *result);
    bool set_TIMebase_WINDow_SCALe(int value);
    bool get_TIMebase_WINDow_SCALe(int *result);
    bool set_TRIGger_COUPle(Trigger_couple value);
    bool get_TRIGger_COUPle(int *result);
    bool set_TRIGger_DELay_TIMe(int value);
    bool get_TRIGger_DELay_TIMe(int *result);
    bool set_TRIGger_DELay_EVENt(int value);
    bool get_TRIGger_DELay_EVENt(int *result);
    bool set_TRIGger_DELay_LEVel(int value);
    bool get_TRIGger_DELay_LEVel(int *result);
    bool set_TRIGger_DELay_MODe(Trigger value);
    bool get_TRIGger_DELay_MODe(int *result);
    bool set_TRIGger_DELay_TYPe(Type value);
    bool get_TRIGger_DELay_TYPe(int *result);
    bool get_TRIGger_FREQuency(int *result);
    bool set_TRIGger_LEVel(int value);
    bool get_TRIGger_LEVel(int *result);
    bool set_TRIGger_MODe(Mode value);
    bool get_TRIGger_MODe(int *result);
    bool set_TRIGger_NREJ(bool state);
    bool get_TRIGger_NREJ(int *result);
    bool set_TRIGger_PULSe_MODe(int value);
    bool get_TRIGger_PULSe_MODe(int *result);
    bool set_TRIGger_PULSe_TIMe(int value);
    bool get_TRIGger_PULSe_TIMe(int *result);
    bool set_TRIGger_REJect(Reject_mode value);
    bool get_TRIGger_REJect(int *result);
    bool set_TRIGger_SLOP(Slope value);
    bool get_TRIGger_SLOP(int *result);
    bool set_TRIGger_SOURce(Trigger_source value);
    bool get_TRIGger_SOURce(int *result);
    bool set_TRIGger_TYPe(Trigger_type value);
    bool get_TRIGger_TYPe(int *result);
    bool set_TRIGger_VIDeo_FIELd(Field value);
    bool get_TRIGger_VIDeo_FIELd(int *result);
    bool set_TRIGger_VIDeo_LINe(int value);
    bool get_TRIGger_VIDeo_LINe(int *result);
    bool set_TRIGger_VIDeo_POLarity(Polarity value);
    bool get_TRIGger_VIDeo_POLarity(int *result);
    bool set_TRIGger_VIDeo_TYPe(Video_type value);
    bool get_TRIGger_VIDeo_TYPe(int *result);
    bool set_WMEMory_DISPlay(Channel channel, bool state);
    bool set_WMEMory_ERASe(Channel channel, QByteArray *output);
    bool set_WMEMory_LOCate(Channel channel, int pos, QByteArray *output);
    bool set_WMEMory_OFFSet(Channel channel, int pos, QByteArray *output);
    bool set_WMEMory_SAVe(Ref_memory channel, Data data);

    bool test(unsigned int test_number);
    bool test_all(void);

public slots:
    void set_timeout(int timeout);

    bool measuring_all_channels(void);
    bool measuring_channel_1(void);
    bool measuring_channel_2(void);

    bool set_AUToset(void);
    bool set_PRINt(void);
    bool set_REFResh(void);
    bool set_RUN(void);
    bool set_STOP(void);

    bool set_CURSor_XDISplay(bool state);
    bool set_CURSor_YDISplay(bool state);

private slots:
    bool find_device(void);
    void port_read(void);
    void port_error(QSerialPort::SerialPortError serial_error);

private:
    Ui::Oscilloscope_GDS_840C *ui = 0;

#ifdef FAKE
    Fake_oscilloscope_GDS_840C serial;
#else
    QSerialPort serial;
#endif

    QByteArray data_oscilloscope;
    bool flag_closed;
    bool is_ready;

    GrapherBox *oscilloscope = 0;
    int channel_1;
    int channel_2;
    int timeout;

    void init(void);
    bool find_oscilloscope(void);
    void wait_msec(int timeout_msec);

    bool send(QByteArray input,
              QByteArray *output,
              bool no_answer = false);
};
//--------------------------------------------------------------------------------
#endif
