//--------------------------------------------------------------------------------
#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QByteArray;
class QString;
class QWidget;
//--------------------------------------------------------------------------------
class Processor : public MyWidget
{
    Q_OBJECT
public:
    Processor(QWidget *parent = 0);
    
signals:
    void output(const QByteArray &);

public slots:
    void input(const QByteArray &data);

private:
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
