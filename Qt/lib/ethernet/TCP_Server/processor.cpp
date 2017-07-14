//--------------------------------------------------------------------------------
#include <QObject>
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "processor.hpp"
//--------------------------------------------------------------------------------
Processor::Processor(QWidget *parent) :
    MyWidget(parent)
{

}
//--------------------------------------------------------------------------------
void Processor::input(const QByteArray &data)
{
#if 0
    emit output(data);
#else
    Q_UNUSED(data)
    emit output(QByteArray("ok"));
#endif
}
//--------------------------------------------------------------------------------
