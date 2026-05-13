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

/// @file GLRenderBuffer.cpp
/// @brief OpenGL渲染缓存封装实现

#include "GLRenderBuffer.h"

GLRenderBuffer::GLRenderBuffer()
    : id(0)
{
}

GLRenderBuffer::~GLRenderBuffer()
{
    freeRBO();
}

void GLRenderBuffer::gen(int width, int height, bool useMultiSample)
{
    freeRBO();
    glGenRenderbuffers(1, &id);
    bind();
    this->useMultiSample = useMultiSample;
    if (useMultiSample)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
}

void GLRenderBuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void GLRenderBuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GLRenderBuffer::freeRBO()
{
    if (id != 0)
    {
        unbind();
        glDeleteRenderbuffers(1, &id);
        id = 0;
    }
}

bool GLRenderBuffer::isValid() const
{
    return id > 0;
}
