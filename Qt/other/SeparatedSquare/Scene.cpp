#include <QMatrix4x4>
#include "Scene.h"

Scene::Scene( QWidget *parent ) :
    QGLWidget( parent ),
    m_program( 0 )
{

}

void Scene::initializeGL()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram( this );
    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex, ":/Shaders/vShader.glsl" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment, ":/Shaders/fShader.glsl" );
    if ( !m_program->link() )
    {
        qWarning( "Error: unable to link a shader program" );
        return;
    }
    m_posAttr = m_program->attributeLocation( "posAttr" );
    m_colAttr = m_program->attributeLocation( "colAttr" );
    m_matrixUniform = m_program->uniformLocation( "matrix" );
}

void Scene::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

    if( !m_program->bind() )
        return;

    QMatrix4x4 matrix;
    matrix.ortho( -2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f );

    glPushMatrix();
    {
        matrix.translate( 0.0f, 0.0f, -1.0f );
        m_program->setUniformValue( m_matrixUniform, matrix );
        drawSquare( m_square );
    }
    glPopMatrix();

    m_program->release();
}

void Scene::resizeGL( int w, int h )
{
    if ( h == 0 )
        h = 1;

    glViewport( 0, 0, w, h );
}

void Scene::drawSquare( const Square &square )
{
    glVertexAttribPointer( m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square.vertices.data() );
    glVertexAttribPointer( m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, square.colors.data() );

    glEnableVertexAttribArray( m_posAttr );
    glEnableVertexAttribArray( m_colAttr );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glDisableVertexAttribArray( m_colAttr );
    glDisableVertexAttribArray( m_posAttr );
}
