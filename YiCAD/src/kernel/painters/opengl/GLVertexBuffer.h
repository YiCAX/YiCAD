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

/// @file GLVertexBuffer.h
/// @brief OpenGL顶点缓冲区对象(VBO)封装

#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

namespace opengl
{

/// @brief 顶点缓冲区
class GLVertexBuffer
{
public:
    /// @brief 顶点缓冲区
    GLVertexBuffer();
    ~GLVertexBuffer();
    GLVertexBuffer(const GLVertexBuffer&) = delete;
    GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;

    /// @brief 生成VBO并上传数据
    /// @param [in] data 顶点数据指针
    /// @param [in] size 数据大小（字节）
    void gen(const void* data, unsigned int size);

    /// @brief 绑定命名缓冲区对象
    void bind();

    /// @brief 取消绑定命名缓冲区对象
    void unbind();

    /// @brief 释放VBO对象
    void freeVBO();

    /// @brief 获取VBO的ID
    /// @return VBO的ID
    unsigned int getID() const { return m_vb_id; }

    /// @brief 检查VBO是否有效
    /// @return 有效返回true
    bool isValid() const;

private:
    unsigned int m_vb_id = 0;
};

}

#endif //GLVERTEXBUFFER_H
