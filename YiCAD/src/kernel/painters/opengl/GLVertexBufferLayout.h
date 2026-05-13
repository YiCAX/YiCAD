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

/// @file GLVertexBufferLayout.h
/// @brief OpenGL顶点缓冲区布局定义，描述顶点属性的数据类型和排列方式

#ifndef GLVERTEXBUFFERLAYOUT_H
#define GLVERTEXBUFFERLAYOUT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace opengl
{

/// @brief 顶点缓冲区元素
struct GLVertexBufferElement
{
    unsigned int type = 0;
    unsigned int count = 0;
    unsigned char normalized = 0;

    /// @brief 获取指定类型的大小（字节）
    /// @param [in] type OpenGL类型常量
    /// @return 字节数
    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            return 0;
        }
    }
};

/// @brief 顶点缓冲区布局
class GLVertexBufferLayout
{
public:
    GLVertexBufferLayout()
        : m_stride(0)
    {
    }

    /// @brief 添加指定类型的元素到布局
    /// @tparam T 数据类型
    /// @param [in] count 元素个数
    template<typename T>
    void push(unsigned int count)
    {
        // static_assert(false);
    }

    /// @brief 获取布局元素列表
    /// @return 元素列表的常量引用
    inline const std::vector<GLVertexBufferElement> getElements() const
    {
        return m_elements;
    }

    /// @brief 获取步长
    /// @return 步长（字节）
    inline unsigned int getStride() const
    {
        return m_stride;
    }

private:
    unsigned int                        m_stride = 0;
    std::vector<GLVertexBufferElement>  m_elements;
};

template<>
inline void GLVertexBufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += GLVertexBufferElement::getSizeOfType(GL_FLOAT) * count;
}

template<>
inline void GLVertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += GLVertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void GLVertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_stride += GLVertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
}

}

#endif //GLVERTEXBUFFERLAYOUT_H
