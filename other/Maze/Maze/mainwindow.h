#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void blockInterface();      // блокирование интерфейса для того что бы можно было нажать сгенерировать лабиринт, только после завершения
                                // передыдущей генерации
    void unBlockInterface();    // разблокирование интерфейса
};

#endif // MAINWINDOW_H
