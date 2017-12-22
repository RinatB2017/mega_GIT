#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webget.h"

#include <QtCore>
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <ctime>
#include <sstream>
#include <string>
#include <iostream>

#include </usr/include/opencv2/core/core.hpp>
#include </usr/include/opencv2/objdetect/objdetect.hpp>
#include </usr/include/opencv2/imgproc/imgproc.hpp>
#include </usr/include/opencv2/highgui/highgui.hpp>
#include </usr/include/opencv2/opencv.hpp>
#include </usr/include/opencv2/face.hpp>

//#include </usr/include/opencv2/imgproc/imgproc.hpp>
//#include </usr/include/opencv2/highgui/highgui.hpp>
//#include <opencv2/contrib/contrib.hpp>
//#include </usr/include/opencv2/objdetect/objdetect.hpp>

/** Global variables **/
std::string faceCascadeName = ":/cascades/lbpcascade_frontalface.xml";
std::string eyesCascadeName = ":/cascades/haarcascade_eye_tree_eyeglasses.xml";
cv::CascadeClassifier faceCascade;
cv::CascadeClassifier eyesCascade;
std::string face_file = (QDir::homePath() + QString("/.uWho/face.xml")).toUtf8().constData();
QFile face1(QDir::homePath() + QString("/.uWho/face.xml") );
QDir uWhoconfig( QDir::homePath() + QString("/.uWho/") );
double lbphThreshold = 60.0 ;
/** end of global variables **/

using namespace std;
using namespace cv;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap webcam(":/icons/webcam.png");
    QPixmap videofile(":/icons/videofile.png");
    QPixmap dirpicfile(":/icons/dirpics.png");
    QPixmap ipfile(":/icons/internet-cloud-icon.jpg");
    ui->webcamButton->setIcon(webcam);
    ui->videofileButton->setIcon(videofile);
    ui->dirpicButton->setIcon(dirpicfile);
    ui->ipButton->setIcon(ipfile);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_videofileButton_clicked()
{
    // Below is the open file dialog, and saves said file selected to videoFileNames
    QFileDialog videoFile(this);
    videoFile.setFileMode(QFileDialog::AnyFile);
    videoFile.setNameFilter(tr("Images (*.avi *.mpg *.mpeg *.mkv *.webm *.ogv *.mp4)"));
    videoFile.setViewMode(QFileDialog::List);
    QStringList vFN;
    if (videoFile.exec()){
        vFN = videoFile.selectedFiles();
    }

    // Check for valid file name. If you cancel, it returns instead of crashes
    QString videoFileName;
    if(vFN.size() != 0){
        videoFileName = vFN.at(0);
    }else{
        return;
    }

    // Attempt to get total frames in video. This may not be accurate, according to OpenCV devs
    qDebug() << ( videoFileName.toUtf8().constData() ) ;
    cv::VideoCapture cap = cv::VideoCapture(( videoFileName.toUtf8().constData() ));
    qDebug() << "Video loaded" ;
    long int videoFrameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);


    // Starts the Classifier, and then tests said classifier with my default data
    std::srand(std::time(NULL));
    Ptr<cv::face::FaceRecognizer> model = cv::face::createLBPHFaceRecognizer(1,8,8,8, lbphThreshold);
    if (face1.exists()){
        model->load(face_file);
        qDebug() << "Loaded model." ;
    }else{
        qDebug() << "Generating starting model..." ;
        vector<cv::Mat> images (10);
        vector<int> labels (10);
        images[0] = (imread(":/startingfaces/josh1.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[0] = 0;
        images[1] = (imread(":/startingfaces/josh2.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[1] = 0;
        images[2] = (imread(":/startingfaces/josh3.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[2] = 0;
        images[3] = (imread(":/startingfaces/josh4.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[3] = 0;
        images[4] = (imread(":/startingfaces/josh5.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[4] = 0;
        images[5] = (imread(":/startingfaces/josh6.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[5] = 0;
        images[6] = (imread(":/startingfaces/josh7.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[6] = 0;
        images[7] = (imread(":/startingfaces/josh8.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[7] = 0;
        images[8] = (imread(":/startingfaces/josh9.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[8] = 0;
        images[9] = (imread(":/startingfaces/josh10.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[9] = 0;
        model->train(images, labels);

        cv::Mat testingImage = (imread(":/startingfaces/josh11.png", CV_LOAD_IMAGE_GRAYSCALE));
        int predicted = -1;  // Sanity check. We throw a face I know is mine to the predictor.
        double confidence ;
        model->predict(testingImage, predicted, confidence);
        qDebug() << "Testing predicted/confidence: " << predicted << confidence ;
    }


    cv::namedWindow("VidWindow");
    cv::Mat frame;
    unsigned long frameNumber = 0;
    do{
        cap.read(frame);
        if(!frame.empty()){
            frameNumber = frameNumber + 1 ;
            std::ostringstream a;
            a << frameNumber ;
            cv::putText(frame, a.str(), cv::Point(0,20),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);
            cv::blur(frame, frame, cv::Size(3,3));
            faceCascade.load(faceCascadeName);
            eyesCascade.load(eyesCascadeName);
            std::vector<cv::Rect> faces;
            cv::Mat frame_gray;
            cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
            faceCascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_SCALE_IMAGE, cv::Size(50,50));
            for(unsigned int i = 0; i < faces.size(); i++){
                std::vector<cv::Mat> facePicture (1);
                std::vector<int> faceIndex (1);
                std::vector<Rect> eyes;
                facePicture[0] = frame(faces[i]);   // Gets the face only as the variable facePicture

                cv::rectangle(frame, faces[i], cv::Scalar(255,0,255), 1, 8, 0); // Draws rectangles on webcam video
                string faceString = static_cast<ostringstream*>( &(ostringstream() << i) )->str();
                cv::putText(frame, faceString, cv::Point(faces[i].x, (faces[i].y+10)),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);

                eyesCascade.detectMultiScale( facePicture[0], eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
                if (eyes.size() != 0){
                    cv::cvtColor(facePicture[0], facePicture[0], CV_BGR2GRAY); // colorspace change to gray
                    int predicted = -1;
                    double confidence ;
                    model->predict(facePicture[0], predicted, confidence); // Check the machine learner and ask if it's seen this face before
                    string predictString = static_cast<ostringstream*>( &(ostringstream() << predicted) )->str();
                    cv::putText(frame, predictString, cv::Point((faces[i].x + faces[i].width - 40), (faces[i].y + 10)),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);
                    if (predicted == -1){
                        faceIndex[0] = std::rand()%30000;
                        model->update(facePicture,faceIndex); // If its not in the FaceRecognizer, add it
                    }else{
                        faceIndex[0] = predicted;
                        model->update(facePicture,faceIndex);  // if the face is already in, add this as another data point
                    }
                    qDebug() << "face ID#" << predicted << "confidence :"<< confidence;
                }
            }
        }
        std::ostringstream b;
        b << videoFrameCount ;
        cv::putText(frame, b.str(), cv::Point((cap.get(CV_CAP_PROP_FRAME_WIDTH)-61),20),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);
        imshow("VidWindow" ,frame);
    }while((cv::waitKey(1)<30) || frame.empty() );
    qDebug() << (QDir::homePath() + QString("/.uWho/face.xml")) ;
    model->save(face_file);
    cv::destroyWindow("VidWindow");
}


//****************************************************************


void MainWindow::on_webcamButton_clicked()
{
    std::srand(std::time(NULL));
    Ptr<cv::face::FaceRecognizer> model = cv::face::createLBPHFaceRecognizer(1,8,8,8, lbphThreshold);
    if (face1.exists()){
        model->load(face_file);
        qDebug() << "Loaded model." ;
    }else{
        qDebug() << "Generating starting model..." ;
        vector<cv::Mat> images (10);
        vector<int> labels (10);
        images[0] = (imread(":/startingfaces/josh1.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[0] = 0;
        images[1] = (imread(":/startingfaces/josh2.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[1] = 0;
        images[2] = (imread(":/startingfaces/josh3.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[2] = 0;
        images[3] = (imread(":/startingfaces/josh4.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[3] = 0;
        images[4] = (imread(":/startingfaces/josh5.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[4] = 0;
        images[5] = (imread(":/startingfaces/josh6.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[5] = 0;
        images[6] = (imread(":/startingfaces/josh7.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[6] = 0;
        images[7] = (imread(":/startingfaces/josh8.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[7] = 0;
        images[8] = (imread(":/startingfaces/josh9.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[8] = 0;
        images[9] = (imread(":/startingfaces/josh10.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[9] = 0;
        model->train(images, labels);
        qDebug() << "Training successful";

        cv::Mat testingImage = (imread(":/startingfaces/josh11.png", CV_LOAD_IMAGE_GRAYSCALE));
        int predicted = -1;  // Sanity check. We throw a face I know is mine to the predictor.
        double confidence ;
        model->predict(testingImage, predicted, confidence);
        qDebug() << "Testing predicted/confidence: " << predicted << confidence ;
    }

    cv::namedWindow("VidWindow");
    cv::VideoCapture cap = cv::VideoCapture(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 800 );
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 600 );
    cv::Mat frame;
    do{
        cap >> frame;
        if(!frame.empty()){
            cv::blur(frame, frame, cv::Size(3,3));
            faceCascade.load(faceCascadeName);
            eyesCascade.load(eyesCascadeName);
            std::vector<cv::Rect> faces;
            cv::Mat frame_gray;
            cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
            faceCascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_SCALE_IMAGE, cv::Size(50,50));
            for(unsigned int i = 0; i < faces.size(); i++){
                std::vector<cv::Mat> facePicture (1);
                std::vector<int> faceIndex (1);
                std::vector<Rect> eyes;
                facePicture[0] = frame(faces[i]);   // Gets the face only as the variable facePicture

                cv::rectangle(frame, faces[i], cv::Scalar(255,0,255), 1, 8, 0); // Draws rectangles on webcam video
                string faceString = static_cast<ostringstream*>( &(ostringstream() << i) )->str();
                cv::putText(frame, faceString, cv::Point(faces[i].x, (faces[i].y+30)),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);

                eyesCascade.detectMultiScale( facePicture[0], eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
                if (eyes.size() != 0){
                    cv::cvtColor(facePicture[0], facePicture[0], CV_BGR2GRAY); // colorspace change to gray
                    int predicted = -1;
                    double confidence ;
                    model->predict(facePicture[0], predicted, confidence); // Check the machine learner and ask if it's seen this face before
                    string predictString = static_cast<ostringstream*>( &(ostringstream() << predicted) )->str();
                    cv::putText(frame, predictString, cv::Point((faces[i].x + faces[i].width - 40), (faces[i].y + 30)),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);
                    if (predicted == -1){
                        faceIndex[0] = std::rand()%30000;
                        model->update(facePicture,faceIndex); // If its not in the FaceRecognizer, add it
                    }else{
                        faceIndex[0] = predicted;
                        model->update(facePicture,faceIndex);  // if the face is already in, add this as another data point
                    }
                    qDebug() << "face # " << predicted << confidence;
                }
            }


            imshow("VidWindow" ,frame);}
    }while(cv::waitKey(30)<30);
    qDebug() << (QDir::homePath() + QString("/.uWho/face.xml")) ;
    model->save(face_file);
    cv::destroyWindow("VidWindow");
}




//***********************************************************************

void MainWindow::on_dirpicButton_clicked()
{
    std::srand(std::time(NULL));
    Ptr<cv::face::FaceRecognizer> model = cv::face::createLBPHFaceRecognizer(1,8,8,8, lbphThreshold);
    if (face1.exists()){
        model->load(face_file);
        qDebug() << "Loaded model." ;
    }else{
        qDebug() << "Generating starting model..." ;
        vector<cv::Mat> images (10);
        vector<int> labels (10);
        images[0] = (imread(":/startingfaces/josh1.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[0] = 0;
        images[1] = (imread(":/startingfaces/josh2.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[1] = 0;
        images[2] = (imread(":/startingfaces/josh3.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[2] = 0;
        images[3] = (imread(":/startingfaces/josh4.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[3] = 0;
        images[4] = (imread(":/startingfaces/josh5.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[4] = 0;
        images[5] = (imread(":/startingfaces/josh6.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[5] = 0;
        images[6] = (imread(":/startingfaces/josh7.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[6] = 0;
        images[7] = (imread(":/startingfaces/josh8.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[7] = 0;
        images[8] = (imread(":/startingfaces/josh9.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[8] = 0;
        images[9] = (imread(":/startingfaces/josh10.png", CV_LOAD_IMAGE_GRAYSCALE));
        labels[9] = 0;
        model->train(images, labels);

        cv::Mat testingImage = (imread(":/startingfaces/josh11.png", CV_LOAD_IMAGE_GRAYSCALE));
        int predicted = -1;  // Sanity check. We throw a face I know is mine to the predictor.
        double confidence ;
        model->predict(testingImage, predicted, confidence);
        qDebug() << "Testing predicted/confidence: " << predicted << confidence ;
    }

    QFileDialog folder(this);
    folder.setFileMode(QFileDialog::DirectoryOnly);
    folder.setViewMode(QFileDialog::List);
    QStringList fileNames;
    if (folder.exec()){
        fileNames = folder.selectedFiles();
    }

    if(fileNames.count() < 1){
        return;
    }

    cv::Mat frame;

    QDir dir(fileNames.at(0));
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    qDebug() << "listing directory contents";
    for (int i = 0; i < list.size(); i++){
        qDebug() << (list.at(i)).absoluteFilePath();
    }
    qDebug() << "done listing dir contents";

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        cv::VideoCapture cap = cv::VideoCapture( fileInfo.absoluteFilePath().toUtf8().constData() );
        qDebug() << "Processing" << fileInfo.absoluteFilePath() ;
        cap >> frame;
        cv::blur(frame, frame, cv::Size(3,3));
        faceCascade.load(faceCascadeName);
        eyesCascade.load(eyesCascadeName);
        std::vector<cv::Rect> faces;
        cv::Mat frame_gray;
        cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);

        faceCascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_SCALE_IMAGE, cv::Size(50,50));
        for(unsigned int j = 0; j < faces.size(); j++){
            std::vector<cv::Mat> facePicture (1);
            std::vector<int> faceIndex (1);
            std::vector<Rect> eyes;
            facePicture[0] = frame(faces[j]);   // Gets the face only as the variable facePicture

            cv::rectangle(frame, faces[j], cv::Scalar(255,0,255), 1, 8, 0); // Draws rectangles on webcam video
            string faceString = static_cast<ostringstream*>( &(ostringstream() << j) )->str();
            cv::putText(frame, faceString, cv::Point(faces[j].x, (faces[j].y+30)),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);

            eyesCascade.detectMultiScale( facePicture[0], eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

            if (eyes.size() != 0){
                cv::cvtColor(facePicture[0], facePicture[0], CV_BGR2GRAY); // colorspace change to gray
                int predicted = -1;
                double confidence ;
                model->predict(facePicture[0], predicted, confidence); // Check the machine learner and ask if it's seen this face before
                string predictString = static_cast<ostringstream*>( &(ostringstream() << predicted) )->str();
                cv::putText(frame, predictString, cv::Point((faces[j].x + faces[j].width - 40), (faces[j].y + 30)),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1,8, false);
                if (predicted == -1){
                    faceIndex[0] = std::rand()%30000;
                    model->update(facePicture,faceIndex); // If its not in the FaceRecognizer, add it
                }else{
                    faceIndex[0] = predicted;
                    model->update(facePicture,faceIndex);  // if the face is already in, add this as another data point
                }
                qDebug() << predicted << confidence;
            }
        }
    }
    qDebug() << "Done processing files";
    qDebug() << (QDir::homePath() + QString("/.uWho/face.xml")) ;
    model->save(face_file);
    cv::destroyWindow("VidWindow");
}

void MainWindow::on_ipButton_clicked()
{
    webget webgetwindow;
    webgetwindow.setModal(true);
    webgetwindow.exec();
}
