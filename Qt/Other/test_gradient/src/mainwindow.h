#include <QApplication>
#include <QMainWindow>
#include <QtGui>

class MainWindow: public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent *);
};
