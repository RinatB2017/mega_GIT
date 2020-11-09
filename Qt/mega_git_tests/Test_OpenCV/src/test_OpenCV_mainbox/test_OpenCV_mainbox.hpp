#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QSettings>
//--------------------------------------------------------------------------------
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include <windows.h>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox( QWidget* parent = nullptr );
    ~MainBox();

private slots:
    void onLoad(void);
    void refreshHSV(void);

    void set_scaleFactor(int value);
    void set_scaleFactor(double value);
    void set_minNeighbors(int value);

    void find_faces(void);

    void s_test(void);

private:
    Ui::MainBox *ui;
    Mat mOrigImage;
    Mat mElabImage;

    QString m_lastLoadPath;

    // ---> Face detectors
    bool create_detectors(void);

    Mat grayFrames;
    CascadeClassifier faceCade;
    vector<Rect> faces;

    qreal scaleFactor = 1.0;
    int minNeighbors = 0;

    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    cv::CascadeClassifier mNoseDetector;
    // <--- Face detectors

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
