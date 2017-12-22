#include "gl_draw_widget.h"
#include <QImage>
#include <QMessageBox>


GLWidget::GLWidget(QWidget *parent)
{

}

GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{




        //let's load up the famique texture
        QImage LoadStuff;

        if(LoadStuff.load("/Users/andro/Desktop/pess/famique/famique-gl-build-desktop/famique-logo.png") == false) return;

        this->m_Texture = new GL_TEXTURE_DEF;
        this->m_Texture->data = new GLubyte[LoadStuff.byteCount()];

        m_Texture->width = LoadStuff.width();
        m_Texture->height = LoadStuff.height();
        m_Texture->bpp = 32;
        m_Texture->texID = 0;
        memcpy(m_Texture->data, LoadStuff.bits(), LoadStuff.byteCount());



        GLuint text_temp;

        glEnable(GL_TEXTURE_2D);

        glGenTextures(1, &text_temp);
        glBindTexture(GL_TEXTURE_2D, text_temp);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, m_Texture->width, m_Texture->height, GL_BGRA, GL_UNSIGNED_BYTE, m_Texture->data);

       // glTexImage2D()
       // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Texture->data);


        /*generate our PPU-output area in video memory */
        glGenTextures(2, &this->NES_texture);
        glBindTexture(GL_TEXTURE_2D, NES_texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_Texture->data);




        m_ActiveTexture = text_temp;



}

void GLWidget::resizeGL(int width, int height)
{
   // int side = qMin(width, height);
     // glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0,0, width, height);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      glOrtho(0, width, height, 0,0,1);

      glMatrixMode(GL_MODELVIEW);

}

void GLWidget::paintGL()
{


    glDisable(GL_DEPTH_TEST);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_ActiveTexture);

    glLoadIdentity();

   // glRotatef(0.01f, 0.0f, 0.0f,1.0f);
    glBegin(GL_QUADS);

    glTexCoord2d(0,0);  glVertex2d(0,0);
    glTexCoord2d(1,0); glVertex2d(width(), 0);

    glTexCoord2d(1,1); glVertex2d(width(), height());
    glTexCoord2d(0,1); glVertex2d(0, height());


    glEnd();



    repaint();

}

void GLWidget::timeOut()
{

}

void GLWidget::UpdateTextureData(unsigned char *ptr)
{
  //  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 256, 240, GL_BGR, GL_UNSIGNED_BYTE, ptr);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, NES_texture);
    m_ActiveTexture = NES_texture;

  //   gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 256, 256, GL_BGRA, GL_UNSIGNED_BYTE, ptr);

//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, ptr);
    glTexSubImage2D(GL_TEXTURE_2D, 0,0,0, 256, 256, GL_BGRA,GL_UNSIGNED_BYTE, ptr);
}
