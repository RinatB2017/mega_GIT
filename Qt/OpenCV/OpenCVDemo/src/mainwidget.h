//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
#ifndef MAINWIDGET_H
#define MAINWIDGET_H
//--------------------------------------------------------------------------------
#include </usr/include/opencv2/objdetect.hpp>
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainWidget;
}
//--------------------------------------------------------------------------------
class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = 0 );
    ~MainWidget();

private slots:
    void onLoadImage();

private:
    Ui::MainWidget* ui;

    cv::CascadeClassifier m_faceClassifier;
};
//--------------------------------------------------------------------------------
#endif // MAINWIDGET_H
