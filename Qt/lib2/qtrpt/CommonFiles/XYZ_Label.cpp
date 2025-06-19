/*
Name: XYZ
Version: 3.1.1
Web-site: https://qtrpt.sourceforge.io
Programmer: Oleksii Osypov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2025 Oleksii Osypov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "XYZ_Label.h"
#include <QDebug>

XYZ_Label::XYZ_Label(QWidget *parent)
    : QLabel(parent)
{
    m_bHover = false;
    setCursor(Qt::PointingHandCursor);
}

XYZ_Label::XYZ_Label(const QString &Text, QWidget *parent)
    : QLabel(Text, parent)
{
    m_bHover = false;
    setCursor(Qt::PointingHandCursor);
}

XYZ_Label::~XYZ_Label()
{}

void XYZ_Label::setHoverText(bool bHover)
{
    m_bHover = bHover;
}

void XYZ_Label::enterEvent(QEvent *)
{
    if (m_bHover) {
        QFont font = this->font();
        font.setUnderline(m_bHover);
        setFont(font);
    }
}

void XYZ_Label::leaveEvent(QEvent *)
{
    if( m_bHover ) {
        QFont font = this->font();
        font.setUnderline(false);
        setFont(font);
    }
}

void XYZ_Label::mousePressEvent(QMouseEvent *)
{
    //emit clicked();
}

void XYZ_Label::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
    qDebug() << "DBL";
}

void XYZ_Label::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
    /*qDebug() << "---------";
    if (timer.isActive())
    {
        timer.stop();
        emit doubleClicked();
        qDebug() << "DBL---------";
    }
    else
    {	
        timer.start(200, this);
    }*/
}

void XYZ_Label::timerEvent(QTimerEvent *)
{
    timer.stop();
	emit clicked();
}
