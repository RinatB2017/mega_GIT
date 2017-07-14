
#include <QColorDialog>
#include <QGLWidget>

#include "cube.h"

Cube::Cube(QWidget *parent)
    : QGLWidget(parent)
{ 
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    rotationX = 0;
    rotationY = 0;
    rotationZ = 0;
    faceColors[0] = Qt::red;
    faceColors[1] = Qt::green;
    faceColors[2] = Qt::blue;
    faceColors[3] = Qt::cyan;
    faceColors[4] = Qt::yellow;
    faceColors[5] = Qt::magenta;
}

void Cube::initializeGL() 
{ 
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Cube::resizeGL(int width, int height) 
{ 
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat)width / height;
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void Cube::paintGL() 
{ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void Cube::draw() 
{ 
    static const GLfloat coords[6][4][3] = {
        { { +1.0, -1.0, +1.0 }, { +1.0, -1.0, -1.0 },
          { +1.0, +1.0, -1.0 }, { +1.0, +1.0, +1.0 } },
        { { -1.0, -1.0, -1.0 }, { -1.0, -1.0, +1.0 },
          { -1.0, +1.0, +1.0 }, { -1.0, +1.0, -1.0 } },
        { { +1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 },
          { -1.0, +1.0, -1.0 }, { +1.0, +1.0, -1.0 } },
        { { -1.0, -1.0, +1.0 }, { +1.0, -1.0, +1.0 },
          { +1.0, +1.0, +1.0 }, { -1.0, +1.0, +1.0 } },
        { { -1.0, -1.0, -1.0 }, { +1.0, -1.0, -1.0 },
          { +1.0, -1.0, +1.0 }, { -1.0, -1.0, +1.0 } },
        { { -1.0, +1.0, +1.0 }, { +1.0, +1.0, +1.0 },
          { +1.0, +1.0, -1.0 }, { -1.0, +1.0, -1.0 } }
    };

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    for (int i = 0; i < 6; ++i)
    {
        glLoadName(i);
        glBegin(GL_QUADS);
        qglColor(faceColors[i]);
        for (int j = 0; j < 4; ++j)
        {
            glVertex3f(coords[i][j][0], coords[i][j][1], coords[i][j][2]);
        }
        glEnd();
    }
}  

void Cube::mousePressEvent(QMouseEvent *event) 
{ 
    lastPos = event->pos();
} 

#include <QDebug>
void Cube::mouseMoveEvent(QMouseEvent *event) 
{ 
    GLfloat dx = (GLfloat)(event->x() - lastPos.x()) / width();
    GLfloat dy = (GLfloat)(event->y() - lastPos.y()) / height();

    qDebug() << event->button();
    if (event->buttons() & Qt::LeftButton)
    {
        qDebug() << "left";
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        qDebug() << "right";
        rotationX += 180 * dy;
        rotationZ += 180 * dx;
        updateGL();
    }
    lastPos = event->pos();
}

void Cube::mouseDoubleClickEvent(QMouseEvent *event) 
{ 
    int face = faceAtPosition(event->pos());
    if (face != -1)
    {
        QColor color = QColorDialog::getColor(faceColors[face],
                                              this);
        if (color.isValid())
        {
            faceColors[face] = color;
            updateGL();
        }
    }
}

int Cube::faceAtPosition(const QPoint &pos) 
{ 
    const int MaxSize = 512;
    GLuint buffer[MaxSize];
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(MaxSize, buffer);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
#if 0
    gluPickMatrix((GLdouble)pos.x(),
                  (GLdouble)(viewport[3] - pos.y()),
            5.0, 5.0, viewport);
#else
    Q_UNUSED(pos);
#endif
    GLfloat x = (GLfloat)width() / height();
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    draw();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    if (!glRenderMode(GL_RENDER))
        return -1;
    return buffer[3];
}

