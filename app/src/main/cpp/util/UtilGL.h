//
// Created by 86422 on 2024/2/1.
//

#ifndef RENDERPLAYGROUND_UTILGL_H
#define RENDERPLAYGROUND_UTILGL_H

#include <GLES3/gl3.h>
#include <string>
#include <glm.hpp>
#include <jni.h>

//namespace UtilGL
class UtilGL
{
public:
    static char* openTextFile(const char* path);

    static void setEnvAndAssetManager(JNIEnv* env, jobject assetManager);

    static GLuint LoadShader(GLenum shaderType, const char *pSource);

    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                                GLuint &vertexShaderHandle,
                                GLuint &fragShaderHandle);

    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource);

    static void DeleteProgram(GLuint &program);

    static void CheckGLError(const char *pGLOperation);

    static void setBool(GLuint programId, const std::string &name, bool value)
    {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), (int) value);
    }

    static void setInt(GLuint programId, const std::string &name, int value) {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }
};
#endif //RENDERPLAYGROUND_UTILGL_H
