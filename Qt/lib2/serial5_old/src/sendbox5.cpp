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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "sendbox5.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
SendBox5::SendBox5(QWidget *parent) :
    QFrame(parent)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));
    }

    btn_send_text = new QToolButton(this);
    btn_send_text->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    btn_send_text->setObjectName("btn_send_text");

    btn_send_bin  = new QToolButton(this);
    btn_send_bin->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    btn_send_bin->setObjectName("btn_send_bin");

    cb_send_text = new QComboBox(this);
    cb_send_text->setEditable(true);
    cb_send_text->setObjectName("cb_send_text");

    cb_send_bin  = new QComboBox(this);
    cb_send_bin->setEditable(true);
    cb_send_bin->setObjectName("le_send_bin");

    cb_send_text->addItem("test");
    cb_send_bin->addItem("0A0B0C0D");

    append_comboBox = new QComboBox(this);
    append_comboBox->setObjectName("append_comboBox");

    append_comboBox->setSizePolicy(QSizePolicy::Fixed,  QSizePolicy::Preferred);

    grid = new QGridLayout;
    grid->addWidget(cb_send_text,       0, 0);
    grid->addWidget(append_comboBox,    0, 1);
    grid->addWidget(btn_send_text,      0, 2);
    grid->addWidget(cb_send_bin,        1, 0);
    grid->addWidget(btn_send_bin,       1, 2);

    setLayout(grid);

    connect(btn_send_text,  SIGNAL(clicked()),  this,   SLOT(send_text()));
    connect(btn_send_bin,   SIGNAL(clicked()),  this,   SLOT(send_bin()));

    connect(cb_send_text->lineEdit(),   SIGNAL(editingFinished()),  this,   SLOT(send_text()));
    connect(cb_send_bin->lineEdit(),    SIGNAL(editingFinished()),  this,   SLOT(send_bin()));

    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}
//--------------------------------------------------------------------------------
SendBox5::~SendBox5()
{
    if(cb_send_text)        delete cb_send_text;
    if(cb_send_bin)         delete cb_send_bin;
    if(append_comboBox)     delete append_comboBox;

    if(btn_send_text)       delete btn_send_text;
    if(btn_send_bin)        delete btn_send_bin;

    if(cb_SendStenToStep)   delete cb_SendStenToStep;

    if(grid)                delete grid;
}
//--------------------------------------------------------------------------------
void SendBox5::send_text(void)
{
    QString data;
    QByteArray ba;

    data = cb_send_text->currentText();
    if(data.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("nothing send"));
        msgBox.exec();
        return;
    }
    ba.clear();
    ba.append(data);
    switch(append_comboBox->currentIndex())
    {
    case 0: // tr("no add")
        break;

    case 1: //  << tr("0x00")
        ba.append((char)0x00);
        break;

    case 2: //  << tr("0x0D")
        ba.append((char)0x0D);
        break;

    case 3: //  << tr("0x0A")
        ba.append((char)0x0A);
        break;

    case 4: //  << tr("0x0D 0x0A")
        ba.append((char)0x0D);
        ba.append((char)0x0A);
        break;
    }
    emit sendData(ba);
}
//--------------------------------------------------------------------------------
void SendBox5::send_bin(void)
{
    QByteArray input_data;
    QByteArray output_data;

    input_data.clear();
    input_data.append(cb_send_bin->currentText());
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
    output_data.append(QByteArray::fromHex(input_data));

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
void SendBox5::block_interface(bool state)
{
    cb_send_text->setDisabled(state);
    cb_send_bin->setDisabled(state);
    append_comboBox->setDisabled(state);

    btn_send_text->setDisabled(state);
    btn_send_bin->setDisabled(state);
}
//--------------------------------------------------------------------------------
void SendBox5::updateText(void)
{
    btn_send_text->setText(tr("send"));
    btn_send_bin->setText(tr("send"));

    append_comboBox->clear();
    append_comboBox->addItems(QStringList() << tr("no add")
                              << tr("0x00")
                              << tr("0x0D")
                              << tr("0x0A")
                              << tr("0x0D 0x0A") );
}
//--------------------------------------------------------------------------------
