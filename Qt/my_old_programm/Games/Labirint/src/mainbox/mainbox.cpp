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
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QPainter>
#include <QSpinBox>
#include <QToolBar>
#include <QDialog>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "qxmlputget.h"
#include "mainbox.hpp"
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
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    id = -1;
    direction_move  = RIGHT;
    player_x = -1;
    player_y = -1;

    createTimer();
    createTestBar();
    createImagesDock();

    load_map(":/map.dat");
}
//--------------------------------------------------------------------------------
void MainBox::createTimer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow*>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_new_map = add_button(testbar,
                                          new QToolButton(this),
                                          qApp->style()->standardIcon(QStyle::SP_FileDialogNewFolder),
                                          "new map",
                                          "new map");
    QToolButton *btn_load_map = add_button(testbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_DialogOpenButton),
                                           "load map",
                                           "load map");
    QToolButton *btn_save_map = add_button(testbar,
                                           new QToolButton(this),
                                           qApp->style()->standardIcon(QStyle::SP_DialogSaveButton),
                                           "save map",
                                           "save map");

    sb_interval = new QSpinBox(this);
    sb_interval->setMinimum(50);
    sb_interval->setMaximum(10000);
    sb_interval->setObjectName("sb_interval");

    testbar->addWidget(sb_interval);

    QToolButton *btn_start = add_button(testbar,
                                        new QToolButton(this),
                                        qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                        "start",
                                        "start");
    QToolButton *btn_stop = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaStop),
                                       "stop",
                                       "stop");

    connect(btn_new_map,  SIGNAL(clicked(bool)), this, SLOT(new_map()));
    connect(btn_load_map, SIGNAL(clicked(bool)), this, SLOT(load_map()));
    connect(btn_save_map, SIGNAL(clicked(bool)), this, SLOT(save_map()));
    connect(btn_start,    SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(btn_stop,     SIGNAL(clicked(bool)), this, SLOT(stop()));
}
//--------------------------------------------------------------------------------
QToolButton * MainBox::create_button(const QString &name,
                                     int id)
{
    QPixmap pixmap;
    pixmap.load(name);

    QIcon icon(pixmap);

    QToolButton *btn = new QToolButton(this);
    btn->setCheckable(true);
    btn->setIconSize(QSize(32, 32));
    btn->setIcon(icon);
    btn->setToolTip(name);
    btn->setAutoExclusive(true);
    btn->setProperty("property_id", id);

    connect(btn, SIGNAL(clicked(bool)), this, SLOT(set_cursor()));

    return btn;
}
//--------------------------------------------------------------------------------
void MainBox::set_cursor(void)
{
    QToolButton *btn = dynamic_cast<QToolButton*>(sender());
    if(!btn)
    {
        emit error("set_cursor");
        return;
    }

    id = btn->property("property_id").toInt();
    setCursor(QCursor(btn->icon().pixmap(32, 32)));
}
//--------------------------------------------------------------------------------
void MainBox::createImagesDock(void)
{
    MainWindow *mw = dynamic_cast<MainWindow*>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);

    int cnt = 1;
    //int angle = 90;
    for(int y=0; y<7; y++)
    {
        for(int x=0; x<5; x++)
        {
            QString filename = QString(":/images/%1.png").arg(cnt);
            if(cnt < 33)
            {
                grid->addWidget(create_button(filename, cnt), y, x);
            }
            cnt++;
        }
    }

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(grid);
    vbox->addStretch(1);

    QFrame *frame = new QFrame();
    frame->setLayout(vbox);

    QDockWidget *dock = new QDockWidget("image_dock");
    dock->setObjectName("image_dock");
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dock->setWidget(frame);

    mw->addDockWidget(Qt::LeftDockWidgetArea, dock);

    QMenu *windowsMenu = mw->get_windows_menu();
    if(windowsMenu)
    {
        windowsMenu->addAction(dock->toggleViewAction());
    }
}
//--------------------------------------------------------------------------------
QPixmap MainBox::rotate(const QString &filename, int angle)
{
    QPixmap shipPixels(filename);
    QPixmap rotatePixmap(shipPixels.size());
    rotatePixmap.fill(Qt::transparent);

    QPainter p(&rotatePixmap);
    p.setRenderHint(QPainter::Antialiasing); // сглаживание
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotatePixmap.size().width() / 2, rotatePixmap.size().height() / 2);
    p.rotate(angle); // градус
    p.translate(-rotatePixmap.size().width() / 2, -rotatePixmap.size().height() / 2);
    p.drawPixmap(0, 0, shipPixels);
    p.end();
    shipPixels = rotatePixmap;
    return shipPixels;
}
//--------------------------------------------------------------------------------
bool MainBox::find_player(void)
{
    int cnt_player = 0;
    for(int y=0; y<ui->grid_map->rowCount(); y++)
    {
        for(int x=0; x<ui->grid_map->columnCount(); x++)
        {
            int id = get_picture_id(x, y);
            if(id == PLAYER_ID)
            {
                player_x = x;
                player_y = y;
                cnt_player++;
            }
        }
    }
    if(cnt_player == 1) return true;
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::start(void)
{
    bool ok = find_player();
    if(!ok)
    {
        QMessageBox::critical(this, "Ошибка!", "Объект player(31) не найден!");
        return;
    }
    timer->start(sb_interval->value());
}
//--------------------------------------------------------------------------------
void MainBox::stop(void)
{
    timer->stop();
}
//--------------------------------------------------------------------------------
void MainBox::update(void)
{
    switch(direction_move)
    {
    case UP:
        //emit info("move UP");
        player_move_up();
        break;

    case DOWN:
        //emit info("move DOWN");
        player_move_down();
        break;

    case LEFT:
        //emit info("move LEFT");
        player_move_left();
        break;

    case RIGHT:
        //emit info("move RIGHT");
        player_move_right();
        break;

    default:
        direction_move = RIGHT;
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::player_move_up(void)
{
    int id_victory = get_picture_id(player_x, player_y-1);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        QMessageBox::information(this, "Победа!", "Цель достигнута!");
        return;
    }

    int id_left = get_picture_id(player_x-1, player_y);
    if(id_left == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x--;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = LEFT;
        return;
    }

    int id_up = get_picture_id(player_x, player_y-1);
    if(id_up == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y--;
        put_picture(PLAYER_ID, player_x, player_y);
    }
    else
    {
        direction_move = RIGHT;
    }
}
//--------------------------------------------------------------------------------
void MainBox::player_move_down(void)
{
    int id_victory = get_picture_id(player_x, player_y+1);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        QMessageBox::information(this, "Победа!", "Цель достигнута!");
        return;
    }

    int id_right = get_picture_id(player_x+1, player_y);
    if(id_right == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x++;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = RIGHT;
        return;
    }

    int id_down = get_picture_id(player_x, player_y+1);
    if(id_down == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y++;
        put_picture(PLAYER_ID, player_x, player_y);
    }
    else
    {
        direction_move = LEFT;
    }
}
//--------------------------------------------------------------------------------
void MainBox::player_move_left(void)
{
    int id_victory = get_picture_id(player_x-1, player_y);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        QMessageBox::information(this, "Победа!", "Цель достигнута!");
        return;
    }

    int id_down = get_picture_id(player_x, player_y+1);
    if(id_down == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y++;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = DOWN;
        return;
    }

    int id_left = get_picture_id(player_x-1, player_y);
    if(id_left == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x--;
        put_picture(PLAYER_ID, player_x, player_y);
    }
    else
    {
        direction_move = UP;
    }
}
//--------------------------------------------------------------------------------
void MainBox::player_move_right(void)
{
    int id_victory = get_picture_id(player_x+1, player_y);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        QMessageBox::information(this, "Победа!", "Цель достигнута!");
        return;
    }

    int id_up = get_picture_id(player_x, player_y-1);
    if(id_up == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y--;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = UP;
        return;
    }

    int id_right = get_picture_id(player_x+1, player_y);
    if(id_right == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x++;
        put_picture(PLAYER_ID, player_x, player_y);
    }
    else
    {
        direction_move = DOWN;
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
        new_map(sb_max_x->value(),
                sb_max_y->value());
    }
}
//--------------------------------------------------------------------------------
void MainBox::put_picture(int id,
                          int x,
                          int y)
{
    QPixmap pixmap;
    bool ok = pixmap.load(QString(":/images/%1.png").arg(id));
    if(!ok) return;

    QLabel *label = new QLabel();
    label->setProperty("property_id", id);

    QLayoutItem *item = ui->grid_map->itemAtPosition(y, x);
    if(item)
    {
        QLabel *w = (QLabel *)item->widget();
        if(w)
        {
            w->setPixmap(pixmap);
            w->setProperty("property_id", id);
        }
    }
}
//--------------------------------------------------------------------------------
int MainBox::get_picture_id(int x, int y)
{
    int id = -1;
    QLayoutItem *item = ui->grid_map->itemAtPosition(y, x);
    if(item)
    {
        QLabel *w = (QLabel *)item->widget();
        if(w)
        {
            id = w->property("property_id").toInt();
        }
    }
    return id;
}
//--------------------------------------------------------------------------------
void MainBox::new_map(int max_x, int max_y)
{
    if(max_x < MIN_WIDTH) max_x = MIN_WIDTH;
    if(max_x > MAX_WIDTH) max_x = MAX_WIDTH;
    if(max_y < MIN_HEIGHT) max_y = MIN_HEIGHT;
    if(max_y > MAX_HEIGHT) max_y = MAX_HEIGHT;

    //---
    QLayoutItem *child;
    while ((child = ui->grid_map->takeAt(0)) != 0)
    {
        if (child->widget())
            delete child->widget();
        if (child->layout())
            delete child->layout();
        delete child;
    }
    //---

    ui->grid_map->setMargin(0);
    ui->grid_map->setSpacing(0);
    // заполнение карты
    for(int y=1; y<(max_y-1); y++)
    {
        for(int x=1; x<(max_x-1); x++)
        {
            QPixmap pixmap;
            pixmap.load(":/images/5.png");

            QLabel *label = new QLabel();
            label->setProperty("property_id", 5);
            label->installEventFilter(this);
            label->setPixmap(pixmap);

            ui->grid_map->addWidget(label, y, x);
        }
    }
    //заполнение бордюра карты
    QPixmap p_wall;
    p_wall.load(":/images/13.png");

    for(int x=0; x<max_x; x++)
    {
        QLabel *l_wall = new QLabel();
        l_wall->setProperty("property_id", 13);
        l_wall->installEventFilter(this);
        l_wall->setPixmap(p_wall);
        ui->grid_map->addWidget(l_wall, 0, x);
    }
    for(int x=0; x<max_x; x++)
    {
        QLabel *l_wall = new QLabel();
        l_wall->setProperty("property_id", 13);
        l_wall->installEventFilter(this);
        l_wall->setPixmap(p_wall);
        ui->grid_map->addWidget(l_wall, max_y, x);
    }
    for(int y=1; y<(max_y-1); y++)
    {
        QLabel *l_wall = new QLabel();
        l_wall->setProperty("property_id", 13);
        l_wall->installEventFilter(this);
        l_wall->setPixmap(p_wall);
        ui->grid_map->addWidget(l_wall, y, 0);
    }
    for(int y=1; y<(max_y-1); y++)
    {
        QLabel *l_wall = new QLabel();
        l_wall->setProperty("property_id", 13);
        l_wall->installEventFilter(this);
        l_wall->setPixmap(p_wall);
        ui->grid_map->addWidget(l_wall, y, max_x-1);
    }
    //
}
//--------------------------------------------------------------------------------
bool MainBox::load_map(const QString &filename)
{
    direction_move   = RIGHT;
    player_x = 1;
    player_y = 1;

    QXmlGet *xmlGet = new QXmlGet();
    bool ok = xmlGet->load(filename);
    if(!ok)
    {
        emit error(QString("file %1 not load!").arg(filename));
        return false;
    }

    ok = xmlGet->find("width");
    if(!ok)
    {
        emit error("find width not found!");
        return false;
    }

    int w = xmlGet->getInt(-1);
    if(w<0)
    {
        emit error("width < 0");
        return false;
    }

    ok = xmlGet->find("height");
    if(!ok)
    {
        emit error("find height not found");
        return false;
    }

    int h = xmlGet->getInt(-1);
    if(h<0)
    {
        emit error("height < 0");
        return false;
    }

    emit info(QString("width %1").arg(w));
    emit info(QString("height %1").arg(h));

    new_map(w, h);

    int y = 0;
    ok = xmlGet->find("items");
    while(ok)
    {
        QString temp = xmlGet->getString("");
        //emit info(QString("find %1").arg(temp));
        QByteArray ba;

        QByteArray ta;
        ta.clear();
        ta.append(temp);
        //emit trace(ta.toHex());

        ba = QByteArray::fromHex(ta);
        //emit trace(ba.toHex());
        for(int x=0; x<ba.length(); x++)
        {
            put_picture(ba.at(x), x, y);
        }

        y++;
        ok = xmlGet->findNext("items");
    }

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::save_map(const QString &filename)
{
    QXmlPut *xmlPut = new QXmlPut("Map");
    xmlPut->putInt("width",  ui->grid_map->columnCount());
    xmlPut->putInt("height", ui->grid_map->rowCount());

    for(int y=0; y<ui->grid_map->rowCount(); y++)
    {
        QByteArray ba;
        ba.clear();

        QString temp;
        temp.clear();
        for(int x=0; x<ui->grid_map->columnCount(); x++)
        {
            QLayoutItem *item = ui->grid_map->itemAtPosition(y, x);
            if(item)
            {
                QWidget *w = item->widget();
                if(w)
                {
                    int id = w->property("property_id").toInt();
                    ba.append(id);
                    temp.append(QString("%1 ").arg(id));
                    //sl.append(QString("%1 ").arg(id));
                }
            }
        }
        emit info(temp);
        xmlPut->putString("items", ba.toHex());
    }

    xmlPut->save(filename);
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_map(void)
{
    emit info("Load MAP");

#if 0
    load_map("map.dat");
#else
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setNameFilter("DAT files (*.dat)");
    dlg->setDefaultSuffix("dat");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    if(dlg->exec())
    {
        QString filename = dlg->selectedFiles().at(0);
        load_map(filename);
    }
    delete dlg;
#endif
}
//--------------------------------------------------------------------------------
void MainBox::save_map(void)
{
    emit info("Save MAP");

#if 0
    save_map("map.dat");
#else
    QFileDialog *dlg;

    dlg = new QFileDialog;
    dlg->setAcceptMode(QFileDialog::AcceptSave);
    dlg->setNameFilter("DAT files (*.dat)");
    dlg->setDefaultSuffix("dat");
    dlg->setOption(QFileDialog::DontUseNativeDialog, true);
    dlg->setDirectory(".");
    dlg->selectFile("noname");
    dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QString filename = dlg->selectedFiles().at(0);
        emit info(filename);

        if(filename.isEmpty() == false)
        {
            save_map(filename);
        }
    }
    delete dlg;
#endif
}
//--------------------------------------------------------------------------------
void MainBox::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QPushButton *pb=qobject_cast<QPushButton *>(btn))
    {
        pb->setDisabled(state);
    }
    if (QToolButton *tb=qobject_cast<QToolButton *>(btn))
    {
        tb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MainBox::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QLabel *label = (QLabel *)obj;
        label->setPixmap(cursor().pixmap());
        label->setProperty("property_id", id);
        return true;
    }
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
