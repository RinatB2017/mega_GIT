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
#include <QDialogButtonBox>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QVariant>
#include <QStyle>
#include <QIcon>
//--------------------------------------------------------------------------------
#include "websettingbox.hpp"
#include "webview.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
WebSettingBox::WebSettingBox(QVector<ATTR> &attr,
                             QWidget *parent) :
    QDialog(parent)
{
    attributes.clear();
    for(int n=0; n<attr.count(); n++)
    {
        attributes.append(attr.at(n));
    }

    init();;
}
//--------------------------------------------------------------------------------
void WebSettingBox::init(void)
{
    setWindowTitle(tr("WebSettings"));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(0);

    for(int n=0; n<attributes.count(); n++)
    {
        QCheckBox *checkBox = new QCheckBox(attributes[n].name);
        attributes[n].object = checkBox;
        checkBox->setProperty("Attribute", attributes.at(n).attribute);
        checkBox->setChecked(attributes[n].value); //QWebSettings::globalSettings()->testAttribute(attributes[n].attribute));
        checkBox->setToolTip(attributes[n].tooltip);
        vbox->addWidget(checkBox);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    buttonBox->setCenterButtons(true);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QPushButton *btn_default = new QPushButton(this);
    QPushButton *btn_select_all = new QPushButton(this);
    QPushButton *btn_clear_all = new QPushButton(this);

    btn_default->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogResetButton));
    btn_default->setText(tr("default"));

    btn_select_all->setText(tr("select all"));
    btn_clear_all->setText(tr("clear all"));

    connect(btn_default,    SIGNAL(clicked()), this, SLOT(set_default()));
    connect(btn_select_all, SIGNAL(clicked()), this, SLOT(select_all()));
    connect(btn_clear_all,  SIGNAL(clicked()), this, SLOT(clear_all()));

    vbox->addWidget(buttonBox);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(btn_select_all);
    hbox->addWidget(btn_clear_all);

    vbox->addLayout(hbox);
    vbox->addWidget(btn_default);

    vbox->addLayout(hbox);
    setLayout(vbox);

    adjustSize();
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void WebSettingBox::set_default(void)
{
    for(int n=0; n<attributes.count(); n++)
    {
        QCheckBox *checkBox = attributes[n].object;
        checkBox->setChecked(attributes[n].value);
    }
}
//--------------------------------------------------------------------------------
void WebSettingBox::select_all(void)
{
    for(int n=0; n<attributes.count(); n++)
    {
        QCheckBox *checkBox = attributes[n].object;
        checkBox->setChecked(true);
    }
}
//--------------------------------------------------------------------------------
void WebSettingBox::clear_all(void)
{
    for(int n=0; n<attributes.count(); n++)
    {
        QCheckBox *checkBox = attributes[n].object;
        checkBox->setChecked(false);
    }
}
//--------------------------------------------------------------------------------
void WebSettingBox::accept(void)
{
    for(int n=0; n<attributes.count(); n++)
    {
        QCheckBox *checkBox = attributes[n].object;
        attributes[n].value = checkBox->isChecked();
    }

    QDialog::accept();
}
//--------------------------------------------------------------------------------
QVector<ATTR> WebSettingBox::get_attributes(void)
{
    return attributes;
}
//--------------------------------------------------------------------------------
