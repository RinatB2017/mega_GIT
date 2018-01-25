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
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#include "oscilloscope_GDS_840C.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    oscilloscope = new Oscilloscope_GDS_840C(this);
    oscilloscope->setMinimumSize(800, 600);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(oscilloscope);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info(tr("Тесты начаты"));

    bool ok = false;
#if 1
    ok = oscilloscope->set_TIMebase_SCALe(Oscilloscope_GDS_840C::HScale_250ms);
    if(!ok) emit error("set_TIMebase_SCALe return false");

    ok = oscilloscope->set_CHANnel_SCALe(Oscilloscope_GDS_840C::A, Oscilloscope_GDS_840C::VScale_100mV);
    if(!ok) emit error("set_CHANnel_SCALe return false");
    ok = oscilloscope->set_CHANnel_SCALe(Oscilloscope_GDS_840C::B, Oscilloscope_GDS_840C::VScale_100mV);
    if(!ok) emit error("set_CHANnel_SCALe return false");

    ok = oscilloscope->set_CHANnel_DISPlay(Oscilloscope_GDS_840C::A, true);
    if(!ok) emit error("set_CHANnel_DISPlay return false");
    ok = oscilloscope->set_CHANnel_DISPlay(Oscilloscope_GDS_840C::B, true);
    if(!ok) emit error("set_CHANnel_DISPlay return false");

    ok = oscilloscope->set_CHANnel_COUPling(Oscilloscope_GDS_840C::A, Oscilloscope_GDS_840C::AC);
    if(!ok) emit error("set_CHANnel_COUPling return false");
    ok = oscilloscope->set_CHANnel_COUPling(Oscilloscope_GDS_840C::B, Oscilloscope_GDS_840C::DC);
    if(!ok) emit error("set_CHANnel_COUPling return false");

    ok = oscilloscope->set_CHANnel_INVert(Oscilloscope_GDS_840C::A, true);
    if(!ok) emit error("set_CHANnel_INVert return false");
    ok = oscilloscope->set_CHANnel_INVert(Oscilloscope_GDS_840C::B, true);
    if(!ok) emit error("set_CHANnel_INVert return false");

    ok = oscilloscope->set_CHANnel_BWLimit(Oscilloscope_GDS_840C::A, true);
    if(!ok) emit error("set_CHANnel_BWLimit return false");
    ok = oscilloscope->set_CHANnel_BWLimit(Oscilloscope_GDS_840C::B, true);
    if(!ok) emit error("set_CHANnel_BWLimit return false");

    ok = oscilloscope->set_CHANnel_PROBe(Oscilloscope_GDS_840C::A, Oscilloscope_GDS_840C::X1);
    if(!ok) emit error("set_CHANnel_PROBe return false");
    ok = oscilloscope->set_CHANnel_PROBe(Oscilloscope_GDS_840C::B, Oscilloscope_GDS_840C::X1);
    if(!ok) emit error("set_CHANnel_PROBe return false");
#endif

#if 0
    bool get_ESE(unsigned char *value);
    bool get_ESR(int *result);
    bool get_IDN(QByteArray *output);
    bool get_LRN(QByteArray *output);
    bool get_OPC(QByteArray *output);
    bool get_SRE(unsigned char *value);
    bool get_STB(int *result);
    bool get_ACQuire_AVERage(int *result);
    bool get_ACQuire_LENGth(int *value);
    bool get_ACQuire_MODe(int *result);
    bool get_ACQuire_POINt(Channel channel, QByteArray *output);
    bool get_CHANnel_BWLimit(Channel channel, int *result);
    bool get_CHANnel_COUPling(Channel channel, int *result);
    bool get_CHANnel_INVert(Channel channel, int *result);
    bool get_CHANnel_MATH(Channel channel, int *result);
    bool get_CHANnel_OFFSet(Channel channel, int *result);
    bool get_CHANnel_PROBe(Channel channel, int *result);
    bool get_CHANnel_SCALe(Channel channel, int *result);
    bool get_CURSor_DELta(Channel channel, int *result);
    bool get_CURSor_SOURce(int *result);
    bool get_DISPlay_ACCumulate(int *result);
    bool get_DISPlay_CONTrast(int *value);
    bool get_DISPlay_GRATicule(int *result);
    bool get_DISPlay_WAVeform(int *result);
    bool get_MEASure_FALL(float *result);
    bool get_MEASure_FREQuency(float *result);
    bool get_MEASure_NWIDth(float *result);
    bool get_MEASure_PDUTy(float *result);
    bool get_MEASure_PERiod(float *result);
    bool get_MEASure_PWIDth(float *result);
    bool get_MEASure_RISe(float *result);
    bool get_MEASure_VAMPlitude(float *result);
    bool get_MEASure_VAVerage(float *result);
    bool get_MEASure_VHI(float *result);
    bool get_MEASure_VLO(float *result);
    bool get_MEASure_VMAX(float *result);
    bool get_MEASure_VMIN(float *result);
    bool get_MEASure_VPP(float *result);
    bool get_MEASure_VRMS(float *result);
    bool get_TIMebase_DELay(int *result);
    bool get_TIMebase_SCALe(int *result);
    bool get_TIMebase_SWEep(int *result);
    bool get_TIMebase_WINDow_DELay(int *result);
    bool get_TIMebase_WINDow_SCALe(int *result);
    bool get_TRIGger_COUPle(int *result);
    bool get_TRIGger_DELay_TIMe(int *result);
    bool get_TRIGger_DELay_EVENt(int *result);
    bool get_TRIGger_DELay_LEVel(int *result);
    bool get_TRIGger_DELay_MODe(int *result);
    bool get_TRIGger_DELay_TYPe(int *result);
    bool get_TRIGger_FREQuency(int *result);
    bool get_TRIGger_LEVel(int *result);
    bool get_TRIGger_MODe(int *result);
    bool get_TRIGger_NREJ(int *result);
    bool get_TRIGger_PULSe_MODe(int *result);
    bool get_TRIGger_PULSe_TIMe(int *result);
    bool get_TRIGger_REJect(int *result);
    bool get_TRIGger_SLOP(int *result);
    bool get_TRIGger_SOURce(int *result);
    bool get_TRIGger_TYPe(int *result);
    bool get_TRIGger_VIDeo_FIELd(int *result);
    bool get_TRIGger_VIDeo_LINe(int *result);
    bool get_TRIGger_VIDeo_POLarity(int *result);
    bool get_TRIGger_VIDeo_TYPe(int *result);
#endif

#if 0
    oscilloscope->test_all();
#endif

    //oscilloscope->set_DISPlay_CONTrast(10);
    emit info(tr("Тесты завершены"));
}
//--------------------------------------------------------------------------------
void MainBox::set_auto(void)
{
    oscilloscope->set_AUToset();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
