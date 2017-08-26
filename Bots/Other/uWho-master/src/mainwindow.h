#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cv.h>
#include <highgui.h>
#include <QtCore>
#include <opencv2/objdetect/objdetect.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_videofileButton_clicked();

    void on_webcamButton_clicked();

    void on_dirpicButton_clicked();

    void on_ipButton_clicked();

private:
    Ui::MainWindow *ui;
//    cv::Mat detectAndDisplay(cv::Mat);
};

#endif // MAINWINDOW_H
