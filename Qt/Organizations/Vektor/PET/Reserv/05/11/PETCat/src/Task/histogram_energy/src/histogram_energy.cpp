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
#include <QSettings>
#include <QTextEdit>
#include <QTime>
#include <QFile>
//--------------------------------------------------------------------------------
#include "histogram_energy.hpp"
#include "ui_histogram_energy.h"
#include "dat3.hpp"
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "metadata_widget.hpp"
//--------------------------------------------------------------------------------
#include "pet_event_flag.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
Histogram_energy::Histogram_energy(QString file_auto_open,
                                   QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Histogram_energy)
{
    init();

    QVariant sb_e_min = 0;
    QVariant sb_e_max = 0;
    QVariant flag_e_min = 0;
    QVariant flag_e_max = 0;
    QVariant sb_N = 0;

    QVariant flag_source_expanded = 0;
    QVariant flag_energy_expanded = 0;
    QVariant flag_events_flag_expanded = 0;
    QVariant flag_param_expanded = 0;

    PETCat_options *o = new PETCat_options();
    o->get_histogram_energy_option(PARAM_histogram_energy_sb_e_min,    &sb_e_min);
    o->get_histogram_energy_option(PARAM_histogram_energy_sb_e_max,    &sb_e_max);
    o->get_histogram_energy_option(PARAM_histogram_energy_flag_e_min,  &flag_e_min);
    o->get_histogram_energy_option(PARAM_histogram_energy_flag_e_max,  &flag_e_max);
    o->get_histogram_energy_option(PARAM_histogram_energy_sb_N,        &sb_N);
    o->get_histogram_energy_option(PARAM_histogram_energy_source_expanded,      &flag_source_expanded);
    o->get_histogram_energy_option(PARAM_histogram_energy_energy_expanded,      &flag_energy_expanded);
    o->get_histogram_energy_option(PARAM_histogram_energy_events_flag_expanded, &flag_events_flag_expanded);
    o->get_histogram_energy_option(PARAM_histogram_energy_param_expanded,       &flag_param_expanded);
    o->deleteLater();

    ui->sb_e_min->setValue(sb_e_min.toInt());
    ui->sb_e_max->setValue(sb_e_max.toInt());
    ui->cb_enable_filter_min_energy->setChecked(flag_e_min.toBool());
    ui->cb_enable_filter_max_energy->setChecked(flag_e_max.toBool());
    ui->sb_N->setValue(sb_N.toInt());

    //TODO
    ui->btn_source_expanded->setChecked(flag_source_expanded.toBool());
    ui->btn_energy_expanded->setChecked(flag_energy_expanded.toBool());
    ui->btn_events_flag_expanded->setChecked(flag_events_flag_expanded.toBool());
    ui->btn_param_expanded->setChecked(flag_param_expanded.toBool());

    //TODO
    if(file_auto_open.isEmpty() == false)
    {
        mtw->setCurrentText(file_auto_open);
    }
}
//--------------------------------------------------------------------------------
Histogram_energy::~Histogram_energy()
{
    PETCat_options *o = new PETCat_options();
    o->set_histogram_energy_option(PARAM_histogram_energy_source_expanded,      ui->btn_source_expanded->isChecked());
    o->set_histogram_energy_option(PARAM_histogram_energy_energy_expanded,      ui->btn_energy_expanded->isChecked());
    o->set_histogram_energy_option(PARAM_histogram_energy_events_flag_expanded, ui->btn_events_flag_expanded->isChecked());
    o->set_histogram_energy_option(PARAM_histogram_energy_param_expanded,       ui->btn_param_expanded->isChecked());
    o->deleteLater();

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    QByteArray ba = main_splitter->saveState();
    if(ba.isEmpty() ==  false)
    {
        settings->beginGroup(PETCAT_OPTIONS_HISTOGRAM_ENERGY_GROUP);
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
void Histogram_energy::init(void)
{
    ui->setupUi(this);

    clear_buf_energy();

    init_widgets();
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
    settings->beginGroup(PETCAT_OPTIONS_HISTOGRAM_ENERGY_GROUP);
    QByteArray ba = settings->value("splitterState").toByteArray();
    if(ba.isEmpty() == false)
    {
        main_splitter->restoreState(ba);
    }
    settings->endGroup();
    settings->deleteLater();

    layout()->addWidget(main_splitter);
    //---
    grapher_refresh();
}
//--------------------------------------------------------------------------------
void Histogram_energy::clear_buf_energy(void)
{
    for(int n=0; n<MAX_SIZE_BUF_ENERGY; n++)
    {
        buf_energy[n] = 0;
    }
}
//--------------------------------------------------------------------------------
void Histogram_energy::init_widgets(void)
{
    //---
    mtw = new MetaData_widget(this);
    ui->metadata_widget_layout->addWidget(mtw);
    //ui->metadata_widget_layout->addStretch(1);
    //---

    //---
    //TODO
    ui->sb_filter_address->setProperty(FLAG_NO_BLOCK, true);
    ui->cb_enable_filter_max_energy->setProperty(FLAG_NO_BLOCK, true);
    ui->cb_enable_filter_min_energy->setProperty(FLAG_NO_BLOCK, true);
    ui->sb_e_max->setProperty(FLAG_NO_BLOCK, true);
    ui->sb_e_min->setProperty(FLAG_NO_BLOCK, true);
    //---
    ui->sb_e_min->setRange(HISTOGRAM_MIN_ENERGY, HISTOGRAM_MAX_ENERGY);
    ui->sb_e_min->setValue(10);
    //---
    ui->sb_e_max->setRange(HISTOGRAM_MIN_ENERGY+1, HISTOGRAM_MAX_ENERGY);
    ui->sb_e_max->setValue(100000);
    //---
    ui->sb_N->setRange(HISTOGRAM_MIN_N, HISTOGRAM_MAX_N);
    ui->sb_N->setValue(2000);
    //---
    ui->sb_filter_address->setRange(0, 0xFF);
    ui->sb_filter_address->setDisabled(true);
    connect(ui->rb_event_all,   SIGNAL(clicked(bool)),  ui->sb_filter_address,  SLOT(setDisabled(bool)));
    connect(ui->rb_event_addr,  SIGNAL(clicked(bool)),  ui->sb_filter_address,  SLOT(setEnabled(bool)));
    //---
    connect(ui->btn_refresh,        SIGNAL(clicked(bool)), this, SLOT(refresh()));
    //---
    connect(ui->cb_enable_filter_min_energy,    SIGNAL(toggled(bool)),  ui->sb_e_min,   SLOT(setEnabled(bool)));
    connect(ui->cb_enable_filter_max_energy,    SIGNAL(toggled(bool)),  ui->sb_e_max,   SLOT(setEnabled(bool)));
    //---
    ui->cb_enable_filter_min_energy->setChecked(true);
    ui->cb_enable_filter_max_energy->setChecked(true);
    //---
    ui->le_events_flags->setReadOnly(true);
    connect(ui->btn_events_flags,   SIGNAL(clicked(bool)),  this,   SLOT(set_event_flags()));
    event_flags = 0xFE;
    ui->le_events_flags->setText(QString("0x%1").arg(event_flags, 2, 16, QChar('0')));
    //---
    ui->cb_type_curve->clear();
    ui->cb_type_curve->addItem("Точечный", DOTS);
    ui->cb_type_curve->addItem("Линейный", LINES);
    ui->cb_type_curve->addItem("Сглаженная кривая", SPLINE_LINES);
    //---
    connect(ui->btn_save_picture_to_file,   SIGNAL(clicked(bool)),  this,   SLOT(save_picture_to_file()));
    ui->btn_save_picture_to_file->setDisabled(true);

    //---
    connect(ui->btn_source_expanded,        SIGNAL(toggled(bool)),  this,   SLOT(source_expanded(bool)));
    connect(ui->btn_energy_expanded,        SIGNAL(toggled(bool)),  this,   SLOT(energy_expanded(bool)));
    connect(ui->btn_events_flag_expanded,   SIGNAL(toggled(bool)),  this,   SLOT(events_flag_expanded(bool)));
    connect(ui->btn_param_expanded,         SIGNAL(toggled(bool)),  this,   SLOT(param_expanded(bool)));

    source_expanded(false);
    energy_expanded(false);
    events_flag_expanded(false);
    param_expanded(false);
    //---

    //---
    block_interface(mtw->count() == 0); //TODO block_interface
    //---
    connect(ui->cb_type_curve, SIGNAL(currentIndexChanged(int)),
            this,   SLOT(grapher_refresh()));
}
//--------------------------------------------------------------------------------
void Histogram_energy::init_dat3(void)
{
    dat3 = new DAT3();
    connect(dat3,   SIGNAL(info(QString)),      parentWidget(), SIGNAL(info(QString)));
    connect(dat3,   SIGNAL(debug(QString)),     parentWidget(), SIGNAL(debug(QString)));
    connect(dat3,   SIGNAL(error(QString)),     parentWidget(), SIGNAL(error(QString)));
    connect(dat3,   SIGNAL(message(QString)),   parentWidget(), SIGNAL(message(QString)));

    connect(dat3,   SIGNAL(send_event(pet_event)),  this,   SLOT(get_event(pet_event)));
}
//--------------------------------------------------------------------------------
void Histogram_energy::init_grapher(void)
{
    grapher = new GrapherBox(this);
    grapher->set_axis_scale_x(HISTOGRAM_MIN_ENERGY, HISTOGRAM_MAX_ENERGY);
    grapher->set_axis_scale_y(0, 100);
    grapher->set_title_axis_X("E");
    grapher->set_title_axis_X("P");

    curve = grapher->add_curve("Гистограмма");
    //---
    QVariant clr_histogramm = 0;
    //---
    PETCat_options *o = new PETCat_options();
    o->get_histogram_energy_option(PARAM_histogram_energy_color_histogramm,  &clr_histogramm);
    o->deleteLater();
    //---
    grapher->set_curve_color(curve, clr_histogramm.toInt());
    //---
    grapher->set_curve_style(curve, QwtPlotCurve::Dots);

    //---
    QwtSymbol *symbol = new QwtSymbol();
    symbol->setStyle(QwtSymbol::Ellipse);
    symbol->setPen(COLOR_CURVE_GRAPHER);
    symbol->setSize(4);
    grapher->set_curve_symbol(curve, symbol);

    grapher->set_zoom(false, false);
    grapher->set_panning(false, false);

    grapher->set_silense(true);

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);

    grapher->set_visible_btn_all_ON(false);
    grapher->set_visible_btn_all_OFF(false);

    grapher->push_btn_Horizontal(false);
    grapher->push_btn_Vertical(true);
}
//--------------------------------------------------------------------------------
bool Histogram_energy::check_event_flags(uint8_t filter_flags, uint8_t event_flags)
{
    if(filter_flags == 0xFF)    return true;
    bool ok = !(filter_flags & event_flags);

    if(PET_EVENT_FLAG_FAULT & event_flags)          cnt_PET_EVENT_FLAG_FAULT_filtered++;
    if(PET_EVENT_FLAG_TS_INVALID & event_flags)     cnt_PET_EVENT_FLAG_TS_INVALID_filtered++;
    if(PET_EVENT_FLAG_GATE_OVERLAY & event_flags)   cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered++;
    if(PET_EVENT_FLAG_QDC_OVERFLOW & event_flags)   cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered++;
    if(PET_EVENT_FLAG_QDC_UNDERFLOW & event_flags)  cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered++;
    if(PET_EVENT_FLAG_ADC_OVERFLOW & event_flags)   cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered++;
    if(PET_EVENT_FLAG_ADC_UNDERFLOW & event_flags)  cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered++;
    if(PET_EVENT_FLAG_SYNC & event_flags)           cnt_PET_EVENT_FLAG_SYNC_filtered++;

    return ok;
}
//--------------------------------------------------------------------------------
bool Histogram_energy::check_filter(float E, float Emin, float Emax)
{
    if(ui->cb_enable_filter_min_energy->isChecked())
    {
        if(E < Emin)
        {
            return false;
        }
    }
    //---
    if(ui->cb_enable_filter_max_energy->isChecked())
    {
        if(E > Emax)
        {
            return false;
        }
    }
    //---
    if(E > HISTOGRAM_MAX_ENERGY)
    {
        return false;
    }
    //---
    return true;
}
//--------------------------------------------------------------------------------
bool Histogram_energy::convert_metadata(QString name, QString *result)
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
void Histogram_energy::refresh(void)
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
    emit debug(QString("event_flags 0x%1").arg(event_flags, 0, 16));
    //---
    Emin = (float)ui->sb_e_min->value();
    Emax = (float)ui->sb_e_max->value();
    N = (float)ui->sb_N->value();
    E = 0;
    max_i = 0;
    //--
    if(ui->cb_enable_filter_min_energy->isChecked() == false)   Emin = HISTOGRAM_MIN_ENERGY;
    if(ui->cb_enable_filter_max_energy->isChecked() == false)   Emax = HISTOGRAM_MAX_ENERGY;
    emit debug(QString("Emin %1 Emax %2")
               .arg(Emin)
               .arg(Emax));
    //--
    delta = ((float)(Emax - Emin)) / ((float)(N - 1));
    emit debug(QString("delta = %1").arg(delta));
    emit debug(QString("N = %1").arg(N));
    //---
    filter_address = ui->sb_filter_address->value();
    ignore_filter_address = ui->rb_event_all->isChecked();
    //---
    clear_buf_energy();
    ok = dat3->read_file(filename);
    if(!ok)
    {
        emit error(QString("Ошибка чтения %1") .arg(filename));
        block_interface(false);
        return;
    }
    //---
    quint64 dat3_cnt = dat3->get_cnt_read_events();
    QTime timer;
    timer.start();
    while(dat3_cnt != cnt_events)
    {
        QCoreApplication::processEvents();
        if(timer.elapsed() > 10000)
        {
            QString text = QString("DAT3 cnt_events %1 != cnt_events %2")
                    .arg(dat3->get_cnt_read_events())
                    .arg(cnt_events);
            messagebox_critical("Ошибка", text);
            emit debug(text);
            block_interface(false);
            return;
        }
    }
    //---
    grapher->set_axis_scale_x(Emin, Emax);
    grapher->clear();
    QList<QPointF> l_point;
    l_point.clear();
    //int min = ui->sb_e_min->value();
    //int max = ui->sb_e_max->value();
    for(int n=0; n<N; n++)
    {
        l_point.append(QPointF(n*delta, buf_energy[n]));
    }
    grapher->add_curve_array(curve, l_point);
    //---

    emit info("Расчёт энергетического спектра");
    emit info(QString("    Файл данных:                        %1")
              .arg(filename));
    emit info(QString("    Канал регистрации:                  %1")
              .arg(ui->rb_event_all->isChecked() ? "Все" : QString("%1").arg(ui->sb_filter_address->value())));
    emit info(QString("    Минимальная энергия:                %1")
              .arg(Emin));
    emit info(QString("    Максимальная энергия:               %1")
              .arg(Emax));
    emit info(QString("    Количество точек:                   %1")
              .arg(N));
    emit info("Зарегистрировано событий:");
    emit info(QString("    всего:                              %1")
              .arg(cnt_events));
    emit info(QString("    по выбранному каналу:               %1")
              .arg(cnt_events_address_filtered));

    print_event_flag_log();

    block_interface(false);
    //TODO
    ui->btn_save_picture_to_file->setEnabled(true);
}
//--------------------------------------------------------------------------------
void Histogram_energy::get_event(pet_event event)
{
    cnt_events++;

    bool ok = false;
    if(ignore_filter_address == false)
    {
        ok = check_filter_address(filter_address, event.src);
        if(!ok)
        {
            cnt_events_address_filtered++;
            return;
        }
    }
    ok = check_event_flags(event_flags, event.flags);
    if(!ok)
    {
        return;
    }

    E = event.xp + event.xn + event.yp + event.yn;
    //int i = (int)(((float)E - (float)Emin) / (float)delta + 0.5f);
    int i = (int)(((float)E - (float)Emin) / (float)delta);
    if(i < 0)
    {
        return;
    }
    if(i > N)
    {
        return;
    }
    if(i > max_i)   max_i = i;
    //---
    if(check_filter(E, Emin, Emax))
    {
        if(i < MAX_SIZE_BUF_ENERGY)
        {
            if(ui->rb_event_all->isChecked())
            {
                buf_energy[i]++;
            }
            else
            {
                if(event.src == ui->sb_filter_address->value())
                {
                    buf_energy[i]++;
                }
            }
        }
        else
        {
            emit error(QString("Ix = %1 (max %2").arg(i).arg(MAX_SIZE_BUF_ENERGY));
            QCoreApplication::processEvents();
        }
    }
    else
    {
        cnt_events_energy_filtered++;
    }
}
//--------------------------------------------------------------------------------
bool Histogram_energy::check_filter_address(uint8_t filter_address, uint8_t event_address)
{
    if(filter_address == event_address) return true;
    return false;
}
//--------------------------------------------------------------------------------
void Histogram_energy::set_event_flags(void)
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
void Histogram_energy::grapher_refresh(void)
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
void Histogram_energy::clear_all(void)
{
    cnt_events = 0;

    Emin = 0;
    Emax = 0;
    N = 0;
    E = 0;
    delta = 0;

    max_i = 0;

    cnt_events = 0;
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
void Histogram_energy::source_expanded(bool state)
{
    ui->frame_source->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_energy::energy_expanded(bool state)
{
    ui->frame_energy->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_energy::events_flag_expanded(bool state)
{
    ui->frame_events_flag->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_energy::param_expanded(bool state)
{
    ui->frame_param->setVisible(state);
}
//--------------------------------------------------------------------------------
void Histogram_energy::save_picture_to_file(void)
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
    text.append("Расчёт энергетического спектра\n");
    text.append(QString("    Канал регистрации:                  %1\n").arg(ui->rb_event_all->isChecked() ? "Все" : QString("%1").arg(ui->sb_filter_address->value())));
    text.append(QString("    Минимальная энергия:                %1\n").arg(Emin));
    text.append(QString("    Максимальная энергия:               %1\n").arg(Emax));
    text.append(QString("    Количество точек:                   %1\n").arg(N));
    text.append("Зарегистрировано событий:\n");
    text.append(QString("    всего:                              %1\n").arg(cnt_events));
    text.append(QString("    по выбранному каналу:               %1\n").arg(cnt_events_address_filtered));

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
            .arg(NAME_HISTOGRAM_ENERGY)
            .arg(dt.toString("dd-MM-yyyy hhmmss"));

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "he_dlg");
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
void Histogram_energy::save_picture_to_file_old(void)
{
    emit debug("create_picture");
    QString filename;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "he_dlg");
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
void Histogram_energy::refresh_metadata(void)
{
    mtw->refresh_metadata();
    block_interface(mtw->count() == 0); //TODO block_interface
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
