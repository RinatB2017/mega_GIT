#ifndef WIDGET1_HPP
#define WIDGET1_HPP

#include <QWidget>
#include "widget2.hpp"

namespace Ui {
    class Widget1;
}

class Widget1 : public Widget2
{
    Q_OBJECT

public:
    explicit Widget1(QWidget *parent = nullptr);
    ~Widget1();

private:
    Ui::Widget1 *ui;
};

#endif // WIDGET1_HPP
