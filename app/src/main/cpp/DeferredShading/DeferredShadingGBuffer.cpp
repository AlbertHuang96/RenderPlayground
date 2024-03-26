//
// Created by 86422 on 2024/2/7.
//


#include <gtc/matrix_transform.hpp>
#include "DeferredShadingGBuffer.h"
#include "../util/UtilGL.h"

const GLenum attachments[3] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
};

/*const GLenum attachments2[2] = {
        GL_COLOR_ATTACHMENT0,
        GL_DEPTH_ATTACHMENT
};*/

const GLenum attachments2[1] = {
        GL_COLOR_ATTACHMENT0
};

GBufferDeferredShading::GBufferDeferredShading()
{
    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;

    mBackpack = nullptr;

    mDepthPass = nullptr;
    mDrawDepthPass = nullptr;
    mGbufferPass = nullptr;
    mLightingPass = nullptr;
    mLightBoxPass = nullptr;

}

GBufferDeferredShading::~GBufferDeferredShading()
{

}

void GBufferDeferredShading::Init(int screenW, int screenH)
{
    if(mBackpack != nullptr)
        return;

    std::string path(DEFAULT_OGL_ASSETS_DIR);
    //mBackpack = new Model(path + "/model/poly/Apricot_02_hi_poly.obj");
    mBackpack = new Model(path + "/model/backpack/backpack.obj");

    objectPositions.push_back(glm::vec3(-5.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3(3.0,  -0.5,  0.0));

    objectPositions.push_back(glm::vec3( 2.0,  2, -5.0));
    objectPositions.push_back(glm::vec3( -1.0,  3, -5.0));

    // reverse-z
    glEnable(GL_DEPTH_TEST);
    glDepthRangef(1.0f, 0.0f);
    glDepthFunc(GL_GREATER);

    zNear = 2.0f, zFar = 100.0f;

    /*glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &gDiffuseSpec);
    glBindTexture(GL_TEXTURE_2D, gDiffuseSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuseSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    //unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glActiveTexture(GL_TEXTURE3);
    glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    //GL_DEPTH_COMPONENT32F
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenW, screenH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, screenW, screenH, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

    *//*glGenRenderbuffers(1, &gDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenW, screenH);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gDepth);*//*

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        LOGCATE("GBufferDeferredShading::Init() GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (mBackpack->ContainsTextures())
    {
        mGbufferPass = new Shader(GBufferVSShaderPath, GBufferFSShaderPath);
    }
    else
    {
        //mGbufferPass = new Shader(vShaderStr, fNoTextureShaderStr);
    }


    mLightingPass = new Shader(LightingVSShaderPath, LightingFSShaderPath);*/

    mDrawDepthPass = new Shader(QuadVSShaderPath, DepthFSShaderPath);

    mDepthPass = new Shader(DepthVSShaderPath, SimpleDepthFSShaderPath);

    // visualize depth
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    glGenTextures(1, &dataBuffer);
    glBindTexture(GL_TEXTURE_2D, dataBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dataBuffer, 0);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenW, screenH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    // attach depth texture as FBO's depth buffer
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glDrawBuffers(1, attachments2);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        LOGCATE("GBufferDeferredShading::Init() GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBufferDeferredShading::RecreateFramebuffers(int screenW, int screenH)
{
    /*glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenW, screenH, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &gDiffuseSpec);
    glBindTexture(GL_TEXTURE_2D, gDiffuseSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gDiffuseSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    //unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glActiveTexture(GL_TEXTURE3);
    glGenTextures(1, &gDepth);
    glBindTexture(GL_TEXTURE_2D, gDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenW, screenH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, screenW, screenH, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);*/

    //There is one very special packed type​ field. It is GL_FLOAT_32_UNSIGNED_INT_24_8_REV. This can only be used in tandem with images that use the GL_DEPTH32F_STENCIL8 image format.
    // It represents two 32-bit values. The first value is a 32-bit floating-point depth value.
    // The second breaks the 32-bit integer value into 24-bits of unused space, followed by 8 bits of stencil

    /*glGenRenderbuffers(1, &gDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenW, screenH);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gDepth);*/

    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    glGenTextures(1, &dataBuffer);
    glBindTexture(GL_TEXTURE_2D, dataBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dataBuffer, 0);

    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenW, screenH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    // es 3.0 glDrawBuffers
    //glDrawBuffers(0, GL_NONE);
    //glReadBuffer(GL_NONE);
    glDrawBuffers(1, attachments2);

    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        LOGCATE("GBufferDeferredShading::RecreateFramebuffers() GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

//com.example.renderplayground A/libc: Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR),
// fault addr 0xa8 in tid 7886 (GLThread 44), pid 7835 (enderplayground)
void GBufferDeferredShading::Draw(int screenW, int screenH)
{
    if(mBackpack == nullptr) return;
    //LOGCATE("GBufferDeferredShading::Draw()");

    //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFrameBuffer);

    //glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    //glClearDepthf(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 Projection;
    UpdateMVPMatrix(Projection, m_AngleX, m_AngleY, screenW, screenH);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, mBackpack->GetMaxViewDistance() * 1.8f), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 modelTmp = m_ModelMatrix;

    /*mGbufferPass->use();
    mGbufferPass->setVec3("lightPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));
    mGbufferPass->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    mGbufferPass->setVec3("viewPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));
    mGbufferPass->setMat4("u_ViewMatrix", View);
    mGbufferPass->setMat4("u_ProjectionMatrix", Projection);
    for (int i = 0; i < objectPositions.size(); i++)
    {
        m_ModelMatrix = modelTmp;
        m_ModelMatrix = glm::translate(m_ModelMatrix, objectPositions[i]);
        m_MVPMatrix = Projection * View * m_ModelMatrix;
        mGbufferPass->setMat4("u_ModelMatrix", m_ModelMatrix);
        //mGbufferPass->setMat4("u_MVPMatrix", m_MVPMatrix);
        mBackpack->Draw((*mGbufferPass));
    }*/

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClearDepthf(0.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    mDepthPass->use();
    mDepthPass->setFloat("zNear", zNear);
    mDepthPass->setFloat("zFar", zFar);
    mDepthPass->setMat4("u_ViewMatrix", View);
    mDepthPass->setMat4("u_ProjectionMatrix", Projection);
    for (int i = 0; i < objectPositions.size(); i++)
    {
        m_ModelMatrix = modelTmp;
        m_ModelMatrix = glm::translate(m_ModelMatrix, objectPositions[i]);
        //m_MVPMatrix = Projection * View * m_ModelMatrix;
        mDepthPass->setMat4("u_ModelMatrix", m_ModelMatrix);
        mBackpack->Draw((*mDepthPass));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // visualize depth value

    glDisable(GL_DEPTH_TEST);
    mDrawDepthPass->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dataBuffer);
    mDrawDepthPass->setInt("depthTexture", 0);
    mDrawDepthPass->setFloat("zNear", zNear);
    mDrawDepthPass->setFloat("zFar", zFar);
    RenderQuad();
    //RenderQuadNoTexcoord();

    /*glClear(GL_COLOR_BUFFER_BIT);
    mLightingPass->use();
    glActiveTexture(GL_TEXTURE0);
    mLightingPass->setInt("gPosition", 0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    //UtilGL::setInt(mLightingPass->ID, "gPosition", 0);

    glActiveTexture(GL_TEXTURE1);
    mLightingPass->setInt("gNormal", 1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    //UtilGL::setInt(mLightingPass->ID, "gNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    mLightingPass->setInt("gDiffuseSpec", 2);
    glBindTexture(GL_TEXTURE_2D, gDiffuseSpec);
    //UtilGL::setInt(mLightingPass->ID, "gDiffuseSpec", 2);

    const float linear = 0.7f;
    const float quadratic = 1.8f;
    mLightingPass->setFloat("linear", linear);
    mLightingPass->setFloat("quadratic", quadratic);
    mLightingPass->setVec3("lightPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));
    mLightingPass->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    mLightingPass->setVec3("viewPos", glm::vec3(0, 0, mBackpack->GetMaxViewDistance()));*/

    //RenderQuad();
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

}

void GBufferDeferredShading::Destroy()
{
    LOGCATE("GBufferDeferredShading::Destroy");
    if (mBackpack != nullptr) {
        mBackpack->Destroy();
        delete mBackpack;
        mBackpack = nullptr;
    }

    if (mGbufferPass != nullptr) {
        mGbufferPass->Destroy();
        delete mGbufferPass;
        mGbufferPass = nullptr;
    }

    if (mLightingPass != nullptr) {
        mLightingPass->Destroy();
        delete mLightingPass;
        mLightingPass = nullptr;
    }

    if (mLightBoxPass != nullptr) {
        mLightBoxPass->Destroy();
        delete mLightBoxPass;
        mLightBoxPass = nullptr;
    }
}

void GBufferDeferredShading::UpdateMVPMatrix(glm::mat4 &Projection, int angleX, int angleY, int screenW, int screenH)
{
    //LOGCATE("GBufferDeferredShading::UpdateMVPMatrix angleX = %d, angleY = %d, screenW = %d, screenH = %d", angleX, angleY, screenW, screenH);
    angleX = angleX % 360;
    angleY = angleY % 360;

    //转化为弧度角
    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);


    // Projection matrix

    // perspective peojection
    float ratio = (float)screenW / screenH;
    //zNear = 2.0f, zFar = 100.0f;
    // the aspect ratio param in perspective is width to height
    //Projection = glm::perspective(45.0f, ratio, near,far);

    // f = 1.0 / tan(fovy_rad / 2)
    //float f = 1.0 / tan(45 / 2);
    float f2 = 1.0 / tan(45 * 3.14159 / 180 / 2);
    Projection = glm::mat4(f2 / ratio, 0, 0, 0,
                            0,     f2, 0, 0,
                            0, 0, - zNear / (zFar - zNear), -1,
                            0, 0, - zNear * zFar / (zFar - zNear), 0);
    /*Projection = glm::mat4(f / ratio, 0, 0, 0,
                           0,     f, 0, 0,
                           0, 0, 0, -1,
                           0, 0, near, 0);*/

    // ortho projection
    // calculate the ratio manually when using ortho projection
    //glm::mat4 Projection;
    /*if (screenW > screenH)
    {
        //ratio = (float)screenW / screenH;
        //Projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, -2.0f, 2.0f);
    }
    else
    {
        //ratio = (float)screenH / screenW;
        //Projection = glm::ortho(-1.0f, 1.0f, -ratio, ratio, -2.0f, 2.0f);
    }*/

    // View matrix
    /*glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, mBackpack->GetMaxViewDistance() * 1.8f), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );*/

    // Model matrix
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(0.3 * m_ScaleX, 0.3 * m_ScaleY, 1.0f));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    //Model = glm::translate(Model, -mBackpack->GetAdjustModelPosVec());
    m_ModelMatrix = Model;
    //mvpMatrix = Projection * View * Model;
    //mvpMatrix = Projection * Model;


}

void GBufferDeferredShading::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
{
    RenderInterface::UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
    m_AngleX = static_cast<int>(rotateX);
    m_AngleY = static_cast<int>(rotateY);
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
}

void GBufferDeferredShading::RenderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void GBufferDeferredShading::RenderQuadNoTexcoord()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions
                -1.0f,  1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f,  1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}