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

#ifndef QFREQ_H
#define QFREQ_H

#include <QtGui>
#include <QtDesigner/QDesignerExportWidget>
#include <stdio.h>
#include <math.h>
#include <fftw3.h>
#include <stdlib.h>


class QDESIGNER_WIDGET_EXPORT QFreq : public QWidget
{
    Q_OBJECT


public:

    QFreq(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;



signals:



public slots:


protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void paintBorder();
    void calculateFFT(int);
    void setZoomPoint(int);
    void paintFreq();
    void paintScale();
    void paintControl();
    void initValue();
    void initColor();
   
private:

    int zoom_point;
    int start_point;
    int end_point;
    int typeFFT;
    double *magnitudeLin;
    double *magnitudeLog;
    double min_val_lin;
    double max_val_lin;
    double min_val_log;
    double max_val_log;
    double freq1;
    double freq2;
    double freq3;
    double freq4;
    double freq5;
    double range_freq;
    bool lockFFT;
    QColor colLin;
    QColor colLog;
    QColor col512;
    QColor col1024;
    QColor col2048;
    QColor col4096;
    QColor colLock;
    QString wavFile;

    
    
};

#endif
