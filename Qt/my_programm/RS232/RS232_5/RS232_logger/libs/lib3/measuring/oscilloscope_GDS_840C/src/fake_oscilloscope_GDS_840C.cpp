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
#include <QByteArray>
#include <QVariant>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "fake_oscilloscope_GDS_840C.hpp"
//--------------------------------------------------------------------------------
Fake_oscilloscope_GDS_840C::Fake_oscilloscope_GDS_840C(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::init(void)
{

}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::isOpen(void)
{
#ifdef QT_DEBUG
    qDebug() << "isOpen" << is_open;
#endif
    return is_open;
}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::open(QIODevice::OpenModeFlag mode)
{
    Q_UNUSED(mode);

#ifdef QT_DEBUG
    qDebug() << "open";
#endif
    is_open = true;
    return is_open;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::close(void)
{
    is_open = false;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::setPort(QSerialPortInfo info)
{
#ifdef QT_DEBUG
    qDebug() << "setPort" << info.portName();
#endif
    Q_UNUSED(info)
}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::setBaudRate(int speed)
{
#ifdef QT_DEBUG
    qDebug() << "setBaudRate" << speed;
#endif
    Q_UNUSED(speed)
    return true;
}
//--------------------------------------------------------------------------------
QByteArray Fake_oscilloscope_GDS_840C::readAll(void)
{
    return ba_output;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::write(QByteArray data)
{
    if(data.isEmpty()) return;

#ifdef QT_DEBUG
    qDebug() << "data" << data;
#endif

    check_CLS(data);
    check_set_ESE(data);
    check_get_ESE(data);
    check_get_ESR(data);
    check_get_IDN(data);
    check_get_LRN(data);
    check_get_OPC(data);
    check_set_OPC(data);
    check_set_RCL(data);
    check_set_RST(data);
    check_set_SAV(data);
    check_get_SRE(data);
    check_set_SRE(data);
    check_get_STB(data);
    check_set_WAI(data);
    check_set_ACQuire_AVERage(data);
    check_get_ACQuire_AVERage(data);
    check_set_ACQuire_LENGth(data);
    check_get_ACQuire_LENGth(data);
    check_set_ACQuire_MODe(data);
    check_get_ACQuire_MODe(data);
    check_get_ACQuire_POINt(data);
    check_set_CHANnel_BWLimit(data);
    check_get_CHANnel_BWLimit(data);
    check_set_CHANnel_COUPling(data);
    check_get_CHANnel_COUPling(data);
    check_set_CHANnel_DISPlay(data);
    check_set_CHANnel_INVert(data);
    check_get_CHANnel_INVert(data);
    check_set_CHANnel_MATH(data);
    check_get_CHANnel_MATH(data);
    check_set_CHANnel_OFFSet(data);
    check_get_CHANnel_OFFSet(data);
    check_set_CHANnel_PROBe(data);
    check_get_CHANnel_PROBe(data);
    check_set_CHANnel_SCALe(data);
    check_get_CHANnel_SCALe(data);
    check_set_CURSor_XPosition(data);
    check_set_CURSor_YPosition(data);
    check_get_CURSor_DELta(data);
    check_set_CURSor_SOURce(data);
    check_get_CURSor_SOURce(data);
    check_set_DISPlay_ACCumulate(data);
    check_get_DISPlay_ACCumulate(data);
    check_set_DISPlay_CONTrast(data);
    check_get_DISPlay_CONTrast(data);
    check_set_DISPlay_GRATicule(data);
    check_get_DISPlay_GRATicule(data);
    check_get_DISPlay_WAVeform(data);
    check_set_DISPlay_WAVeform(data);
    check_get_MEASure_FALL(data);
    check_get_MEASure_FREQuency(data);
    check_get_MEASure_NWIDth(data);
    check_get_MEASure_PDUTy(data);
    check_get_MEASure_PERiod(data);
    check_get_MEASure_PWIDth(data);
    check_get_MEASure_RISe(data);
    check_set_MEASure_SOURce(data);
    check_get_MEASure_VAMPlitude(data);
    check_get_MEASure_VAVerage(data);
    check_get_MEASure_VHI(data);
    check_get_MEASure_VLO(data);
    check_get_MEASure_VMAX(data);
    check_get_MEASure_VMIN(data);
    check_get_MEASure_VPP(data);
    check_get_MEASure_VRMS(data);
    check_set_SYSTem_UNLock(data);
    check_set_TIMebase_DELay(data);
    check_get_TIMebase_DELay(data);
    check_set_TIMebase_SCALe(data);
    check_get_TIMebase_SCALe(data);
    check_set_TIMebase_SWEep(data);
    check_get_TIMebase_SWEep(data);
    check_set_TIMebase_WINDow_DELay(data);
    check_get_TIMebase_WINDow_DELay(data);
    check_set_TIMebase_WINDow_SCALe(data);
    check_get_TIMebase_WINDow_SCALe(data);
    check_set_TRIGger_COUPle(data);
    check_get_TRIGger_COUPle(data);
    check_set_TRIGger_DELay_TIMe(data);
    check_get_TRIGger_DELay_TIMe(data);
    check_set_TRIGger_DELay_EVENt(data);
    check_get_TRIGger_DELay_EVENt(data);
    check_set_TRIGger_DELay_LEVel(data);
    check_get_TRIGger_DELay_LEVel(data);
    check_set_TRIGger_DELay_MODe(data);
    check_get_TRIGger_DELay_MODe(data);
    check_set_TRIGger_DELay_TYPe(data);
    check_get_TRIGger_DELay_TYPe(data);
    check_get_TRIGger_FREQuency(data);
    check_set_TRIGger_LEVel(data);
    check_get_TRIGger_LEVel(data);
    check_set_TRIGger_MODe(data);
    check_get_TRIGger_MODe(data);
    check_set_TRIGger_NREJ(data);
    check_get_TRIGger_NREJ(data);
    check_set_TRIGger_PULSe_MODe(data);
    check_get_TRIGger_PULSe_MODe(data);
    check_set_TRIGger_PULSe_TIMe(data);
    check_get_TRIGger_PULSe_TIMe(data);
    check_set_TRIGger_REJect(data);
    check_get_TRIGger_REJect(data);
    check_set_TRIGger_SLOP(data);
    check_get_TRIGger_SLOP(data);
    check_set_TRIGger_SOURce(data);
    check_get_TRIGger_SOURce(data);
    check_set_TRIGger_TYPe(data);
    check_get_TRIGger_TYPe(data);
    check_set_TRIGger_VIDeo_FIELd(data);
    check_get_TRIGger_VIDeo_FIELd(data);
    check_set_TRIGger_VIDeo_LINe(data);
    check_get_TRIGger_VIDeo_LINe(data);
    check_set_TRIGger_VIDeo_POLarity(data);
    check_get_TRIGger_VIDeo_POLarity(data);
    check_set_TRIGger_VIDeo_TYPe(data);
    check_get_TRIGger_VIDeo_TYPe(data);
    check_set_WMEMory_DISPlay(data);
    check_set_WMEMory_ERASe(data);
    check_set_WMEMory_LOCate(data);
    check_set_WMEMory_OFFSet(data);
    check_set_WMEMory_SAVe(data);
    check_set_CURSor_XDISplay(data);
    check_set_CURSor_YDISplay(data);
    check_set_PRINt(data);
    check_set_REFResh(data);
    check_set_RUN(data);
    check_set_STOP(data);
    check_set_AUToset(data);

    emit trace(data);
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::write(const char *data, int len)
{
#ifdef QT_DEBUG
    qDebug() << "write";
#endif

    QByteArray ba;
    ba.clear();
    ba.append(data, len);
    write(ba);
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_CLS(QByteArray data)
{
    if(data.contains("*CLS"))
    {
        emit debug("ничего не надо возвращать");
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_ESE(QByteArray data)
{
    if(data.contains("*ESE "))
    {
        emit debug("ничего не надо возвращать");
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_ESE(QByteArray data)
{
    if(data.contains("*ESE?"))
    {
        ba_output.clear();
        ba_output.append("0");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_ESR(QByteArray data)
{
    if(data.contains("*ESR?"))
    {
        ba_output.clear();
        ba_output.append("0");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_IDN(QByteArray data)
{
    if(data.contains("*IDN?"))
    {
        ba_output.clear();
        ba_output.append("GW,GDS-840C,EI120422,V2.27");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_LRN(QByteArray data)
{
    if(data.contains("*LRN?"))
    {
        ba_output.clear();
        ba_output.append(":ACQuire:AVERage 1;LENGth 500;MODe 0;POINt;:ACQuire:AVERage 1;LENGth 500;MODe 0;POINt;:ACQuire:AVERage 1;LENGth 500;MODe 0;POINt;:TIMebase:DELay 0.000e+00;SCALe 5.000e-06;SWEep 0;WINDow:DELay 0.000e+00;SCALe 1.000e-09;:TIMebase:DELay 0.000e+00;SCALe 5.000e-06;SWEep 0;WINDow:DELay 0.000e+00;SCALe 1.000e-09;:DISPlay:WAVeform 0;DISPCONTrast 10;GRATicule 1;:DISPlay:WAVeform 0;DISPCONTrast 10;GRATicule 1;:MEASure:SOURce 1;FALL 1.377us;FREQuency 43.85kHz;NWIDth 5.196us;PDUTy 77.16%;PERiod 22.80us;PWIDth 17.59us;RISe 17.19us;VAMPlitude 416mV;VAVerage 5.41mV;VHI 352mV;VLO -64.0mV;VMAX 352mV;VMIN -112mV;VPP 464mV;VRMS 107mV;:MEASure:SOURce 2;FALL ?;FREQuency ?;NWIDth ?;PDUTy ?;PERiod ?;PWIDth ?;RISe ?;VAMPlitude chan off!;VAVerage chan off!;VHI chan off!;VLO chan off!;VMAX chan off!;VMIN chan off!;VPP chan off!;VRMS chan off!;:AUToset;:PRINt;:REFResh;:RUN;:STOP");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_OPC(QByteArray data)
{
    if(data.contains("*OPC?"))
    {
        ba_output.clear();
        ba_output.append("1");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_OPC(QByteArray data)
{
    if(data.contains("*OPC?"))
    {
        emit debug("ничего не надо возвращать");
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_RCL(QByteArray data)
{
    if(data.contains("*RCL ")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_RST(QByteArray data)
{
    if(data.contains("*RST")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_SAV(QByteArray data)
{
    if(data.contains("*SAV ")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_SRE(QByteArray data)
{
    if(data.contains("*SRE?")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_SRE(QByteArray data)
{
    if(data.contains("*SRE ")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_STB(QByteArray data)
{
    if(data.contains("*STB?")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_WAI(QByteArray data)
{
    if(data.contains("*WAI")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_ACQuire_AVERage(QByteArray data)
{
    if(data.contains(":ACQuire:AVERage")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_ACQuire_AVERage(QByteArray data)
{
    if(data.contains(":ACQuire:AVERage?")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_ACQuire_LENGth(QByteArray data)
{
    if(data.contains(":ACQuire:LENGth ")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_ACQuire_LENGth(QByteArray data)
{
    if(data.contains(":ACQuire:LENGth?")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_ACQuire_MODe(QByteArray data)
{
    if(data.contains(":ACQuire:MODe ")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_ACQuire_MODe(QByteArray data)
{
    if(data.contains(":ACQuire:MODe?")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_ACQuire_POINt(QByteArray data)
{
    if(data.contains(":ACQuire%1:POINt")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_BWLimit(QByteArray data)
{
    if(data.contains(":CHANnel%1:BWLimit ")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_BWLimit(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_COUPling(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_COUPling(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_DISPlay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_INVert(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_INVert(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_MATH(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_MATH(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_OFFSet(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_OFFSet(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_PROBe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_PROBe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CHANnel_SCALe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CHANnel_SCALe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CURSor_XPosition(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CURSor_YPosition(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CURSor_DELta(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CURSor_SOURce(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_CURSor_SOURce(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_DISPlay_ACCumulate(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_DISPlay_ACCumulate(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_DISPlay_CONTrast(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_DISPlay_CONTrast(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_DISPlay_GRATicule(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_DISPlay_GRATicule(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_DISPlay_WAVeform(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_DISPlay_WAVeform(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_FALL(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_FREQuency(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_NWIDth(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_PDUTy(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_PERiod(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_PWIDth(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_RISe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_MEASure_SOURce(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VAMPlitude(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VAVerage(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VHI(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VLO(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VMAX(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VMIN(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VPP(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_MEASure_VRMS(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_SYSTem_UNLock(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TIMebase_DELay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TIMebase_DELay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TIMebase_SCALe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TIMebase_SCALe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TIMebase_SWEep(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TIMebase_SWEep(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TIMebase_WINDow_DELay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TIMebase_WINDow_DELay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TIMebase_WINDow_SCALe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TIMebase_WINDow_SCALe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_COUPle(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_COUPle(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_DELay_TIMe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_DELay_TIMe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_DELay_EVENt(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_DELay_EVENt(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_DELay_LEVel(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_DELay_LEVel(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_DELay_MODe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_DELay_MODe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_DELay_TYPe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_DELay_TYPe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_FREQuency(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_LEVel(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_LEVel(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_MODe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_MODe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_NREJ(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_NREJ(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_PULSe_MODe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_PULSe_MODe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_PULSe_TIMe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_PULSe_TIMe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_REJect(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_REJect(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_SLOP(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_SLOP(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_SOURce(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_SOURce(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_TYPe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_TYPe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_VIDeo_FIELd(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_VIDeo_FIELd(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_VIDeo_LINe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_VIDeo_LINe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_VIDeo_POLarity(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_VIDeo_POLarity(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_TRIGger_VIDeo_TYPe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_get_TRIGger_VIDeo_TYPe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_WMEMory_DISPlay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_WMEMory_ERASe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_WMEMory_LOCate(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_WMEMory_OFFSet(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_WMEMory_SAVe(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CURSor_XDISplay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_CURSor_YDISplay(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_PRINt(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_REFResh(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_RUN(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_STOP(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::check_set_AUToset(QByteArray data)
{
    if(data.contains("???")) //FIXME надо дописать
    {
        ba_output.clear();
        ba_output.append("!!!");
        ba_output.append(0x0A);

        emit output(ba_output);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::receive(const QVariant &data)
{
    emit trace(data.toString());
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Fake_oscilloscope_GDS_840C::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Fake_oscilloscope_GDS_840C::save_setting(void)
{

}
//--------------------------------------------------------------------------------
