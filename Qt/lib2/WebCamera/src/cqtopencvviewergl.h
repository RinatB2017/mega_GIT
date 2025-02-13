#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <QGLWidget>

#ifdef Q_OS_LINUX
#   include <opencv2/core/core.hpp>
#else
#   include <opencv2/core/core.hpp>
#endif

using namespace cv;

class CQtOpenCVViewerGl : public QGLWidget
{
    Q_OBJECT

public:
    explicit CQtOpenCVViewerGl(QWidget *parent = nullptr);

signals:
    void imageSizeChanged(int outW, int outH); /// Used to resize the image outside the widget

public slots:
    bool showImage(Mat image); /// Used to set the image to be viewed

protected:
    void initializeGL(void);                // OpenGL initialization
    void paintGL();                         // OpenGL Rendering
    void resizeGL(int width, int height);   // Widget Resize Event

    void updateScene(void);
    void renderImage();

private:
    bool mSceneChanged;     // Indicates when OpenGL view is to be redrawn

    QImage  mRenderQtImg;   // Qt image to be rendered
    Mat mOrigImage;     // original OpenCV image to be shown

    QColor mBgColor;		// Background color

    int   mOutH;            // Resized Image height
    int   mOutW;            // Resized Image width
    float mImgRatio;        // height/width ratio

    int   mPosX;            // Top left X position to render image in the center of widget
    int   mPosY;            // Top left Y position to render image in the center of widget
};

#endif // CQTOPENCVVIEWERGL_H
