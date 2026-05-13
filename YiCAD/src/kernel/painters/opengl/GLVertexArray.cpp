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

/// @file GLVertexArray.cpp
/// @brief OpenGL顶点数组对象(VAO)封装实现

#include "GLVertexArray.h"
#include <GL/glew.h>

using namespace opengl;

GLVertexArray::GLVertexArray()
    : m_va_id(0)
{
}

GLVertexArray::~GLVertexArray()
{
    freeVAO();
}

void GLVertexArray::gen()
{
    freeVAO();
    glGenVertexArrays(1, &m_va_id);
    glBindVertexArray(m_va_id);
}

void GLVertexArray::addBuffer(GLVertexBuffer& vb, GLVertexBufferLayout& layout)
{
    vb.bind();  //  layout of this vb (vertex buffer) (binding it for being sure its is the vb)

    const auto& elements = layout.getElements();

    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), reinterpret_cast<const void*>(static_cast<uintptr_t>(offset)));
        glEnableVertexAttribArray(i);

        offset += element.count * GLVertexBufferElement::getSizeOfType(element.type);
    }
}

void GLVertexArray::bind()
{
    glBindVertexArray(m_va_id);
}

void GLVertexArray::unbind()
{
    glBindVertexArray(0);
}

void GLVertexArray::freeVAO()
{
    if (m_va_id != 0)
    {
        unbind();
        glDeleteVertexArrays(1, &m_va_id);
        m_va_id = 0;
    }
}

bool opengl::GLVertexArray::isValid() const
{
    return m_va_id > 0;
}
