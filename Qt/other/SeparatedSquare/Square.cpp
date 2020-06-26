#include "Square.h"

Square::Square( float size ) :
    m_size( size )
{
    initVertices();
    initColors();
}

void Square::setSize(float size)
{
    m_size = size;
    initVertices();
}

float Square::size()
{
    return m_size;
}

void Square::initVertices()
{
    vertices.clear();
    vertices.resize( 12 );
    // 0
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    // 1
    vertices[2] = m_size;
    vertices[3] = 0;
    // 2
    vertices[4] = 0;
    vertices[5] = m_size;
    // 3
    vertices[6] = 0;
    vertices[7] = m_size;
    // 4
    vertices[8] = m_size;
    vertices[9] = 0;
    // 5
    vertices[10] = m_size;
    vertices[11] = m_size;
}

void Square::initColors()
{
    // 0
    colors.clear();
    colors.resize( 18 );
    // 1
    colors[0] = 1.0f;
    colors[1] = 0.0f;
    colors[2] = 0.0f;
    // 2
    colors[3] = 0.0f;
    colors[4] = 1.0f;
    colors[5] = 0.0f;
    // 3
    colors[6] = 0.0f;
    colors[7] = 0.0f;
    colors[8] = 1.0f;
    // 4
    colors[9] = 1.0f;
    colors[10] = 0.0f;
    colors[11] = 0.0f;
    // 5
    colors[12] = 0.0f;
    colors[13] = 1.0f;
    colors[14] = 0.0f;
    // 6
    colors[15] = 0.0f;
    colors[16] = 0.0f;
    colors[17] = 1.0f;
}
