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

/// @file GLVertexBuffer.cpp
/// @brief OpenGL顶点缓冲区对象(VBO)封装实现

#include "GLVertexBuffer.h"
#include <GL/glew.h>

using namespace opengl;

GLVertexBuffer::GLVertexBuffer()
    : m_vb_id(0)
{
}

GLVertexBuffer::~GLVertexBuffer()
{
    freeVBO();
}

void GLVertexBuffer::gen(const void* data, unsigned int size)
{
    freeVBO();
    glGenBuffers(1, &m_vb_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

// 绑定命名缓冲区对象
void GLVertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vb_id);
}

void GLVertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::freeVBO()
{
    if (m_vb_id != 0)
    {
        unbind();
        glDeleteBuffers(1, &m_vb_id);
        m_vb_id = 0;
    }
}

bool opengl::GLVertexBuffer::isValid() const
{
    return m_vb_id > 0;
}
