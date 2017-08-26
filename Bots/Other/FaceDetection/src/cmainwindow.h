#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H
//--------------------------------------------------------------------------------
#include <QMainWindow>
//--------------------------------------------------------------------------------
#include </usr/include/opencv2/core/core.hpp>
#include </usr/include/opencv2/objdetect/objdetect.hpp>
#include </usr/include/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv2/highgui/highgui.hpp>
#include </usr/include/opencv2/opencv.hpp>
//--------------------------------------------------------------------------------
namespace Ui {
    class CMainWindow;
}
//--------------------------------------------------------------------------------
class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

protected:
    virtual void timerEvent(QTimerEvent *);

private slots:
    void start(void);
    void stop(void);
    void test(void);

    void set_brightness(int value);
    void set_contrast(int value);
    void set_saturation(int value);
    void set_hue(int value);

private:
    Ui::CMainWindow *ui;

    int mCameraEventId;

    cv::Mat mOrigImage;
    cv::Mat mElabImage;

    cv::VideoCapture mCapture;

    // ---> Face detectors
    void create_detectors(void);
    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    cv::CascadeClassifier mNoseDetector;
    // <--- Face detectors

    void show_image_cv(void);
    void show_image_hw(void);
};
//--------------------------------------------------------------------------------
#endif // CMAINWINDOW_H
