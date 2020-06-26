#ifndef SQUARE_H
#define SQUARE_H

#include <vector>
#include <cstddef>

class Square
{
public:
    Square( float size = 1.0f );

    void initVertices();
    void initColors();

    void setSize( float size );
    float size();

    std::vector<float> vertices;
    std::vector<float> colors;

private:
    float m_size;
};

#endif // SQUARE_H
