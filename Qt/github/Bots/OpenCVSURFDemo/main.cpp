#include <QApplication>
#include <QFileDialog>
#include <QDebug>

#include </usr/include/opencv2/core.hpp>
#include </usr/include/opencv2/highgui.hpp>
#include </usr/include/opencv2/imgproc.hpp>
#include </usr/include/opencv2/calib3d.hpp>
#include </usr/include/opencv2/xfeatures2d.hpp>

using namespace cv;

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

#if 0
    static const QString IMAGE_FILES_TMPL = "Images (*.png *.jpg *.jpeg)";

    QString objFileName = QFileDialog::getOpenFileName( 0, "Object image", ".", IMAGE_FILES_TMPL );
    if( objFileName.isEmpty() )
    {
        return -1;
    }

    QString sceneFileName = QFileDialog::getOpenFileName( 0, "Scene image", ".", IMAGE_FILES_TMPL );
    if( sceneFileName.isEmpty() )
    {
        return -2;
    }
#else
    QString sceneFileName = "pic/test_picture.png";
    QString objFileName = "pic/stop.png";
#endif

    Mat objImage = imread( objFileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE );
    Mat sceneImage = imread( sceneFileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE );

    if( !objImage.data || !sceneImage.data )
    {
        qDebug() << "error data";
        return -3;
    }

    // Шаг 1: Найдем ключевые точки с помощью SURF-детектора
    static const int MIN_HESSIAN = 800;

    Ptr< xfeatures2d::SurfFeatureDetector > detector = xfeatures2d::SURF::create( MIN_HESSIAN );

    std::vector< KeyPoint > objKeypoints, sceneKeypoints;

    detector->detect( objImage, objKeypoints );
    detector->detect( sceneImage, sceneKeypoints );

    // Шаг 2: Высчитаем описатели или дескрипторы (векторы характерстик)
    Ptr< xfeatures2d::SurfDescriptorExtractor > extractor = xfeatures2d::SurfDescriptorExtractor::create();

    Mat objDescriptors, sceneDescriptors;

    extractor->compute( objImage, objKeypoints, objDescriptors );
    extractor->compute( sceneImage, sceneKeypoints, sceneDescriptors );

    // Шаг 3: Сопоставим векторы дескрипторов с помощью FLANN
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( objDescriptors, sceneDescriptors, matches );

    double max_dist = 0;
    double min_dist = 100;

    // Найдем максимальное и минимальное расстояние между ключевыми точками
    for( int i = 0; i < objDescriptors.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist )
        {
            min_dist = dist;
        }
        if( dist > max_dist )
        {
            max_dist = dist;
        }
    }

    // Нарисуем только "хорошие" совпадения (т.е. те, для которых расстояние меньше, чем 3*min_dist)
    std::vector< DMatch > goodMatches;

    for( int i = 0; i < objDescriptors.rows; i++ )
    {
        if( matches[ i ].distance < 3 * min_dist )
        {
            goodMatches.push_back( matches[ i ] );
        }
    }

    Mat imgMatches;
    drawMatches( objImage, objKeypoints, sceneImage, sceneKeypoints,
                 goodMatches, imgMatches, Scalar::all( -1 ), Scalar::all( -1 ),
                 std::vector< char >(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    // Найдем объект на сцене
    std::vector< Point2f > obj;
    std::vector< Point2f > scene;

    for( size_t i = 0; i < goodMatches.size(); i++ )
    {
        // Отбираем ключевые точки из хороших совпадений
        obj.push_back( objKeypoints[ goodMatches[ i ].queryIdx ].pt );
        scene.push_back( sceneKeypoints[ goodMatches[ i ].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, CV_RANSAC );

    // Занесем в вектор углы искомого объекта
    std::vector< Point2f > objCorners( 4 );
    objCorners[ 0 ] = cvPoint( 0, 0 );
    objCorners[ 1 ] = cvPoint( objImage.cols, 0 );
    objCorners[ 2 ] = cvPoint( objImage.cols, objImage.rows );
    objCorners[ 3 ] = cvPoint( 0, objImage.rows );

    std::vector< Point2f > sceneCorners( 4 );
    perspectiveTransform( objCorners, sceneCorners, H );

    // Нарисуем линии между углами (отображение искоромого объекта на сцене)
    line( imgMatches, sceneCorners[ 0 ] + Point2f( objImage.cols, 0 ), sceneCorners[ 1 ] + Point2f( objImage.cols, 0 ), Scalar( 0, 255, 0 ), 4 );
    line( imgMatches, sceneCorners[ 1 ] + Point2f( objImage.cols, 0 ), sceneCorners[ 2 ] + Point2f( objImage.cols, 0 ), Scalar( 0, 255, 0 ), 4 );
    line( imgMatches, sceneCorners[ 2 ] + Point2f( objImage.cols, 0 ), sceneCorners[ 3 ] + Point2f( objImage.cols, 0 ), Scalar( 0, 255, 0 ), 4 );
    line( imgMatches, sceneCorners[ 3 ] + Point2f( objImage.cols, 0 ), sceneCorners[ 0 ] + Point2f( objImage.cols, 0 ), Scalar( 0, 255, 0 ), 4 );

    // Покажем найденные совпадения
    imshow( "Good Matches & Object detection", imgMatches );

    waitKey( 0 );

    return 0;
}
