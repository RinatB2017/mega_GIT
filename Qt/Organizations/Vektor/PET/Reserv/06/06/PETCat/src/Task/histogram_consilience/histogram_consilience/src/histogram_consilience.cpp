/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QSplitter>
#include <QRadioButton>
#include <QPushButton>
#include <QToolButton>
#include <QTextEdit>
#include <QSettings>
#include <QSpinBox>
#include <QPicture>
#include <QtMath>
#include <QTime>
//--------------------------------------------------------------------------------
#include <qwt_picker_machine.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_draw.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
#include "histogram_consilience.hpp"
#include "ui_histogram_consilience.h"
#include "dat3.hpp"
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
#include "grapherbox.hpp"
#include "pet_event_flag.hpp"
//--------------------------------------------------------------------------------
#include "metadata_widget.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_errno.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
class MyScaleDraw : public QwtScaleDraw
{
public:
    MyScaleDraw(double val)
    {
        divider = val;
    }
    QwtText label(double value) const
    {
        return QString("%1").arg(value / divider, 0, 'f', 1);
    }
private:
    double divider = 1.0;
};
//--------------------------------------------
class PlotPicker: public QwtPlotPicker
{
public:
    explicit PlotPicker(int xAxis,
                        int yAxis,
                        RubberBand rubberBand,
                        DisplayMode trackerMode,
                        double val,
                        QWidget * wgt)
        :QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, wgt)
    {
        divider = val;
    }

    QwtText trackerText(const QPoint &point) const
    {
        QwtText text;
        text.setText(QString("%1:%2")
                     .arg(invTransform(point).x() / divider)
                     .arg(invTransform(point).y()));
        return text;
    }
private:
    double divider = 1.0;
};
//--------------------------------------------------------------------------------
Histogram_consilience::Histogram_consilience(QString file_auto_open,
                                             QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Histogram_consilience)
{
    init();

    QVariant sb_nudChANumber = 0;
    QVariant sb_nudChBNumber = 0;
    QVariant dsb_nudChACorr = 0;
    QVariant dsb_nudChBCorr = 0;
    QVariant dsb_nudTInt = 0;

    QVariant dsb_nudWindow = 0;
    QVariant dsb_nudN = 0;

    QVariant flag_channel_a_expanded = 0;
    QVariant flag_channel_b_expanded = 0;
    QVariant flag_ts_expanded = 0;
    QVariant flag_consillence_expanded = 0;
    QVariant flag_analiz_expanded = 0;
    QVariant flag_events_flag_expanded = 0;
    QVariant flag_param_expanded = 0;

    //---
    PETCat_options *o = new PETCat_options();
    o->get_histogram_consulience_option(PARAM_histogram_consulience_sb_nudChANumber,    &sb_nudChANumber);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_sb_nudChBNumber,    &sb_nudChBNumber);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudChACor,      &dsb_nudChACorr);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudChBCor,      &dsb_nudChBCorr);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudTInt,        &dsb_nudTInt);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudWindow,  &dsb_nudWindow);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_dsb_nudN,       &dsb_nudN);

    o->get_histogram_consulience_option(PARAM_histogram_consulience_channel_a_expanded,   &flag_channel_a_expanded);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_channel_b_expanded,   &flag_channel_b_expanded);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_ts_expanded,          &flag_ts_expanded);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_consillence_expanded, &flag_consillence_expanded);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_analiz_expanded,      &flag_analiz_expanded);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_events_flag_expanded, &flag_events_flag_expanded);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_param_expanded,       &flag_param_expanded);

    o->deleteLater();
    //---

    ui->sb_nudChANumber->setValue(sb_nudChANumber.toInt());
    ui->sb_nudChBNumber->setValue(sb_nudChBNumber.toInt());
    ui->dsb_nudChACorr->setValue(dsb_nudChACorr.toDouble());
    ui->dsb_nudChBCorr->setValue(dsb_nudChBCorr.toDouble());
    ui->dsb_nudTInt->setValue(dsb_nudTInt.toDouble());

    ui->dsb_nudWindow->setValue(dsb_nudWindow.toDouble());
    ui->dsb_nudN->setValue(dsb_nudN.toDouble());

    //TODO
    ui->btn_channel_a_expanded->setChecked(flag_channel_a_expanded.toBool());
    ui->btn_channel_b_expanded->setChecked(flag_channel_b_expanded.toBool());
    ui->btn_ts_expanded->setChecked(flag_ts_expanded.toBool());
    ui->btn_consillence_expanded->setChecked(flag_consillence_expanded.toBool());
    ui->btn_analiz_expanded->setChecked(flag_analiz_expanded.toBool());
    ui->btn_events_flag_expanded->setChecked(flag_events_flag_expanded.toBool());
    ui->btn_param_expanded->setChecked(flag_param_expanded.toBool());

    //TODO
    if(file_auto_open.isEmpty() == false)
    {
        mtw->setCurrentText(file_auto_open);
    }
}
//--------------------------------------------------------------------------------
Histogram_consilience::~Histogram_consilience()
{
    PETCat_options *o = new PETCat_options();
    o->set_histogram_consulience_option(PARAM_histogram_consulience_channel_a_expanded,   ui->btn_channel_a_expanded->isChecked());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_channel_b_expanded,   ui->btn_channel_b_expanded->isChecked());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_ts_expanded,          ui->btn_ts_expanded->isChecked());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_consillence_expanded, ui->btn_consillence_expanded->isChecked());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_analiz_expanded,      ui->btn_analiz_expanded->isChecked());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_events_flag_expanded, ui->btn_events_flag_expanded->isChecked());
    o->set_histogram_consulience_option(PARAM_histogram_consulience_param_expanded,       ui->btn_param_expanded->isChecked());
    o->deleteLater();

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    QByteArray ba = main_splitter->saveState();
    if(ba.isEmpty() ==  false)
    {
        settings->beginGroup(PETCAT_OPTIONS_HISTOGRAM_CONSULIENCE_GROUP);
        settings->setValue("splitterState", ba);
        settings->endGroup();
    }
    settings->deleteLater();

    if(grapher)
    {
        grapher->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Histogram_consilience::init(void)
{
    ui->setupUi(this);

    //---
    mtw = new MetaData_widget(this);
    ui->metadata_widget_layout->addWidget(mtw);
    //ui->metadata_widget_layout->addStretch(1);
    //---

    ui->sb_nudChANumber->setRange(0, 0xFF);
    //ui->sb_nudChANumber->setValue(2);  // 1

    ui->sb_nudChBNumber->setRange(0, 0xFF);
    //ui->sb_nudChBNumber->setValue(3);  // 0

    ui->dsb_nudChACorr->setRange(-100000, 100000);
    ui->dsb_nudChBCorr->setRange(-100000, 100000);

    ui->dsb_nudTInt->setRange(0, 100);
    ui->le_tbTts->setReadOnly(true);

    ui->dsb_nudWindow->setRange(0, 100000);
    ui->dsb_nudN->setRange(1, 100000);

    ui->dsb_nudTInt->setDecimals(1);
    ui->dsb_nudWindow->setDecimals(1);
    ui->dsb_nudN->setDecimals(1);

    ui->le_tbTotalEvents->setReadOnly(true);
    ui->le_tbCmpNum->setReadOnly(true);
    ui->le_tbM->setReadOnly(true);
    ui->le_tbSKO->setReadOnly(true);

    connect(ui->btn_run,                    SIGNAL(clicked(bool)),  this,   SLOT(refresh_click()));
    connect(ui->btn_save_picture_to_file,   SIGNAL(clicked(bool)),  this,   SLOT(save_picture_to_file()));
    ui->btn_save_picture_to_file->setDisabled(true);
    //---
    connect(ui->dsb_nudTInt,    SIGNAL(valueChanged(double)),   this,   SLOT(set_tbTts(double)));
    //---
    ui->cb_type_curve->clear();
    ui->cb_type_curve->addItem("Точечный", DOTS);
    ui->cb_type_curve->addItem("Линейный", LINES);
    ui->cb_type_curve->addItem("Сглаженная кривая", SPLINE_LINES);

    connect(ui->cb_type_curve, SIGNAL(currentIndexChanged(int)),
            this,   SLOT(grapher_refresh()));

    //---
    ui->le_events_flags->setReadOnly(true);
    connect(ui->btn_events_flags,   SIGNAL(clicked(bool)),  this,   SLOT(set_event_flags()));

    //---
    QVariant f_value = 0xFE;
    PETCat_options *o = new PETCat_options();
    o->get_histogram_consulience_option(PARAM_histogram_consulience_events_flags,     &f_value);
    o->deleteLater();
    event_flags = f_value.toInt();  //TODO
    //---

    ui->le_events_flags->setText(QString("0x%1").arg(event_flags, 2, 16, QChar('0')));
    //---

    //---
    connect(ui->btn_channel_a_expanded,     SIGNAL(toggled(bool)),  this,   SLOT(channel_a_expanded(bool)));
    connect(ui->btn_channel_b_expanded,     SIGNAL(toggled(bool)),  this,   SLOT(channel_b_expanded(bool)));
    connect(ui->btn_ts_expanded,            SIGNAL(toggled(bool)),  this,   SLOT(ts_expanded(bool)));
    connect(ui->btn_consillence_expanded,   SIGNAL(toggled(bool)),  this,   SLOT(consillence_expanded(bool)));
    connect(ui->btn_analiz_expanded,        SIGNAL(toggled(bool)),  this,   SLOT(analiz_expanded(bool)));
    connect(ui->btn_events_flag_expanded,   SIGNAL(toggled(bool)),  this,   SLOT(events_flag_expanded(bool)));
    connect(ui->btn_param_expanded,         SIGNAL(toggled(bool)),  this,   SLOT(param_expanded(bool)));

    channel_a_expanded(false);
    channel_b_expanded(false);
    ts_expanded(false);
    consillence_expanded(false);
    analiz_expanded(false);
    events_flag_expanded(false);
    param_expanded(false);
    //---
    block_interface(mtw->count() == 0); //TODO block_interface
    //---

    //---
    init_dat3();
    init_grapher();
    //---
    main_splitter = new QSplitter(Qt::Horizontal, this);
    main_splitter->setObjectName("main_splitter");
    main_splitter->setChildrenCollapsible(false);

    ui->frame_main->setParent(main_splitter);
    grapher->setParent(main_splitter);
    main_splitter->addWidget(ui->frame_main);
    main_splitter->addWidget(grapher);

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    settings->beginGroup(PETCAT_OPTIONS_HISTOGRAM_CONSULIENCE_GROUP);
    QByteArray ba = settings->value("splitterState").toByteArray();
    if(ba.isEmpty() == false)
    {
        main_splitter->restoreState(ba);
    }
    settings->endGroup();
    settings->deleteLater();

    layout()->addWidget(main_splitter);
    //---
    block_wheel();

    grapher_refresh();
}
//--------------------------------------------------------------------------------
void Histogram_consilience::refresh_click(void)
{
    refresh();
}
//--------------------------------------------------------------------------------
void Histogram_consilience::set_tbTts(double value)
{
    double timeStep = 1000.0 * value / (double)UINT16_MAX;
    ui->le_tbTts->setText(QString("%1").arg(timeStep, 0, 'f', 15));
}
//--------------------------------------------------------------------------------
void Histogram_consilience::set_event_flags(void)
{
    PET_event_flag *dlg = new PET_event_flag();
    dlg->set(event_flags);
    int btn = dlg->exec();
    if(btn != QDialog::Accepted) return;

    event_flags = dlg->get();
    ui->le_events_flags->setText(QString("0x%1").arg(event_flags, 2, 16, QChar('0')));
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void Histogram_consilience::init_dat3(void)
{
    dat3 = new DAT3();
    connect(dat3,   SIGNAL(info(QString)),      parentWidget(), SIGNAL(info(QString)));
    connect(dat3,   SIGNAL(debug(QString)),     parentWidget(), SIGNAL(debug(QString)));
    connect(dat3,   SIGNAL(error(QString)),     parentWidget(), SIGNAL(error(QString)));
    connect(dat3,   SIGNAL(message(QString)),   parentWidget(), SIGNAL(message(QString)));

    connect(dat3,   SIGNAL(send_event(pet_event)),  this,   SLOT(get_event(pet_event)));
}
//--------------------------------------------------------------------------------
void Histogram_consilience::init_grapher(void)
{
    int min = -(ui->dsb_nudWindow->value() / 2.0);
    int max = (ui->dsb_nudWindow->value() / 2.0);

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(min, max);
    grapher->set_axis_scale_y(0, 10000);
    
    //---
    curve_Pdt = grapher->add_curve("P(∆t)");
    curve_gauss = grapher->add_curve("Gaus");
    //---
    QVariant clr_p = 0;
    QVariant clr_gauss = 0;
    //---
    PETCat_options *o = new PETCat_options();
    o->get_histogram_consulience_option(PARAM_histogram_consulience_clr_p,      &clr_p);
    o->get_histogram_consulience_option(PARAM_histogram_consulience_clr_gauss,  &clr_gauss);
    o->deleteLater();
    //---
    grapher->set_curve_color(curve_Pdt,     clr_p.toInt());
    grapher->set_curve_color(curve_gauss,   clr_gauss.toInt());
    //---

    grapher->set_curve_style(curve_Pdt, QwtPlotCurve::Dots);

    //---
    QwtSymbol *symbol_Pdt = new QwtSymbol();
    symbol_Pdt->setStyle(QwtSymbol::Ellipse);
    symbol_Pdt->setPen(COLOR_CURVE_GRAPHER);
    symbol_Pdt->setSize(4);
    grapher->set_curve_symbol(curve_Pdt, symbol_Pdt);

    grapher->legend_off(curve_gauss);

    grapher->set_zoom(false, true);
    grapher->set_panning(false, false);

    grapher->set_visible_btn_all_ON(false);
    grapher->set_visible_btn_all_OFF(false);

    grapher->set_silense(true);

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);

#ifdef QT_DEBUG
    grapher->set_visible_btn_Horizontal(true);
    grapher->set_visible_btn_Vertical(true);
#endif

    grapher->push_btn_Horizontal(false);
    grapher->push_btn_Vertical(false);

    //ui->layout_scope->addWidget(grapher);
}
//--------------------------------------------------------------------------------
#if 0
Канал А
№ канала        sb_nudChANumber
Поправка, пс    sb_nudChACorr

Канал Б
№ канала        sb_nudChBNumber
Поправка, пс    sb_nudChBCorr

Метка времени
Tint, нс        dsb_nudTInt
Tts, нс         le_tbTts

Окно совпадений
Tw, нс          dsb_nudWindow
N               dsb_nudN

// ui->le_tbTotalEvents
// ui->le_tbCmpNum
// ui->le_tbM
// ui->le_tbSKO

#endif
//--------------------------------------------------------------------------------
bool Histogram_consilience::convert_metadata(QString name, QString *result)
{
    if(name.isEmpty())
    {
        return false;
    }

    MetaData *md = new MetaData();
    connect(md, SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(md, SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    QStringList sl;
    bool ok = false;
    QFileInfo fi;
    sl.clear();
    fi.setFile(name);
    ok = md->find_data(fi.baseName(), &sl);
    if(ok)
    {
        emit debug("-----");
        foreach (QString name, sl)
        {
            *result = name;
            disconnect(md);
            md->deleteLater();
            return true;
        }
        emit debug("-----");
    }
    disconnect(md);
    md->deleteLater();
    return false;
}
//--------------------------------------------------------------------------------
void Histogram_consilience::refresh(void)
{
    QString filename;
    bool ok = false;
    //---
    convert_metadata(mtw->currentText(),
                     &filename);
    //---

    if(filename.isEmpty())
    {
        messagebox_critical("Ошибка", "Сначала выберите файл данных!");
        return;
    }

    block_interface(true);
    clear_all();

    //---
    grapher->clear();
    //---
    mSourceA = ui->sb_nudChANumber->value();
    mSourceB = ui->sb_nudChBNumber->value();
    //---
    int M = ui->dsb_nudN->value();
    //---
    buf = (int *)calloc(M+1, sizeof(int));
    memset(buf, 0, M+1);
    //---
    EventTotal = 0;
    CmpNum = 0;
    //---
    ok = dat3->read_file(filename);
    if(!ok)
    {
        block_interface(false);
        return;
    }
    //---
    quint64 dat3_cnt = dat3->get_cnt_read_events();
    QTime timer;
    timer.start();
    while(dat3_cnt != EventTotal)
    {
        QCoreApplication::processEvents();
        if(timer.elapsed() > 1000)
        {
            QString text = QString("DAT3 cnt_events %1 != EventTotal %2")
                    .arg(dat3->get_cnt_read_events())
                    .arg(EventTotal);
            emit debug(text);
            block_interface(false);
            break;
        }
    }
    //---
    double min = -(ui->dsb_nudN->value() / 2.0);
    double max = (ui->dsb_nudN->value() / 2.0);
    emit debug(QString("min %1").arg(min));
    emit debug(QString("max %1").arg(max));
    //---

    //---
    grapher->clear();
    
    // небольшой хак с нижней шкалой
    double divider = ui->dsb_nudN->value() / ui->dsb_nudWindow->value();
    //emit debug(QString("divider %1").arg(divider));
    //grapher->setAxisScaleDraw(QwtPlot::xBottom, new MyScaleDraw(divider));

    //TODO
    grapher->setAxisScaleDraw(QwtPlot::xBottom, new MyScaleDraw(divider));

    grapher->clear_d_picker();
    PlotPicker *new_picker =  new PlotPicker(QwtPlot::xBottom,
                                             QwtPlot::yLeft,
                                             QwtPlotPicker::CrossRubberBand,
                                             QwtPicker::AlwaysOn,
                                             divider,
                                             grapher->get_qwtPlot_canvas());
    new_picker->setRubberBandPen(QColor(Qt::green));
    new_picker->setRubberBand(QwtPicker::CrossRubberBand);
    new_picker->setTrackerPen(QColor(Qt::blue));
    new_picker->setStateMachine(new QwtPickerDragPointMachine());

    grapher->set_d_picker(new_picker);
    //---

    grapher->set_axis_scale_x(min, max);

    grapher->set_visible_btn_Vertical(false);
    grapher->set_visible_btn_Horizontal(false);
    grapher->push_btn_Vertical(true);
    //---

    double max_n = ui->dsb_nudN->value();
    for(int n=-(max_n/2.0); n<(max_n/2.0); n++)
    {
        grapher->add_curve_data(curve_Pdt, n, buf[n+(int)(max_n/2.0+0.5)]);    //TODO
    }
    //---
    double mean = 0;
    uint mIntNum = ui->dsb_nudN->value();
    for (unsigned int i=0; i<mIntNum; i++)
    {
        //mean += (double)(i - (M - 1) / 2) * (double)buf[i] / (double)CmpNum;
        mean += (double)(i - (M - 1) / 2.0) * (double)buf[i] / (double)CmpNum;
    }
    //---
    double mean2 = 0;
    for (int i=0; i<M; i++)
    {
        //mean2 += qPow((double)(i - (M - 1) / 2), 2) * (double)buf[i] / (double)CmpNum;
        mean2 += qPow((double)(i - (M - 1) / 2.0), 2.0) * (double)buf[i] / (double)CmpNum;
    }
    double variance = qSqrt(mean2 - qPow(mean, 2.0));
    //---
    double mCmpWindow = ui->dsb_nudWindow->value();
    //---
    double mu = mean;           // cmp.Mean();      среднее
    double sigma = variance;    // cmp.Variance();  дисперсия
    double delta = mCmpWindow / ((double)M - 1.0);
    //---
    ShowGaus(CmpNum, mu, sigma);
    //---
    ui->le_tbTotalEvents->setText(QString("%1").arg(EventTotal));
    ui->le_tbCmpNum->setText(QString("%1 (%2%)")
                             .arg(CmpNum)
                             .arg(100.0 * (double)CmpNum / (double)EventTotal, 0, 'f', 2));
    ui->le_tbM->setText(QString("%1").arg(mu * delta, 0, 'f', 3));
    ui->le_tbSKO->setText(QString("%1").arg(sigma * delta, 0, 'f', 3));
    //---
    QString title = QString("Совпадения каналов %1-%2 (µ = %3 нс, σ = %4 нс)")
            .arg(ui->sb_nudChANumber->value())
            .arg(ui->sb_nudChBNumber->value())
            .arg(mu * delta, 0, 'f', 3)
            .arg(sigma * delta, 0, 'f', 3);
    grapher->set_title_axis_X(title);
    //---
    free(buf);
    //---
    emit info("Анализ совпадений");
    emit info(QString("    Файл данных:                        %1")
              .arg(filename));
    emit info(QString("    Канал A:                            %1")
              .arg(ui->sb_nudChANumber->value()));
    emit info(QString("    Канал B:                            %1")
              .arg(ui->sb_nudChBNumber->value()));
    emit info("    Метка времени:");
    emit info(QString("    TInt, нс:                           %1")
              .arg(ui->dsb_nudTInt->value()));
    emit info(QString("    Tts,  нс:                           %1")
              .arg(ui->le_tbTts->text()));
    emit info("    Окно совпадений:");
    emit info(QString("    Tw, нс:                             %1")
              .arg(ui->dsb_nudWindow->value()));
    emit info(QString("    N:                                  %1")
              .arg(ui->dsb_nudN->value()));
    emit info("Зарегистрировано событий:");
    emit info(QString("    всего:                              %1")
              .arg(ui->le_tbTotalEvents->text()));
    emit info(QString("    совпадений:                         %1")
              .arg(ui->le_tbCmpNum->text()));
    emit info(QString("    M:                                  %1")
              .arg(ui->le_tbM->text()));
    emit info(QString("    CKO:                                %1")
              .arg(ui->le_tbSKO->text()));
    //---
    print_event_flag_log();
    //---
    block_interface(false);
    //---
    //TODO
    ui->btn_save_picture_to_file->setEnabled(true);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::ShowGaus(long NNN, double mu, double sigma)
{
    //double timeStep = 1000.0 * (double)ui->dsb_nudTInt->value() / (double)UINT16_MAX;

    float N = ui->dsb_nudN->value();
    //for (int i = -(M/2); i < (int)ui->dsb_nudN->value() / 2; i++)
    for(int i=-(N/2); i<(N/2); i++)
    {
        double x = (double)i;
        double p = (double)NNN * (1.0 / (sigma * qSqrt(2.0 * M_PI))) * qExp(-(qPow(x - mu, 2)) / (2.0 * qPow(sigma, 2)));

#if 1
        grapher->add_curve_data(curve_gauss, i, p); //TODO
#else
        grapher->add_curve_data(curve_gauss, x * timeStep, p);
        //chart.Series["Gaus"].Points.AddXY(x * timeStep, p);
#endif
    }
}
//--------------------------------------------------------------------------------
void Histogram_consilience::save_picture_to_file(void)
{
    emit debug("create_picture");

    //---
    QWidget *w_grapher = dynamic_cast<QWidget *>(grapher);
    if(w_grapher == nullptr)
    {
        return;
    }
    QPixmap pix_grapher = w_grapher->grab(QRect(0, 0, w_grapher->width(), w_grapher->height()));
    //---
    QString text;
    text.append("Анализ совпадений\n");
    text.append(QString("    Канал A:                            %1\n").arg(ui->sb_nudChANumber->value()));
    text.append(QString("    Канал B:                            %1\n").arg(ui->sb_nudChBNumber->value()));
    text.append("    Метка времени:\n");
    text.append(QString("    TInt, нс:                           %1\n").arg(ui->dsb_nudTInt->value()));
    text.append(QString("    Tts,  нс:                           %1\n").arg(ui->le_tbTts->text()));
    text.append("    Окно совпадений:\n");
    text.append(QString("    Tw, нс:                             %1\n").arg(ui->dsb_nudWindow->value()));
    text.append(QString("    N:                                  %1\n").arg(ui->dsb_nudN->value()));
    text.append("Зарегистрировано событий:\n");
    text.append(QString("    всего:                              %1\n").arg(ui->le_tbTotalEvents->text()));
    text.append(QString("    совпадений:                         %1\n").arg(ui->le_tbCmpNum->text()));
    text.append(QString("    M:                                  %1\n").arg(ui->le_tbM->text()));
    text.append(QString("    CKO:                                %1\n").arg(ui->le_tbSKO->text()));

#ifdef LEFT_PICTURE
    QWidget *frame = new QWidget;
    QTextEdit *te = new QTextEdit;
    QFont font("Courier", 10);
    te->setFont(font);
    te->setReadOnly(true);
    te->setPlainText(text);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->addWidget(te);
    frame->setLayout(vbox);
    frame->setFixedSize(te->width(), w_grapher->height());
    //---
    QPixmap left_pixmap = ((QWidget *)frame)->grab(QRect(0, 0, frame->width(), frame->height()));
    QPixmap *main_pixmap = new QPixmap(left_pixmap.width() + pix_grapher.width(), w_grapher->height());
#else
    QPixmap *main_pixmap = new QPixmap(pix_grapher.width(), w_grapher->height());
#endif
    //---
    QPainter painter;
    painter.begin(main_pixmap);
    //painter.drawPixmap(0, 0, left_pixmap);
    //painter.drawPixmap(left_pixmap.width(), 0, pix_grapher);
    painter.drawPixmap(0, 0, pix_grapher);
    painter.end();
    //---
#if SHOW_PICTURE
    QLabel *label = new QLabel;
    label->setPixmap(*main_pixmap);
    label->show();
    //---
#ifdef LEFT_PICTURE
    frame->deleteLater();
#endif
#else
    QString filename;

    QDateTime dt = QDateTime::currentDateTime();
    QString default_name = QString("%1 %2")
            .arg(NAME_HISTOGRAM_CONSULIENCE)
            .arg(dt.toString("dd-MM-yyyy hhmmss"));

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "hc_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("Файлы изображений (*.png)");
    dlg->setDefaultSuffix(tr("png"));
    dlg->selectFile(default_name);
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename.clear();
        filename.append(files.at(0));
    }
    dlg->deleteLater();

    if(filename.isEmpty())
    {
        return;
    }

    bool ok = main_pixmap->save(filename);
    if(ok)
    {
        emit info(QString("Файл %1 успешно создан").arg(filename));
    }
    else
    {
        QString temp_str = QString("Файл %1 не создан").arg(filename);
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
    }
#endif
    //ui->btn_save_picture_to_file->setDisabled(true);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::save_picture_to_file_old(void)
{
    emit debug("create_picture");

    QString filename;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "hc_dlg");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("Файлы изображений (*.png)");
    dlg->setDefaultSuffix(tr("png"));
    dlg->selectFile("без имени");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename.clear();
        filename.append(files.at(0));
    }
    dlg->deleteLater();

    if(filename.isEmpty())
    {
        return;
    }

    QWidget *w = (QWidget *)this;
    if(w)
    {
        QPixmap pix = w->grab(QRect(0, 0, w->width(), w->height()));
        bool ok = pix.save(filename);
        if(ok)
        {
            emit info(QString("Файл %1 успешно создан").arg(filename));
        }
        else
        {
            QString temp_str = QString("Файл %1 не создан").arg(filename);
            emit error(temp_str);
            messagebox_critical("Ошибка", temp_str);
        }
    }
}
//--------------------------------------------------------------------------------
void Histogram_consilience::get_event(pet_event event)
{
    if(check_event_flags(event_flags, event.flags) == true)
    {
        return;
    }
    //---
    double CorrectionA = ui->dsb_nudChACorr->value();
    double CorrectionB = ui->dsb_nudChBCorr->value();
    //---

    double mTimeScale = ui->dsb_nudTInt->value();
    double mStep = (double)ui->dsb_nudWindow->value() / ((double)ui->dsb_nudN->value() - 1.0);
    int M = ui->dsb_nudN->value();

    double ts = mTimeScale * ((double)event.ts + (double)event.ts_fract / (double)UINT16_MAX);

    if (event.src == mSourceA)
    {
        tsA = ts + CorrectionA;
    }
    else if (event.src == mSourceB)
    {
        tsB = ts + CorrectionB;
    }
    else return;

    int ix = (int)(((M - 1.0) / 2.0) + (tsB - tsA) / mStep);

    if (ix >= 0 && ix < M)
    {
        if (++buf[ix] > mMax)
        {
            mMax++;
        }
        CmpNum++;
    }

    EventTotal++;
}
//--------------------------------------------------------------------------------
void Histogram_consilience::grapher_refresh(void)
{
    int type = ui->cb_type_curve->itemData(ui->cb_type_curve->currentIndex()).toInt();
    switch(type)
    {
    case DOTS:
        for(int n=0; n<grapher->get_curves_count(); n++)
        {
            QwtSymbol *symbol = new QwtSymbol();
            symbol->setStyle(QwtSymbol::Ellipse);
            symbol->setPen(grapher->get_curve_color(n));
            symbol->setSize(4);

            grapher->set_curve_symbol(n, symbol);
            grapher->set_curve_style(n, QwtPlotCurve::Dots);
            grapher->set_curve_fitter(n, 0);
        }
        grapher->updateGraphics();

        //emit debug("set_curve_style(curve, QwtPlotCurve::Dots);");
        break;

    case LINES:
        for(int n=0; n<grapher->get_curves_count(); n++)
        {
            grapher->set_curve_symbol(n, 0);
            grapher->set_curve_style(n, QwtPlotCurve::Lines);
            grapher->set_curve_fitter(n, 0);
        }
        grapher->updateGraphics();

        //emit debug("set_curve_style(curve, QwtPlotCurve::Lines);");
        break;

    case SPLINE_LINES:
        for(int n=0; n<grapher->get_curves_count(); n++)
        {
            QwtSplineCurveFitter *fitter=new QwtSplineCurveFitter;
            fitter->setFitMode(QwtSplineCurveFitter::Spline);

            grapher->set_curve_symbol(n, 0);
            grapher->set_curve_style(n, QwtPlotCurve::Lines);

            grapher->set_curve_attribute(n, QwtPlotCurve::Fitted);
            grapher->set_curve_fitter(n, fitter);
        }
        grapher->updateGraphics();
        emit debug("spline");
        break;
    }
}
//--------------------------------------------------------------------------------
void Histogram_consilience::test(void)
{

}
//--------------------------------------------------------------------------------
void Histogram_consilience::clear_all(void)
{
    event_flags = 0;

    EventTotal = 0;
    CmpNum = 0;

    mMax = 0;

    bad_flags   = 0;
    all_filtered = 0;

    cnt_events  = 0;
    cnt_events_address_filtered = 0;
    cnt_PET_EVENT_FLAG_FAULT_filtered = 0;
    cnt_PET_EVENT_FLAG_TS_INVALID_filtered = 0;
    cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered = 0;
    cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered = 0;
    cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered = 0;
    cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered = 0;
    cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered = 0;
    cnt_PET_EVENT_FLAG_SYNC_filtered = 0;
    cnt_events_min_energy_filtered = 0;
    cnt_events_max_energy_filtered = 0;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Histogram_consilience::channel_a_expanded(bool state)
{
    ui->frame_channel_A->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::channel_b_expanded(bool state)
{
    ui->frame_channel_B->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::ts_expanded(bool state)
{
    ui->frame_TS->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::consillence_expanded(bool state)
{
    ui->frame_consillence->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::analiz_expanded(bool state)
{
    ui->frame_analiz->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::events_flag_expanded(bool state)
{
    ui->frame_mask->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::param_expanded(bool state)
{
    ui->frame_param->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_consilience::refresh_metadata(void)
{
    mtw->refresh_metadata();
    block_interface(mtw->count() == 0); //TODO block_interface
}
//--------------------------------------------------------------------------------
bool Histogram_consilience::check_event_flags(uint8_t filter_flags, uint8_t event_flags)
{
    if((filter_flags & PET_EVENT_FLAG_FAULT) & event_flags)
    {
        cnt_PET_EVENT_FLAG_FAULT_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_TS_INVALID) & event_flags)
    {
        cnt_PET_EVENT_FLAG_TS_INVALID_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_GATE_OVERLAY) & event_flags)
    {
        cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_QDC_OVERFLOW) & event_flags)
    {
        cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_QDC_UNDERFLOW) & event_flags)
    {
        cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_ADC_OVERFLOW) & event_flags)
    {
        cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_ADC_UNDERFLOW) & event_flags)
    {
        cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered++;
        bad_flags++;
        return true;
    }
    if((filter_flags & PET_EVENT_FLAG_SYNC) & event_flags)
    {
        cnt_PET_EVENT_FLAG_SYNC_filtered++;
        bad_flags++;
        return true;
    }

    return false;
}
//--------------------------------------------------------------------------------
