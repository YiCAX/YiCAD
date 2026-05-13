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

/// @file GLVertexArray.h
/// @brief OpenGL顶点数组对象(VAO)封装

#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

#include "GLVertexBuffer.h"
#include "GLVertexBufferLayout.h"

namespace opengl
{

/// @brief 顶点数组
class GLVertexArray
{
public:
    /// @brief 顶点数组
    GLVertexArray();
    ~GLVertexArray();
    GLVertexArray(const GLVertexArray&) = delete;
    GLVertexArray& operator=(const GLVertexArray&) = delete;

    /// @brief 生成VAO对象
    void gen();

    /// @brief 绑定VAO
    void bind();

    /// @brief 取消绑定VAO
    void unbind();

    /// @brief 释放VAO对象
    void freeVAO();

    /// @brief 检查VAO是否有效
    /// @return 有效返回true
    bool isValid() const;

    /// @brief 向VAO添加顶点缓冲区及布局
    /// @param [in,out] vb 顶点缓冲区
    /// @param [in] layout 顶点布局
    void addBuffer(GLVertexBuffer& vb, GLVertexBufferLayout& layout);

    /// @brief 获取VAO的ID
    /// @return VAO的ID
    unsigned int getID() const { return m_va_id; }

private:
    unsigned int m_va_id = 0;
};

}

#endif //GLVERTEXARRAY_H
