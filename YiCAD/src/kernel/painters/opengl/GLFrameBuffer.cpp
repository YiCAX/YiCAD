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

/// @file GLFrameBuffer.cpp
/// @brief OpenGL帧缓存封装实现

#include "GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer()
    : id(0)
    , default_id(0)
{
}

GLFrameBuffer::~GLFrameBuffer()
{
    freeFBO();
}

void GLFrameBuffer::gen(unsigned int default_id)
{
    freeFBO();
    this->default_id = default_id;
    glGenFramebuffers(1, &id);
    bind();
}

void GLFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void GLFrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, default_id);
}

void GLFrameBuffer::freeFBO()
{
    if (id != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, default_id);
        glDeleteFramebuffers(1, &id);
        id = 0;
    }
}

bool GLFrameBuffer::isValid() const
{
    return id > 0;
}
