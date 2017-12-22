//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//--------------------------------------------------------------------------------
#include <QMainWindow>
#include <QFileDialog>
//--------------------------------------------------------------------------------
#include </usr/include/opencv/cv.h>
#include </usr/include/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv/highgui.h>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainWindow;
}
//--------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    QString     FileOpName; // declare FileOpName as IplImage
    IplImage*   imagerd;    // declare imagerd as IplImage

private slots:
    void on_pushButton_clicked();
    void openImage();       // add a new openImage function to the project

private:
    Ui::MainWindow *ui;

    QRect searchObjectByTemplate(QString srcImgName, QString templImgName);

};
//--------------------------------------------------------------------------------
#endif // MAINWINDOW_H
