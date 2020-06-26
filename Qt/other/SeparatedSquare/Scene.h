#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "Square.h"

class Scene : public QGLWidget, public QOpenGLFunctions
{
public:
    Scene( QWidget *parent = 0 );

private:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h);

    void drawSquare( const Square &square );

    QOpenGLShaderProgram *m_program;

    GLint m_posAttr;
    GLint m_colAttr;
    GLint m_matrixUniform;

    Square m_square;
};

#endif // SCENE_H
