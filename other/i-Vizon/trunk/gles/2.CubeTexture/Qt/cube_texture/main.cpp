#include "../../cube_texture.h"

int main()
{
    F_LOG;
    iv::RenderConfig renConfig = {iv::EGL_SURFACE, 1024, 768, iv::MD_WINDOW_RGB, "Cube"};
    iv::GLESWrapper *cubeTexture = new CubeTexture();

    cubeTexture->initEGL(renConfig);
    cubeTexture->initGLES();
    cubeTexture->startRender();
}
