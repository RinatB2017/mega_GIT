#ifndef MYTOOLBAR_HPP
#define MYTOOLBAR_HPP

#include <QToolButton>
#include <QToolBar>

namespace Ui {
    class MyToolBar;
}

class MyToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit MyToolBar(QWidget *parent = nullptr);
    ~MyToolBar();

private:
    Ui::MyToolBar *ui;
};

#endif // MYTOOLBAR_HPP
