#ifndef WIDGET2_HPP
#define WIDGET2_HPP

#include <QWidget>

namespace Ui {
class Widget2;
}

class Widget2 : public QWidget
{
    Q_OBJECT

public:
    explicit Widget2(QWidget *parent = nullptr);
    ~Widget2();

private:
    Ui::Widget2 *ui;
};

#endif // WIDGET2_HPP
