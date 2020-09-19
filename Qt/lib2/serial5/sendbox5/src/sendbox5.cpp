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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_sendbox5.h"
//--------------------------------------------------------------------------------
#include "sendbox5.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
SendBox5::SendBox5(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SendBox5)
{
    ui->setupUi(this);

    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));
    }

    ui->btn_send_text->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_send_text->setObjectName("btn_send_text");

    ui->btn_send_bin->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_send_bin->setObjectName("btn_send_bin");

    ui->btn_send_bin_remove->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->btn_send_text_remove->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));

    ui->cb_send_text->setEditable(true);
    ui->cb_send_text->setObjectName("cb_send_text");

    ui->cb_send_bin->setEditable(true);
    ui->cb_send_bin->setObjectName("le_send_bin");

    ui->append_comboBox->setObjectName("append_comboBox");

    ui->append_comboBox->setSizePolicy(QSizePolicy::Fixed,  QSizePolicy::Preferred);

    connect(ui->btn_send_text,  SIGNAL(clicked()),  this,   SLOT(send_text()));
    connect(ui->btn_send_bin,   SIGNAL(clicked()),  this,   SLOT(send_bin()));

    connect(ui->btn_send_text_remove,  SIGNAL(clicked()),  this,   SLOT(send_text_remove()));
    connect(ui->btn_send_bin_remove,   SIGNAL(clicked()),  this,   SLOT(send_bin_remove()));

    connect(ui->cb_send_text->lineEdit(),   SIGNAL(returnPressed()),  this,   SLOT(send_text()));
    connect(ui->cb_send_bin->lineEdit(),    SIGNAL(returnPressed()),  this,   SLOT(send_bin()));

    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}
//--------------------------------------------------------------------------------
SendBox5::~SendBox5()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void SendBox5::send_text(void)
{
    QString data;
//    QByteArray ba;

    data = ui->cb_send_text->currentText();
    if(data.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("nothing send"));
        msgBox.exec();
        return;
    }
//    ba.clear();
//    ba.append(data);
    switch(ui->append_comboBox->currentIndex())
    {
    case 0: // tr("no add")
        break;

    case 1: //  << tr("0x00")
//        ba.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        break;

    case 2: //  << tr("0x0D")
//        ba.append(static_cast<char>(0x0D));
        data.append(static_cast<char>(0x0D));
        break;

    case 3: //  << tr("0x0A")
//        ba.append(static_cast<char>(0x0A));
        data.append(static_cast<char>(0x0A));
        break;

    case 4: //  << tr("0x0D 0x0A")
//        ba.append(static_cast<char>(0x0D));
//        ba.append(static_cast<char>(0x0A));
        data.append(static_cast<char>(0x0D));
        data.append(static_cast<char>(0x0A));
        break;
    }
//    emit sendData(ba);
    emit sendData(data.toLatin1());
}
//--------------------------------------------------------------------------------
void SendBox5::send_bin(void)
{
//    QByteArray input_data;
    QByteArray output_data;

//    input_data.clear();
//    input_data.append(ui->cb_send_bin->currentText());
    QString input_data = ui->cb_send_bin->currentText();
    if(input_data.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("nothing send"));
        msgBox.exec();
        return;
    }

    output_data.clear();
    output_data.append(QByteArray::fromHex(input_data.toLatin1()));

    if(input_data.toUpper() != output_data.toHex().toUpper())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("invalid HEX-data"));
        msgBox.exec();
        return;
    }

    emit sendData(output_data);
}
//--------------------------------------------------------------------------------
void SendBox5::send_text_remove(void)
{
    ui->cb_send_text->removeItem(ui->cb_send_text->currentIndex());
}
//--------------------------------------------------------------------------------
void SendBox5::send_bin_remove(void)
{
    ui->cb_send_bin->removeItem(ui->cb_send_bin->currentIndex());
}
//--------------------------------------------------------------------------------
void SendBox5::block_interface(bool state)
{
    ui->cb_send_text->setDisabled(state);
    ui->cb_send_bin->setDisabled(state);
    ui->append_comboBox->setDisabled(state);

    ui->btn_send_text->setDisabled(state);
    ui->btn_send_bin->setDisabled(state);

    ui->btn_send_bin_remove->setDisabled(state);
    ui->btn_send_text_remove->setDisabled(state);
}
//--------------------------------------------------------------------------------
void SendBox5::updateText(void)
{
    ui->btn_send_text->setText(tr("send"));
    ui->btn_send_bin->setText(tr("send"));

    ui->append_comboBox->clear();
    ui->append_comboBox->addItems(QStringList() << tr("no add")
                                  << tr("0x00")
                                  << tr("0x0D")
                                  << tr("0x0A")
                                  << tr("0x0D 0x0A") );
}
//--------------------------------------------------------------------------------
