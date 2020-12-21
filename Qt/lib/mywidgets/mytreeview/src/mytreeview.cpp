#include "mytreeview.hpp"

#include <QtCore>


MyTreeView::MyTreeView(QWidget *parent) : QTreeView(parent)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(onSingleClick()));
}


void MyTreeView::mouseDoubleClickEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    qDebug() << "This happens on double click";
    timer.stop();
}


void MyTreeView::mousePressEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    timer.start(250);
}

void MyTreeView::onSingleClick()
{
    qDebug() << "This happens on single click";
    timer.stop();
}
