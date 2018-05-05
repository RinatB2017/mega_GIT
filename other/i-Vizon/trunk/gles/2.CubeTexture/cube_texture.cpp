#include "cube_texture.h"

GLint CubeTexture::initGLES()
{
    F_LOG;
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);
    glEnable(GL_TEXTURE_2D);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    _programObject = getGLProgramID("../../cubeVertex.vsh", "../../cubeFragment.fsh");
    _locPosition = glGetAttribLocation(_programObject, "a_position");
    _locMVPMatrix = glGetUniformLocation(_programObject, "u_mvpMatrix");

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
}

GLint CubeTexture::updateMVP()
{
    // Compute the window aspect ratio
    GLfloat aspect = (GLfloat) _u32width / (GLfloat) _u32height ;

    //Now we start with out Model View Projection Matrix
    iv::Matrix perspective;
    iv::Matrix modelView;

    // Generate a perspective matrix with a 60 degree FOV
    //FOV -> Feild Of View
    iv::LoadIdentity(&perspective);
    iv::Perspective(&perspective, 60.0f, aspect, 1.0f, 20.0f );

    // Generate a model view matrix to rotate/translate the cube
    iv::LoadIdentity( &modelView );
    // Translate away from the viewer
    iv::Translate( &modelView, 0.0, 0.0, -2.0 );
    // Rotate the cube
    iv::Rotate( &modelView, _rotatingAngle, 1.0, 0.0, 1.0 );

    // Compute the final MVP by multiplying the
    // modevleiw and perspective matrices together
    iv::MatrixMultiply( &_mvpMatrix, &modelView, &perspective );

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            std::cout<<_mvpMatrix.m[i][j]<<"\t";
        }
        std::cout<<std::endl;
    }

    GLfloat cubeVerts[] =
    {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
    };

    GLuint cubeIndices[] =
    {
        0, 2, 1,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 15, 14,
        12, 14, 13,
        16, 17, 18,
        16, 18, 19,
        20, 23, 22,
        20, 22, 21
    };

    int numVertices = 24;
    int numIndices = 36;
    _vertices = (GLfloat*) malloc ( sizeof(GLfloat) * 3 * numVertices );
    memcpy( _vertices, cubeVerts, sizeof( cubeVerts ) );
    _indices = (GLuint*) malloc ( sizeof(GLuint) * numIndices );
    memcpy( _indices, cubeIndices, sizeof( cubeIndices ) );

    _rotatingAngle += 1;
    if(_rotatingAngle >=360.0f)
        _rotatingAngle = 0.0f;
}
void CubeTexture::drawFrames()
{
    updateMVP();
    glViewport ( 0, 0, _u32width, _u32height );


    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( _programObject );

    // Load the vertex position
    glVertexAttribPointer ( _locPosition, 3, GL_FLOAT,
                            GL_FALSE, 3 * sizeof(GLfloat), _vertices );

    glEnableVertexAttribArray ( _locPosition );


    // Load the MVP matrix
    glUniformMatrix4fv( _locMVPMatrix, 1, GL_FALSE, (GLfloat*) &_mvpMatrix.m[0][0] );


    // Draw the cube
    glDrawElements ( GL_TRIANGLES, 36/*(numIndecies)*/, GL_UNSIGNED_INT, _indices );
}
