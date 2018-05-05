#include <iv_gles_wrapper.h>
#include <opencv2/highgui/highgui.hpp>
#include "banana.h"

class TestWrapper : public iv::GLESWrapper
{
    //=================================================================
    ///Method to clear the EGL surface and make it ready for
    ///GL ES to draw
    GLint initGLES();
    //=================================================================
    ///Method to put graphics content in each frame
    void drawFrames();

    GLint _u32aPositionLoc;
    GLint _u32uTextureLoc;
    GLint _u32samplerLoc;
    GLuint _u32aTextures[1];
    cv::VideoCapture cap;
    cv::Mat mat;
};

//=================================================================
GLint TestWrapper::initGLES()
{
    GLbyte vShaderStr[] =
            "attribute vec4 aPosition;  \n"
            "attribute vec2 aTexCoord;  \n"
            "varying vec2 vTexCoord;    \n"
            "void main()                \n"
            "{                          \n"
            "gl_Position = aPosition;   \n"
            "vTexCoord = aTexCoord;     \n"
            "}                          \n";

    GLbyte fShaderStr[] =
            "precision mediump float;   \n"
            "varying vec2 vTexCoord;    \n"
            "uniform sampler2D sTexture;\n"
            "void main()                \n"
            "{                          \n"
            "vec4 tcolor = texture2D(sTexture, vTexCoord); \n"
            "gl_FragColor = vec4(tcolor.b,tcolor.g,tcolor.r,tcolor.a); \n"
            "//gl_FragColor = vec4(1.0, 1.0, 0.0, 0.5); \n"
            "}                          \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader ( GL_VERTEX_SHADER, (const char*) vShaderStr );
    fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, (const char*) fShaderStr );

    std::cout<<"vertexShader :"<<vertexShader<<std::endl;
    std::cout<<"fragmentShader :"<<fragmentShader<<std::endl;

    // Create the program object
    programObject = glCreateProgram ( );

    if ( programObject == 0 )
        return 0;

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    // Link the program
    glLinkProgram ( programObject );

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked )
    {
        GLint infoLen = 0;
        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );
            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            free ( infoLog );
        }
        glDeleteProgram ( programObject );
        return GL_FALSE;
    }

    _u32programObject = programObject;

    _u32samplerLoc = glGetUniformLocation(programObject, "sTexture");
    _u32uTextureLoc = glGetAttribLocation(programObject, "aTexCoord");
     _u32aPositionLoc = glGetAttribLocation(programObject, "aPosition");
    if(_u32aPositionLoc < 0 || _u32uTextureLoc < 0 || _u32samplerLoc < 0)
    {
        std::cout<<"GetAttribLocation Error!!!\n";
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);

    glGenTextures(1, &_u32aTextures[0]);
    glBindTexture(GL_TEXTURE_2D, _u32aTextures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    cap.open(-1);
    return GL_TRUE;
}
//=================================================================
void TestWrapper::drawFrames()
{
    std::cerr<<"drawframes...\n";

    GLfloat vVertices[] = { -1.0f, -1.0f, 0.0f,  // Position 0
                             1.0f, -1.0f, 0.0f,  // Position 1
                             1.0f,  1.0f, 0.0f,  // Position 2
                            -1.0f,  1.0f, 0.0f,  // Position 3

                          };
    GLfloat vTexCoord[] = { 0.0f,  1.0f,  // Texture Coord 0
                            1.0f,  1.0f,  // Texture Coord 1
                            1.0f,  0.0f,  // Texture Coord 2
                            0.0f,  0.0f,  // Texture Coord 3
                          };
    GLubyte indices[] = { 0, 1, 2, 0, 2, 3};
    cap >> mat;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.size().width,
                 mat.size().height,  0, GL_RGB, GL_UNSIGNED_BYTE,
                 mat.data);

    // Set the viewport
    glViewport ( 0, 0, _u32width, _u32height );

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( _u32programObject );


    // Load the vertex position
    glVertexAttribPointer ( _u32aPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( _u32aPositionLoc );
    // Load the texture coordinate
    glVertexAttribPointer ( _u32uTextureLoc, 2, GL_FLOAT, GL_FALSE, 0, vTexCoord);
    glEnableVertexAttribArray ( _u32uTextureLoc );

    //Banna
    // Load the vertex position
//    glVertexAttribPointer ( _u32aPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, bananaVerts );
//    glEnableVertexAttribArray ( _u32aPositionLoc );
//    // Load the texture coordinate
//    glVertexAttribPointer ( _u32uTextureLoc, 2, GL_FLOAT, GL_FALSE, 0, bananaTexCoords);
//    glEnableVertexAttribArray ( _u32uTextureLoc );

    // Bind the base map
    glActiveTexture ( GL_TEXTURE0);
    glBindTexture ( GL_TEXTURE_2D, _u32aTextures[0] );

    // Set the base map sampler to texture unit to 0
    glUniform1i ( _u32uTextureLoc, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices );
    //banna
     //glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, bananaNumVerts );

}

int main(int argc, char *argv[])
{
    iv::RenderConfig renConfig = {iv::EGL_SURFACE, 1024, 768, iv::MD_WINDOW_RGB, "TestWrapper"};
    iv::GLESWrapper *testRender = new TestWrapper();

    testRender->initEGL(renConfig);
    testRender->initGLES();
    testRender->startRender();
}
