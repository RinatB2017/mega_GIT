#ifndef MAINWIDGET_H
#define MAINWIDGET_H
//--------------------------------------------------------------------------------
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
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

private:
    Ui::MainWidget* ui;
    Mat mOrigImage;

    QString m_lastLoadPath;
    QSettings m_settings;
};
//--------------------------------------------------------------------------------
#endif // MAINWIDGET_H
