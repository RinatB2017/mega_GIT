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
#ifndef FAKE_GDS_840C_HPP
#define FAKE_GDS_840C_HPP
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include <QFrame>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Fake_oscilloscope_GDS_840C : public MyWidget
{
    Q_OBJECT
public:
    Fake_oscilloscope_GDS_840C(QWidget *parent = nullptr);

    bool isOpen(void);
    void close(void);
    void setPort(QSerialPortInfo info);
    bool setBaudRate(int speed);

    bool open(QIODevice::OpenModeFlag mode);

    QByteArray readAll(void);

    void write(QByteArray data);
    void write(const char *data, int len);

signals:
    void readyRead(void);
    void error(QSerialPort::SerialPortError);

    void output(const QByteArray &);

public slots:
    void receive(const QVariant &data);

private:
    QByteArray ba_input;
    QByteArray ba_output;

    bool is_open = false;

    void init(void);

    void check_CLS(QByteArray data);
    void check_set_ESE(QByteArray data);
    void check_get_ESE(QByteArray data);
    void check_get_ESR(QByteArray data);
    void check_get_IDN(QByteArray data);
    void check_get_LRN(QByteArray data);
    void check_get_OPC(QByteArray data);
    void check_set_OPC(QByteArray data);
    void check_set_RCL(QByteArray data);
    void check_set_RST(QByteArray data);
    void check_set_SAV(QByteArray data);
    void check_get_SRE(QByteArray data);
    void check_set_SRE(QByteArray data);
    void check_get_STB(QByteArray data);
    void check_set_WAI(QByteArray data);
    void check_set_ACQuire_AVERage(QByteArray data);
    void check_get_ACQuire_AVERage(QByteArray data);
    void check_set_ACQuire_LENGth(QByteArray data);
    void check_get_ACQuire_LENGth(QByteArray data);
    void check_set_ACQuire_MODe(QByteArray data);
    void check_get_ACQuire_MODe(QByteArray data);
    void check_get_ACQuire_POINt(QByteArray data);
    void check_set_CHANnel_BWLimit(QByteArray data);
    void check_get_CHANnel_BWLimit(QByteArray data);
    void check_set_CHANnel_COUPling(QByteArray data);
    void check_get_CHANnel_COUPling(QByteArray data);
    void check_set_CHANnel_DISPlay(QByteArray data);
    void check_set_CHANnel_INVert(QByteArray data);
    void check_get_CHANnel_INVert(QByteArray data);
    void check_set_CHANnel_MATH(QByteArray data);
    void check_get_CHANnel_MATH(QByteArray data);
    void check_set_CHANnel_OFFSet(QByteArray data);
    void check_get_CHANnel_OFFSet(QByteArray data);
    void check_set_CHANnel_PROBe(QByteArray data);
    void check_get_CHANnel_PROBe(QByteArray data);
    void check_set_CHANnel_SCALe(QByteArray data);
    void check_get_CHANnel_SCALe(QByteArray data);
    void check_set_CURSor_XPosition(QByteArray data);
    void check_set_CURSor_YPosition(QByteArray data);
    void check_get_CURSor_DELta(QByteArray data);
    void check_set_CURSor_SOURce(QByteArray data);
    void check_get_CURSor_SOURce(QByteArray data);
    void check_set_DISPlay_ACCumulate(QByteArray data);
    void check_get_DISPlay_ACCumulate(QByteArray data);
    void check_set_DISPlay_CONTrast(QByteArray data);
    void check_get_DISPlay_CONTrast(QByteArray data);
    void check_set_DISPlay_GRATicule(QByteArray data);
    void check_get_DISPlay_GRATicule(QByteArray data);
    void check_get_DISPlay_WAVeform(QByteArray data);
    void check_set_DISPlay_WAVeform(QByteArray data);
    void check_get_MEASure_FALL(QByteArray data);
    void check_get_MEASure_FREQuency(QByteArray data);
    void check_get_MEASure_NWIDth(QByteArray data);
    void check_get_MEASure_PDUTy(QByteArray data);
    void check_get_MEASure_PERiod(QByteArray data);
    void check_get_MEASure_PWIDth(QByteArray data);
    void check_get_MEASure_RISe(QByteArray data);
    void check_set_MEASure_SOURce(QByteArray data);
    void check_get_MEASure_VAMPlitude(QByteArray data);
    void check_get_MEASure_VAVerage(QByteArray data);
    void check_get_MEASure_VHI(QByteArray data);
    void check_get_MEASure_VLO(QByteArray data);
    void check_get_MEASure_VMAX(QByteArray data);
    void check_get_MEASure_VMIN(QByteArray data);
    void check_get_MEASure_VPP(QByteArray data);
    void check_get_MEASure_VRMS(QByteArray data);
    void check_set_SYSTem_UNLock(QByteArray data);
    void check_set_TIMebase_DELay(QByteArray data);
    void check_get_TIMebase_DELay(QByteArray data);
    void check_set_TIMebase_SCALe(QByteArray data);
    void check_get_TIMebase_SCALe(QByteArray data);
    void check_set_TIMebase_SWEep(QByteArray data);
    void check_get_TIMebase_SWEep(QByteArray data);
    void check_set_TIMebase_WINDow_DELay(QByteArray data);
    void check_get_TIMebase_WINDow_DELay(QByteArray data);
    void check_set_TIMebase_WINDow_SCALe(QByteArray data);
    void check_get_TIMebase_WINDow_SCALe(QByteArray data);
    void check_set_TRIGger_COUPle(QByteArray data);
    void check_get_TRIGger_COUPle(QByteArray data);
    void check_set_TRIGger_DELay_TIMe(QByteArray data);
    void check_get_TRIGger_DELay_TIMe(QByteArray data);
    void check_set_TRIGger_DELay_EVENt(QByteArray data);
    void check_get_TRIGger_DELay_EVENt(QByteArray data);
    void check_set_TRIGger_DELay_LEVel(QByteArray data);
    void check_get_TRIGger_DELay_LEVel(QByteArray data);
    void check_set_TRIGger_DELay_MODe(QByteArray data);
    void check_get_TRIGger_DELay_MODe(QByteArray data);
    void check_set_TRIGger_DELay_TYPe(QByteArray data);
    void check_get_TRIGger_DELay_TYPe(QByteArray data);
    void check_get_TRIGger_FREQuency(QByteArray data);
    void check_set_TRIGger_LEVel(QByteArray data);
    void check_get_TRIGger_LEVel(QByteArray data);
    void check_set_TRIGger_MODe(QByteArray data);
    void check_get_TRIGger_MODe(QByteArray data);
    void check_set_TRIGger_NREJ(QByteArray data);
    void check_get_TRIGger_NREJ(QByteArray data);
    void check_set_TRIGger_PULSe_MODe(QByteArray data);
    void check_get_TRIGger_PULSe_MODe(QByteArray data);
    void check_set_TRIGger_PULSe_TIMe(QByteArray data);
    void check_get_TRIGger_PULSe_TIMe(QByteArray data);
    void check_set_TRIGger_REJect(QByteArray data);
    void check_get_TRIGger_REJect(QByteArray data);
    void check_set_TRIGger_SLOP(QByteArray data);
    void check_get_TRIGger_SLOP(QByteArray data);
    void check_set_TRIGger_SOURce(QByteArray data);
    void check_get_TRIGger_SOURce(QByteArray data);
    void check_set_TRIGger_TYPe(QByteArray data);
    void check_get_TRIGger_TYPe(QByteArray data);
    void check_set_TRIGger_VIDeo_FIELd(QByteArray data);
    void check_get_TRIGger_VIDeo_FIELd(QByteArray data);
    void check_set_TRIGger_VIDeo_LINe(QByteArray data);
    void check_get_TRIGger_VIDeo_LINe(QByteArray data);
    void check_set_TRIGger_VIDeo_POLarity(QByteArray data);
    void check_get_TRIGger_VIDeo_POLarity(QByteArray data);
    void check_set_TRIGger_VIDeo_TYPe(QByteArray data);
    void check_get_TRIGger_VIDeo_TYPe(QByteArray data);
    void check_set_WMEMory_DISPlay(QByteArray data);
    void check_set_WMEMory_ERASe(QByteArray data);
    void check_set_WMEMory_LOCate(QByteArray data);
    void check_set_WMEMory_OFFSet(QByteArray data);
    void check_set_WMEMory_SAVe(QByteArray data);
    void check_set_CURSor_XDISplay(QByteArray data);
    void check_set_CURSor_YDISplay(QByteArray data);
    void check_set_PRINt(QByteArray data);
    void check_set_REFResh(QByteArray data);
    void check_set_RUN(QByteArray data);
    void check_set_STOP(QByteArray data);
    void check_set_AUToset(QByteArray data);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
