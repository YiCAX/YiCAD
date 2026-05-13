/*
 * Copyright (C) 2026 YiCAD Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file GLTexture.cpp
/// @brief OpenGL纹理封装实现

#include "GLTexture.h"

GLTexture::GLTexture()
    : id(0)
    , useMultiSample(true)
    , useAlpha(true)
{
}

GLTexture::~GLTexture()
{
    freeTexture();
}

void GLTexture::gen(int width, int height, bool useMultiSample, bool useAlpha)
{
    freeTexture();
    glGenTextures(1, &id);
    bind();
    this->useAlpha = useAlpha;
    this->useMultiSample = useMultiSample;
    if (useMultiSample)
    {
        if (useAlpha)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_TRUE);
        }
        else
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, id, 0);
    }
    else
    {
        if (useAlpha)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    }
}

void GLTexture::bind()
{
    if (useMultiSample)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }
}

void GLTexture::unbind()
{
    if (useMultiSample)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void GLTexture::freeTexture()
{
    if (id != 0)
    {
        unbind();
        glDeleteTextures(1, &id);
        id = 0;
    }
}

bool GLTexture::isValid() const
{
    return id > 0;
}
