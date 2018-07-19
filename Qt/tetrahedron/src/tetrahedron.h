#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H
 
#include <QGLWidget>
#include <GL/glu.h>

class Tetrahedron : public QGLWidget
{
    Q_OBJECT
 
public:
    Tetrahedron(QWidget *parent = 0);
 
protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
 
private:
    void draw();
    void draw_sphere();
    void draw_cylinder();
    int faceAtPosition(const QPoint &pos);
 
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    QColor faceColors[4];
    QPoint lastPos;

    GLUquadricObj *cylinder;
    GLUquadricObj *sphere;

    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;
};
 
#endif
