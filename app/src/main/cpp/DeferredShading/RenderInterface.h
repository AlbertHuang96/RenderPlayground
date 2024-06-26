//
// Created by 86422 on 2024/2/1.
//

#ifndef RENDERPLAYGROUND_RENDERINTERFACE_H
#define RENDERPLAYGROUND_RENDERINTERFACE_H

#include <GLES3/gl3.h>

#define MATH_PI 3.1415926535897932384626433832802

#define DEFAULT_OGL_ASSETS_DIR "/sdcard/Android/data/com.example.renderplayground/files/Download"

class RenderInterface
{
public:
    RenderInterface()
    {
        rProgramID      = 0;
        rVertesShader   = 0;
        rFragmentShader = 0;
    }
    virtual ~RenderInterface()
    {

    }
    virtual void Init(int screenW, int screenH) = 0;
    virtual void Draw(int screenW, int screenH) = 0;

    virtual void Destroy() = 0;

    virtual void RecreateFramebuffers(int screenW, int screenH)
    {}

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
    {}
protected:
    GLuint rProgramID;
    GLuint rVertesShader;
    GLuint rFragmentShader;

};
#endif //RENDERPLAYGROUND_RENDERINTERFACE_H
