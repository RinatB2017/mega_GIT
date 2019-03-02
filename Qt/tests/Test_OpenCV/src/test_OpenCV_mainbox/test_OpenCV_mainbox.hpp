#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QSettings>
//--------------------------------------------------------------------------------
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
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

    void s_default(void);
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

    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    cv::CascadeClassifier mNoseDetector;
    // <--- Face detectors

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
