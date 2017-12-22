#ifndef GL_DRAW_WIDGET_H
#define GL_DRAW_WIDGET_H


#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QtOpenGL>
#include <glu.h>

#include <QTimer>
#include <QObject>

typedef struct
{
    GLubyte *data;
    GLuint bpp;
    GLuint width;
    GLuint height;
    GLuint texID;
    GLuint type;

} GL_TEXTURE_DEF;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
   GLWidget(QWidget *parent = 0);
   ~GLWidget();
//protected:
   void initializeGL();
   void resizeGL(int width, int height);
   void paintGL();

   void timeOut();
   void UpdateTextureData(unsigned char *ptr);

signals:

protected slots:

  // void timeOutSlot();

public slots:

private:
   QTimer *m_Timer;
   GL_TEXTURE_DEF *m_Texture;
   GLuint NES_texture;
   GLuint m_ActiveTexture;


};

#endif // GL_DRAW_WIDGET_H
