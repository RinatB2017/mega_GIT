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
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include "grapherbox.hpp"
#include "ui_oscilloscope.h"
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
#include "oscilloscope.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "myfiledialog.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_errno.h"
#include "pet.h"
//--------------------------------------------------------------------------------
OscilloscopeBox::OscilloscopeBox(QList<pet_discovery_data> pet_config,
                                 QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::OscilloscopeBox),
    p_pet_config(pet_config)
{
    init();

    QVariant cb_sync = 0;
    QVariant cb_auto = 0;
    QVariant cb_type_curve = 0;
    QVariant cb_channel = 0;
    QVariant sb_time = 0;
    QVariant sb_vert_scale_min = 0;
    QVariant sb_vert_scale_max = 0;

    PETCat_options *o = new PETCat_options();
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_sync,              &cb_sync);
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_auto,              &cb_auto);
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_type_curve,        &cb_type_curve);
    o->get_oscilloscope_option(PARAM_oscilloscope_cb_channel,           &cb_channel);
    o->get_oscilloscope_option(PARAM_oscilloscope_sb_time,              &sb_time);
    o->get_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_min,    &sb_vert_scale_min);
    o->get_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_max,    &sb_vert_scale_max);
    o->deleteLater();

    ui->cb_sync->setCurrentIndex(cb_sync.toInt());
    ui->cb_auto->setCurrentIndex(cb_auto.toInt());
    ui->cb_type_curve->setCurrentIndex(cb_type_curve.toInt());
    ui->cb_channel->setCurrentIndex(cb_channel.toInt());
    ui->sb_time->setValue(sb_time.toInt());
    ui->sb_vert_scale_min->setValue(sb_vert_scale_min.toInt());
    ui->sb_vert_scale_max->setValue(sb_vert_scale_max.toInt());
}
//--------------------------------------------------------------------------------
OscilloscopeBox::~OscilloscopeBox()
{
    qDebug() << "~OscilloscopeBox";

    PETCat_options *o = new PETCat_options();
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_sync,              ui->cb_sync->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_auto,              ui->cb_auto->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_type_curve,        ui->cb_type_curve->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_cb_channel,           ui->cb_channel->currentIndex());
    o->set_oscilloscope_option(PARAM_oscilloscope_sb_time,              ui->sb_time->value());
    o->set_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_min,    ui->sb_vert_scale_min->value());
    o->set_oscilloscope_option(PARAM_oscilloscope_sb_vert_scale_max,    ui->sb_vert_scale_max->value());
    o->deleteLater();

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    QByteArray ba = main_splitter->saveState();
    if(ba.isEmpty() ==  false)
    {
        settings->beginGroup(PETCAT_OPTIONS_OSCILLOSCOPE_GROUP);
        settings->setValue("splitterState", ba);
        settings->endGroup();
    }
    else
    {
        qDebug() << "ba.isEmpty()";
    }
    settings->deleteLater();

    if(timer)
    {
        timer->stop();
        timer->deleteLater();
    }
    if(pet_brg)
    {
        pet_brg->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::init(void)
{
    ui->setupUi(this);

    //---
    pet_brg = new PET_device_brg(this);
    //---
    ui->le_device->setReadOnly(true);
    ui->le_device->setText(QString("%1:%2 (Адрес %3)")
                           .arg(p_pet_config.at(0).host.toString())
                           .arg(p_pet_config.at(0).port)
                           .arg(p_pet_config.at(0).addr));

    //---
    QHostAddress addr = p_pet_config.at(0).host;
    PET_RESULT err = pet_brg->set_comm_param(addr,
                                             PETCat_options::get_port_cmd());
    if(err != PET_SUCCESS)
    {
        emit error(PET_device_base::get_PET_RESULT_string(err));
    }
    bool ok = pet_brg->bind(QHostAddress::AnyIPv4);
    if(ok == false)
    {
        emit error(pet_brg->get_last_error());
    }
    //pet_brg->set_max_waiting_msec(5000);
    //---

    ui->cb_sync->clear();
    ui->cb_sync->addItem("Непрерывно",      PET_WAVE_MODE_SYNC_NONE);
    ui->cb_sync->addItem("По фотоимпульсу", PET_WAVE_MODE_SYNC_PULSE);
    //---
    ui->cb_auto->clear();
    ui->cb_auto->addItem("Ручной",          PET_WAVE_MODE_SYNC_NONE);
    ui->cb_auto->addItem("Автоматический",  PET_WAVE_MODE_SYNC_PULSE);
    //---
    ui->cb_channel->clear();
    ui->cb_channel->addItem("1", 0);
    ui->cb_channel->addItem("2", 1);
    ui->cb_channel->addItem("3", 2);
    ui->cb_channel->addItem("4", 3);
    //---

    connect(ui->cb_auto,    SIGNAL(currentIndexChanged(int)),   this, SLOT(set_auto_mode(int)));
    connect(ui->btn_run,    SIGNAL(clicked(bool)),              this, SLOT(run()));

    set_auto_mode(0);
    //---

    cnt_samples = 0;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    grapher = new GrapherBox(this);
    grapher->set_axis_scale_y(0, 640);
    grapher->set_axis_scale_y(-32768, 32768);

    grapher->set_silense(true);

#if 0
    grapher->set_zoom(true, true);
    grapher->set_panning(true, true);
#else
    grapher->set_zoom(false, false);
    grapher->set_panning(false, false);
#endif

    grapher->set_visible_btn_Options(false);
    grapher->set_visible_btn_Load(false);
    grapher->set_visible_btn_Save(false);
    grapher->set_visible_btn_Statistic(false);
    //grapher->set_visible_btn_Clear(false);
    grapher->push_btn_Horizontal(false);
    grapher->push_btn_Vertical(false);
    grapher->set_visible_btn_all_ON(false);
    grapher->set_visible_btn_all_OFF(false);

    grapher->set_visible_btn_Horizontal(false);
    grapher->set_visible_btn_Vertical(false);

    ui->sb_interval->setRange(100, INT_MAX);
    ui->sb_interval->setValue(100);

    //---
    curve_xp = grapher->add_curve("X+");
    curve_xn = grapher->add_curve("X-");
    curve_yp = grapher->add_curve("Y+");
    curve_yn = grapher->add_curve("Y-");

    curve_sum = grapher->add_curve("SUM");
    //---
    QVariant clr_x_plus = 0;
    QVariant clr_x_minus = 0;
    QVariant clr_y_plus = 0;
    QVariant clr_y_minus = 0;
    QVariant clr_sum = 0;
    //---
    PETCat_options *o = new PETCat_options();
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_x_plus,  &clr_x_plus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_x_minus, &clr_x_minus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_y_plus,  &clr_y_plus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_y_minus, &clr_y_minus);
    o->get_oscilloscope_option(PARAM_oscilloscope_clr_sum,     &clr_sum);
    o->deleteLater();
    //---
    grapher->set_curve_color(curve_xp,  clr_x_plus.toInt());
    grapher->set_curve_color(curve_xn,  clr_x_minus.toInt());
    grapher->set_curve_color(curve_yp,  clr_y_plus.toInt());
    grapher->set_curve_color(curve_yn,  clr_y_minus.toInt());
    grapher->set_curve_color(curve_sum, clr_sum.toInt());
    //---
    grapher->legend_off(curve_sum);
    //---

    //---
    ui->sb_time->setRange(SB_TIME_MIN, SB_TIME_MAX);
    ui->sb_time->setSingleStep(SB_TIME_STEP);
    ui->sb_time->setValue(SB_TIME_DEFAULT);
    connect(ui->sb_time,    SIGNAL(valueChanged(int)),  this,   SLOT(redraw_grapher_axis_x(int)));
    redraw_grapher_axis_x(SB_TIME_DEFAULT);
    //---
    ui->cb_type_curve->clear();
    ui->cb_type_curve->addItem("Точечный", DOTS);
    ui->cb_type_curve->addItem("Линейный", LINES);
    ui->cb_type_curve->addItem("Сглаженная кривая", SPLINE_LINES);
    ui->cb_type_curve->setCurrentIndex(1);
    //---
    ui->sb_vert_scale_min->setRange(-(4 * 0xFFFF), 0);
    ui->sb_vert_scale_max->setRange(0, (4 * 0xFFFF));
    ui->sb_vert_scale_min->setValue(-1);
    ui->sb_vert_scale_max->setValue(1);
    //---
    connect(ui->sb_vert_scale_min,  SIGNAL(editingFinished()),  this,   SLOT(set_grapher_axes_y()));
    connect(ui->sb_vert_scale_max,  SIGNAL(editingFinished()),  this,   SLOT(set_grapher_axes_y()));
    //---
    connect(ui->btn_save_picture_to_file,   SIGNAL(clicked(bool)),  this,   SLOT(save_picture_to_file()));
    ui->btn_save_picture_to_file->setDisabled(true);
    //---

    connect(ui->cb_type_curve, SIGNAL(currentIndexChanged(int)),
            this,   SLOT(grapher_refresh()));
    //---
    main_splitter = new QSplitter(Qt::Horizontal, this);
    main_splitter->setObjectName("main_splitter");
    main_splitter->setChildrenCollapsible(false);

    ui->frame_main->setParent(main_splitter);
    grapher->setParent(main_splitter);
    main_splitter->addWidget(ui->frame_main);
    main_splitter->addWidget(grapher);
    main_splitter->setStretchFactor(0, 0);
    main_splitter->setStretchFactor(1, 1);

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    settings->beginGroup(PETCAT_OPTIONS_OSCILLOSCOPE_GROUP);
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
void OscilloscopeBox::set_grapher_axes_y(void)
{
    int min = ui->sb_vert_scale_min->value();
    int max = ui->sb_vert_scale_max->value();

    if(max <= min)
    {
        return;
    }

    grapher->set_axis_scale_y(min, max);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::redraw_grapher_axis_x(int value)
{
    grapher->set_axis_scale_x(0, value);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::set_auto_mode(int index)
{
    uint16_t flag_auto = ui->cb_auto->itemData(index).toInt();
    ui->sb_interval->setEnabled(flag_auto == PET_WAVE_MODE_SYNC_PULSE);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::block_interface_widgets(bool state)
{
    ui->le_device->setDisabled(state);
    ui->cb_channel->setDisabled(state);
    ui->cb_sync->setDisabled(state);
    ui->sb_time->setDisabled(state);
    ui->cb_auto->setDisabled(state);

    ui->sb_interval->setDisabled(state);
    ui->cb_type_curve->setDisabled(state);

    ui->sb_vert_scale_min->setDisabled(state);
    ui->sb_vert_scale_max->setDisabled(state);

    ui->btn_save_picture_to_file->setDisabled(state);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::run(void)
{
    if(timer->isActive())
    {
        timer->stop();
        ui->btn_run->setText("Пуск");
        block_interface_widgets(false);
        return;
    }

    uint16_t flag_auto = ui->cb_auto->itemData(ui->cb_auto->currentIndex()).toInt();
    if((flag_auto == PET_WAVE_MODE_SYNC_PULSE))
    {
        ui->btn_run->setText("Стоп");
        timer->start(ui->sb_interval->value());
        block_interface_widgets(true);
    }
    else
    {
        block_interface_widgets(true);
        ui->btn_run->setDisabled(true);
        update();
        ui->btn_run->setDisabled(false);
        block_interface_widgets(false);
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::get_packet(QHostAddress host, quint16 port, QByteArray packet)
{
    if(host != p_pet_config.at(0).host)
    {
        emit debug(QString("OscilloscopeBox::get_packet: host %1 != %2")
                   .arg(host.toString())
                   .arg(p_pet_config.at(0).host.toString()));
        return;
    }
    if(port != p_pet_config.at(0).port)
    {
        emit debug(QString("OscilloscopeBox::get_packet: port %1 != %2")
                   .arg(port)
                   .arg(p_pet_config.at(0).port));
        return;
    }

    if(packet.isEmpty())
    {
        return;
    }
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        return;
    }

    //---
    is_ok = true;
    //---

    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        emit debug(QString("OscilloscopeBox::get_packet: packet.lenght %1 <= %2")
                   .arg(packet.length())
                   .arg(sizeof(pet_frame_t)));
        return;
    }
    pet_frame_t *frame = (pet_frame_t *)packet.data();
    Q_CHECK_PTR(frame);
    uint8_t cmd = frame->cmd & 0x7F;
    if(cmd != PET_CMD_WAVE)
    {
        return;
    }
    if(frame->cmd & PET_FRAME_CMD_FAULT_MSK)
    {
        pet_frame_error_t *err_frame = (pet_frame_error_t *)packet.data();
        emit error(pet_brg->get_PET_RESULT_string(err_frame->result_cmd));
        return;
    }

    pet_wave_t *pet_wave = (pet_wave_t *)frame->data;
    //---
    grapher->set_silense(true);
    grapher->clear();
    if(pet_wave->count == 0)
    {
        emit error("Ошибка: pet_wave->count == 0");
        return;
    }
    for(unsigned int n=0; n<pet_wave->count; n++)
    {
        grapher->add_curve_data(curve_xp, n*10, pet_wave->point[n].xp);
        grapher->add_curve_data(curve_xn, n*10, pet_wave->point[n].xn);
        grapher->add_curve_data(curve_yp, n*10, pet_wave->point[n].yp);
        grapher->add_curve_data(curve_yn, n*10, pet_wave->point[n].yn);
    }
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::update(void)
{
    bool ok = cmd_WAVE();

    uint16_t flag_auto = ui->cb_auto->itemData(ui->cb_auto->currentIndex()).toInt();
    if((flag_auto == PET_WAVE_MODE_SYNC_NONE))
    {
        if(ok == false)
        {
            timer->stop();
            ui->btn_run->setText("Старт");
            block_interface_widgets(false);
        }
    }
}
//--------------------------------------------------------------------------------
bool OscilloscopeBox::cmd_WAVE(void)
{
    pet_wave_t i_wave;
    int i_wave_len = sizeof(i_wave);
    //---
    pet_wave_data_t o_data;
    memset((char *)&o_data, 0, sizeof(o_data));
    int o_wave_len = sizeof(o_data);
    //---

    int sel = 0;
    int sync = ui->cb_sync->itemData(ui->cb_sync->currentIndex()).toInt();

    i_wave.channel = ui->cb_channel->itemData(ui->cb_channel->currentIndex()).toInt();
    i_wave.mode = (sel | sync | PET_WAVE_MODE_EN_MSK);
    i_wave.count = ui->sb_time->value() / 10 + 1; // не забыть 0 точку

    pet_brg->set_pet_addr(p_pet_config.at(0).addr);
    PET_RESULT res = pet_brg->get_oscillogram(&i_wave,  &i_wave_len,
                                              &o_data,  &o_wave_len);
    if(res != PET_SUCCESS)
    {
#if 0
        //TODO костыль
        if(res == PET_ERR_BAD_DATA)
        {
            return false;
        }
        //---
#endif

        emit error(pet_brg->get_PET_RESULT_string(res));
        uint16_t flag_auto = ui->cb_auto->itemData(ui->cb_auto->currentIndex()).toInt();
        if((flag_auto == PET_WAVE_MODE_SYNC_NONE))
        {
            messagebox_critical("Ошибка", pet_brg->get_PET_RESULT_string(res));
        }
        return false;
    }
    //---
    if(o_data.count > 64)
    {
        emit error(QString("o_data.wave.channel = %1").arg(o_data.channel));
        emit error(QString("o_data.wave.mode = %1").arg(o_data.mode));
        emit error(QString("o_data.wave.count = %1").arg(o_data.count));
        return false;
    }

    if(o_data.count == 0)
    {
        emit error("Нет точек для отображения (count == 0)");
        return false;
    }

    //---
    grapher->clear();
    set_grapher_axes_y();
    //---

    pet_wave_t *w_data = (pet_wave_t *)&o_data;
    //---
    grapher->set_axis_scale_y(ui->sb_vert_scale_min->value(),
                              ui->sb_vert_scale_max->value());
    //---
    for(int n=0; n<w_data->count; n++)
    {
        show_data(w_data->point[n], n*10);
    }
    //---
    ui->btn_save_picture_to_file->setEnabled(true);
    return true;
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::show_data(pet_wave_point sample, int index)
{
    grapher->add_curve_data(curve_xp, index, sample.xp);
    grapher->add_curve_data(curve_xn, index, sample.xn);
    grapher->add_curve_data(curve_yp, index, sample.yp);
    grapher->add_curve_data(curve_yn, index, sample.yn);

    grapher->add_curve_data(curve_sum,  index,  sample.xp + sample.xn + sample.yp + sample.yn);
}
//--------------------------------------------------------------------------------
void OscilloscopeBox::grapher_refresh(void)
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
void OscilloscopeBox::save_picture_to_file(void)
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
    text.append(QString("%1\n").arg(NAME_OSCILLOSCOPE));
    text.append(QString("    Канал:                              %1\n").arg(ui->cb_channel->currentText()));
    text.append(QString("    Синхронизация:                      %1\n").arg(ui->cb_sync->currentText()));
    text.append(QString("    Развертка,нс:                       %1\n").arg(ui->sb_time->value()));
    text.append(QString("    Режим:                              %1\n").arg(ui->cb_auto->currentText()));
    text.append(QString("    Период обновления:                  %1\n").arg(ui->sb_interval->value()));
    text.append(QString("    Вид:                                %1\n").arg(ui->cb_type_curve->currentText()));
    text.append(QString("Диапазон:                               \n"));
    text.append(QString("    max:                                %1\n").arg(ui->sb_vert_scale_max->value()));
    text.append(QString("    min:                                %1\n").arg(ui->sb_vert_scale_min->value()));

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
            .arg(NAME_OSCILLOSCOPE)
            .arg(dt.toString("dd-MM-yyyy hhmmss"));

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "ob_dlg");
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
        emit info(QString("Файл %1 успешyо создан").arg(filename));
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
void OscilloscopeBox::save_picture_to_file_old(void)
{
    emit debug("create_picture");

    QString filename;

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "ob_dlg");
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
