/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "ui_labyrinth_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "qxmlputget.h"
#include "labyrinth_mainbox.hpp"
#include "minimap.hpp"
#include "defines.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    init_widgets();
    createImagesDock();

    ui->map_widget->load_map(":/test_map.dat");

    load_widgets();
    unlock_widgets();

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->btn_new_map->setIcon(qApp->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->btn_load_map->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_save_map->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_refresh->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->sb_interval->setRange(MIN_INTERVAL_MS, MAX_INTERVAL_MS);
    ui->sb_interval->setObjectName("sb_interval");

    connect(ui->btn_new_map,   SIGNAL(clicked(bool)),  this,    SLOT(new_map()));
    connect(ui->btn_load_map,   SIGNAL(clicked(bool)),  this,   SLOT(load_map()));
    connect(ui->btn_save_map,   SIGNAL(clicked(bool)),  this,   SLOT(save_map()));

    connect(ui->btn_new_game,       SIGNAL(clicked(bool)),  this,   SLOT(new_game()));

    connect(ui->btn_start,      SIGNAL(clicked(bool)),  this,   SLOT(start()));
    connect(ui->btn_stop,       SIGNAL(clicked(bool)),  this,   SLOT(stop()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(ui->map_widget,     SIGNAL(victory()),      this,   SLOT(update_map()));

    connect(ui->btn_step,       SIGNAL(clicked(bool)),  ui->map_widget, SLOT(update()));
    connect(ui->btn_step,       SIGNAL(clicked(bool)),  this,           SLOT(show_minimap()));
}
//--------------------------------------------------------------------------------
void MainBox::update_map(void)
{
    ui->btn_start->setEnabled(true);
    ui->btn_stop->setDisabled(true);

    unlock_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::lock_widgets(void)
{
    ui->btn_new_map->setDisabled(true);
    ui->btn_load_map->setDisabled(true);
    ui->btn_save_map->setDisabled(true);

    ui->btn_new_game->setDisabled(true);

    ui->btn_start->setDisabled(true);
    ui->btn_stop->setDisabled(false);
    ui->btn_refresh->setDisabled(true);

    ui->sb_interval->setDisabled(true);
}
//--------------------------------------------------------------------------------
void MainBox::unlock_widgets(void)
{
    ui->btn_new_map->setEnabled(true);
    ui->btn_load_map->setEnabled(true);
    ui->btn_save_map->setEnabled(true);

    ui->btn_new_game->setEnabled(true);

    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);
    ui->btn_refresh->setEnabled(true);

    ui->sb_interval->setEnabled(true);
}
//--------------------------------------------------------------------------------
QToolButton * MainBox::create_button(const QString &name,
                                     int id)
{
    QPixmap pixmap;
    bool ok = pixmap.load(name);
    if(!ok)
    {
        return nullptr;
    }

    QIcon icon(pixmap);

    QToolButton *btn = new QToolButton(this);
    btn->setCheckable(true);
    btn->setIconSize(QSize(32, 32));
    btn->setIcon(icon);
    btn->setToolTip(name);
    btn->setAutoExclusive(true);
    btn->setProperty(PROPERTY_ID, id);

    connect(btn, SIGNAL(clicked(bool)), ui->map_widget, SLOT(set_cursor()));

    return btn;
}
//--------------------------------------------------------------------------------
void MainBox::createImagesDock(void)
{
    MainWindow *mw = dynamic_cast<MainWindow*>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *image_bar = new QToolBar("image_bar");
    image_bar->setObjectName("image_bar");

    for(int i=0; i<32; i++)
    {
        QString temp = QString(":/images/%1.png").arg(i);
        QToolButton *tb = create_button(temp, i);
        if(tb)
        {
            image_bar->addWidget(tb);
        }
    }

    mw->addToolBar(Qt::TopToolBarArea, image_bar);
}
//--------------------------------------------------------------------------------
void MainBox::start(void)
{
    bool ok = ui->map_widget->start(ui->sb_interval->value());
    if(ok)
    {
        lock_widgets();
    }
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    ui->map_widget->stop();
    unlock_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::refresh(void)
{
    ui->map_widget->refresh();
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");

#if 0
    ui->map_widget->remove_player();
    ui->map_widget->set_begin_player();
#endif

#if 0
    for(int y=0; y<5; y++)
    {
        for(int x=0; x<5; x++)
        {
            //ui->map_widget->add_item(x, y, WALL_ID);
            ui->map_widget->put_picture(WALL_ID, x, y);
        }
    }
#endif

#if 0
    MiniMap *map = new MiniMap;
    connect(map,    SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(map,    SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(map,    SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(map,    SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    map->new_map(25, 25);
    for(int y=0; y<25; y++)
    {
        for(int x=0; x<25; x++)
        {
            //map->set(x, y, ui->map_widget->get_id(x,y));
            map->set(x, y, WALL_ID);
        }
    }
    map->show();
#endif
}
//--------------------------------------------------------------------------------
void MainBox::show_minimap(void)
{
    bool ok = false;
    int player_x = 0;
    int player_y = 0;
    ok = ui->map_widget->find_player(&player_x, &player_y);
    if(!ok)
    {
        emit error("Player not found!");
        return;
    }
    emit debug(QString("player_x %1").arg(player_x));
    emit debug(QString("player_y %1").arg(player_y));

    int start_x = player_x - 1;
    int start_y = player_y - 1;
    int end_x = player_x + 1;
    int end_y = player_y + 1;

    for(int y=start_y; y<=end_y; y++)
    {
        for(int x=start_x; x<=end_x; x++)
        {
            int id = ui->map_widget->get_id(x, y);
            ok = ui->mouse_widget->set(x - start_x,
                                       y - start_y,
                                       id);
            if(!ok)
            {
                emit error(QString("ui->mouse_widget->set(%1,%2,%3)")
                           .arg(x)
                           .arg(y)
                           .arg(id));
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::new_map(void)
{
    emit info("Create MAP");

    QDialog *dlg = new QDialog();

    QSpinBox *sb_max_x = new QSpinBox();
    sb_max_x->setMinimum(MIN_WIDTH);
    sb_max_x->setMaximum(MAX_WIDTH);
    sb_max_x->setValue(MAX_WIDTH);

    QSpinBox *sb_max_y = new QSpinBox();
    sb_max_y->setMinimum(MIN_HEIGHT);
    sb_max_y->setMaximum(MAX_HEIGHT);
    sb_max_y->setValue(MAX_HEIGHT);

    QString w = QString("width (%1-%2").arg(MIN_WIDTH).arg(MAX_WIDTH);
    QString h = QString("height (%1-%2)").arg(MIN_HEIGHT).arg(MAX_HEIGHT);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(w), 0, 0);
    grid->addWidget(sb_max_x,      0, 1);
    grid->addWidget(new QLabel(h), 1, 0);
    grid->addWidget(sb_max_y,      1, 1);

    QDialogButtonBox *btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(btn_box, SIGNAL(rejected()), dlg, SLOT(reject()));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(grid);
    vbox->addWidget(btn_box);
    dlg->setLayout(vbox);

    int btn = dlg->exec();
    if(btn == dlg->Accepted)
    {
        ui->map_widget->new_map(sb_max_x->value(),
                                sb_max_y->value());
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_map(void)
{
    emit info("Load MAP");

#if 0
    ui->map_widget->load_map("map.dat");
#else
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter("DAT files (*.dat)");
    dlg->setDefaultSuffix("dat");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    //dlg->setDirectory(".");
    dlg->selectFile("noname");
    if(dlg->exec())
    {
        QString filename = dlg->selectedFiles().at(0);
        ui->map_widget->load_map(filename);
    }
    delete dlg;
#endif
}
//--------------------------------------------------------------------------------
void MainBox::save_map(void)
{
    emit info("Save MAP");

#if 0
    ui->map_widget->save_map("map.dat");
#else
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter("DAT files (*.dat)");
    dlg->setDefaultSuffix("dat");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setOption(QFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QString filename = dlg->selectedFiles().at(0);
        emit info(filename);

        if(filename.isEmpty() == false)
        {
            ui->map_widget->save_map(filename);
        }
    }
    delete dlg;
#endif
}
//--------------------------------------------------------------------------------
void MainBox::new_game(void)
{
    ui->map_widget->remove_player();
    ui->map_widget->set_begin_player();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
