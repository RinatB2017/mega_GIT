/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QLabel>
#include "b590_toolbar.hpp"
#include "b590_packets.hpp"
//--------------------------------------------------------------------------------
B590_toolbar::B590_toolbar(const QString &title, QWidget *parent) :
    QToolBar(parent)
{
    QToolBar::setWindowTitle(title);
    init();
}
//--------------------------------------------------------------------------------
B590_toolbar::B590_toolbar(QWidget *parent) :
    QToolBar(parent)
{
    init();
}
//--------------------------------------------------------------------------------
B590_toolbar::~B590_toolbar()
{
    if(c1) c1->deleteLater();
    if(c2) c2->deleteLater();
    if(btn_run) btn_run->deleteLater();
}
//--------------------------------------------------------------------------------
void B590_toolbar::init(void)
{
    c1 = new QComboBox();
    c2 = new QComboBox();

    btn_run = new QPushButton();
    btn_run->setMinimumWidth(50);
    btn_run->setText("run");

    c1->setObjectName("c1");
    c2->setObjectName("c2");

    c1->addItem("0x0XX", QVariant(Qt::UserRole + 0x00));
    c1->addItem("0x1XX", QVariant(Qt::UserRole + 0x01));
    c1->addItem("0x4XX", QVariant(Qt::UserRole + 0x04));
    c1->addItem("0x5XX", QVariant(Qt::UserRole + 0x05));
    c1->addItem("0x6XX", QVariant(Qt::UserRole + 0x06));
    c1->addItem("0x7XX", QVariant(Qt::UserRole + 0x07));

    c2->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    addWidget(new QLabel(tr("Б5-90")));
    addWidget(c1);
    addWidget(c2);
    addWidget(btn_run);

    connect(c1,     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,   &B590_toolbar::currentIndexChanged);
    connect(btn_run,    &QPushButton::released,
            this,       &B590_toolbar::run);
    currentIndexChanged(0);
}
//--------------------------------------------------------------------------------
void B590_toolbar::currentIndexChanged(int index)
{
    bool ok = false;
    int cmd = c1->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;

    switch(cmd)
    {
    case 0:
        break;

    case 1:
        c2->clear();
        c2->addItem(B590_CMD_0x11_TEXT, QVariant(Qt::UserRole + B590_CMD_0x11));
        c2->addItem(B590_CMD_0x12_TEXT, QVariant(Qt::UserRole + B590_CMD_0x12));
        c2->addItem(B590_CMD_0x15_TEXT, QVariant(Qt::UserRole + B590_CMD_0x15));
        c2->addItem(B590_CMD_0x16_TEXT, QVariant(Qt::UserRole + B590_CMD_0x16));
        c2->addItem(B590_CMD_0x17_TEXT, QVariant(Qt::UserRole + B590_CMD_0x17));
        break;

    case 2:
        break;

    case 3:
        break;

    case 4:
        c2->clear();
        c2->addItem(B590_CMD_0x41_TEXT, QVariant(Qt::UserRole + B590_CMD_0x41));
        c2->addItem(B590_CMD_0x42_TEXT, QVariant(Qt::UserRole + B590_CMD_0x42));
        c2->addItem(B590_CMD_0x46_TEXT, QVariant(Qt::UserRole + B590_CMD_0x46));
        c2->addItem(B590_CMD_0x47_TEXT, QVariant(Qt::UserRole + B590_CMD_0x47));
        c2->addItem(B590_CMD_0x48_TEXT, QVariant(Qt::UserRole + B590_CMD_0x48));
        c2->addItem(B590_CMD_0x49_TEXT, QVariant(Qt::UserRole + B590_CMD_0x49));
        c2->addItem(B590_CMD_0x4A_TEXT, QVariant(Qt::UserRole + B590_CMD_0x4A));
        break;

    case 5:
        c2->clear();
        c2->addItem(B590_CMD_0x54_TEXT, QVariant(Qt::UserRole + B590_CMD_0x54));
        c2->addItem(B590_CMD_0x55_TEXT, QVariant(Qt::UserRole + B590_CMD_0x55));
        c2->addItem(B590_CMD_0x56_TEXT, QVariant(Qt::UserRole + B590_CMD_0x56));
        c2->addItem(B590_CMD_0x57_TEXT, QVariant(Qt::UserRole + B590_CMD_0x57));
        c2->addItem(B590_CMD_0x5A_TEXT, QVariant(Qt::UserRole + B590_CMD_0x5A));
        c2->addItem(B590_CMD_0x5B_TEXT, QVariant(Qt::UserRole + B590_CMD_0x5B));
        c2->addItem(B590_CMD_0x5C_TEXT, QVariant(Qt::UserRole + B590_CMD_0x5C));
        c2->addItem(B590_CMD_0x5D_TEXT, QVariant(Qt::UserRole + B590_CMD_0x5D));
        c2->addItem(B590_CMD_0x5E_TEXT, QVariant(Qt::UserRole + B590_CMD_0x5E));
        c2->addItem(B590_CMD_0x5F_TEXT, QVariant(Qt::UserRole + B590_CMD_0x5F));
        break;

    case 6:
        c2->clear();
        c2->addItem(B590_CMD_0x60_TEXT, QVariant(Qt::UserRole + B590_CMD_0x60));
        c2->addItem(B590_CMD_0x61_TEXT, QVariant(Qt::UserRole + B590_CMD_0x61));
        c2->addItem(B590_CMD_0x62_TEXT, QVariant(Qt::UserRole + B590_CMD_0x62));
        c2->addItem(B590_CMD_0x63_TEXT, QVariant(Qt::UserRole + B590_CMD_0x63));
        c2->addItem(B590_CMD_0x64_TEXT, QVariant(Qt::UserRole + B590_CMD_0x64));
        c2->addItem(B590_CMD_0x65_TEXT, QVariant(Qt::UserRole + B590_CMD_0x65));
        c2->addItem(B590_CMD_0x66_TEXT, QVariant(Qt::UserRole + B590_CMD_0x66));
        c2->addItem(B590_CMD_0x67_TEXT, QVariant(Qt::UserRole + B590_CMD_0x67));
        c2->addItem(B590_CMD_0x68_TEXT, QVariant(Qt::UserRole + B590_CMD_0x68));
        c2->addItem(B590_CMD_0x69_TEXT, QVariant(Qt::UserRole + B590_CMD_0x69));
        c2->addItem(B590_CMD_0x6A_TEXT, QVariant(Qt::UserRole + B590_CMD_0x6A));
        c2->addItem(B590_CMD_0x6B_TEXT, QVariant(Qt::UserRole + B590_CMD_0x6B));
        c2->addItem(B590_CMD_0x6C_TEXT, QVariant(Qt::UserRole + B590_CMD_0x6C));
        c2->addItem(B590_CMD_0x6D_TEXT, QVariant(Qt::UserRole + B590_CMD_0x6D));
        c2->addItem(B590_CMD_0x6E_TEXT, QVariant(Qt::UserRole + B590_CMD_0x6E));
        c2->addItem(B590_CMD_0x6F_TEXT, QVariant(Qt::UserRole + B590_CMD_0x6F));
        break;

    case 7:
        c2->clear();
        c2->addItem(B590_CMD_0x71_TEXT, QVariant(Qt::UserRole + B590_CMD_0x71));
        c2->addItem(B590_CMD_0x72_TEXT, QVariant(Qt::UserRole + B590_CMD_0x72));
        c2->addItem(B590_CMD_0x73_TEXT, QVariant(Qt::UserRole + B590_CMD_0x73));
        c2->addItem(B590_CMD_0x75_TEXT, QVariant(Qt::UserRole + B590_CMD_0x75));
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void B590_toolbar::run(void)
{
    bool ok = false;
    int cmd = c2->itemData(c2->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;

    emit press(cmd);
}
//--------------------------------------------------------------------------------
