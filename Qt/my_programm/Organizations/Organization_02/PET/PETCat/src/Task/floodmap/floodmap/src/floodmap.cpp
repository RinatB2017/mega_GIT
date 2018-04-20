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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <qwt_color_map.h>
//--------------------------------------------------------------------------------
#include "floodmap.hpp"
//--------------------------------------------------------------------------------
#include "ui_floodmap.h"
//--------------------------------------------------------------------------------
#include "viewer.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "dat3.hpp"
#include "grapherbox.hpp"
#include "pet_event_flag.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "metadata.hpp"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
#include "metadata_widget.hpp"
//--------------------------------------------------------------------------------
FloodMapBox::FloodMapBox(QString file_auto_open,
                         QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::FloodMapBox)
{
    init();

    //TODO
    if(file_auto_open.isEmpty() == false)
    {
        mtw->setCurrentText(file_auto_open);
    }
}
//--------------------------------------------------------------------------------
FloodMapBox::~FloodMapBox()
{
    //---
    PETCat_options *o = new PETCat_options();
    o->set_floodmap_option(PARAM_floodmap_source_expanded,      ui->btn_source_expanded->isChecked());
    o->set_floodmap_option(PARAM_floodmap_energy_expanded,      ui->btn_energy_expanded->isChecked());
    o->set_floodmap_option(PARAM_floodmap_events_flag_expanded, ui->btn_event_flag_expanded->isChecked());
    o->set_floodmap_option(PARAM_floodmap_picture_expanded,     ui->btn_picture_expanded->isChecked());
    o->deleteLater();

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    QByteArray ba = main_splitter->saveState();
    if(ba.isEmpty() ==  false)
    {
        settings->beginGroup(PETCAT_OPTIONS_FLOODMAP_GROUP);
        settings->setValue("splitterState", ba);
        settings->endGroup();
    }
    settings->deleteLater();
    //---

    delete ui;
}
//--------------------------------------------------------------------------------
void FloodMapBox::init(void)
{
    ui->setupUi(this);

    init_widgets();

    //---
    main_splitter = new QSplitter(Qt::Horizontal, this);

    main_splitter->setObjectName("main_splitter");
    main_splitter->setChildrenCollapsible(false);

    ui->frame_main->setParent(main_splitter);
    ui->scrollArea->setParent(main_splitter);
    main_splitter->addWidget(ui->frame_main);
    main_splitter->addWidget(ui->scrollArea);
    main_splitter->setStretchFactor(0, 0);
    main_splitter->setStretchFactor(1, 1);

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    settings->beginGroup(PETCAT_OPTIONS_FLOODMAP_GROUP);
    QByteArray ba = settings->value("splitterState").toByteArray();
    if(ba.isEmpty() == false)
    {
        main_splitter->restoreState(ba);
    }
    settings->endGroup();
    settings->deleteLater();

    layout()->addWidget(main_splitter);

    connect(ui->cb_picture_size,    SIGNAL(currentIndexChanged(int)),   this,   SLOT(block_btn_save_picture_to_file()));
    connect(ui->cb_palette,         SIGNAL(currentIndexChanged(int)),   this,   SLOT(block_btn_save_picture_to_file()));
    connect(ui->btn_save_picture_to_clipboard,  SIGNAL(clicked(bool)),  this,   SLOT(save_picture_to_clipboard()));
    //---

    QVariant sb_min_energy = 0;
    QVariant sb_max_energy = 0;
    QVariant flag_min_energy = 0;
    QVariant flag_max_energy = 0;

    QVariant flag_floodmap_source_expanded = 0;
    QVariant flag_floodmap_energy_expanded = 0;
    QVariant flag_floodmap_event_flag_expanded = 0;
    QVariant flag_floodmap_picture_expanded = 0;

    PETCat_options *o = new PETCat_options();
    o->get_floodmap_option(PARAM_floodmap_sb_min_energy,        &sb_min_energy);
    o->get_floodmap_option(PARAM_floodmap_sb_max_energy,        &sb_max_energy);
    o->get_floodmap_option(PARAM_floodmap_flag_min_energy,      &flag_min_energy);
    o->get_floodmap_option(PARAM_floodmap_flag_max_energy,      &flag_max_energy);
    o->get_floodmap_option(PARAM_floodmap_source_expanded,      &flag_floodmap_source_expanded);
    o->get_floodmap_option(PARAM_floodmap_energy_expanded,      &flag_floodmap_energy_expanded);
    o->get_floodmap_option(PARAM_floodmap_events_flag_expanded, &flag_floodmap_event_flag_expanded);
    o->get_floodmap_option(PARAM_floodmap_picture_expanded,     &flag_floodmap_picture_expanded);
    o->deleteLater();

    ui->sb_min_energy->setValue(sb_min_energy.toInt());
    ui->sb_max_energy->setValue(sb_max_energy.toInt());

    ui->cb_enable_filter_min_energy->setChecked(flag_min_energy.toBool());
    ui->cb_enable_filter_max_energy->setChecked(flag_max_energy.toBool());

    ui->btn_source_expanded->setChecked(flag_floodmap_source_expanded.toBool());
    ui->btn_energy_expanded->setChecked(flag_floodmap_energy_expanded.toBool());
    ui->btn_event_flag_expanded->setChecked(flag_floodmap_event_flag_expanded.toBool());
    ui->btn_picture_expanded->setChecked(flag_floodmap_picture_expanded.toBool());
}
//--------------------------------------------------------------------------------
void FloodMapBox::block_btn_save_picture_to_file(void)
{
    ui->btn_save_picture_to_file->setDisabled(true);
    ui->btn_save_picture_to_clipboard->setDisabled(true);
}
//--------------------------------------------------------------------------------
void FloodMapBox::init_widgets(void)
{
    //---
    //TODO
    ui->sb_channel->setProperty(FLAG_NO_BLOCK, true);
    //---
    mtw = new MetaData_widget(this);
    ui->metadata_widget_layout->addWidget(mtw);
    //ui->metadata_widget_layout->addStretch(1);
    //---
    ui->scrollArea->setStyleSheet("background:black;");

    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);
    //---

    ui->sb_min_energy->setRange(0, 4 * 0xFFFF);
    ui->sb_min_energy->setValue(0);

    ui->sb_max_energy->setRange(0, 4 * 0xFFFF);
    ui->sb_max_energy->setValue(4 * 0xFFFF);

    ui->sb_channel->setRange(0, 0xFF);

    ui->cb_palette->addItem("Серая",    GRAY);
    ui->cb_palette->addItem("Цветная",  COLOR);

    ui->cb_palette->setCurrentIndex(1);

    ui->cb_picture_size->addItem("256x256", QVariant(RESOLUTION_256_256));
    ui->cb_picture_size->addItem("512x512", QVariant(RESOLUTION_512_512));
    ui->cb_picture_size->addItem("1024x1024", QVariant(RESOLUTION_1024_1024));
    ui->cb_picture_size->setCurrentIndex(1);

    ui->sb_channel->setDisabled(true);
    connect(ui->rb_channel_all,     SIGNAL(clicked(bool)),  ui->sb_channel, SLOT(setDisabled(bool)));
    connect(ui->rb_channel_one,     SIGNAL(clicked(bool)),  ui->sb_channel, SLOT(setEnabled(bool)));

    connect(ui->btn_refresh,        SIGNAL(clicked(bool)),  this,   SLOT(refresh_click()));

    connect(ui->btn_save_picture_to_file,   SIGNAL(clicked(bool)),  this,   SLOT(save_picture_to_file()));
    ui->btn_save_picture_to_file->setDisabled(true);
    ui->btn_save_picture_to_clipboard->setDisabled(true);

    //---
    ui->le_events_flags->setReadOnly(true);
    connect(ui->btn_events_flags,   SIGNAL(clicked(bool)),  this,   SLOT(set_event_flags()));

    //---
    QVariant f_value = 0xFE;
    PETCat_options *o = new PETCat_options();
    o->get_floodmap_option(PARAM_floodmap_events_flags,     &f_value);
    o->deleteLater();
    event_flags = f_value.toInt();  //TODO
    //---

    ui->le_events_flags->setText(QString("0x%1").arg(event_flags, 2, 16, QChar('0')));
    //---

    //---
    connect(ui->btn_source_expanded,        SIGNAL(toggled(bool)),  this,   SLOT(source_expanded(bool)));
    connect(ui->btn_energy_expanded,        SIGNAL(toggled(bool)),  this,   SLOT(energy_expanded(bool)));
    connect(ui->btn_event_flag_expanded,    SIGNAL(toggled(bool)),  this,   SLOT(event_flag_expanded(bool)));
    connect(ui->btn_picture_expanded,       SIGNAL(toggled(bool)),  this,   SLOT(picture_expanded(bool)));
    //---

    //---
    source_expanded(false);
    energy_expanded(false);
    event_flag_expanded(false);
    picture_expanded(false);
    //---
    block_interface(mtw->count() == 0); //TODO block_interface
    //---

    init_dat3();
    init_filter_model();
    init_viewer();
    init_floodmap_glass();
}
//--------------------------------------------------------------------------------
void FloodMapBox::refresh_click(void)
{
    refresh();
}
//--------------------------------------------------------------------------------
void FloodMapBox::add_metadata_file(void)
{
    MyComboBox *cb = dynamic_cast<MyComboBox *>(sender());
    if(cb == nullptr)
    {
        return;
    }
    int id = cb->itemData(cb->currentIndex(), Qt::UserRole).toInt();
    if(id == ID_METADATA_APPEND)
    {
        emit debug("emit signal_add_metadata_file()");
        emit signal_add_metadata_file();
    }
}
//--------------------------------------------------------------------------------
void FloodMapBox::init_viewer(void)
{
    viewer = new Viewer(this);
}
//--------------------------------------------------------------------------------
void FloodMapBox::init_dat3(void)
{
    dat3 = new DAT3();
    if(parentWidget())
    {
        connect(dat3,   SIGNAL(info(QString)),      parentWidget(), SIGNAL(info(QString)));
        connect(dat3,   SIGNAL(debug(QString)),     parentWidget(), SIGNAL(debug(QString)));
        connect(dat3,   SIGNAL(error(QString)),     parentWidget(), SIGNAL(error(QString)));
        connect(dat3,   SIGNAL(trace(QString)),     parentWidget(), SIGNAL(trace(QString)));
    }
    connect(dat3,   SIGNAL(send_event(pet_event)),  this,   SLOT(get_event(pet_event)));
}
//--------------------------------------------------------------------------------
void FloodMapBox::init_filter_model(void)
{
    filter_model = new QStandardItemModel(8, 1); // 8 rows, 1 col
    for(int i=0; i<8; i++)
    {
        QStandardItem* item = new QStandardItem(QString("bit_%1").arg(i));

        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        filter_model->setItem(i, 0, item);
    }
}
//--------------------------------------------------------------------------------
void FloodMapBox::init_floodmap_glass(void)
{
#ifdef SHOW_LOCAL_ENERGY
    FloodMap_glass *glass = new FloodMap_glass;
    glass->install(ui->imageLabel);

    btn_show_le = new QPushButton;
    btn_show_le->setText("Рассчитать");
    connect(btn_show_le,    SIGNAL(clicked(bool)),  this,   SLOT(show_le()));

    grapher_le = new GrapherBox(0, 1000,
                                0, 1000,
                                "", "", "",
                                false,
                                this);
    grapher_le->set_visible_btn_all_ON(false);
    grapher_le->set_visible_btn_all_OFF(false);
    grapher_le->set_visible_btn_Horizontal(false);
    grapher_le->set_visible_btn_Vertical(false);
    grapher_le->set_visible_btn_Options(false);
    grapher_le->set_visible_btn_Load(false);
    grapher_le->set_visible_btn_Save(false);
    grapher_le->set_visible_btn_Statistic(false);
    grapher_le->set_visible_btn_Clear(false);

    ui->grapher_layout->addWidget(grapher_le);
    ui->grapher_layout->addWidget(btn_show_le);
    //ui->grapher_layout->addStretch(1);
#endif
}
//--------------------------------------------------------------------------------
#ifdef SHOW_LOCAL_ENERGY
void FloodMapBox::show_le(void)
{
    emit error(TODODO);
    message_critical("Ошибка", TODODO);

    clear_buf_energy();
}
#endif
//--------------------------------------------------------------------------------
void FloodMapBox::set_event_flags(void)
{
    PET_event_flag *dlg = new PET_event_flag();
    dlg->set(event_flags);
    int btn = dlg->exec();
    if(btn != QDialog::Accepted)
    {
        dlg->deleteLater();
        return;
    }

    event_flags = dlg->get();
    ui->le_events_flags->setText(QString("0x%1").arg(event_flags, 2, 16, QChar('0')));
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void FloodMapBox::get_event(pet_event event)
{
    cnt_events++;

    if(ignore_filter_address == false)
    {
        if(check_filter_address(filter_address, event.src) == false)
        {
            cnt_events_address_filtered++;
            return;
        }
    }
    if(check_event_flags(event_flags, event.flags) == true)
    {
        return;
    }

    quint64 energy = event.xp + event.xn + event.yp + event.yn;
    if(flag_enable_filter_min_energy)
    {
        if(energy < min_energy)
        {
            cnt_events_min_energy_filtered++;
            return;
        }
    }
    if(flag_enable_filter_max_energy)
    {
        if(energy > max_energy)
        {
            cnt_events_max_energy_filtered++;
            return;
        }
    }

    double dx = ((double)event.xp - (double)event.xn) / ((double)event.xp + (double)event.xn);
    double dy = ((double)event.yp - (double)event.yn) / ((double)event.yp + (double)event.yn);
    int x = (int)(((double)floodmap_size / 2.0) * (1.0 + dx) + 0.5);
    int y = (int)(((double)floodmap_size / 2.0) * (1.0 + dy) + 0.5);

    if (x < 0 || x >= floodmap_size || y < 0 || y >= floodmap_size) return;

    if(x > max_x) max_x = x;
    if(y > max_y) max_y = y;

    eventCount++;
    if(++array[x][y] > maxCount)
    {
        maxCount++;
    }

#ifdef SHOW_LOCAL_ENERGY

#endif
}
//--------------------------------------------------------------------------------
bool FloodMapBox::check_filter_address(uint8_t filter_address, uint8_t event_address)
{
    if(filter_address == event_address) return true;
    return false;
}
//--------------------------------------------------------------------------------
bool FloodMapBox::check_event_flags(uint8_t filter_flags, uint8_t event_flags)
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
bool FloodMapBox::convert_metadata(QString name, QString *result)
{
    if(name.isEmpty())
    {
        return false;
    }

    MetaData *md = new MetaData();
    connect(md, SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(md, SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(md, SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(md, SIGNAL(trace(QString)),     this,   SIGNAL(trace(QString)));

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
void FloodMapBox::refresh(void)
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
    emit debug(QString("event_flags 0x%1").arg(event_flags,0, 16));
    //---
    filter_address = ui->sb_channel->value();
    ignore_filter_address = ui->rb_channel_all->isChecked();
    flag_enable_filter_min_energy = ui->cb_enable_filter_min_energy->isChecked();
    flag_enable_filter_max_energy = ui->cb_enable_filter_max_energy->isChecked();
    min_energy = ui->sb_min_energy->value();
    max_energy = ui->sb_max_energy->value();
    floodmap_size = ui->cb_picture_size->itemData(ui->cb_picture_size->currentIndex()).toInt();
    //---
    ok = dat3->read_file(filename);
    if(!ok)
    {
        block_interface(false);
        ui->sb_channel->setDisabled(ui->rb_channel_all->isChecked());
        ui->btn_save_picture_to_file->setDisabled(true);
        ui->btn_save_picture_to_clipboard->setDisabled(true);
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
            ui->btn_save_picture_to_file->setDisabled(true);
            ui->btn_save_picture_to_clipboard->setDisabled(true);
            return;
        }
    }
    //---
    viewer->set_floodmap_size(floodmap_size);
    //---
    for(int y=0; y<floodmap_size; y++)
    {
        for(int x=0; x<floodmap_size; x++)
        {
            viewer->set_point(x, y, array[x][y]);
        }
    }
    int pal = ui->cb_palette->itemData(ui->cb_palette->currentIndex()).toInt();

    //---
    QPixmap *image = viewer->create_image(maxCount,
                                          (PALETTE)pal);
    if(!image)
    {
        QString temp_str = "Ошибка создания изображения";
        emit error(temp_str);
        messagebox_critical("Ошибка", temp_str);
        block_interface(false);
        ui->sb_channel->setDisabled(ui->rb_channel_all->isChecked());
        ui->btn_save_picture_to_file->setDisabled(true);
        ui->btn_save_picture_to_clipboard->setDisabled(true);
        return;
    }

    //---
    ui->imageLabel->setPixmap(*image);
    //---
    const QPixmap *p = ui->imageLabel->pixmap();
    if(p)
    {
        QSize pixSize = p->size();
        int x = ui->scrollArea->height();
        pixSize.scale(x, x, Qt::KeepAspectRatio);
        ui->imageLabel->setFixedSize(pixSize);
    }
    //---
    emit debug(QString("image w = %1 h = %2")
               .arg(image->width())
               .arg(image->height()));
    //---

    block_interface(false);
    ui->sb_channel->setDisabled(ui->rb_channel_all->isChecked());

    print_event_flag_log();

    //TODO
    ui->btn_save_picture_to_file->setEnabled(true);
    ui->btn_save_picture_to_clipboard->setEnabled(true);
}
//--------------------------------------------------------------------------------
void FloodMapBox::clear_all(void)
{
    for(int y=0; y<MAX_SIZE_Y; y++)
    {
        for(int x=0; x<MAX_SIZE_X; x++)
        {
            array[x][y] = 0;
        }
    }

    floodmap_size = DEFAULT_SIZE_PICTURE;

    max_x = 0;
    max_y = 0;

    bad_flags   = 0;
    all_filtered = 0;

    maxCount    = 0;
    eventCount  = 0;

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
void FloodMapBox::show_error(const QString &text)
{
    messagebox_critical("Ошибка", text);
}
//--------------------------------------------------------------------------------
void FloodMapBox::save_picture_to_file(void)
{
    emit debug("create_picture");

    //---
#if 0
    QWidget *w_grapher = dynamic_cast<QWidget *>(ui->floodmap_widget);
    if(w_grapher == nullptr)
    {
        return;
    }
    QPixmap pix_grapher = w_grapher->grab(QRect(0, 0, w_grapher->width(), w_grapher->height()));
#endif
    //---
    int picture_size = ui->cb_picture_size->itemData(ui->cb_picture_size->currentIndex()).toInt();
    //---
    QString text;
    text.append("FloodMap\n");
    text.append(QString("    размер изображения:                 %1\n").arg(picture_size));
    text.append(QString("Всего событий:                          %1\n").arg(cnt_events));
    text.append(QString("Отфильтровано:\n"));
    text.append(QString("    всего:                              %1\n").arg(all_filtered));
    text.append(QString("    по адресу:                          %1\n").arg((float)cnt_events_address_filtered / (float)cnt_events * 100.0f));
    text.append(QString("    по мин. энергии:                    %1\n").arg((float)cnt_events_min_energy_filtered / (float)cnt_events * 100.0f));
    text.append(QString("    по макс. энергии:                   %1\n").arg((float)cnt_events_max_energy_filtered / (float)cnt_events * 100.0f));

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

    QPixmap left_pixmap = ((QWidget *)frame)->grab(QRect(0, 0, frame->width(), frame->height()));
    QPixmap *main_pixmap = new QPixmap(left_pixmap.width() + pix_grapher.width(), w_grapher->height());
#else
    //QPixmap *main_pixmap = new QPixmap(pix_grapher.width(), w_grapher->height());
#endif

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
            .arg(NAME_FLOODMAP)
            .arg(dt.toString("dd-MM-yyyy hhmmss"));

    MyFileDialog *dlg = new MyFileDialog("filedialog_states", "fm_dlg");
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

    int pal = ui->cb_palette->itemData(ui->cb_palette->currentIndex()).toInt();
    QPixmap *image = viewer->create_image(maxCount,
                                          (PALETTE)pal);

    bool ok = image->save(filename);
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
void FloodMapBox::save_picture_to_clipboard(void)
{
    QClipboard* c = QApplication::clipboard();
    if(c)
    {
        const QPixmap *pixmap = ui->imageLabel->pixmap();
        if(pixmap)
        {
            c->setPixmap(*pixmap);
        }
    }
}
//--------------------------------------------------------------------------------
#ifdef SHOW_LOCAL_ENERGY
void FloodMapBox::clear_buf_energy(void)
{
    for(int n=0; n<MAX_SIZE_BUF_ENERGY; n++)
    {
        buf_energy[n] = 0;
    }
}
#endif
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void FloodMapBox::source_expanded(bool state)
{
    ui->frame_source->setVisible(state);
}
//--------------------------------------------------------------------------------
void FloodMapBox::energy_expanded(bool state)
{
    ui->frame_energy->setVisible(state);
}
//--------------------------------------------------------------------------------
void FloodMapBox::event_flag_expanded(bool state)
{
    ui->frame_event_flag->setVisible(state);
}
//--------------------------------------------------------------------------------
void FloodMapBox::picture_expanded(bool state)
{
    ui->frame_picture->setVisible(state);
}
//--------------------------------------------------------------------------------
void FloodMapBox::refresh_metadata(void)
{
    mtw->refresh_metadata();
    block_interface(mtw->count() == 0); //TODO block_interface
}
//--------------------------------------------------------------------------------
void FloodMapBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    const QPixmap *p = ui->imageLabel->pixmap();
    if(p)
    {
        QSize pixSize = p->size();
        int x = ui->scrollArea->height();
        pixSize.scale(x, x, Qt::KeepAspectRatio);
        ui->imageLabel->setFixedSize(pixSize);
    }
}
//--------------------------------------------------------------------------------
