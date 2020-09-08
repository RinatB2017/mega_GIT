/***************************************************************************
 *   Copyright (C) 2008 by P. Sereno                                       *
 *   http://www.sereno-online.com                                          *
 *   http://www.qt4lab.org                                                 *
 *   http://www.qphoton.org                                                *
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

#include <QColor>
#include <QtGlobal>
#include <QtGui>
#include <QPolygon>
#include <math.h>
#include "qmeter.h"
#include <QPainter>

void QMeter::drawCrown(QPainter *painter)
{
	QRadialGradient haloGradientw(0, 0, 49, 0, 0);
	haloGradientw.setColorAt(0.0, Qt::lightGray);
	haloGradientw.setColorAt(0.8, Qt::darkGray);
	haloGradientw.setColorAt(0.9, Qt::white);
	haloGradientw.setColorAt(1.0, Qt::black);
	painter->setPen(Qt::NoPen);
	painter->setBrush(haloGradientw);
	painter->drawEllipse(-49, -49, 98, 98);	
}


void QMeter::drawBackground(QPainter *painter)
{
	painter->setBrush(m_background);
	painter->drawEllipse(-45, -45, 90, 90);
}


void QMeter::drawTicks(QPainter *painter)
{
	painter->save();
	painter->setPen(m_foreground);
	int nSteps=m_steps;
	painter->rotate(-m_startAngle) ;	
	double angleStep=(-m_endAngle+m_startAngle)/m_steps;
	for ( int i=0; i<=nSteps*2;i++ ) 
	{
		if(i&0x0001)
		 painter->drawLine(22,0,24,0 );
		else
  		 painter->drawLine(20,0,25,0 );
		painter->rotate( angleStep/2 );		
	}		
	painter->restore();	
}


void QMeter::drawScale(QPainter *painter)
{
	painter->save();
	painter->setPen(m_foreground);
	double startRad=m_startAngle*3.14/180.0;
	startRad+=3.14/2;
	double deltaRad=(m_endAngle-m_startAngle)/(m_steps)*(3.14/180);
	double sina,cosa;
	for ( int i=0; i<=m_steps; i++ ) 
	{
		sina=sin((double)(startRad+i*deltaRad));
		cosa=cos((double)(startRad+i*deltaRad));			
		double tmpVal=i*((m_maxValue-m_minValue)/m_steps);
		tmpVal+=m_minValue;
		QString str = QString( "%1" ).arg(tmpVal,0,'f',m_precision);
		QFontMetricsF fm( this->font() );
		double w=fm.size(Qt::TextSingleLine,str).width();
		double h=fm.size(Qt::TextSingleLine,str).height();
		int x=(int)((35*sina)-(w/2));
		int y=(int)((35*cosa)+(h/4));
		painter->drawText(x,y,str);	
	}		
	painter->restore();	
}


void QMeter::drawUnits(QPainter *painter)
{
	painter->setPen(m_foreground);
	painter->setBrush(m_foreground);
	QString str(m_units);
    QFont f=font();
    f.setPointSize(4);
    QFontMetricsF fm( f );
	double w=fm.size(Qt::TextSingleLine,str).width();
    QPainterPath path;
    path.addText(QPointF(-w/2,-10),f,m_units);
	painter->drawPath(path);	
}


void QMeter::drawNumericValue(QPainter *painter)
{
	
	QString str = QString( "%1" ).arg(m_value,0,'f',m_precision);
    QFont f=font();
    QFontMetricsF fm( f );
	double w=fm.size(Qt::TextSingleLine,str).width();
	QColor color;
	
	if(!m_thresholdFlag)
     color=QColor(150, 150, 200);
	else
	 color=Qt::red;
	
	painter->setPen(Qt::NoPen);
	painter->setBrush(color);
	painter->drawRect(-20,20,40,14);		
	painter->setBrush(m_background);	
	painter->drawRect(-18,22,36,10);
	painter->setPen(m_foreground);
	painter->setBrush(m_foreground);
    QPainterPath path;
    path.addText(QPointF(-w/2,31),f,str);
	painter->drawPath(path);	 	
}


void QMeter::drawIndicator(QPainter *painter)
{
	painter->save();

	painter->rotate(-90.0); 
	QPolygon pts;
	pts.setPoints( 3, -2,0,  2,0,  0,25 );
	int degRotate=0;

    degRotate = (int)( m_startAngle+(m_endAngle-m_startAngle)/(m_maxValue-m_minValue)*(m_value-m_minValue) );	 
	 
	painter->rotate( -degRotate );
    QRadialGradient haloGradient(0, 0, 20, 0, 0);
    haloGradient.setColorAt(0.0, QColor(255,120,120));
    haloGradient.setColorAt(1.0, QColor(200,20,20));
    painter->setPen(Qt::darkRed);	
	painter->setBrush(haloGradient);
	painter->drawConvexPolygon( pts );
    //painter->restore();
	
	// draw indicator hat
	QColor niceBlue(150, 150, 200);
	QConicalGradient coneGradient(0, 0, -90.0);
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);
    painter->setPen(Qt::NoPen);
    painter->setBrush(coneGradient);
    painter->drawEllipse(-5, -5, 10, 10);	
		
	
	painter->restore();	
}


void QMeter::drawThresholdLine(QPainter *painter)
{
    QColor color(Qt::red);
    color.setAlpha(80);
    QPen pen(color);

    double thresholdAngle = ( m_startAngle+(m_endAngle-m_startAngle)/(m_maxValue-m_minValue)*(m_threshold-m_minValue) );	     
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawArc(-22,-22,44,44,(int)thresholdAngle*16,(int)(-thresholdAngle+m_endAngle)*16);
      	
}

