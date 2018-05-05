#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pcl/recognition/face_detection/rf_face_detector_trainer.h>
#include <pcl/common/time.h>
#include <pcl/console/parse.h>
#include <pcl/features/integral_image_normal.h>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <QVTKWidget.h>
#include <QFileDialog>

#include <face_detection_apps_utils.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),grabber("#1"), vis_timer(new QTimer(this))
    //faceHeatMap(new pcl::PointCloud<pcl::PointXYZI>)
    //intensity_cloud (new pcl::PointCloud<pcl::PointXYZI>)
{
    ui->setupUi(this);

    if(!grabber.providesCallback<pcl::OpenNIGrabber::sig_cb_openni_point_cloud_rgb>())
    {
        PCL_ERROR("Device #1 not connected!");

    }
      vis_timer->start(5);
      connect(vis_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));

    this->setWindowTitle("3D Face Detection");
    pclVis.reset(new pcl::visualization::PCLVisualizer("", false));
    ui->QVTKwidget->SetRenderWindow(pclVis->getRenderWindow());
    pclVis->setupInteractor(ui->QVTKwidget->GetInteractor(), ui->QVTKwidget->GetRenderWindow());
    pclVis->getInteractorStyle()->setKeyboardModifier(pcl::visualization::INTERACTOR_KB_MOD_SHIFT);
    ui->QVTKwidget->update();

    boost::function<void (const CloudConstPtr&)> f = boost::bind (&MainWindow::cloud_cb, this, _1);
    boost::signals2::connection c = grabber.registerCallback (f);

     grabber.start();

         pclVis->setBackgroundColor(0,0,0);
         pclVis->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3);
         pclVis->addCoordinateSystem(1.0, 3);
         pclVis->initCameraParameters();


    passfilter.setFilterFieldName("z");
    passfilter.setFilterLimits(0.5, 0.5);

    winWidth = 500;
    winHeight = 500;

    STRIDE_SW = 4;
    use_normals = 0;
    trans_max_variance = 1600.f;
    min_votes_size = 300;
    face_threshold = 0.99f;
    heat_map = 1;
    show_votes = 0;
    pose_refinement_ = 0;
    icp_iterations = 5;

    std::string forest_fn = "forest_example.txt";
    std::string model_path_ = "model.pcd";

        palette = new QPalette();


    fdrf.setForestFilename (forest_fn);
    fdrf.setWSize (80);
    fdrf.setUseNormals (static_cast<bool> (use_normals));
    fdrf.setWStride (STRIDE_SW);
    fdrf.setLeavesFaceMaxVariance (trans_max_variance);
    fdrf.setLeavesFaceThreshold (face_threshold);
    fdrf.setFaceMinVotes (min_votes_size);

    if (pose_refinement_)
     {
       fdrf.setPoseRefinement (true, icp_iterations);
       fdrf.setModelPath (model_path_);
     }

   //load forest from file and pass it to the detector
     std::filebuf fb;
     fb.open (forest_fn.c_str (), std::ios::in);
     std::istream os (&fb);


     typedef pcl::face_detection::RFTreeNode<pcl::face_detection::FeatureType> NodeType;
       pcl::DecisionForest<NodeType> forest;
       forest.deserialize (os);
       fb.close ();

       fdrf.setForest (forest);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeOutSlot()
{

        QString noOfHeads = QString::number(detectedFaces);

        palette->setColor(QPalette::WindowText,Qt::red);
        ui->lcdNumber->setPalette(*palette);
        ui->lcdNumber->display(detectedFaces);

       boost::this_thread::sleep (boost::posix_time::milliseconds(1));
       ui->QVTKwidget->update();


}

void MainWindow::cloud_cb(const CloudConstPtr& rcvCloud)
{

    cout<<"\ntest..."<<"\n";
    if(!rcvCloud)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        return;
    }
    cout<<"\ntest1..."<<"\n";
   // QMutexLocker locker(&mtx);
    FPS_CALC("computation");
    faces.clear();

     pcl::PointCloud<pcl::PointXYZ>::Ptr scene (new pcl::PointCloud<pcl::PointXYZ> ());
     pcl::copyPointCloud (*rcvCloud, *scene);
     fdrf.setInputCloud (scene);

     pcl::ScopeTime t ("Detect faces...");
     fdrf.detectFaces ();
  //  fdrf.setFaceHeatMapCloud (intensity_cloud);

    fdrf.getDetectedFaces(faces);
     std::cout<<"\n\nFAces:Row:"<<faces.size()<<"\n";
//     for(int i=0; i< faces.size(); i++)
//     {
//        std:cout<<"\n\ni========="<<i<<"\n";

//         std::cout<<"\n\nFAces:Cols:"<<faces[i][0];
//         std::cout<<"\n\nFAces:Cols:"<<faces[i][1];
//         std::cout<<"\n\nFAces:Cols:"<<faces[i][2];
//         std::cout<<"\n\nFAces:Cols:"<<faces[i][3];
//         std::cout<<"\n\nFAces:Cols:"<<faces[i][4];
//         std::cout<<"\n\nFAces:Cols:"<<faces[i][5]<<"\n";


//     }
    detectedFaces = faces.size();

   // fdrf.getFaceHeatMap(faceHeatMap);

   // pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(rcvCloud);
   // pclVis->addPointCloud(rcvCloud, "sample 3dcloud");



//     if(pclVis->updateColorHandlerIndex(faceHeatMap))
//     {
//          pclVis->addPointCloudIntensityGradients(faceHeatMap);
//     }
    face_detection_apps_utils::displayHeads (faces, *pclVis);
//    pcl::visualization::PointCloudColorHandlerGenericField < pcl::PointXYZI > handler_keypoints (faceHeatMap, "intensity");
//    pclVis->addPointCloud (faceHeatMap, handler_keypoints, "heatMap");

       if(!pclVis->updatePointCloud(rcvCloud, "cloud_pass"))
      {

           pclVis->addPointCloud (rcvCloud,"cloud_pass");
          // pclVis->resetCameraViewpoint("cloud_pass");
       }

      FPS_CALC ("visualization");
      ui->QVTKwidget->update();

}

void MainWindow::on_pushButton_clicked()
{

}
