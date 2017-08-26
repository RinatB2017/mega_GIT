//--------------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------------
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
//--------------------------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//--------------------------------------------------------------------------------
// Open image function call
void MainWindow::openImage()
{
    // declare FileOpName as the choosen opened file name
    FileOpName = QFileDialog::getOpenFileName(this,
                                              tr("Open File"),
                                              QDir::currentPath(),tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    // Check if FileOpName exist or not
    if (!FileOpName.isEmpty())
    {
        QImage image(FileOpName);
        if (image.isNull())
        {
            QMessageBox::information(this,
                                     tr("Face Recognition"),
                                     tr("Cannot load %1.").arg(FileOpName));

            return;
        }

    }
    // function to load the image whenever fName is not empty
    if( FileOpName.size() )
    {
        imagerd = cvLoadImage(FileOpName.toLocal8Bit().data());
        QImage imageView = QImage((const unsigned char*)(imagerd->imageData), imagerd->width,imagerd->height,QImage::Format_RGB888).rgbSwapped();
        ui->label->setPixmap(QPixmap::fromImage(imageView));
    }
}
//--------------------------------------------------------------------------------
QRect MainWindow::searchObjectByTemplate(QString srcImgName, QString templImgName)
{
    QRect resultRect = QRect(-1, -1, -1, -1);
    QRect CCORR_result;
    QRect SQDIFF_result;
    QPair <QRect, QRect> resultPair;

    resultPair.first = QRect(0, 0, 0, 0);
    resultPair.second = QRect(1, 1, 0, 0);

    double min,max;
    CvPoint minpos, maxpos;

    char* imgName =  srcImgName.toLocal8Bit().data();
    char* templName = templImgName.toLocal8Bit().data();

    IplImage *src = NULL;
    IplImage *templ = NULL;
    IplImage *result = NULL;

    src = cvLoadImage(imgName, CV_LOAD_IMAGE_GRAYSCALE);

    templ = cvLoadImage(templName, CV_LOAD_IMAGE_GRAYSCALE);

    if(!src || !templ)
    {
        qDebug("LOADING ERROR!!!");
        return resultRect;
    }

    result = cvCreateImage(cvSize( src->width - templ->width + 1,src->height - templ->height + 1),32,1);

    cvMatchTemplate( src, templ, result, CV_TM_CCORR_NORMED);
    cvNormalize( result, result, 1, 0, CV_MINMAX );
    cvMinMaxLoc(result,&min,&max,&minpos,&maxpos);

    CCORR_result = QRect(maxpos.x, maxpos.y, templ->width, templ->height);

    cvMatchTemplate( src, templ, result, CV_TM_SQDIFF_NORMED);
    cvNormalize( result, result, 1, 0, CV_MINMAX );
    cvMinMaxLoc(result,&min,&max,&minpos,&maxpos);

    SQDIFF_result = QRect(minpos.x, minpos.y, templ->width, templ->height);

    resultPair.first = CCORR_result;
    resultPair.second = SQDIFF_result;

    if(resultPair.first.x() == resultPair.second.x() &&
            resultPair.first.x() == resultPair.second.x())
    {
        resultRect = resultPair.first;
    }

    cvReleaseImage(&src);
    cvReleaseImage(&templ);
    cvReleaseImage(&result);

    return resultRect;
}
//--------------------------------------------------------------------------------
void MainWindow::on_pushButton_clicked()
{
    openImage();
}
//--------------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
