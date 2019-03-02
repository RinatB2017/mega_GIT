#ifndef MAINWIDGET_H
#define MAINWIDGET_H
//--------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QSettings>
//--------------------------------------------------------------------------------
namespace Ui {
    class MainWidget;
}
//--------------------------------------------------------------------------------
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = nullptr );
    ~MainWidget();

private slots:
    void onLoad();
    void refreshHSV();

    void s_default(void);
    void s_test(void);

private:
    Ui::MainWidget* ui;
    cv::Mat m_mat;

    QString m_lastLoadPath;
    QSettings m_settings;

    // ---> Face detectors
    bool create_detectors(void);
    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    cv::CascadeClassifier mNoseDetector;
    // <--- Face detectors
};
//--------------------------------------------------------------------------------
#endif // MAINWIDGET_H
