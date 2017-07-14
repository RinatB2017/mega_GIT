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
#include <QTreeWidgetItem>
#include <QProgressDialog>
#include <QTreeWidget>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QModelIndex>
#include <QSplitter>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QPixmap>
#include <QAction>
#include <QTime>
#include <QMenu>

#include <QCursor>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "b590_powersupply.hpp"
#include "b590_packets.hpp"

#include "b590_toolbar.hpp"
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "b590.hpp"
#include "ui_b590.h"
#include "crc.h"
#include "defines.hpp"
#include "B590_fram.hpp"
//--------------------------------------------------------------------------------
#include "qhexedit.h"
//--------------------------------------------------------------------------------
B590::B590(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::B590)
{
    init();
}
//--------------------------------------------------------------------------------
B590::~B590()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void B590::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),    topLevelWidget(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)),   topLevelWidget(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)),   topLevelWidget(), SIGNAL(error(QString)));
    connect(this, SIGNAL(message(QString)), topLevelWidget(), SIGNAL(message(QString)));
}
//--------------------------------------------------------------------------------
void B590::create_hex_views(void)
{
    //TODO create_hex_views
    he_fram = new QHexEdit(this);

    QFont font("Liberation Mono", 10);
    he_fram->setFont(font);

    he_fram->setToolTip("he");
    he_fram->setReadOnly(true);
    //he_fram->setContextMenuPolicy(Qt::CustomContextMenu);

    //he_fram->setStyleSheet("text-color:green");
    //he_fram->setStyleSheet("background-color:black");

    ui->hex_layout->addWidget(he_fram);
}
//--------------------------------------------------------------------------------
void B590::calc_crc16_user_U(void)
{
#if 0
    uint8_t data[0xFFFF] = { 0 };
    uint16_t len = he_user_U->data().length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_user_U->data().at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(user_U) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B590::calc_crc16_user_I(void)
{
#if 0
    uint8_t data[1024] = { 0 };
    uint16_t len = he_user_I->data().length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_user_I->data().at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(user_I) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B590::calc_crc16_factory_U(void)
{
#if 0
    uint8_t data[1024] = { 0 };
    uint16_t len = he_factory_U->data().length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_factory_U->data().at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(factory_U) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B590::calc_crc16_factory_I(void)
{
#if 0
    uint8_t data[1024] = { 0 };
    uint16_t len = he_factory_I->data().length();
    for(int n=0; n<len; n++)
    {
        data[n] = he_factory_I->data().at(n);
    }
    uint16_t crc16 = CRC::powersupply_crc16(data, len-2);
    emit info(QString("len %1").arg(len));
    emit info(QString("CRC16(factory_I) 0x%1").arg(crc16, 0, 16));
#endif
}
//--------------------------------------------------------------------------------
void B590::init(void)
{
    ui->setupUi(this);
    connect_log();

    powersupply = new Powersupply_B590();

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        connect(powersupply, SIGNAL(info(QString)),    mw, SIGNAL(info(QString)));
        connect(powersupply, SIGNAL(debug(QString)),   mw, SIGNAL(debug(QString)));
        connect(powersupply, SIGNAL(error(QString)),   mw, SIGNAL(error(QString)));
        connect(powersupply, SIGNAL(message(QString)), mw, SIGNAL(message(QString)));
    }

    ui->btn_find_devices->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->sb_address->setMinimum(MIN_ADDRESS_B590);
    ui->sb_address->setMaximum(MAX_ADDRESS_B590);

    //
    create_hex_views();

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);

    ui->tw_fram->setParent(splitter);
    ui->hex_layout->setParent(splitter);

    QFrame *hex_frame = new QFrame();
    hex_frame->setLayout(ui->hex_layout);

    splitter->addWidget(hex_frame);
    splitter->addWidget(ui->tw_fram);
    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 1);

    ui->horizontalLayout->addWidget(splitter);
    //

    connect(ui->btn_find_devices, SIGNAL(pressed()), this, SLOT(find_devices()));
    connect(ui->tw_fram, SIGNAL(clicked(QModelIndex)), this, SLOT(tw_fram_clicked(QModelIndex)));

    block_interface(true);

    add_menu();

#ifdef TEST
    createPowerSupplyBar();
#endif

    ui->btn_find_devices->setDisabled(false);
    ui->sb_address->setDisabled(false);

    ui->tw_fram->setHeaderLabel("data");
}
//--------------------------------------------------------------------------------
void B590::tw_fram_clicked(QModelIndex index)
{
    int address = 0;
    uint16_t size_data = 0;

    //TODO tw_fram_clicked
    address = index.data(QTreeWidgetItem::UserType + DATA_ADDRESS).toInt();
    size_data = index.data(QTreeWidgetItem::UserType + DATA_SIZE).toInt();
    emit info(QString("address %1").arg(address));
    emit info(QString("size %1").arg(size_data));

    he_fram->clearHighlight(0, sizeof(FRAM_B590));
    he_fram->highlightForeground(address, address + size_data - 1, Qt::black);
    he_fram->highlightBackground(address, address + size_data - 1, Qt::yellow);
}
//--------------------------------------------------------------------------------
void B590::add_menu(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw)
    {
        emit error("menu is failed!");
        return;
    }
    QMenu *menu = new QMenu(tr("Б5-90"));

    QAction *a_load_fram = new QAction(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton), "load fram", this);
    QAction *a_save_fram = new QAction(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton), "save fram", this);
    QAction *a_load_fram_from_file = new QAction(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton), "load fram from file", this);
    QAction *a_save_fram_to_file = new QAction(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton), "save fram to file", this);
    QAction *a_test = new QAction(qApp->style()->standardIcon(QStyle::SP_CommandLink), "test", this);

    connect(a_load_fram, SIGNAL(triggered(bool)), this, SLOT(load_fram()));
    connect(a_save_fram, SIGNAL(triggered(bool)), this, SLOT(save_fram()));
    connect(a_load_fram_from_file, SIGNAL(triggered(bool)), this, SLOT(load_fram_from_file()));
    connect(a_save_fram_to_file, SIGNAL(triggered(bool)), this, SLOT(save_fram_to_file()));
    connect(a_test, SIGNAL(triggered(bool)), this, SLOT(test()));

    menu->addAction(a_load_fram);
    menu->addAction(a_save_fram);
    menu->addSeparator();
    menu->addAction(a_load_fram_from_file);
    menu->addAction(a_save_fram_to_file);
    menu->addSeparator();
    menu->addAction(a_test);

    mw->add_menu(1, menu);
}
//--------------------------------------------------------------------------------
void B590::dataChanged(void)
{
    //TODO dataChanged
#if 0
    uint16_t address = he_fram->cursorPos();
    uint8_t data = he_fram->data()->at(address);
    emit trace(QString("address 0x%1 data 0x%2")
                 .arg(address, 0, 16)
                 .arg(data, 0, 16));
    bool ok = save_data(address, data);
    if(!ok)
    {
        emit error(QString("address 0x%1 is bad!")
                   .arg(address, 0, 16));
    }
#endif
}
//--------------------------------------------------------------------------------
void B590::find_devices(void)
{
    bool ok = false;

    ui->btn_find_devices->setEnabled(false);
    ui->sb_address->setEnabled(false);

    emit info(tr("Поиск устройств"));

    powersupply->set_address(ui->sb_address->value());

    powersupply->set_state_silence(true);
    ok = powersupply->find_device();
    powersupply->set_state_silence(false);
    if(!ok)
    {
        emit error("Источник питания Б5-90 НЕ найден");
        ui->btn_find_devices->setEnabled(true);
        ui->sb_address->setEnabled(true);
        return;
    }

    emit info(tr("Поиск устройств закончен"));
    block_interface(false);
}
//--------------------------------------------------------------------------------
void B590::block_this_button(bool state)
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
void B590::block_interface(bool state)
{
    ui->btn_find_devices->setDisabled(state);
    ui->sb_address->setDisabled(state);
}
//--------------------------------------------------------------------------------
QToolButton *B590::add_button(QToolBar *tool_bar,
                              QToolButton *tool_button,
                              QIcon icon,
                              const QString &text,
                              const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void B590::createPowerSupplyBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    B590_toolbar *psBar = new B590_toolbar(tr("powersupply B5-90"), this);
    psBar->setObjectName("ps_B590");
    mw->addToolBarBreak();
    mw->addToolBar(Qt::TopToolBarArea, psBar);

    connect(psBar, SIGNAL(press(int)), powersupply, SLOT(run_dialog(int)));
}
//--------------------------------------------------------------------------------
void B590::load_fram(void)
{
    //TODO load_fram
    block_this_button(true);
    emit info("read_fram begin");

    fram_data.clear();

    powersupply->set_state_silence(true);
    QProgressDialog *pd = new QProgressDialog("Operation in progress.", "Cancel", 0, MAX_ADDRESS_FRAM_B590);
    pd->setWindowModality(Qt::WindowModal);
    pd->setFixedWidth(320);
    for(uint16_t address = 0; address < MAX_ADDRESS_FRAM_B590; address+=100)
    {
        pd->setValue(address);
        if(pd->wasCanceled())
        {
            break;
        }

        QByteArray data;
        data.clear();

        int len = MAX_ADDRESS_FRAM_B590 - address;
        if(len > 100) len=100;
        if(len < 1) len=1;
        bool ok = powersupply->fram_read(address, len, &data);
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
        fram_data.append(data);
    }
    pd->close();
    powersupply->set_state_silence(false);

    he_fram->setData(QHexEditData::fromMemory(fram_data));
    emit info(QString("read %1 bytes").arg(fram_data.size()));

    emit info("read_fram end");
    analize();
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void B590::save_fram(void)
{
    int btn = QMessageBox::question(this,
                                    "save data",
                                    tr("Сохранить данные в FRAM?"),
                                    QMessageBox::No | QMessageBox::Yes);
    if(btn != QMessageBox::Yes) return;

    block_this_button(true);
    emit info("save_data_to_fram begin");
    powersupply->set_state_silence(true);
    for(uint16_t address = 0; address < MAX_ADDRESS_FRAM_B590; address+=100)
    {
        int len = MAX_ADDRESS_FRAM_B590 - address;
        if(len > 100) len=100;
        if(len < 1) len=1;
        bool ok = powersupply->fram_write(address, len, fram_data.mid(address, len));
        if(!ok)
        {
            powersupply->print_last_error();
            return;
        }
    }
    powersupply->set_state_silence(false);

    he_fram->setData(QHexEditData::fromMemory(fram_data));
    emit info(QString("save %1 bytes").arg(fram_data.size()));
    emit info("save_data_to_fram end");
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void B590::load_fram_from_file(void)
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load data"), ".", tr("Data Files (*.dat)"));
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.read(MAX_ADDRESS_FRAM_B590);
        if(ba.length() != MAX_ADDRESS_FRAM_B590)
        {
            emit error(QString("Error read: size %1 != %2")
                       .arg(ba.length())
                       .arg(MAX_ADDRESS_FRAM_B590));
            return;
        }
        fram_data.clear();
        fram_data.append(ba);
        file.close();
    }
    analize();
}
//--------------------------------------------------------------------------------
void B590::save_fram_to_file(void)
{
    if(fram_data.isEmpty())
    {
        emit error("data is empty!");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save data"), ".", tr("Data Files (*.dat)"));
    if(filename.isEmpty()) return;

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(fram_data);
        file.close();
    }
}
//--------------------------------------------------------------------------------
void B590::add_signature(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("signature 0x%1").arg(fram->signature, 0, 16));

    int address = offsetof(FRAM_B590, anti_turn);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->signature)));
}
//--------------------------------------------------------------------------------
void B590::add_user_points_U(void)
{
    //TODO add_user_points_U
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *user_points_U   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    user_points_U->setText(0, "user_points_U");

    QTreeWidgetItem *points_U   = new QTreeWidgetItem(user_points_U, QTreeWidgetItem::Type);
    points_U->setText(0, "points");
    int address = offsetof(FRAM_B590, user_points_U);
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_U, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->user_points_U[n]));

        item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address + (n * sizeof(fram->user_points_U[0])))));
        item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->user_points_U[0])));
    }

    QTreeWidgetItem *item_crc16_U   = new QTreeWidgetItem(user_points_U, QTreeWidgetItem::Type);
    item_crc16_U->setText(0, QString("user_CRC16_U %1").arg(fram->user_points_crc16_U, 0, 16));

    address = offsetof(FRAM_B590, user_points_crc16_U);
    item_crc16_U->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(address));
    item_crc16_U->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->user_points_crc16_U)));
}
//--------------------------------------------------------------------------------
void B590::add_user_points_I(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *user_points_I   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    user_points_I->setText(0, "user_points_I");

    QTreeWidgetItem *points_I   = new QTreeWidgetItem(user_points_I, QTreeWidgetItem::Type);
    points_I->setText(0, "points");
    int address = offsetof(FRAM_B590, user_points_I);
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_I, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->user_points_I[n]));

        item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address + (n * sizeof(fram->user_points_I[0])))));
        item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->user_points_I[0])));
    }
    QTreeWidgetItem *item_crc16_I   = new QTreeWidgetItem(user_points_I, QTreeWidgetItem::Type);
    item_crc16_I->setText(0, QString("user_CRC16_I %1").arg(fram->user_points_crc16_I, 0, 16));

    address = offsetof(FRAM_B590, user_points_crc16_I);
    item_crc16_I->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(address));
    item_crc16_I->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->user_points_crc16_I)));
}
//--------------------------------------------------------------------------------
void B590::add_factory_points_U(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *factory_points_U   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    factory_points_U->setText(0, "factory_points_U");

    QTreeWidgetItem *points_U   = new QTreeWidgetItem(factory_points_U, QTreeWidgetItem::Type);
    points_U->setText(0, "points");
    int address = offsetof(FRAM_B590, factory_points_U);
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_U; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_U, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->factory_points_U[n]));

        item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address + (n * sizeof(fram->factory_points_U[0])))));
        item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factory_points_U[0])));
    }
    QTreeWidgetItem *item_crc16_U   = new QTreeWidgetItem(factory_points_U, QTreeWidgetItem::Type);
    item_crc16_U->setText(0, QString("factory_CRC16_U %1").arg(fram->factory_points_crc16_U, 0, 16));

    address = offsetof(FRAM_B590, factory_points_crc16_U);
    item_crc16_U->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(address));
    item_crc16_U->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factory_points_crc16_U)));
}
//--------------------------------------------------------------------------------
void B590::add_factory_points_I(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *factory_points_I   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    factory_points_I->setText(0, "factory_points_I");

    QTreeWidgetItem *points_I   = new QTreeWidgetItem(factory_points_I, QTreeWidgetItem::Type);
    points_I->setText(0, "points");
    int address = offsetof(FRAM_B590, factory_points_I);
    for(int n=0; n<MAX_CALIBRATION_POINTS_B590_I; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(points_I, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->factory_points_I[n]));

        item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address + (n * sizeof(fram->factory_points_I[0])))));
        item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factory_points_I[0])));
    }
    QTreeWidgetItem *item_crc16_I   = new QTreeWidgetItem(factory_points_I, QTreeWidgetItem::Type);
    item_crc16_I->setText(0, QString("factory_CRC16_I %1").arg(fram->factory_points_crc16_I, 0, 16));

    address = offsetof(FRAM_B590, factory_points_crc16_I);
    item_crc16_I->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(address));
    item_crc16_I->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factory_points_crc16_I)));
}
//--------------------------------------------------------------------------------
void B590::add_profile_points(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *profiles   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    profiles->setText(0, "profiles");
    for(int i_profile=0; i_profile<MAX_PROFILE_B590; i_profile++)
    {
        QTreeWidgetItem *pr = new QTreeWidgetItem(profiles, QTreeWidgetItem::Type);
        pr->setText(0, QString("profile %1").arg(i_profile));
        for(int i_point=0; i_point<MAX_POINT_B590; i_point++)
        {
            QTreeWidgetItem *p = new QTreeWidgetItem(pr, QTreeWidgetItem::Type);
            QTreeWidgetItem *p_voltage  = new QTreeWidgetItem(p, QTreeWidgetItem::Type);
            QTreeWidgetItem *p_current  = new QTreeWidgetItem(p, QTreeWidgetItem::Type);
            QTreeWidgetItem *p_time     = new QTreeWidgetItem(p, QTreeWidgetItem::Type);

            p->setText(0, QString("point %1").arg(i_point));
            p_voltage->setText(0, QString("voltage %1").arg(fram->profiles[i_profile].point[i_point].voltage));
            p_current->setText(0, QString("current %1").arg(fram->profiles[i_profile].point[i_point].current));
            p_time->setText(0, QString("time %1").arg(fram->profiles[i_profile].point[i_point].time));

            s_point point;
            int a_point = offsetof(FRAM_B590, profiles) + (sizeof(s_profile) * i_profile) + (sizeof(s_point) * i_point);

            int a_voltage = a_point + offsetof(s_point, voltage);
            p_voltage->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_voltage));
            p_voltage->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(point.voltage)));

            int a_current = a_point + offsetof(s_point, current);
            p_current->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_current));
            p_current->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(point.current)));

            int a_time    = a_point + offsetof(s_point, time);
            p_time->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_time));
            p_time->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(point.time)));
        }
        QTreeWidgetItem *r = new QTreeWidgetItem(profiles, QTreeWidgetItem::Type);
        r->setText(0, QString("repeat_count %1").arg(fram->profiles[i_profile].repeat_count));

        s_profile profile;
        int address = offsetof(FRAM_B590, profiles) + (sizeof(s_profile) * i_profile) + offsetof(s_profile, repeat_count);
        r->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(address));
        r->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(profile.repeat_count)));
    }
}
//--------------------------------------------------------------------------------
void B590::add_user_Value_K_Zero_U(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *value_U   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    value_U->setText(0, "user_Value_K_Zero_U");
    QTreeWidgetItem *item_new_Value_ADC_U_0 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_U_1 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_U_2 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_U_3 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_new_Value_DAC_U_0 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_U_1 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_U_2 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_U_3 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_parrot_in_point = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    QTreeWidgetItem *temp_cal_U_0 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *temp_cal_U_1 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *temp_cal_U_2 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *temp_cal_U_3 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    item_new_Value_ADC_U_0->setText(0, QString("Value_ADC_U_0 %1").arg(fram->user_Value_K_Zero_U.Value_ADC_U[0]));
    item_new_Value_ADC_U_1->setText(0, QString("Value_ADC_U_1 %1").arg(fram->user_Value_K_Zero_U.Value_ADC_U[1]));
    item_new_Value_ADC_U_2->setText(0, QString("Value_ADC_U_2 %1").arg(fram->user_Value_K_Zero_U.Value_ADC_U[2]));
    item_new_Value_ADC_U_3->setText(0, QString("Value_ADC_U_3 %1").arg(fram->user_Value_K_Zero_U.Value_ADC_U[3]));

    item_new_Value_DAC_U_0->setText(0, QString("Value_DAC_U_0 %1").arg(fram->user_Value_K_Zero_U.Value_DAC_U[0]));
    item_new_Value_DAC_U_1->setText(0, QString("Value_DAC_U_1 %1").arg(fram->user_Value_K_Zero_U.Value_DAC_U[1]));
    item_new_Value_DAC_U_2->setText(0, QString("Value_DAC_U_2 %1").arg(fram->user_Value_K_Zero_U.Value_DAC_U[2]));
    item_new_Value_DAC_U_3->setText(0, QString("Value_DAC_U_3 %1").arg(fram->user_Value_K_Zero_U.Value_DAC_U[3]));

    item_parrot_in_point->setText(0, QString("parrot_in_point %1").arg(fram->user_Value_K_Zero_U.parrot_in_point));

    temp_cal_U_0->setText(0, QString("temp_cal_U[0] %1").arg(fram->user_Value_K_Zero_U.temp_cal_U[0]));
    temp_cal_U_1->setText(0, QString("temp_cal_U[1] %1").arg(fram->user_Value_K_Zero_U.temp_cal_U[1]));
    temp_cal_U_2->setText(0, QString("temp_cal_U[2] %1").arg(fram->user_Value_K_Zero_U.temp_cal_U[2]));
    temp_cal_U_3->setText(0, QString("temp_cal_U[3] %1").arg(fram->user_Value_K_Zero_U.temp_cal_U[3]));

    //--
    int a_ADC_U_0 = offsetof(s_zero_b590_U, Value_ADC_U[0]);
    int a_ADC_U_1 = offsetof(s_zero_b590_U, Value_ADC_U[1]);
    int a_ADC_U_2 = offsetof(s_zero_b590_U, Value_ADC_U[2]);
    int a_ADC_U_3 = offsetof(s_zero_b590_U, Value_ADC_U[3]);

    int a_DAC_U_0 = offsetof(s_zero_b590_U, Value_DAC_U[0]);
    int a_DAC_U_1 = offsetof(s_zero_b590_U, Value_DAC_U[1]);
    int a_DAC_U_2 = offsetof(s_zero_b590_U, Value_DAC_U[2]);
    int a_DAC_U_3 = offsetof(s_zero_b590_U, Value_DAC_U[3]);

    int a_parrot_in_point = offsetof(s_zero_b590_U, parrot_in_point);

    int a_temp_cal_U_0 = offsetof(s_zero_b590_U, temp_cal_U[0]);
    int a_temp_cal_U_1 = offsetof(s_zero_b590_U, temp_cal_U[1]);
    int a_temp_cal_U_2 = offsetof(s_zero_b590_U, temp_cal_U[2]);
    int a_temp_cal_U_3 = offsetof(s_zero_b590_U, temp_cal_U[3]);
    //--
    int a_ADC_U = offsetof(FRAM_B590, user_Value_K_Zero_U);
    s_zero_b590_U zero_U;
    //---
    item_new_Value_ADC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_0));
    item_new_Value_ADC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[0])));
    item_new_Value_ADC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_1));
    item_new_Value_ADC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[1])));
    item_new_Value_ADC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_2));
    item_new_Value_ADC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[2])));
    item_new_Value_ADC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_3));
    item_new_Value_ADC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[3])));

    item_new_Value_DAC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_0));
    item_new_Value_DAC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[0])));
    item_new_Value_DAC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_1));
    item_new_Value_DAC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[1])));
    item_new_Value_DAC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_2));
    item_new_Value_DAC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[2])));
    item_new_Value_DAC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_3));
    item_new_Value_DAC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[3])));

    item_parrot_in_point->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_parrot_in_point));
    item_parrot_in_point->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.parrot_in_point)));

    temp_cal_U_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_0));
    temp_cal_U_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[0])));
    temp_cal_U_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_1));
    temp_cal_U_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[1])));
    temp_cal_U_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_2));
    temp_cal_U_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[2])));
    temp_cal_U_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_3));
    temp_cal_U_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[3])));
    //---
}
//--------------------------------------------------------------------------------
void B590::add_user_Value_K_Zero_I(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *value_I   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    value_I->setText(0, "user_Value_K_Zero_I");
    QTreeWidgetItem *item_new_Value_ADC_I_0 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_1 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_2 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_3 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_4 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_5 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_6 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_7 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_8 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_9 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_10 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_11 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_12 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_new_Value_DAC_I_0 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_1 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_2 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_3 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_4 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_5 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_6 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_7 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_8 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_9 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_10 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_11 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_12 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_temp_cal_I_0 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_1 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_2 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_3 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_4 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_5 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_6 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_7 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_8 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_9 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_10 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_11 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_12 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);

    item_new_Value_ADC_I_0->setText(0, QString("Value_ADC_I[0] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[0]));
    item_new_Value_ADC_I_1->setText(0, QString("Value_ADC_I[1] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[1]));
    item_new_Value_ADC_I_2->setText(0, QString("Value_ADC_I[2] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[2]));
    item_new_Value_ADC_I_3->setText(0, QString("Value_ADC_I[3] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[3]));
    item_new_Value_ADC_I_4->setText(0, QString("Value_ADC_I[4] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[4]));
    item_new_Value_ADC_I_5->setText(0, QString("Value_ADC_I[5] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[5]));
    item_new_Value_ADC_I_6->setText(0, QString("Value_ADC_I[6] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[6]));
    item_new_Value_ADC_I_7->setText(0, QString("Value_ADC_I[7] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[7]));
    item_new_Value_ADC_I_8->setText(0, QString("Value_ADC_I[8] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[8]));
    item_new_Value_ADC_I_9->setText(0, QString("Value_ADC_I[9] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[9]));
    item_new_Value_ADC_I_10->setText(0, QString("Value_ADC_I[10] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[10]));
    item_new_Value_ADC_I_11->setText(0, QString("Value_ADC_I[11] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[11]));
    item_new_Value_ADC_I_12->setText(0, QString("Value_ADC_I[12] %1").arg(fram->user_Value_K_Zero_I.Value_ADC_I[12]));

    item_new_Value_DAC_I_0->setText(0, QString("Value_DAC_I[0] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[0]));
    item_new_Value_DAC_I_1->setText(0, QString("Value_DAC_I[1] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[1]));
    item_new_Value_DAC_I_2->setText(0, QString("Value_DAC_I[2] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[2]));
    item_new_Value_DAC_I_3->setText(0, QString("Value_DAC_I[3] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[3]));
    item_new_Value_DAC_I_4->setText(0, QString("Value_DAC_I[4] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[4]));
    item_new_Value_DAC_I_5->setText(0, QString("Value_DAC_I[5] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[5]));
    item_new_Value_DAC_I_6->setText(0, QString("Value_DAC_I[6] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[6]));
    item_new_Value_DAC_I_7->setText(0, QString("Value_DAC_I[7] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[7]));
    item_new_Value_DAC_I_8->setText(0, QString("Value_DAC_I[8] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[8]));
    item_new_Value_DAC_I_9->setText(0, QString("Value_DAC_I[9] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[9]));
    item_new_Value_DAC_I_10->setText(0, QString("Value_DAC_I[10] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[10]));
    item_new_Value_DAC_I_11->setText(0, QString("Value_DAC_I[11] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[11]));
    item_new_Value_DAC_I_12->setText(0, QString("Value_DAC_I[12] %1").arg(fram->user_Value_K_Zero_I.Value_DAC_I[12]));

    item_temp_cal_I_0->setText(0, QString("temp_cal_I[0] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[0]));
    item_temp_cal_I_1->setText(0, QString("temp_cal_I[1] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[1]));
    item_temp_cal_I_2->setText(0, QString("temp_cal_I[2] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[2]));
    item_temp_cal_I_3->setText(0, QString("temp_cal_I[3] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[3]));
    item_temp_cal_I_4->setText(0, QString("temp_cal_I[4] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[4]));
    item_temp_cal_I_5->setText(0, QString("temp_cal_I[5] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[5]));
    item_temp_cal_I_6->setText(0, QString("temp_cal_I[6] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[6]));
    item_temp_cal_I_7->setText(0, QString("temp_cal_I[7] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[7]));
    item_temp_cal_I_8->setText(0, QString("temp_cal_I[8] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[8]));
    item_temp_cal_I_9->setText(0, QString("temp_cal_I[9] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[9]));
    item_temp_cal_I_10->setText(0, QString("temp_cal_I[10] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[10]));
    item_temp_cal_I_11->setText(0, QString("temp_cal_I[11] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[11]));
    item_temp_cal_I_12->setText(0, QString("temp_cal_I[12] %1").arg(fram->user_Value_K_Zero_I.temp_cal_I[12]));

    //--
    int a_ADC_I_0 = offsetof(s_zero_b590_I, Value_ADC_I[0]);
    int a_ADC_I_1 = offsetof(s_zero_b590_I, Value_ADC_I[1]);
    int a_ADC_I_2 = offsetof(s_zero_b590_I, Value_ADC_I[2]);
    int a_ADC_I_3 = offsetof(s_zero_b590_I, Value_ADC_I[3]);
    int a_ADC_I_4 = offsetof(s_zero_b590_I, Value_ADC_I[4]);
    int a_ADC_I_5 = offsetof(s_zero_b590_I, Value_ADC_I[5]);
    int a_ADC_I_6 = offsetof(s_zero_b590_I, Value_ADC_I[6]);
    int a_ADC_I_7 = offsetof(s_zero_b590_I, Value_ADC_I[7]);
    int a_ADC_I_8 = offsetof(s_zero_b590_I, Value_ADC_I[8]);
    int a_ADC_I_9 = offsetof(s_zero_b590_I, Value_ADC_I[9]);
    int a_ADC_I_10 = offsetof(s_zero_b590_I, Value_ADC_I[10]);
    int a_ADC_I_11 = offsetof(s_zero_b590_I, Value_ADC_I[11]);
    int a_ADC_I_12 = offsetof(s_zero_b590_I, Value_ADC_I[12]);

    int a_DAC_I_0 = offsetof(s_zero_b590_I, Value_DAC_I[0]);
    int a_DAC_I_1 = offsetof(s_zero_b590_I, Value_DAC_I[1]);
    int a_DAC_I_2 = offsetof(s_zero_b590_I, Value_DAC_I[2]);
    int a_DAC_I_3 = offsetof(s_zero_b590_I, Value_DAC_I[3]);
    int a_DAC_I_4 = offsetof(s_zero_b590_I, Value_DAC_I[4]);
    int a_DAC_I_5 = offsetof(s_zero_b590_I, Value_DAC_I[5]);
    int a_DAC_I_6 = offsetof(s_zero_b590_I, Value_DAC_I[6]);
    int a_DAC_I_7 = offsetof(s_zero_b590_I, Value_DAC_I[7]);
    int a_DAC_I_8 = offsetof(s_zero_b590_I, Value_DAC_I[8]);
    int a_DAC_I_9 = offsetof(s_zero_b590_I, Value_DAC_I[9]);
    int a_DAC_I_10 = offsetof(s_zero_b590_I, Value_DAC_I[10]);
    int a_DAC_I_11 = offsetof(s_zero_b590_I, Value_DAC_I[11]);
    int a_DAC_I_12 = offsetof(s_zero_b590_I, Value_DAC_I[12]);

    int a_temp_cal_I_0 = offsetof(s_zero_b590_I, temp_cal_I[0]);
    int a_temp_cal_I_1 = offsetof(s_zero_b590_I, temp_cal_I[1]);
    int a_temp_cal_I_2 = offsetof(s_zero_b590_I, temp_cal_I[2]);
    int a_temp_cal_I_3 = offsetof(s_zero_b590_I, temp_cal_I[3]);
    int a_temp_cal_I_4 = offsetof(s_zero_b590_I, temp_cal_I[4]);
    int a_temp_cal_I_5 = offsetof(s_zero_b590_I, temp_cal_I[5]);
    int a_temp_cal_I_6 = offsetof(s_zero_b590_I, temp_cal_I[6]);
    int a_temp_cal_I_7 = offsetof(s_zero_b590_I, temp_cal_I[7]);
    int a_temp_cal_I_8 = offsetof(s_zero_b590_I, temp_cal_I[8]);
    int a_temp_cal_I_9 = offsetof(s_zero_b590_I, temp_cal_I[9]);
    int a_temp_cal_I_10 = offsetof(s_zero_b590_I, temp_cal_I[10]);
    int a_temp_cal_I_11 = offsetof(s_zero_b590_I, temp_cal_I[11]);
    int a_temp_cal_I_12 = offsetof(s_zero_b590_I, temp_cal_I[12]);

    //--
    int a_ADC_I = offsetof(FRAM_B590, user_Value_K_Zero_I);
    s_zero_b590_I zero_I;
    //---
    item_new_Value_ADC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_0));
    item_new_Value_ADC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[0])));
    item_new_Value_ADC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_1));
    item_new_Value_ADC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[1])));
    item_new_Value_ADC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_2));
    item_new_Value_ADC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[2])));
    item_new_Value_ADC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_3));
    item_new_Value_ADC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[3])));
    item_new_Value_ADC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_4));
    item_new_Value_ADC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[4])));
    item_new_Value_ADC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_5));
    item_new_Value_ADC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[5])));
    item_new_Value_ADC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_6));
    item_new_Value_ADC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[6])));
    item_new_Value_ADC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_7));
    item_new_Value_ADC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[7])));
    item_new_Value_ADC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_8));
    item_new_Value_ADC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[8])));
    item_new_Value_ADC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_9));
    item_new_Value_ADC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[9])));
    item_new_Value_ADC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_10));
    item_new_Value_ADC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[10])));
    item_new_Value_ADC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_11));
    item_new_Value_ADC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[11])));
    item_new_Value_ADC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_12));
    item_new_Value_ADC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[12])));

    item_new_Value_DAC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_0));
    item_new_Value_DAC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[0])));
    item_new_Value_DAC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_1));
    item_new_Value_DAC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[1])));
    item_new_Value_DAC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_2));
    item_new_Value_DAC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[2])));
    item_new_Value_DAC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_3));
    item_new_Value_DAC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[3])));
    item_new_Value_DAC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_4));
    item_new_Value_DAC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[4])));
    item_new_Value_DAC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_5));
    item_new_Value_DAC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[5])));
    item_new_Value_DAC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_6));
    item_new_Value_DAC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[6])));
    item_new_Value_DAC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_7));
    item_new_Value_DAC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[7])));
    item_new_Value_DAC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_8));
    item_new_Value_DAC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[8])));
    item_new_Value_DAC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_9));
    item_new_Value_DAC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[9])));
    item_new_Value_DAC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_10));
    item_new_Value_DAC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[10])));
    item_new_Value_DAC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_11));
    item_new_Value_DAC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[11])));
    item_new_Value_DAC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_12));
    item_new_Value_DAC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[12])));

    item_temp_cal_I_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_0));
    item_temp_cal_I_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[0])));
    item_temp_cal_I_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_1));
    item_temp_cal_I_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[1])));
    item_temp_cal_I_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_2));
    item_temp_cal_I_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[2])));
    item_temp_cal_I_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_3));
    item_temp_cal_I_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[3])));
    item_temp_cal_I_4->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_4));
    item_temp_cal_I_4->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[4])));
    item_temp_cal_I_5->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_5));
    item_temp_cal_I_5->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[5])));
    item_temp_cal_I_6->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_6));
    item_temp_cal_I_6->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[6])));
    item_temp_cal_I_7->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_7));
    item_temp_cal_I_7->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[7])));
    item_temp_cal_I_8->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_8));
    item_temp_cal_I_8->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[8])));
    item_temp_cal_I_9->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_9));
    item_temp_cal_I_9->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[9])));
    item_temp_cal_I_10->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_10));
    item_temp_cal_I_10->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[10])));
    item_temp_cal_I_11->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_11));
    item_temp_cal_I_11->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[11])));
    item_temp_cal_I_12->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_12));
    item_temp_cal_I_12->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[12])));
    //---
}
//--------------------------------------------------------------------------------
void B590::add_factory_Value_K_Zero_U(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *value_U   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    value_U->setText(0, "factory_Value_K_Zero_U");
    QTreeWidgetItem *item_new_Value_ADC_U_0 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_U_1 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_U_2 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_U_3 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_new_Value_DAC_U_0 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_U_1 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_U_2 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_U_3 = new QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_parrot_in_point = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    QTreeWidgetItem *temp_cal_U_0 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *temp_cal_U_1 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *temp_cal_U_2 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);
    QTreeWidgetItem *temp_cal_U_3 = new  QTreeWidgetItem(value_U, QTreeWidgetItem::Type);

    item_new_Value_ADC_U_0->setText(0, QString("Value_ADC_U_0 %1").arg(fram->factory_Value_K_Zero_U.Value_ADC_U[0]));
    item_new_Value_ADC_U_1->setText(0, QString("Value_ADC_U_1 %1").arg(fram->factory_Value_K_Zero_U.Value_ADC_U[1]));
    item_new_Value_ADC_U_2->setText(0, QString("Value_ADC_U_2 %1").arg(fram->factory_Value_K_Zero_U.Value_ADC_U[2]));
    item_new_Value_ADC_U_3->setText(0, QString("Value_ADC_U_3 %1").arg(fram->factory_Value_K_Zero_U.Value_ADC_U[3]));

    item_new_Value_DAC_U_0->setText(0, QString("Value_DAC_U_0 %1").arg(fram->factory_Value_K_Zero_U.Value_DAC_U[0]));
    item_new_Value_DAC_U_1->setText(0, QString("Value_DAC_U_1 %1").arg(fram->factory_Value_K_Zero_U.Value_DAC_U[1]));
    item_new_Value_DAC_U_2->setText(0, QString("Value_DAC_U_2 %1").arg(fram->factory_Value_K_Zero_U.Value_DAC_U[2]));
    item_new_Value_DAC_U_3->setText(0, QString("Value_DAC_U_3 %1").arg(fram->factory_Value_K_Zero_U.Value_DAC_U[3]));

    item_parrot_in_point->setText(0, QString("parrot_in_point %1").arg(fram->factory_Value_K_Zero_U.parrot_in_point));

    temp_cal_U_0->setText(0, QString("temp_cal_U[0] %1").arg(fram->factory_Value_K_Zero_U.temp_cal_U[0]));
    temp_cal_U_1->setText(0, QString("temp_cal_U[1] %1").arg(fram->factory_Value_K_Zero_U.temp_cal_U[1]));
    temp_cal_U_2->setText(0, QString("temp_cal_U[2] %1").arg(fram->factory_Value_K_Zero_U.temp_cal_U[2]));
    temp_cal_U_3->setText(0, QString("temp_cal_U[3] %1").arg(fram->factory_Value_K_Zero_U.temp_cal_U[3]));

    //--
    int a_ADC_U_0 = offsetof(s_zero_b590_U, Value_ADC_U[0]);
    int a_ADC_U_1 = offsetof(s_zero_b590_U, Value_ADC_U[1]);
    int a_ADC_U_2 = offsetof(s_zero_b590_U, Value_ADC_U[2]);
    int a_ADC_U_3 = offsetof(s_zero_b590_U, Value_ADC_U[3]);

    int a_DAC_U_0 = offsetof(s_zero_b590_U, Value_DAC_U[0]);
    int a_DAC_U_1 = offsetof(s_zero_b590_U, Value_DAC_U[1]);
    int a_DAC_U_2 = offsetof(s_zero_b590_U, Value_DAC_U[2]);
    int a_DAC_U_3 = offsetof(s_zero_b590_U, Value_DAC_U[3]);

    int a_parrot_in_point = offsetof(s_zero_b590_U, parrot_in_point);

    int a_temp_cal_U_0 = offsetof(s_zero_b590_U, temp_cal_U[0]);
    int a_temp_cal_U_1 = offsetof(s_zero_b590_U, temp_cal_U[1]);
    int a_temp_cal_U_2 = offsetof(s_zero_b590_U, temp_cal_U[2]);
    int a_temp_cal_U_3 = offsetof(s_zero_b590_U, temp_cal_U[3]);
    //--
    int a_ADC_U = offsetof(FRAM_B590, factory_Value_K_Zero_U);
    s_zero_b590_U zero_U;
    //---
    item_new_Value_ADC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_0));
    item_new_Value_ADC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[0])));
    item_new_Value_ADC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_1));
    item_new_Value_ADC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[1])));
    item_new_Value_ADC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_2));
    item_new_Value_ADC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[2])));
    item_new_Value_ADC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_ADC_U_3));
    item_new_Value_ADC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_ADC_U[3])));

    item_new_Value_DAC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_0));
    item_new_Value_DAC_U_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[0])));
    item_new_Value_DAC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_1));
    item_new_Value_DAC_U_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[1])));
    item_new_Value_DAC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_2));
    item_new_Value_DAC_U_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[2])));
    item_new_Value_DAC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_DAC_U_3));
    item_new_Value_DAC_U_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.Value_DAC_U[3])));

    item_parrot_in_point->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_parrot_in_point));
    item_parrot_in_point->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.parrot_in_point)));

    temp_cal_U_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_0));
    temp_cal_U_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[0])));
    temp_cal_U_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_1));
    temp_cal_U_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[1])));
    temp_cal_U_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_2));
    temp_cal_U_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[2])));
    temp_cal_U_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_U + a_temp_cal_U_3));
    temp_cal_U_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_U.temp_cal_U[3])));
    //---
}
//--------------------------------------------------------------------------------
void B590::add_factory_Value_K_Zero_I(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *value_I   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    value_I->setText(0, "factory_Value_K_Zero_I");
    QTreeWidgetItem *item_new_Value_ADC_I_0 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_1 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_2 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_3 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_4 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_5 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_6 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_7 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_8 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_9 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_10 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_11 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_ADC_I_12 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_new_Value_DAC_I_0 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_1 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_2 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_3 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_4 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_5 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_6 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_7 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_8 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_9 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_10 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_11 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_new_Value_DAC_I_12 = new QTreeWidgetItem(value_I, QTreeWidgetItem::Type);

    QTreeWidgetItem *item_temp_cal_I_0 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_1 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_2 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_3 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_4 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_5 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_6 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_7 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_8 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_9 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_10 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_11 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);
    QTreeWidgetItem *item_temp_cal_I_12 = new  QTreeWidgetItem(value_I, QTreeWidgetItem::Type);

    item_new_Value_ADC_I_0->setText(0, QString("Value_ADC_I[0] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[0]));
    item_new_Value_ADC_I_1->setText(0, QString("Value_ADC_I[1] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[1]));
    item_new_Value_ADC_I_2->setText(0, QString("Value_ADC_I[2] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[2]));
    item_new_Value_ADC_I_3->setText(0, QString("Value_ADC_I[3] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[3]));
    item_new_Value_ADC_I_4->setText(0, QString("Value_ADC_I[4] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[4]));
    item_new_Value_ADC_I_5->setText(0, QString("Value_ADC_I[5] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[5]));
    item_new_Value_ADC_I_6->setText(0, QString("Value_ADC_I[6] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[6]));
    item_new_Value_ADC_I_7->setText(0, QString("Value_ADC_I[7] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[7]));
    item_new_Value_ADC_I_8->setText(0, QString("Value_ADC_I[8] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[8]));
    item_new_Value_ADC_I_9->setText(0, QString("Value_ADC_I[9] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[9]));
    item_new_Value_ADC_I_10->setText(0, QString("Value_ADC_I[10] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[10]));
    item_new_Value_ADC_I_11->setText(0, QString("Value_ADC_I[11] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[11]));
    item_new_Value_ADC_I_12->setText(0, QString("Value_ADC_I[12] %1").arg(fram->factory_Value_K_Zero_I.Value_ADC_I[12]));

    item_new_Value_DAC_I_0->setText(0, QString("Value_DAC_I[0] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[0]));
    item_new_Value_DAC_I_1->setText(0, QString("Value_DAC_I[1] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[1]));
    item_new_Value_DAC_I_2->setText(0, QString("Value_DAC_I[2] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[2]));
    item_new_Value_DAC_I_3->setText(0, QString("Value_DAC_I[3] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[3]));
    item_new_Value_DAC_I_4->setText(0, QString("Value_DAC_I[4] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[4]));
    item_new_Value_DAC_I_5->setText(0, QString("Value_DAC_I[5] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[5]));
    item_new_Value_DAC_I_6->setText(0, QString("Value_DAC_I[6] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[6]));
    item_new_Value_DAC_I_7->setText(0, QString("Value_DAC_I[7] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[7]));
    item_new_Value_DAC_I_8->setText(0, QString("Value_DAC_I[8] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[8]));
    item_new_Value_DAC_I_9->setText(0, QString("Value_DAC_I[9] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[9]));
    item_new_Value_DAC_I_10->setText(0, QString("Value_DAC_I[10] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[10]));
    item_new_Value_DAC_I_11->setText(0, QString("Value_DAC_I[11] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[11]));
    item_new_Value_DAC_I_12->setText(0, QString("Value_DAC_I[12] %1").arg(fram->factory_Value_K_Zero_I.Value_DAC_I[12]));

    item_temp_cal_I_0->setText(0, QString("temp_cal_I[0] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[0]));
    item_temp_cal_I_1->setText(0, QString("temp_cal_I[1] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[1]));
    item_temp_cal_I_2->setText(0, QString("temp_cal_I[2] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[2]));
    item_temp_cal_I_3->setText(0, QString("temp_cal_I[3] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[3]));
    item_temp_cal_I_4->setText(0, QString("temp_cal_I[4] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[4]));
    item_temp_cal_I_5->setText(0, QString("temp_cal_I[5] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[5]));
    item_temp_cal_I_6->setText(0, QString("temp_cal_I[6] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[6]));
    item_temp_cal_I_7->setText(0, QString("temp_cal_I[7] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[7]));
    item_temp_cal_I_8->setText(0, QString("temp_cal_I[8] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[8]));
    item_temp_cal_I_9->setText(0, QString("temp_cal_I[9] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[9]));
    item_temp_cal_I_10->setText(0, QString("temp_cal_I[10] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[10]));
    item_temp_cal_I_11->setText(0, QString("temp_cal_I[11] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[11]));
    item_temp_cal_I_12->setText(0, QString("temp_cal_I[12] %1").arg(fram->factory_Value_K_Zero_I.temp_cal_I[12]));

    //--
    int a_ADC_I_0 = offsetof(s_zero_b590_I, Value_ADC_I[0]);
    int a_ADC_I_1 = offsetof(s_zero_b590_I, Value_ADC_I[1]);
    int a_ADC_I_2 = offsetof(s_zero_b590_I, Value_ADC_I[2]);
    int a_ADC_I_3 = offsetof(s_zero_b590_I, Value_ADC_I[3]);
    int a_ADC_I_4 = offsetof(s_zero_b590_I, Value_ADC_I[4]);
    int a_ADC_I_5 = offsetof(s_zero_b590_I, Value_ADC_I[5]);
    int a_ADC_I_6 = offsetof(s_zero_b590_I, Value_ADC_I[6]);
    int a_ADC_I_7 = offsetof(s_zero_b590_I, Value_ADC_I[7]);
    int a_ADC_I_8 = offsetof(s_zero_b590_I, Value_ADC_I[8]);
    int a_ADC_I_9 = offsetof(s_zero_b590_I, Value_ADC_I[9]);
    int a_ADC_I_10 = offsetof(s_zero_b590_I, Value_ADC_I[10]);
    int a_ADC_I_11 = offsetof(s_zero_b590_I, Value_ADC_I[11]);
    int a_ADC_I_12 = offsetof(s_zero_b590_I, Value_ADC_I[12]);

    int a_DAC_I_0 = offsetof(s_zero_b590_I, Value_DAC_I[0]);
    int a_DAC_I_1 = offsetof(s_zero_b590_I, Value_DAC_I[1]);
    int a_DAC_I_2 = offsetof(s_zero_b590_I, Value_DAC_I[2]);
    int a_DAC_I_3 = offsetof(s_zero_b590_I, Value_DAC_I[3]);
    int a_DAC_I_4 = offsetof(s_zero_b590_I, Value_DAC_I[4]);
    int a_DAC_I_5 = offsetof(s_zero_b590_I, Value_DAC_I[5]);
    int a_DAC_I_6 = offsetof(s_zero_b590_I, Value_DAC_I[6]);
    int a_DAC_I_7 = offsetof(s_zero_b590_I, Value_DAC_I[7]);
    int a_DAC_I_8 = offsetof(s_zero_b590_I, Value_DAC_I[8]);
    int a_DAC_I_9 = offsetof(s_zero_b590_I, Value_DAC_I[9]);
    int a_DAC_I_10 = offsetof(s_zero_b590_I, Value_DAC_I[10]);
    int a_DAC_I_11 = offsetof(s_zero_b590_I, Value_DAC_I[11]);
    int a_DAC_I_12 = offsetof(s_zero_b590_I, Value_DAC_I[12]);

    int a_temp_cal_I_0 = offsetof(s_zero_b590_I, temp_cal_I[0]);
    int a_temp_cal_I_1 = offsetof(s_zero_b590_I, temp_cal_I[1]);
    int a_temp_cal_I_2 = offsetof(s_zero_b590_I, temp_cal_I[2]);
    int a_temp_cal_I_3 = offsetof(s_zero_b590_I, temp_cal_I[3]);
    int a_temp_cal_I_4 = offsetof(s_zero_b590_I, temp_cal_I[4]);
    int a_temp_cal_I_5 = offsetof(s_zero_b590_I, temp_cal_I[5]);
    int a_temp_cal_I_6 = offsetof(s_zero_b590_I, temp_cal_I[6]);
    int a_temp_cal_I_7 = offsetof(s_zero_b590_I, temp_cal_I[7]);
    int a_temp_cal_I_8 = offsetof(s_zero_b590_I, temp_cal_I[8]);
    int a_temp_cal_I_9 = offsetof(s_zero_b590_I, temp_cal_I[9]);
    int a_temp_cal_I_10 = offsetof(s_zero_b590_I, temp_cal_I[10]);
    int a_temp_cal_I_11 = offsetof(s_zero_b590_I, temp_cal_I[11]);
    int a_temp_cal_I_12 = offsetof(s_zero_b590_I, temp_cal_I[12]);

    //--
    int a_ADC_I = offsetof(FRAM_B590, factory_Value_K_Zero_I);
    s_zero_b590_I zero_I;
    //---
    item_new_Value_ADC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_0));
    item_new_Value_ADC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[0])));
    item_new_Value_ADC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_1));
    item_new_Value_ADC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[1])));
    item_new_Value_ADC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_2));
    item_new_Value_ADC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[2])));
    item_new_Value_ADC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_3));
    item_new_Value_ADC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[3])));
    item_new_Value_ADC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_4));
    item_new_Value_ADC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[4])));
    item_new_Value_ADC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_5));
    item_new_Value_ADC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[5])));
    item_new_Value_ADC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_6));
    item_new_Value_ADC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[6])));
    item_new_Value_ADC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_7));
    item_new_Value_ADC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[7])));
    item_new_Value_ADC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_8));
    item_new_Value_ADC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[8])));
    item_new_Value_ADC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_9));
    item_new_Value_ADC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[9])));
    item_new_Value_ADC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_10));
    item_new_Value_ADC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[10])));
    item_new_Value_ADC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_11));
    item_new_Value_ADC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[11])));
    item_new_Value_ADC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_ADC_I_12));
    item_new_Value_ADC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_ADC_I[12])));

    item_new_Value_DAC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_0));
    item_new_Value_DAC_I_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[0])));
    item_new_Value_DAC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_1));
    item_new_Value_DAC_I_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[1])));
    item_new_Value_DAC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_2));
    item_new_Value_DAC_I_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[2])));
    item_new_Value_DAC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_3));
    item_new_Value_DAC_I_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[3])));
    item_new_Value_DAC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_4));
    item_new_Value_DAC_I_4->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[4])));
    item_new_Value_DAC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_5));
    item_new_Value_DAC_I_5->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[5])));
    item_new_Value_DAC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_6));
    item_new_Value_DAC_I_6->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[6])));
    item_new_Value_DAC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_7));
    item_new_Value_DAC_I_7->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[7])));
    item_new_Value_DAC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_8));
    item_new_Value_DAC_I_8->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[8])));
    item_new_Value_DAC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_9));
    item_new_Value_DAC_I_9->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[9])));
    item_new_Value_DAC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_10));
    item_new_Value_DAC_I_10->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[10])));
    item_new_Value_DAC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_11));
    item_new_Value_DAC_I_11->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[11])));
    item_new_Value_DAC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_DAC_I_12));
    item_new_Value_DAC_I_12->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.Value_DAC_I[12])));

    item_temp_cal_I_0->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_0));
    item_temp_cal_I_0->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[0])));
    item_temp_cal_I_1->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_1));
    item_temp_cal_I_1->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[1])));
    item_temp_cal_I_2->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_2));
    item_temp_cal_I_2->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[2])));
    item_temp_cal_I_3->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_3));
    item_temp_cal_I_3->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[3])));
    item_temp_cal_I_4->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_4));
    item_temp_cal_I_4->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[4])));
    item_temp_cal_I_5->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_5));
    item_temp_cal_I_5->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[5])));
    item_temp_cal_I_6->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_6));
    item_temp_cal_I_6->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[6])));
    item_temp_cal_I_7->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_7));
    item_temp_cal_I_7->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[7])));
    item_temp_cal_I_8->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_8));
    item_temp_cal_I_8->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[8])));
    item_temp_cal_I_9->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_9));
    item_temp_cal_I_9->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[9])));
    item_temp_cal_I_10->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_10));
    item_temp_cal_I_10->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[10])));
    item_temp_cal_I_11->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_11));
    item_temp_cal_I_11->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[11])));
    item_temp_cal_I_12->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant(a_ADC_I + a_temp_cal_I_12));
    item_temp_cal_I_12->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(zero_I.temp_cal_I[12])));
    //---
}
//--------------------------------------------------------------------------------
void B590::add_Bright(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Bright %1").arg(fram->Bright));

    int address = offsetof(FRAM_B590, Bright);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Bright)));
}
//--------------------------------------------------------------------------------
void B590::add_Sound(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Sound %1").arg(fram->Sound));

    int address = offsetof(FRAM_B590, Sound);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Sound)));
}
//--------------------------------------------------------------------------------
void B590::add_Count_Last_U_I(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Count_Last_U_I %1").arg(fram->count_Last_U_I));

    int address = offsetof(FRAM_B590, count_Last_U_I);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->count_Last_U_I)));
}
//--------------------------------------------------------------------------------
void B590::add_Speed_UART(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Speed_UART %1").arg(fram->Speed_UART));

    int address = offsetof(FRAM_B590, Speed_UART);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Speed_UART)));
}
//--------------------------------------------------------------------------------
void B590::add_Adr_ModBus(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Adr_ModBus %1").arg(fram->Adr_ModBus));

    int address = offsetof(FRAM_B590, Adr_ModBus);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Adr_ModBus)));
}
//--------------------------------------------------------------------------------
void B590::add_moto_time(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("moto_time %1").arg(fram->moto_time));

    int address = offsetof(FRAM_B590, moto_time);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->moto_time)));
}
//--------------------------------------------------------------------------------
void B590::add_Year_Factory(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Year_Factory %1").arg(fram->Year_Factory));

    int address = offsetof(FRAM_B590, Year_Factory);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Year_Factory)));
}
//--------------------------------------------------------------------------------
void B590::add_Month_Factory(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Month_Factory %1").arg(fram->Month_Factory));

    int address = offsetof(FRAM_B590, Month_Factory);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Month_Factory)));
}
//--------------------------------------------------------------------------------
void B590::add_Serial_Factory(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("Serial_Factory %1").arg(fram->Serial_Factory));

    int address = offsetof(FRAM_B590, Serial_Factory);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->Serial_Factory)));
}
//--------------------------------------------------------------------------------
void B590::add_factor_k_DAC(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("factor_k_DAC %1").arg(fram->factor_k_DAC));

    int address = offsetof(FRAM_B590, factor_k_DAC);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factor_k_DAC)));
}
//--------------------------------------------------------------------------------
void B590::add_factor_k_ADC(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("factor_k_ADC %1").arg(fram->factor_k_ADC));

    int address = offsetof(FRAM_B590, factor_k_ADC);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factor_k_ADC)));
}
//--------------------------------------------------------------------------------
void B590::add_factor_S(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("factor_S %1").arg(fram->factor_S));

    int address = offsetof(FRAM_B590, factor_S);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->factor_S)));
}
//--------------------------------------------------------------------------------
void B590::add_cal_temp(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("cal_temp %1").arg(fram->cal_temp));

    int address = offsetof(FRAM_B590, cal_temp);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->cal_temp)));
}
//--------------------------------------------------------------------------------
void B590::add_anti_turn(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item   = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item->setText(0, QString("anti_turn %1").arg(fram->anti_turn));

    int address = offsetof(FRAM_B590, anti_turn);
    item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
    item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->anti_turn)));
}
//--------------------------------------------------------------------------------
void B590::add_last_U(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item_U = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item_U->setText(0, "last_U");
    for(int n=0; n<SIZE_TABLE_ELEMENT; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(item_U, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->last_U[n]));

        int address = offsetof(FRAM_B590, last_U) + (n * sizeof(fram->last_U[0]));
        item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
        item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->last_U[0])));
    }
}
//--------------------------------------------------------------------------------
void B590::add_last_I(void)
{
    FRAM_B590 *fram = (FRAM_B590 *)fram_data.data();

    QTreeWidgetItem *item_I = new QTreeWidgetItem(ui->tw_fram, QTreeWidgetItem::Type);
    item_I->setText(0, "last_I");
    for(int n=0; n<SIZE_TABLE_ELEMENT; n++)
    {
        QTreeWidgetItem *item   = new QTreeWidgetItem(item_I, QTreeWidgetItem::Type);
        item->setText(0, QString("%1 %2").arg(n).arg(fram->last_I[n]));

        int address = offsetof(FRAM_B590, last_I) + (n * sizeof(fram->last_I[0]));
        item->setData(0, QTreeWidgetItem::UserType + DATA_ADDRESS,  QVariant((int)(address)));
        item->setData(0, QTreeWidgetItem::UserType + DATA_SIZE,     QVariant((int)sizeof(fram->last_I[0])));
    }
}
//--------------------------------------------------------------------------------
void B590::analize(void)
{
    if(fram_data.isEmpty())
    {
        emit error("data is empty!");
        return;
    }

    ui->tw_fram->clear();

    add_signature();

    add_user_points_U();
    add_user_points_I();

    add_factory_points_U();
    add_factory_points_I();

    add_profile_points();

    add_user_Value_K_Zero_U();
    add_user_Value_K_Zero_I();

    add_factory_Value_K_Zero_U();
    add_factory_Value_K_Zero_I();

    add_Bright();
    add_Sound();
    add_Count_Last_U_I();
    add_Speed_UART();
    add_Adr_ModBus();
    add_moto_time();
    add_Year_Factory();
    add_Month_Factory();
    add_Serial_Factory();

    add_factor_k_DAC();
    add_factor_k_ADC();
    add_factor_S();

    add_cal_temp();

    add_anti_turn();
    add_last_U();
    add_last_I();
}
//--------------------------------------------------------------------------------
bool B590::save_data(uint16_t address, uint8_t data)
{
    if(address > fram_data.size())
    {
        return false;
    }

    bool ok = powersupply->fram_write_8(address, data);
    if(!ok)
    {
        powersupply->print_last_error();
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} XXX;
typedef struct
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    XXX xxx[2];
    uint16_t d;
} TEST;
#pragma pack(pop)
//--------------------------------------------------------------------------------
void B590::test(void)
{
    emit info("Test begin");
    emit info(QString("a %1").arg(offsetof(TEST, a)));
    emit info(QString("b %1").arg(offsetof(TEST, b)));
    emit info(QString("c %1").arg(offsetof(TEST, c)));
    emit info(QString("d %1").arg(offsetof(TEST, d)));

    emit info(QString("sizeof %1").arg(sizeof(FRAM_B590)));
    emit info("Test end");
}
//--------------------------------------------------------------------------------
