#include <common.h>
#include <GLESWrapper.h>

//We will try to learn the 3D Maths of Projection and Tranformation
class Rectangle : public iv::GLESWrapper
{
    // GLESWrapper interface
public:
    GLint initGLES();
    void drawFrames();
protected:

private:
    GLuint _locAPosition;
    GLuint _locUDegree;
    GLuint _programObject;

    GLfloat  _degree;

};

GLint Rectangle::initGLES()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);

    _programObject = getGLProgramID("../../vRectangle.vsh", "../../fRectangle.fsh");
    _locAPosition = glGetAttribLocation(_programObject, "a_position");
    _locUDegree = glGetUniformLocation(_programObject, "u_deg");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    _degree = 0;
}

void Rectangle::drawFrames()
{
    glViewport(0, 0, _u32width, _u32height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(_programObject);

    static GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    _degree = _degree + 0.01f;
    if(_degree > 360.0f)
    {
        _degree = 0.0f;
    }

    static GLuint indices[] = { 0, 1, 2, 0, 2, 3};

    glVertexAttribPointer(_locAPosition, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), vertices );
    glEnableVertexAttribArray(_locAPosition);

    glUniform1f(_locUDegree, _degree);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

}

int main()
{
    F_LOG;
    iv::RenderConfig renConfig = {iv::EGL_SURFACE, 1024, 768, iv::MD_WINDOW_RGB, "Cube"};
    iv::GLESWrapper *rectangle = new Rectangle();

    rectangle->initEGL(renConfig);
    rectangle->initGLES();
    rectangle->startRender();
}

