

#include <QtGlobal>
#include <QtGui>
#include <QPolygon>


#include "qswitch.h"

QSwitch::QSwitch(QWidget *parent)
    : QWidget(parent)
{
   m_value=false;
   m_type=Vertical;
   setFocusPolicy(Qt::StrongFocus);
   setMinimumSize(QSize(50,50));
   
}

void QSwitch::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setWindow( -501,-501,1002,1002);
	
	if(hasFocus())
	{
	  QPen pen(Qt::DotLine);
	  painter.setPen(pen);
	  painter.drawRect(-500,-500,999,999);
	}
	
	painter.setPen(Qt::white);
	painter.drawArc(-250,-250,500,500,0,5670);
	painter.drawArc(-333+1,-333,666,666,0,5670);
	painter.setPen(Qt::darkGray);
	painter.drawArc(-333-1,-333,666,666,3400,3000);
	
	QRadialGradient radialGrad(QPointF(-250, -250), 333);
    radialGrad.setColorAt(0, Qt::lightGray);
    radialGrad.setColorAt(0.4, Qt::white);
    radialGrad.setColorAt(1, QColor(80,80,80)); 

	QBrush brush(radialGrad);
    painter.setBrush(brush);
	painter.drawEllipse(-250,-250,500,500);
	
	if(m_type==Horizontal)
	 painter.rotate(90.0);
	
	QPolygon a(4);
	QPen pen(QColor(70,70,70));
	painter.setPen(pen);
	if(m_value)
	{
	  a.setPoint(0,-60,0);
	  a.setPoint(1,60,0);
	  a.setPoint(2,350,-400);
	  a.setPoint(3,-350,-400);	
	  
      QLinearGradient linearGrad(QPointF(-350, -400), QPoint(60,0));
      linearGrad.setColorAt(0, Qt::white);
      linearGrad.setColorAt(0.8, QColor(80,80,80)); 		
		
	  QBrush brush(linearGrad);
	  painter.setBrush(brush); 	
	  painter.drawPolygon(a);
      a.setPoint(0,-250,-500);
      a.setPoint(1,250,-500);
	  a.setPoint(2,350,-400);
	  a.setPoint(3,-350,-400);	
	  painter.drawPolygon(a);	
	}
	else
	{
	  a.setPoint(0,-60,0);
	  a.setPoint(1,60,0);
	  a.setPoint(2,350,400);
	  a.setPoint(3,-350,400);	
	  
      QLinearGradient linearGrad(QPointF(-60,0), QPoint(350,400));
      linearGrad.setColorAt(0.7, QColor(80,80,80));
      linearGrad.setColorAt(0, Qt::white); 		

	  QBrush brush(linearGrad);
	  painter.setBrush(brush); 	
	  painter.drawPolygon(a);
      a.setPoint(0,-250,499);
      a.setPoint(1,250,499);
	  a.setPoint(2,350,400);
	  a.setPoint(3,-350,400);	

      QLinearGradient linearGrad2(QPointF(-350,499), QPoint(350,400));
      linearGrad2.setColorAt(0.7, QColor(80,80,80));
      linearGrad2.setColorAt(0, Qt::white); 		
      QBrush brush2(linearGrad2);
	  painter.setBrush(brush2); 	
	  painter.drawPolygon(a);			
	}
	
}


void QSwitch::setValue(bool value)
{
   m_value=value;
   update();
}

void QSwitch::setType(SwitchType type)
{
   m_type=type;
   update();
}

void QSwitch::toggleValue()
{ 
	m_value=!m_value;
	update();
	return; 
}


void QSwitch::mouseReleaseEvent(QMouseEvent *)
{
   toggleValue();  
   emit valueChanged(m_value);
}


void QSwitch::keyReleaseEvent(QKeyEvent *e)
{
int keyPressed=e->key();

	if(keyPressed==Qt::Key_Return || keyPressed==Qt::Key_Space)
	{
		toggleValue();
		emit valueChanged(m_value);
	}
}
