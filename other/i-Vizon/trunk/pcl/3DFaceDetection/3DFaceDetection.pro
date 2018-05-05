#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T18:04:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DFaceDetection
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        openni_frame_source.cpp

HEADERS  += mainwindow.h \
         openni_frame_source.h\
        face_detection_apps_utils.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/pcl-1.7\
                /opt/pcl-trunk/ml/include\
                /usr/local/include\
                /usr/include/ni\
                /usr/include/eigen3\
                /usr/include/vtk-5.8\
                /usr/include/openni\
                /usr/include/qhull\
                /usr/include/eigen3\
                /usr/include/eigen2\
                /usr/include/eigen3/Eigen/src/Core\
                /usr/include/eigen3/Eigen/src/Cholesky\
                /usr/include/eigen3/Eigen/src/Eigen2Support\
                /usr/include/eigen3/Eigen/src/Geometry\
                /usr/include/eigen3/Eigen/src/Jacobi\
                /usr/include/eigen3/Eigen/src/misc\
                /usr/include/eigen3/Eigen/src/QR\
                /usr/include/eigen3/Eigen/src/StlSupport\
                /usr/include/eigen3/Eigen/src/Eigenvalues\
                /usr/include/eigen3/Eigen/src/Householder\
                /usr/include/eigen3/Eigen/src/LU\
                /usr/include/eigen3/Eigen/src/plugins\
                /usr/include/eigen3/Eigen/src/Sparse\
                /usr/include/vtk-5.8/


QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-deprecated-writable-strings -Wno-deprecated -Wno-write-strings

QMAKE_CXXFLAGS_DEBUG += -Wno-unused-variable -Wno-deprecated-writable-strings -Wno-deprecated -Wno-write-strings
QMAKE_CXXFLAGS_RELEASE += -Wno-unused-variable -Wno-deprecated-writable-strings -Wno-deprecated -Wno-write-strings

#QMAKE_MOC = $$QMAKE_MOC -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED

#QMAKE_CXXFLAGS += -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED


LIBS += \
        -L/usr/lib\
        -L/usr/lib/x86_64-linux-gnu\
        -L/usr/local/lib\
        -lpcl_2d\
        #-lpcl_geometry\
        -lpcl_ml\
        -lpcl_sample_consensus\
        -lpcl_tracking\
        -lpcl_apps\
        -lpcl_io_ply\
        -lpcl_octree\
        -lpcl_search\
        -lpcl_visualization\
        -lpcl_common\
        -lpcl_io\
        -lpcl_outofcore\
        -lpcl_segmentation\
        -lpcl_features\
        -lpcl_kdtree\
        -lpcl_recognition\
        -lpcl_stereo\
        -lpcl_filters\
        -lpcl_keypoints\
        -lpcl_registration\
        -lpcl_surface\
        -lflann_cpp_s\
        -lusb-1.0\
        -lOpenNI\
        -lqhull\
        -lQVTK\
        -lvtkalglib\
        -lvtkftgl\
        -lvtkInfovis\
        -lvtkRendering\
        -lvtkCharts\
        -lvtkGenericFiltering\
        -lvtkIO\
        -lvtksys\
        -lvtkCommon\
        -lvtkGeovis\
        -lvtkmetaio\
        -lvtkverdict\
        -lvtkDICOMParser\
        -lvtkGraphics\
        -lvtkParallel\
        -lvtkViews\
        -lvtkexoIIc\
        -lvtkHybrid\
        -lvtkproj4\
        -lvtkVolumeRendering\
        -lvtkFiltering\
        -lvtkImaging\
        -lvtkQtChart\
        -lvtkWidgets\
        -lrt \
        -ldl\
        -lboost_system\
        -lboost_filesystem\
        -lboost_thread\
        -lboost_date_time\
        -lboost_iostreams\
        -lboost_date_time\
        -lpthread \
         -pthread

#Check boost version
#dpkg -s libboost-dev | grep 'Version'
#dpkg -S /usr/include/boost/version.hpp
#sudo apt-get autoremove package
#sudo apt-get update
#sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev
#wget -O boost_1_55_0.tar.gz http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.gz/download
#tar xzvf boost_1_55_0.tar.gz
#./bootstrap.sh --prefix=/usr/local
#user_configFile=`find $PWD -name user-config.jam`
#using mpi ;
#sudo ./b2 --with=all -j4 install
