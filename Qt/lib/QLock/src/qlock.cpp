/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "qlock.h"

QLock::QLock(QWidget *parent) : QWidget(parent)
{
    val1 = 0;
    val2 = 0;
    val3 = 0;
    val4 = 0;
    val5 = 0;
    delta = 0;
    combination = 0;
    colLock = QColor(60, 60, 60);
    colUnlock = QColor(60, 60, 60);
}

void QLock::paintEvent(QPaintEvent *)
{
    paintBorder();
    paintWheels();
    paintNumbers();
    paintState();
}

void QLock::paintBorder()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 150, 100);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QLinearGradient linGrad(0, 50, 75, 70);
    linGrad.setColorAt(0, Qt::gray);
    linGrad.setColorAt(1, Qt::white);
    linGrad.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad);
    QRectF border(5, 5, 140, 90);
    painter.drawRoundRect(border, 10, 15);

    painter.setBrush(Qt::black);
    painter.drawRect(15, 15, 20, 50);
    painter.drawRect(40, 15, 20, 50);
    painter.drawRect(65, 15, 20, 50);
    painter.drawRect(90, 15, 20, 50);
    painter.drawRect(115, 15, 20, 50);

}

void QLock::paintWheels()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 150, 100);
    painter.setRenderHint(QPainter::Antialiasing);

    QRadialGradient radGrad1(25, 41, 35);
    radGrad1.setColorAt(0, Qt::white);
    radGrad1.setColorAt(1, Qt::darkGray);
    painter.setBrush(radGrad1);
    QRectF wheel1(16, 16, 18, 48);
    painter.drawRoundRect(wheel1, 30, 10);

    QRadialGradient radGrad2(50, 43, 35);
    radGrad2.setColorAt(0, Qt::white);
    radGrad2.setColorAt(1, Qt::darkGray);
    painter.setBrush(radGrad2);
    QRectF wheel2(41, 16, 18, 48);
    painter.drawRoundRect(wheel2, 30, 10);

    QRadialGradient radGrad3(75, 45, 35);
    radGrad3.setColorAt(0, Qt::white);
    radGrad3.setColorAt(1, Qt::darkGray);
    painter.setBrush(radGrad3);
    QRectF wheel3(66, 16, 18, 48);
    painter.drawRoundRect(wheel3, 30, 10);

    QRadialGradient radGrad4(100, 48, 35);
    radGrad4.setColorAt(0, Qt::white);
    radGrad4.setColorAt(1, Qt::darkGray);
    painter.setBrush(radGrad4);
    QRectF wheel4(91, 16, 18, 48);
    painter.drawRoundRect(wheel4, 30, 10);

    QRadialGradient radGrad5(125, 51, 35);
    radGrad5.setColorAt(0, Qt::white);
    radGrad5.setColorAt(1, Qt::darkGray);
    painter.setBrush(radGrad5);
    QRectF wheel5(116, 16, 18, 48);
    painter.drawRoundRect(wheel5, 30, 10);

    painter.setPen(QPen(Qt::darkGray, 2));

    painter.drawLine(18, 24, 32, 24);
    painter.drawLine(18, 56, 32, 56);

    painter.drawLine(43, 24, 57, 24);
    painter.drawLine(43, 56, 57, 56);

    painter.drawLine(68, 24, 82, 24);
    painter.drawLine(68, 56, 82, 56);

    painter.drawLine(93, 24, 107, 24);
    painter.drawLine(93, 56, 107, 56);

    painter.drawLine(118, 24, 132, 24);
    painter.drawLine(118, 56, 132, 56);
}

void QLock::paintNumbers()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 150, 100);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont valFont("Arial", 14, QFont::Bold);
    painter.setFont(valFont);

    QRectF R1(16, 16, 18, 48);
    QString str1 = QString("%1").arg(val1);
    painter.drawText(R1, Qt::AlignCenter, str1);
    QRectF R2(41, 16, 18, 48);
    QString str2 = QString("%1").arg(val2);
    painter.drawText(R2, Qt::AlignCenter, str2);
    QRectF R3(66, 16, 18, 48);
    QString str3 = QString("%1").arg(val3);
    painter.drawText(R3, Qt::AlignCenter, str3);
    QRectF R4(91, 16, 18, 48);
    QString str4 = QString("%1").arg(val4);
    painter.drawText(R4, Qt::AlignCenter, str4);
    QRectF R5(116, 16, 18, 48);
    QString str5 = QString("%1").arg(val5);
    painter.drawText(R5, Qt::AlignCenter, str5);

    int value = val1*10000+val2*1000+val3*100+val4*10+val5;
    combination = QByteArray::number(value);
}

void QLock::paintState()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 150, 100);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QRadialGradient radGrad1(25, 85, 10);
    radGrad1.setColorAt(0, Qt::white);
    radGrad1.setColorAt(1, colLock);
    painter.setBrush(radGrad1);
    painter.drawEllipse(15, 75, 10, 10);

    QRadialGradient radGrad2(90, 85, 10);
    radGrad2.setColorAt(0, Qt::white);
    radGrad2.setColorAt(1, colUnlock);
    painter.setBrush(radGrad2);
    painter.drawEllipse(80, 75, 10, 10);

    QFont stateFont("Arial", 8, QFont::Normal);
    painter.setFont(stateFont);

    QRectF lock (28, 75, 30, 15);
    painter.drawText(lock , Qt::AlignLeft, "LOCK");

    QRectF unlock(93, 75, 45, 15);
    painter.drawText(unlock, Qt::AlignLeft, "UNLOCK");

}

void QLock::wheelEvent(QWheelEvent *e)
{
    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 150;
    double hstart = 100;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    int delta = e->delta()/120;

    if (y >=(16*dy) && y <=(64*dy))
    {
        if (x >=(16*dx) && x <=(34*dx))
        {
            val1 = val1+delta;
            if (val1 == 10)
                val1 = 0;
            if (val1 == -1)
                val1 = 9;
            update();
        }
        if (x >=(41*dx) && x <=(59*dx))
        {
            val2 = val2+delta;
            if (val2 == 10)
                val2 = 0;
            if (val2 == -1)
                val2 = 9;
            update();
        }
        if (x >=(66*dx) && x <=(84*dx))
        {
            val3 = val3+delta;
            if (val3 == 10)
                val3 = 0;
            if (val3 == -1)
                val3 = 9;
            update();
        }
        if (x >=(91*dx) && x <=(109*dx))
        {
            val4 = val4+delta;
            if (val4 == 10)
                val4 = 0;
            if (val4 == -1)
                val4 = 9;
            update();
        }
        if (x >=(116*dx) && x <=(134*dx))
        {
            val5 = val5+delta;
            if (val5 == 10)
                val5 = 0;
            if (val5 == -1)
                val5 = 9;
            update();
        }
    }
}

void QLock::mousePressEvent(QMouseEvent *e)
{
    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 150;
    double hstart = 100;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    if (y >=(75*dy) && y <=(85*dy) && x >=(15*dx) && x <=(25*dx))
    {
        QString path = QCoreApplication::applicationDirPath();
        QFile file(path + "/code.txt");
        if (file.exists())
            return;
        if (!file.open(QIODevice::ReadWrite))
            return;

        int combination =  val1*10000+val2*1000+val3*100+val4*10+val5;
        QByteArray md5 = QCryptographicHash::hash(QByteArray::number(combination), QCryptographicHash::Md5);

        file.write(md5.toHex());
        file.flush();
        file.close();
        colLock = Qt::red;
        colUnlock = QColor(60, 60, 60);
        update();

        emit isLocked(true);
    }

    if (y >=(75*dy) && y <=(85*dy) && x >=(80*dx) && x <=(90*dx))
    {
        QString path = QCoreApplication::applicationDirPath();
        QFile file(path + "/code.txt");
        if (!file.exists())
            return;
        if (!file.open(QIODevice::ReadWrite))
            return;

        QByteArray hex = file.readAll();
        QByteArray md5 = QByteArray::fromHex(hex);
        QByteArray code = QCryptographicHash::hash(combination, QCryptographicHash::Md5);

        if (code == md5)
        {
            file.remove();
            colUnlock = Qt::green;
            colLock = QColor(60, 60, 60);
            update();

            emit isLocked(false);
        }
    }
}

QSize QLock::minimumSizeHint() const
{
    return QSize(15, 10);
}

QSize QLock::sizeHint() const
{
    return QSize(150, 100);
}
