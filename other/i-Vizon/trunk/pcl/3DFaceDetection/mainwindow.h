#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QApplication>
#include<QMutex>
#include<QThread>
#include <QtCore>

#ifndef Q_MOC_RUN
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/openni_grabber.h>
#include <pcl/common/time.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>


#include "pcl/recognition/face_detection/rf_face_detector_trainer.h"
#include "openni_frame_source.h"
//#include "face_detection_apps_utils.h"
#endif
// Useful macros
#define FPS_CALC(_WHAT_) \
do \
{ \
    static unsigned count = 0;\
    static double last = pcl::getTime ();\
    double now = pcl::getTime (); \
    ++count; \
    if (now - last >= 1.0) \
    { \
      std::cout << "Average framerate("<< _WHAT_ << "): " << double(count)/double(now - last) << " Hz" <<  std::endl; \
      count = 0; \
      last = now; \
    } \
}while(false)


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    typedef pcl::PointCloud<pcl::PointXYZRGBA> Cloud;
    typedef Cloud::Ptr CloudPtr;
    typedef Cloud::ConstPtr CloudConstPtr;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void cloud_cb (const CloudConstPtr& rcvCloud);


    
private slots:
    void on_pushButton_clicked();
    void timeOutSlot();


private:
    Ui::MainWindow *ui;

    QMutex mtx;
    QTimer *vis_timer;

    QRect rect;
    int winWidth;
    int winHeight;

// FaceDetection Variables
    int STRIDE_SW;
    int use_normals ;
    float trans_max_variance ;
    int min_votes_size ;
    float face_threshold;
    int heat_map;
    int show_votes;
    int pose_refinement_ ;
    int icp_iterations ;
    std::string forest_fn;
    std::string model_path_;

    pcl::RFFaceDetectorTrainer fdrf;

    int detectedFaces;
    std::vector<Eigen::VectorXf> faces;

    pcl::PointCloud<pcl::PointXYZI>::Ptr intensity_cloud;

    pcl::PointCloud<pcl::PointXYZI>::Ptr faceHeatMap;
    QPalette *palette;

protected:
    boost::shared_ptr<pcl::visualization::PCLVisualizer> pclVis;
    pcl::OpenNIGrabber grabber;
    std::string device_id;
    CloudPtr cloudAftrFilter;
    pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr dataToView;
    pcl::PassThrough<pcl::PointXYZRGBA> passfilter;
};

#endif // MAINWINDOW_H
