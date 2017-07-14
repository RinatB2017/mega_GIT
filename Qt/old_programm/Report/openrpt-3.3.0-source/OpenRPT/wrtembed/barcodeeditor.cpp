/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2011 by OpenMFG, LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact info@openmfg.com with any questions on this license.
 */

#include "barcodeeditor.h"

#include <QVariant>
#include <QValidator>

BarcodeEditor::BarcodeEditor(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(cbFormat, SIGNAL(activated(int)), this, SLOT(cbFormat_activated(int)));
    connect(sliderMaxVal, SIGNAL(valueChanged(int)), labelMaxVal, SLOT(setNum(int)));
    connect(sliderMaxVal, SIGNAL(sliderMoved(int)), this, SLOT(sliderMaxVal_sliderMoved(int)));
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));

    leXPos->setValidator(new QDoubleValidator(0.0,100.0,3,leXPos));
    leYPos->setValidator(new QDoubleValidator(0.0,100.0,3,leYPos));
    
    leWidth->setValidator(new QDoubleValidator(0.01,100.0,3,leWidth));
    leHeight->setValidator(new QDoubleValidator(0.01,100.0,3,leHeight));
    leNarrowBarWidth->setValidator(new QDoubleValidator(0.005,0.1,3,leNarrowBarWidth));
}

BarcodeEditor::~BarcodeEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

void BarcodeEditor::languageChange()
{
    retranslateUi(this);
}

void BarcodeEditor::cbFormat_activated(int)
{
    // the format was changed so we need to recalculate our minimum sizes
}

void BarcodeEditor::sliderMaxVal_sliderMoved( int )
{
    // the slider for the max value length has been changed so we need to recalculate the minimum sizes
}

