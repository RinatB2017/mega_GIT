#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class FilterWindow;
}

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget *parent = 0);
    ~FilterWindow();

private:
    Ui::FilterWindow *ui;
};

#endif // FILTERWINDOW_H
