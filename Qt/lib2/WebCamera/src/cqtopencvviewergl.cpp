
#include "cqtopencvviewergl.h"
//--------------------------------------------------------------------------------
CQtOpenCVViewerGl::CQtOpenCVViewerGl(QWidget *parent) :
    QGLWidget(parent)
{
    mSceneChanged = false;
    mBgColor = QColor::fromRgb(150, 150, 150);

    mOutH = 0;
    mOutW = 0;
    mImgRatio = 4.0f/3.0f;

    mPosX = 0;
    mPosY = 0;
}
//--------------------------------------------------------------------------------
void CQtOpenCVViewerGl::initializeGL(void)
{
    makeCurrent();
    qglClearColor(mBgColor.darker());
}
//--------------------------------------------------------------------------------
void CQtOpenCVViewerGl::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, 0, height, 0, 1);	// To Draw image in the center of the area

    glMatrixMode(GL_MODELVIEW);

    // ---> Scaled Image Sizes
    mOutH = static_cast<int>(width/mImgRatio);
    mOutW = width;

    if(mOutH>height)
    {
        mOutW = static_cast<int>(height*mImgRatio);
        mOutH = height;
    }

    emit imageSizeChanged(mOutW, mOutH);
    // <--- Scaled Image Sizes

    mPosX = (width-mOutW)/2;
    mPosY = (height-mOutH)/2;

    mSceneChanged = true;

    updateScene();
}
//--------------------------------------------------------------------------------
void CQtOpenCVViewerGl::updateScene(void)
{
    if(mSceneChanged && this->isVisible())
    {
        updateGL();
    }
}
//--------------------------------------------------------------------------------
void CQtOpenCVViewerGl::paintGL(void)
{
    makeCurrent();

    if(!mSceneChanged)
    {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();

    mSceneChanged = false;
}
//--------------------------------------------------------------------------------
void CQtOpenCVViewerGl::renderImage(void)
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if(!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        QImage image; // the image rendered

        glPushMatrix();
        int imW = mRenderQtImg.width();
        int imH = mRenderQtImg.height();

        // The image is to be resized to fit the widget?
        if(imW != this->size().width() && imH != this->size().height())
        {
            image = mRenderQtImg.scaled(QSize(mOutW,mOutH),
                                        Qt::IgnoreAspectRatio,
                                        Qt::FastTransformation);

            //qDebug( QString( "Image size: (%1x%2)").arg(imW).arg(imH).toAscii() );
        }
        else
        {
            image = mRenderQtImg;
        }

        // ---> Centering image in draw area

        glRasterPos2i(mPosX, mPosY);
        // <--- Centering image in draw area

        imW = image.width();
        imH = image.height();

        glDrawPixels(imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        glPopMatrix();

        // end
        glFlush();
    }
}
//--------------------------------------------------------------------------------
bool CQtOpenCVViewerGl::showImage(Mat image)
{
    image.copyTo(mOrigImage);

    mImgRatio = static_cast<float>(image.cols)/static_cast<float>(image.rows);

    if(mOrigImage.channels() == 3)
    {
        mRenderQtImg = QImage(static_cast<const unsigned char*>(mOrigImage.data),
                              mOrigImage.cols,
                              mOrigImage.rows,
                              static_cast<int>(mOrigImage.step),
                              QImage::Format_RGB888).rgbSwapped();
    }
    else if( mOrigImage.channels() == 1)
    {
        mRenderQtImg = QImage(static_cast<const unsigned char*>(mOrigImage.data),
                              mOrigImage.cols,
                              mOrigImage.rows,
                              static_cast<int>(mOrigImage.step),
                              QImage::Format_Indexed8);
    }
    else
    {
        return false;
    }

    mRenderQtImg = QGLWidget::convertToGLFormat(mRenderQtImg);

    mSceneChanged = true;

    updateScene();

    return true;
}
//--------------------------------------------------------------------------------
